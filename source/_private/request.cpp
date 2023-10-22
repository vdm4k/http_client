#include <charconv>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <system/stack_allocator.h>
#include <network/tcp/ssl/send/settings.h>
#include <http_client/_private/request.h>
#include <http_client/_private/uri_parser.h>

namespace bro::net::http::client::_private {

void request::add_header(std::string_view const &type, std::string_view const &value) {
    _total_size += value.size() + type.size() + e_header_add_size;
    _headers_v.push_back({type, value});
}

void request::add_header(std::string const &type, std::string const &value) {
    _total_size += value.size() + type.size() + e_header_add_size;
    _headers_s.push_back({type, value});
}

void request::add_body(std::string const &body, std::string const &content_type, bool compress_body) {
    if(!body.empty())
        return;
    add_header(header::to_string(header::types::e_Content_Type), content_type);
    if(compress_body)
        add_header(header::to_string(header::types::e_Content_Encoding), "gzip, deflate");
    _compress_body = compress_body;
    _body_s = body;
}

void request::add_body(std::string_view const &body, std::string const &content_type, bool compress_body) {
    if(!body.empty())
        return;
    add_header(header::to_string(header::types::e_Content_Type), content_type);
    if(compress_body)
        add_header(header::to_string(header::types::e_Content_Encoding), "gzip, deflate");
    _compress_body = compress_body;
    _body_v = body;
}

bool request::create_stream() {

    if (_send_stream && _send_stream->get_state() == strm::stream::state::e_established) {
        generate_message();
        return true;
    }

    switch (_connection_type) {
    case connection_type::e_http: {
        bro::net::tcp::send::settings send_set;
        send_set._peer_addr = _server_address;
        _send_stream = _factory->create_stream(&send_set);
        break;
    }
    case connection_type::e_https: {
        bro::net::tcp::ssl::send::settings send_set;
        send_set._peer_addr = _server_address;
        send_set._host_name = _host;
        _send_stream = _factory->create_stream(&send_set);
        break;
    }
    }
    if (!_send_stream->is_active()) {
        std::string error = fmt::sprintf("Create stream failed with error -  %s", _send_stream->get_error_description());
        set_error(error.c_str());
        return false;
    }

    _factory->bind(_send_stream);
    _send_stream->set_state_changed_cb(
        [&](strm::stream *strm, std::any) {
            if(_config._processed_events)
                (*_config._processed_events)++;
            if (strm->get_state() == strm::stream::state::e_established)
                generate_message();
            if(!strm->is_active()) {
                std::string error = fmt::sprintf("Stream failed with error: %s", strm->get_error_description());
                set_error(error.c_str());
            }
        },
        nullptr);
    _send_stream->set_received_data_cb(
        [&](strm::stream *strm, std::any) {
            if(_config._processed_events)
                (*_config._processed_events)++;
            system::stack_allocator<std::byte, e_max_msg_size> st_allocator;
            auto res = strm->receive(st_allocator.get_array(), st_allocator.get_size());
            if (res > 0) {
                auto err = llhttp_execute(&_parser, (char *)st_allocator.get_array(), res);
                if (err != HPE_OK) {
                    std::string error = fmt::sprintf("Parse error: %s %s", llhttp_errno_name(err), _parser.reason);
                    set_error(error.c_str());
                }
            }
        },
        nullptr);
    return true;
}

void request::init_parser() {
    /* Initialize user callbacks and settings */
    llhttp_settings_init(&_parser_settings);
    /* Set user callback */
    _parser_settings.on_message_complete = handle_on_message_complete;
    _parser_settings.on_status = on_status;
    _parser_settings.on_body = on_body;
    _parser_settings.on_version = on_version;
    _parser_settings.on_header_field = on_header_field;
    _parser_settings.on_header_value = on_header_value;
    llhttp_init(&_parser, HTTP_RESPONSE, &_parser_settings);
    _parser.data = this;
}

void request::generate_message() {
    // add headers base on settings
    if (_config._support_gzip)
        add_header(header::to_string(header::types::e_Accept_Encoding), "gzip, deflate");
    if (_config._close_connection) {
        char const *close = "close";
        add_header(header::to_string(header::types::e_Connection), close);
    } else {
        char const *keep_alive = "keep-alive";
        add_header(header::to_string(header::types::e_Connection), keep_alive);
    }

    int body_size = _body_s.empty() ? _body_v.size() : _body_s.size();
    if(body_size) {
        add_header(header::to_string(header::types::e_Content_Length), std::to_string(body_size));
    }

    // count total size
    int path_header_size = client::request::to_string(_type).size() + _path.size() + header::to_string(_config._version).size() + e_eol_size
                           + e_2_spoce_size;                                                                   // path size
    int host_header_size = header::to_string(header::types::e_Host).size() + _host.size() + e_header_add_size; // host size

    int total = _total_size + body_size + e_eol_size + path_header_size + host_header_size;
    // generate data
    system::stack_allocator<std::byte, e_max_msg_size> st_allocator(total);
    auto res = fmt::format_to_n((char *) st_allocator.get_array(),
                                total,
                                "{} {} {}\r\n",
                                client::request::to_string(_type),
                                _path,
                                header::to_string(_config._version));
    total -= res.size;
    res = fmt::format_to_n(res.out, total, "{}: {}\r\n", header::to_string(header::types::e_Host), _host);
    total -= res.size;
    for (auto const &hdr : _headers_s) {
        res = fmt::format_to_n(res.out, total, "{}: {}\r\n", hdr.first, hdr.second);
        total -= res.size;
    }
    for (auto const &hdr : _headers_v) {
        res = fmt::format_to_n(res.out, total, "{}: {}\r\n", hdr.first, hdr.second);
        total -= res.size;
    }
    res = fmt::format_to_n(res.out, total, "\r\n");
    total -= res.size;
    if(body_size) {
        if(!_body_s.empty()) {
            fmt::format_to_n(res.out, total, "{}", _body_s);
        } else {
            fmt::format_to_n(res.out, total, "{}", _body_v);
        }
    }

    _send_stream->send(st_allocator.get_array(), st_allocator.get_size());
    init_parser();
}

void request::proceed_events() {
    _resolver->proceed();
    _factory->proceed();
}

void request::proceed_logic() {
    if(_do_cleanup) {
        reset();
        _do_cleanup = false;
    }
    if(_do_reset_send_stream) {
        _send_stream.reset();
        _do_reset_send_stream = false;
    }
}

int request::on_status(llhttp_t *parser, char const *at, size_t length) {
    request *req = (request *) parser->data;
    req->_response._status_code = status::to_code({at, length});
    return 0;
}

void request::decoded_data(Bytef *data, size_t lenght, std::any user_data, char const *error) {
    request *req = std::any_cast<request *>(user_data);
    if (error) {
        req->set_error(error);
    } else {
        req->_response._body.append((char const *) data, lenght);
    }
}

int request::on_body(llhttp_t *parser, char const *at, size_t length) {
    request *req = (request *) parser->data;
    if (req->_response._is_gzip_encoded) {
        req->_zstream.process((Bytef *) at, length, req, decoded_data);
    } else
        req->_response._body.append(at, length);
    return 0;
}

int request::on_version(llhttp_t *parser, char const *at, size_t length) {
    request *req = (request *) parser->data;
    req->_response._version = header::to_version({at, length});
    return 0;
}

int request::on_header_field(llhttp_t *parser, char const *at, size_t length) {
    request *req = (request *) parser->data;
    response::header_data hdr;
    hdr._type.append(at, length);
    req->_response._headers.push_back(hdr);
    return 0;
}

int request::on_header_value(llhttp_t *parser, char const *at, size_t length) {
    request *req = (request *) parser->data;
    if (req->_response._headers.empty())
        return 0;
    auto &hdr = req->_response._headers.back();
    hdr._value.append(at, length);
    if (hdr._type == header::to_string(header::types::e_Content_Encoding))
        req->_response._is_gzip_encoded = req->_config._try_decompress_response &&
                                          (hdr._value.find("gzip") != std::string::npos || hdr._value.find("deflate") != std::string::npos)
                                          && req->_zstream.init(bro::zlib::stream::type::e_decompressor);

    return 0;
}

void request::redirect() {
    auto it = std::find_if(_response._headers.begin(), _response._headers.end(), [](auto const &hdrs) {
        return hdrs._type == header::to_string(header::types::e_Location);
    });
    if (it == _response._headers.end()) {
        set_error("receive redirect, but couldn't find location");
        return;
    }

    _host = {};
    _url = it->_value;
    _response = {};
    _do_reset_send_stream = true;
    generate_from_url();
}

int request::handle_on_message_complete(llhttp_t *h) {
    if (!h->data)
        return -1;
    request *req = (request *) h->data;
    auto &res = req->_response;
    if (req->_config._support_redirect && status::code::e_Moved_Permanently <= res.get_status_code()
        && res.get_status_code() <= status::code::e_Permanent_Redirect) {
        req->redirect();
    } else {
        req->_result._cb(std::move(res), nullptr, req->_result._data);
    }
    return 0;
}

bool request::parse_uri() {
    uri_parser u_parser;
    // parse full URI ( syntax check )
    int rc = u_parser.parse(_url);
    if (rc != URI_SUCCESS) {
        set_error(uri_parser::uri_parser_error_to_string(rc));
        return false;
    }

    auto const &uri = u_parser.get_uri();
    // check host is set (ip or host name)
    if (!uri.hostText.first) {
        set_error("Uri parse error: host not set");
        return false;
    }

    // after host must be all path
    _path = std::string_view(uri.hostText.afterLast);

    // set ip address or host
    if (uri.hostData.ip4 || uri.hostData.ip6) {
        if (uri.hostData.ip4) {
            _server_address.set_address(proto::ip::v4::address(uri.hostData.ip4->data));
        } else {
            _server_address.set_address(proto::ip::v6::address(uri.hostData.ip6->data));
        }
    } else if (uri.hostText.first) {
        _host = std::string_view(uri.hostText.first, std::distance(uri.hostText.first, uri.hostText.afterLast));
    } else {
        set_error("Uri parse error: need to set ip address or host name in URI");
        return false;
    }

    // check scheme is set
    if (!uri.scheme.first) {
        set_error("Uri parse error: scheme not set (http/https)");
        return false;
    }

    // get connection type
    std::string_view scheme(uri.scheme.first, std::distance(uri.scheme.first, uri.scheme.afterLast));
    if (scheme == "http") {
        _connection_type = connection_type::e_http;
    } else if (scheme == "https") {
        _connection_type = connection_type::e_https;
    } else {
        set_error("Uri parse error: not supported scheme type (not http/https)");
        return false;
    }

    // set port
    if (uri.portText.first) {
        uint16_t port{0};
        auto [ptr, ec]{std::from_chars(uri.portText.first, uri.portText.afterLast, port)};
        if (ec != std::errc()) {
            set_error("Uri parse error: couldn't parse port in url");
            return false;
        }
        _server_address.set_port(port);
        _path = std::string_view(uri.portText.afterLast);
    } else {
        _connection_type == connection_type::e_http ? _server_address.set_port(80) : _server_address.set_port(443);
    }
    return true;
}

void request::set_error(char const *error) {
    _state = request_state::e_failed;
    _result._cb({}, error, _result._data);    
    _do_cleanup = true;
}

void request::reset() {
    soft_reset();
    _send_stream.reset();    
}

void request::soft_reset() {
    _type = {};
    _result = {};
    _total_size = 0;
    _headers_v.clear();
    _headers_s.clear();
    _body_s.clear();
    _body_v = {};
    _config = {};
    _state = {request_state::e_idle};
    _response._body.clear();
    _response._headers.clear();
    _response._is_gzip_encoded = {false};                                 ///< is commpressed by gzip
    _response._status_code = {status::code::e_Unknown_Code};      ///< status code
    _response._version = {header::version::e_Unknown_Version}; ///< http version
    _zstream.cleanup();
}

void request::resolve_host() {
    _resolver->resolve({_host.begin(), _host.end()},
                       proto::ip::address::version::e_v4,
                       [this](bro::net::proto::ip::address const &addr, std::string const &hostname, char const *err) {
                           if(_config._processed_events)
                               (*_config._processed_events)++;
                           if (err) {
                               std::string res("dns resolver failed with error ");
                               res += err;
                               res += ", for hostname - " + hostname;
                               set_error(res.c_str());
                           } else {
                               _server_address.set_address(addr);
                               create_stream();
                           }
                       });
}

bool request::generate_from_url() {
    if (!parse_uri())
        return false;
    if (!_host.empty())
        resolve_host();
    else
        return create_stream();
    return true;
}

bool request::send(client::request::type tp, std::string const & url, client::request::result &&res, client::request::config *conf) {
    if (get_state() == request_state::e_active) {
        res._cb({}, "request is in active state", res._data); // just for test
        return false;
    }
    _state = request_state::e_active;
    _type = tp;
    if (conf)
        _config = *conf;
    _result = std::move(res);
    _url = url;
    return compress_body() && generate_from_url();
}

/**
 * \brief data wrapper
 * 
 */
struct compress_node {
    request *_request = nullptr;   ///< pointer on current request
    std::string *_result = nullptr; ///< pointer on string to save 
};

bool request::compress_body() {
    if(!_compress_body)
        return true;
    zlib::stream zstream;
    if(!zstream.init(bro::zlib::stream::type::e_compressor)) {
        set_error("couldn't init compressor");
        return false;
    }

    char const * at = _body_s.empty() ? _body_v.data() : _body_s.data();
    size_t length = _body_s.empty() ? _body_v.size() : _body_s.size();
    std::string result;
    compress_node c_node{this, &result};
    auto fun = [](Bytef *data, size_t lenght, std::any user_data, char const *error) {
        auto [req, result] = *std::any_cast<compress_node *>(user_data);
        if (error) {
            req->set_error(error);
        } else {
            result->append((char const *) data, lenght);
        }
    };
    if(!zstream.process((Bytef *) at, length, &c_node, fun, true))
        return false;
    swap(result, _body_s);
    return true;
}
} // namespace bro::net::http::client::private_


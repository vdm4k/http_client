#include <string_view>
#include <string>
#include <vector>
#include <charconv>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <dns/resolver.h>
#include <uriparser/Uri.h>
#include <network/tcp/ssl/send/settings.h>
#include <protocols/ip/full_address.h>
#include <llhttp.h>
#include <network/stream/factory.h>
#include <http_client/request.h>
#include <http_client/common.h>
#include <http_client/zlib_helper.h>

namespace bro::net::http::client {

template<typename ValueType, size_t Size>
struct stack_allocator {
    /**
  * default ctor
  */

    explicit stack_allocator() noexcept = default;

    explicit stack_allocator(size_t size) : _size(size) {
        if(_size > Size)
            _begin = new ValueType[size];
    }

    ~stack_allocator() {
        if(_begin != _array)
            delete []_begin;
    }

    /**
  * default copy ctor
  */
    stack_allocator(stack_allocator const &res) = delete;
    /**
  * default move ctor
  */
    stack_allocator(stack_allocator &&res) = delete;
    /**
  * default assign operator
  */
    stack_allocator &operator=(stack_allocator const &res) = delete;
    /**
  * default assign move operator
  */
    stack_allocator &operator=(stack_allocator &&res) = delete;

    size_t get_size() const noexcept {
        return _size;
    }

    /**
   * get point on array
   *
   * @return pointer on array
   */
    ValueType * get_array() const noexcept { return _begin; }

private:
    ValueType _array[Size];     ///< default array
    ValueType *_begin = _array; ///< pointer on array
    size_t _size = Size;
};

/**
 * \brief request implementation
 */
class request_impl {
public:

    /**
  * \brief request state
  */
    enum class state {
        e_idle,  ///< not active
        e_active ///< active
    };

    /**
   * default ctor
   */
    request_impl()  : _resolver({}) {}

    /*! \brief send function
   * \param [in] tp request type
   * \param [in] url specific url
   * \param [in] res request result
   * \param [in] set settings per request (optional)
   * \result if request initialization complete successfuly
   */
    bool send(request::type tp, std::string url, request::result const &res, request::config *conf = nullptr);

    /*! \brief add headers
   * \param [in] type header type
   * \param [in] value header value
   *
   * \note optimization if we use same headers ( not generate them )
   */
    void add_header(std::string_view const &type, std::string_view const &value);

    /*! \brief add headers
   * \param [in] type header type
   * \param [in] value header value
   */
    void add_header(std::string const &type, std::string const &value);

    /*! \brief add body
   * \param [in] body body value
   * \param [in] compress_body do compress
   */
    void add_body(std::string const &body, std::string const &content_type, bool compress_body = false);

    /*! \brief add body
   * \param [in] body body value
   * \param [in] compress_body do compress
   *
   * \note optimization if we use same headers ( not generate them )
   */
    void add_body(std::string_view const &body, std::string const &content_type, bool compress_body = false);

    /*! \brief proceed request function. need only if it's in active state
   */
    void proceed();

    /*! \brief check is active
   * \result if request in active state return true
   */
    bool is_active() { return _state == state::e_active; }

private:
    /**
  * \brief connection type
  */
    enum class connection_type {
        e_http, ///< http
        e_https ///< https
    };

    /**
  * \brief create send stream base on url
  */
    bool create_stream();

    /**
  * \brief resolv host base on ursl
  */
    void resolve_host();

    /**
  * \brief generate full request
  */
    void generate_message();

    /**
  * \brief parse uri
  */
    bool parse_uri();

    /**
  * \brief init http parser
  */
    void init_parser();

    /**
  * \brief set error and cleanup all
  */
    void set_error(char const *error);

    /**
  * \brief cleanup
  */
    void cleanup();

    /**
  * \brief redict original message
  */
    void redirect();

    /**
  * \brief generate message
  */
    bool generate_from_url();

    /**
  * \brief cb for http parser call if on_message_complete event happened
  */
    static int handle_on_message_complete(llhttp_t *h);

    /**
  * \brief cb for http parser call if on_status event happened
  */
    static int on_status(llhttp_t *parser, char const *at, size_t length);

    /**
  * \brief cb for http parser call if on_body event happened
  */
    static int on_body(llhttp_t *parser, char const *at, size_t length);

    /**
  * \brief cb for http parser call if on_version event happened
  */
    static int on_version(llhttp_t *parser, char const *at, size_t length);

    /**
  * \brief cb for http parser call if on_header_field event happened
  */
    static int on_header_field(llhttp_t *parser, char const *at, size_t length);

    /**
  * \brief cb for http parser call if on_header_value event happened
  */
    static int on_header_value(llhttp_t *parser, char const *at, size_t length);

    /**
  * \brief cb _zstream
  */
    static void decoded_data(Bytef *data, size_t lenght, std::any user_data, char const *error);


    bool compress_body();

    state _state{state::e_idle};                                           ///< current state
    request::type _type{};                                                 ///< request type
    std::string _url;                                                      ///< url for request
    request::result _result;                                               ///< request result
    size_t _total_size = 0;                                                ///< total size of request message
    std::vector<std::pair<std::string_view, std::string_view>> _headers_v; ///< headers to set in request
    std::vector<std::pair<std::string, std::string>> _headers_s;           ///< headers to set in request
    std::string _body_s;                                                   ///< body to set in request
    std::string_view _body_v;                                              ///< body to set in request as string view
    bool _compress_body = false;                                           ///< compress request body
    dns::resolver _resolver;                                               ///< dns resolver for url
    connection_type _connection_type{connection_type::e_https};            ///< connection type
    bro::net::ev::factory _factory;                                        ///< send stream factory
    strm::stream_ptr _send_stream;                                         ///< connection to server
    proto::ip::full_address _server_address;                               ///< server address
    std::string_view _path;                                                ///< url path
    std::string_view _host;                                                ///< url host
    response _response;                                                    ///< response
    llhttp_t _parser;                                                      ///< http parser
    llhttp_settings_t _parser_settings{};                                  ///< settings for parser http
    zlib::stream _zstream;                                                 ///< zlib decoder
    request::config _config;                                               ///< settings per request
};

void request_impl::add_header(std::string_view const &type, std::string_view const &value) {
    _total_size += value.size() + type.size() + e_header_add_size;
    _headers_v.push_back({type, value});
}

void request_impl::add_header(std::string const &type, std::string const &value) {
    _total_size += value.size() + type.size() + e_header_add_size;
    _headers_s.push_back({type, value});
}

void request_impl::add_body(std::string const &body, std::string const &content_type, bool compress_body) {
    if(!body.empty())
        return;
    add_header(header::to_string(header::types::e_Content_Type), content_type);
    if(compress_body)
        add_header(header::to_string(header::types::e_Content_Encoding), "gzip, deflate");
    _compress_body = compress_body;
    _body_s = body;
}

void request_impl::add_body(std::string_view const &body, std::string const &content_type, bool compress_body) {
    if(!body.empty())
        return;
    add_header(header::to_string(header::types::e_Content_Type), content_type);
    if(compress_body)
        add_header(header::to_string(header::types::e_Content_Encoding), "gzip, deflate");
    _compress_body = compress_body;
    _body_v = body;
}

bool request_impl::create_stream() {
    switch (_connection_type) {
    case connection_type::e_http: {
        bro::net::tcp::send::settings send_set;
        send_set._peer_addr = _server_address;
        _send_stream = _factory.create_stream(&send_set);
        break;
    }
    case connection_type::e_https: {
        bro::net::tcp::ssl::send::settings send_set;
        send_set._peer_addr = _server_address;
        send_set._host_name = _host;
        _send_stream = _factory.create_stream(&send_set);
        break;
    }
    }
    if (!_send_stream->is_active()) {
        std::string error = fmt::sprintf("Create stream failed with error -  %s", _send_stream->get_error_description());
        set_error(error.c_str());
        return false;
    }

    _factory.bind(_send_stream);
    _send_stream->set_state_changed_cb(
        [&](strm::stream *strm, std::any) {
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
            stack_allocator<std::byte, e_max_msg_size> st_allocator;
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

void request_impl::init_parser() {
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

void request_impl::generate_message() {
    // add headers base on settings
    if (_config._support_gzip)
        add_header(header::to_string(header::types::e_Accept_Encoding), "gzip, deflate");
    if (_config._close_connection) {
        char const *close = "close";
        add_header(header::to_string(header::types::e_Connection), close);
    }

    int body_size = _body_s.empty() ? _body_v.size() : _body_s.size();
    if(body_size) {
        add_header(header::to_string(header::types::e_Content_Length), std::to_string(body_size));
    }

    // count total size
    _total_size += request::to_string(_type).size() + _path.size() + header::to_string(_config._version).size() + e_eol_size
                   + e_2_spoce_size;                                                                   // path size
    _total_size += header::to_string(header::types::e_Host).size() + _host.size() + e_header_add_size; // host size

    int total = _total_size + body_size + e_eol_size;
    // generate data
    stack_allocator<std::byte, e_max_msg_size> st_allocator(total);
    auto res = fmt::format_to_n((char *) st_allocator.get_array(),
                                total,
                                "{} {} {}\r\n",
                                request::to_string(_type),
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

void request_impl::proceed() {
    _resolver.proceed();
    _factory.proceed();
}

int request_impl::on_status(llhttp_t *parser, char const *at, size_t length) {
    request_impl *req = (request_impl *) parser->data;
    req->_response._status_code = status::to_code({at, length});
    return 0;
}

void request_impl::decoded_data(Bytef *data, size_t lenght, std::any user_data, char const *error) {
    request_impl *req = std::any_cast<request_impl *>(user_data);
    if (error) {
        req->set_error(error);
    } else {
        req->_response._body.append((char const *) data, lenght);
    }
}

int request_impl::on_body(llhttp_t *parser, char const *at, size_t length) {
    request_impl *req = (request_impl *) parser->data;
    if (req->_response._is_gzip_encoded) {
        req->_zstream.process((Bytef *) at, length, req, decoded_data);
    } else
        req->_response._body.append(at, length);
    return 0;
}

int request_impl::on_version(llhttp_t *parser, char const *at, size_t length) {
    request_impl *req = (request_impl *) parser->data;
    req->_response._version = header::to_version({at, length});
    return 0;
}

int request_impl::on_header_field(llhttp_t *parser, char const *at, size_t length) {
    request_impl *req = (request_impl *) parser->data;
    response::header_data hdr;
    hdr._type.append(at, length);
    req->_response._headers.push_back(hdr);
    return 0;
}

int request_impl::on_header_value(llhttp_t *parser, char const *at, size_t length) {
    request_impl *req = (request_impl *) parser->data;
    if (req->_response._headers.empty())
        return 0;
    auto &hdr = req->_response._headers.back();
    hdr._value.append(at, length);
    if (hdr._type == header::to_string(header::types::e_Content_Encoding))
        req->_response._is_gzip_encoded = hdr._value.find("gzip") != std::string::npos
                                          && req->_zstream.init(bro::zlib::stream::type::e_decompressor);

    return 0;
}

void request_impl::redirect() {
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
    generate_from_url();
}

int request_impl::handle_on_message_complete(llhttp_t *h) {
    if (!h->data)
        return -1;
    request_impl *req = (request_impl *) h->data;
    auto &res = req->_response;
    if (req->_config._support_redirect && status::code::e_Moved_Permanently <= res.get_status_code()
        && res.get_status_code() <= status::code::e_Permanent_Redirect) {
        req->redirect();
    } else {
        req->_result._cb(std::move(res), nullptr, req->_result._data);
        req->cleanup();
    }
    return 0;
}

/**
 * \brief wrapper for uri parser lib
 */
struct uri_parser {
    /**
   * default ctor
   */
    uri_parser() { _state.uri = &_uri; }

    ~uri_parser() { uriFreeUriMembersA(&_uri); }

    /*! \brief parse url
  * \param [in] url request type
  * \result uri parse result
  */
    int parse(std::string const &url) { return uriParseUriA(&_state, url.c_str()); }

    /*! \brief get url
  * \result uri
  */
    UriUriA const &get_uri() const { return _uri; }

    /*! \brief error to string converter
  * \result string representation of error
  */
    static char const *uri_parser_error_to_string(int error) {
        switch (error) {
        case URI_SUCCESS:
            return "Uri parse error: success";
        case URI_ERROR_SYNTAX:
            return "Uri parse error: Parsed text violates expected format";
        case URI_ERROR_NULL:
            return "Uri parse error: One of the params passed was NULL although it mustn't be";
        case URI_ERROR_MALLOC:
            return "Uri parse error: Requested memory could not be allocated";
        case URI_ERROR_OUTPUT_TOO_LARGE:
            return "Uri parse error: Some output is to large for the receiving buffer";
        case URI_ERROR_NOT_IMPLEMENTED:
            return "Uri parse error: The called function is not implemented yet";
        case URI_ERROR_RANGE_INVALID:
            return "Uri parse error: The parameters passed contained invalid ranges";
        case URI_ERROR_MEMORY_MANAGER_INCOMPLETE:
            return "Uri parse error: The UriMemoryManager passed does not implement all needed functions";
        case URI_ERROR_ADDBASE_REL_BASE:
            return "Uri parse error: Given base is not absolute";
        case URI_ERROR_REMOVEBASE_REL_BASE:
            return "Uri parse error: Given base is not absolute";
        case URI_ERROR_REMOVEBASE_REL_SOURCE:
            return "Uri parse error: Given base is not absolute";
        case URI_ERROR_MEMORY_MANAGER_FAULTY:
            return "Uri parse error: The UriMemoryManager given did not pass the test suite";
        }
        return "unknown error";
    }

private:
    UriUriA _uri{};           ///< uri
    UriParserStateA _state{}; ///< state
};

bool request_impl::parse_uri() {
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

void request_impl::set_error(char const *error) {
    _result._cb({}, error, _result._data);
    cleanup();
}

void request_impl::cleanup() {
    _type = {};
    _result = {};

    _total_size = 0;
    _headers_v.clear();
    _headers_s.clear();
    _body_s.clear();
    _body_v = {};
    _send_stream.reset();
    _state = {state::e_idle};
    _response = {};
    _zstream.cleanup();
    _config = {};
}

void request_impl::resolve_host() {
    _resolver.resolve({_host.begin(), _host.end()},
                      proto::ip::address::version::e_v4,
                      [this](bro::net::proto::ip::address const &addr, std::string const &hostname, char const *err) {
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

bool request_impl::generate_from_url() {
    if (!parse_uri())
        return false;
    if (!_host.empty())
        resolve_host();
    else
        create_stream();
    return true;
}

bool request_impl::send(request::type tp, std::string url, const request::result &res, request::config *conf) {
    if (is_active()) {
        res._cb({}, "request is in active state", res._data); // just for test
        return false;
    }
    _state = state::e_active;
    _type = tp;
    if (conf)
        _config = *conf;
    _result = std::move(res);
    _url = std::move(url);
    return compress_body() && generate_from_url();
}

struct compress_node {
    request_impl *_request;
    std::string *_result;
};

bool request_impl::compress_body() {
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

request::request() : _request(std::make_unique<request_impl>()) {}

request::request(request &&req) : _request(std::move(req._request)) {}

request& request::operator=(request &&req) {
    std::swap(_request, req._request);
    return *this;
}

request::~request() {}

bool request::send(type tp, std::string url, result const &res, config *conf) {
    return _request->send(tp, std::move(url), res, conf);
}

void request::add_header(std::string_view const &type, std::string_view const &value)  {
    _request->add_header(type, value);
}

void request::add_header(std::string const &type, std::string const &value)  {
    _request->add_header(type, value);
}

void request::add_body(std::string const &body, std::string const &content_type, bool do_compress) {
    _request->add_body(body, content_type, do_compress);
}

void request::add_body(std::string_view const &body, std::string const &content_type, bool do_compress) {
    _request->add_body(body, content_type, do_compress);
}

void request::proceed()  {
    _request->proceed();
}

bool request::is_active() {
    return _request->is_active();
}

std::string_view request::to_string(type tp) {
    switch (tp) {
    case type::e_GET:
        return "GET";
    case type::e_HEAD:
        return "HEAD";
    case type::e_POST:
        return "POST";
    case type::e_PUT:
        return "PUT";
    case type::e_DELETE:
        return "DELETE";
    case type::e_CONNECT:
        return "CONNECT";
    case type::e_OPTIONS:
        return "OPTIONS";
    case type::e_TRACE:
        return "TRACE";
    case type::e_Unknown_Type:
        return "Unknown Type";
    }
    return "";
}

request::type request::to_type(std::string_view const &tp) {
    static std::unordered_map<std::string_view, type> umap{
                                                           {to_string(type::e_GET), type::e_GET},
                                                           {to_string(type::e_HEAD), type::e_HEAD},
                                                           {to_string(type::e_POST), type::e_POST},
                                                           {to_string(type::e_PUT), type::e_PUT},
                                                           {to_string(type::e_DELETE), type::e_DELETE},
                                                           {to_string(type::e_CONNECT), type::e_CONNECT},
                                                           {to_string(type::e_OPTIONS), type::e_OPTIONS},
                                                           {to_string(type::e_TRACE), type::e_TRACE},
                                                           };
    if (auto it = umap.find(tp); it != umap.end())
        return it->second;
    return request::type::e_Unknown_Type;
}

} // namespace bro::net::http::client

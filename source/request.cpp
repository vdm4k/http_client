#include "fmt/core.h"
#include "fmt/printf.h"
#include <http_client/request.h>
#include <uriparser/Uri.h>
#include <charconv>
#include <network/tcp/ssl/send/settings.h>
#include <iostream>

namespace bro::net::http {

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
  static std::unordered_map<std::string_view, request::type> umap{
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
  return type::e_Unknown_Type;
}

enum {
  e_rn_size = 2,
  e_semicolon_space_size = 2,
  e_header_add_size = e_semicolon_space_size + e_rn_size,
  e_2_spoce_size = 2
};

request::request()
  : _resolver({}) {}

void request::add_header(std::string_view const &type, std::string_view const &value) {
  _total_size += value.size() + type.size() + e_header_add_size;
  _headers_v.push_back({type, value});
}

void request::add_header(std::string const &type, std::string const &value) {
  _total_size += value.size() + type.size() + e_header_add_size;
  _headers_s.push_back({type, value});
}

void request::add_body(std::string const &value) {
  _body_s = value;
}

void request::add_body(std::string_view const &value) {
  _body_v = value;
}

bool request::create_stream() {
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
    },
    nullptr);
  _send_stream->set_received_data_cb(
    [&](strm::stream *strm, std::any) {
      static const size_t buffer_size = 16384;
      char buffer[buffer_size] = {};
      auto res = strm->receive((std::byte *) buffer, buffer_size);
      if (res > 0) {
        auto err = llhttp_execute(&_parser, buffer, res);
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
  if (_client_setting._support_gzip)
    add_header(header::to_string(header::types::e_Accept_Encoding), "gzip, deflate");
  if (_client_setting._close_connection) {
    char const *close = "close";
    add_header(header::to_string(header::types::e_Connection), close);
  }

  // count total size
  _total_size += to_string(_type).size() + _path.size() + header::to_string(_client_setting._version).size() + e_rn_size
                 + e_2_spoce_size;                                                                   // path size
  _total_size += header::to_string(header::types::e_Host).size() + _host.size() + e_header_add_size; // host size
  int total = _total_size + e_rn_size;

  // generate data
  std::byte buffer[total];
  auto res = fmt::format_to_n((char *) buffer,
                              total,
                              "{} {} {}\r\n",
                              to_string(_type),
                              _path,
                              header::to_string(_client_setting._version));
  res.out = fmt::format_to_n(res.out, total, "{}: {}\r\n", header::to_string(header::types::e_Host), _host).out;
  for (auto const &hdr : _headers_s)
    res.out = fmt::format_to_n(res.out, total, "{}: {}\r\n", hdr.first, hdr.second).out;
  for (auto const &hdr : _headers_v)
    res.out = fmt::format_to_n(res.out, total, "{}: {}\r\n", hdr.first, hdr.second).out;
  res.out = fmt::format_to_n(res.out, total, "\r\n").out;

  _send_stream->send(buffer, total);
  init_parser();
}

void request::proceed() {
  _resolver.proceed();
  _factory.proceed();
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
    req->_decoder.process((Bytef *) at, length, req, decoded_data);
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
    req->_response._is_gzip_encoded = hdr._value.find("gzip") != std::string::npos
                                      && req->_decoder.init(bro::zlib::stream::type::e_decompressor);

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
  generate_from_url();
}

int request::handle_on_message_complete(llhttp_t *h) {
  if (!h->data)
    return -1;
  request *req = (request *) h->data;
  auto &res = req->_response;
  if (req->_client_setting._support_redirect && status::code::e_Moved_Permanently <= res.get_status_code()
      && res.get_status_code() <= status::code::e_Permanent_Redirect) {
    req->redirect();
  } else {
    req->_result._cb(std::move(res), nullptr, req->_result._data);
    req->cleanup();
  }
  return 0;
}

struct uri_parser {
  uri_parser() { _state.uri = &_uri; }

  int parse(std::string const &url) { return uriParseUriA(&_state, url.c_str()); }
  UriUriA const &get_uri() const { return _uri; }
  ~uri_parser() { uriFreeUriMembersA(&_uri); }
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
  UriUriA _uri{};
  UriParserStateA _state{};
};

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
  } else {
    _connection_type == connection_type::e_http ? _server_address.set_port(80) : _server_address.set_port(443);
  }
  return true;
}

void request::set_error(char const *error) {
  _result._cb({}, error, _result._data);
  cleanup();
}

void request::cleanup() {
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
  _decoder.cleanup();
  _client_setting = {};
}

void request::resolve_host() {
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

bool request::generate_from_url() {
  if (!parse_uri())
    return false;
  if (!_host.empty())
    resolve_host();
  else
    generate_message();
  return true;
}

bool request::send(type tp, std::string url, result const &result, settings *set) {
  if (is_active()) {
    result._cb({}, "request is in active state", result._data); // just for test
    return false;
  }
  _state = state::e_active;
  _type = tp;
  if (set)
    _client_setting = *set;
  _result = std::move(result);
  _url = std::move(url);
  return generate_from_url();
}

} // namespace bro::net::http

#include "fmt/core.h"
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

void request::add_header(header::types type, std::string const &value) {
  if (type > header::types::e_Star)
    return;
  _total_size += value.size() + header::to_string(type).size() + e_header_add_size;
  _headers.push_back({type, value});
}

std::optional<connection_type> get_connection_type(UriUriA const &uri) {
  if (!uri.scheme.first) {
    return std::nullopt;
  }

  std::string_view sheme(uri.scheme.first, std::distance(uri.scheme.first, uri.scheme.afterLast));
  if (sheme == "http") {
    return connection_type::e_http;
  } else if (sheme == "https") {
    return connection_type::e_https;
  }
  return std::nullopt;
}

std::optional<uint16_t> get_port(UriUriA const &uri, connection_type conn_type) {
  std::optional<uint16_t> port;
  if (uri.portText.first) {
    auto [ptr, ec]{std::from_chars(uri.portText.first, uri.portText.afterLast, *port)};
    if (ec != std::errc()) {
      return std::nullopt;
    }
  }

  return port ? *port : conn_type == connection_type::e_http ? 80 : 443;
}

void request::resolve_host() {}

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
    _send_stream = _factory.create_stream(&send_set);
    break;
  }
  default:
    // handle error
    return false;
  }
  if (!_send_stream->is_active()) {
    // handle error
    return false;
  }

  _factory.bind(_send_stream);
  _send_stream->set_state_changed_cb(
    [&](strm::stream *strm, std::any) {
      switch (strm->get_state()) {
      case strm::stream::state::e_established:
        send_data();
        //_ready_send_data = true;
        break;
      case strm::stream::state::e_wait:
        return;
        break;
      default:
        //        _result(std::move(_response), "brum", this); // just for test
        // handle error
        break;
      }
    },
    nullptr);
  _send_stream->set_received_data_cb(
    [&](strm::stream *strm, std::any) {
      char buffer[2000] = {};
      auto res = strm->receive((std::byte *) buffer, 2000);
      if (res > 0) {
        _data.append(buffer, res);
        enum llhttp_errno err = llhttp_execute(&parser, buffer, res);
        if (err == HPE_OK) {
          /* Successfully parsed! */
        } else {
          fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err), parser.reason);
        }
      }
    },
    nullptr);

  //  _send_stream->send((std::byte *) buffer, _total_size + 2);
  return true;
}

void request::send_data() {
  int total = _total_size + e_rn_size;
  std::byte buffer[total];
  //  memset(buffer, 0, total);

  //  GET /all/ HTTP/1.1
  //  Host: mobile-review.com
  //  Connection: keep-alive
  //  Upgrade-Insecure-Requests: 1
  //  User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36
  //  Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
  // Accept-Encoding: gzip, deflate
  // Accept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7

  //  _total_size += header::to_string(header::types::e_Host).size() + host.size() + 4;      // host size
  //  _total_size += to_string(_type).size() + path.size() + to_string(_version).size() + 4; // path size

  auto res
    = fmt::format_to_n((char *) buffer, total, "{} {} {}\r\n", to_string(_type), _path, header::to_string(_version));
  res.out = fmt::format_to_n(res.out, total, "{}: {}\r\n", header::to_string(header::types::e_Host), _host).out;
  for (auto const &hdr : _headers) {
    res.out = fmt::format_to_n(res.out, total, "{}: {}\r\n", header::to_string(hdr.first), hdr.second).out;
  }
  res.out = fmt::format_to_n(res.out, total, "\r\n").out;
  _send_stream->send(buffer, total);
}

void request::proceed() {
  _resolver.proceed();
  _factory.proceed();
}

int request::on_status(llhttp_t *parser, char const *at, size_t length) {
  response *resp = (response *) parser->data;
  resp->_status_code = status::to_code({at, length});
  return 0;
}

int request::on_body(llhttp_t *parser, char const *at, size_t length) {
  response *resp = (response *) parser->data;
  if (!resp->_body.start)
    resp->_body.start = std::distance(resp->_full_message.data(), (char *) at);
  resp->_body.size += length;
  return 0;
}

int request::on_version(llhttp_t *parser, char const *at, size_t length) {
  response *resp = (response *) parser->data;
  resp->_version = header::to_version({at, length});
  return 0;
}

int request::on_header_field(llhttp_t *parser, char const *at, size_t length) {
  response *resp = (response *) parser->data;
  response::headers hdrs;
  hdrs._type = header::to_type({at, length});
  resp->_headers.push_back(hdrs);
  return 0;
}

int request::on_header_value(llhttp_t *parser, char const *at, size_t length) {
  response *resp = (response *) parser->data;
  if (resp->_headers.empty())
    return 0;
  auto &val = resp->_headers.back();
  if (!val.start)
    val.start = std::distance(resp->_full_message.data(), (char *) at);
  val.size += length;
  return 0;
}

int request::handle_on_message_complete(llhttp_t *h) {
  if (!h->data)
    return -1;
  request *req = (request *) h->data;
  llhttp_t parser;
  llhttp_settings_t settings{};
  llhttp_settings_init(&settings);
  settings.on_status = on_status;
  settings.on_body = on_body;
  settings.on_version = on_version;
  settings.on_header_field = on_header_field;
  settings.on_header_value = on_header_value;
  llhttp_init(&parser, HTTP_BOTH, &settings);

  response resp;
  resp._full_message = std::move(req->_data);
  std::cout << resp._full_message.size() << std::endl;
  parser.data = &resp;
  enum llhttp_errno err = llhttp_execute(&parser, resp._full_message.data(), resp._full_message.size());
  if (err == HPE_OK) {
    req->_result._cb(std::move(resp), nullptr, req->_result._data); // just for test
    /* Successfully parsed! */
  } else {
    fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err), parser.reason);
  }
  return 0;
}

bool request::send() {
  /* Initialize user callbacks and settings */
  llhttp_settings_init(&settings);
  /* Set user callback */
  settings.on_message_complete = handle_on_message_complete;
  /* Initialize the parser in HTTP_BOTH mode, meaning that it will select between
 * HTTP_REQUEST and HTTP_RESPONSE parsing automatically while reading the first
 * input.
 */
  llhttp_init(&parser, HTTP_BOTH, &settings);
  parser.data = this;

  UriParserStateA state;
  UriUriA uri;
  state.uri = &uri;
  if (uriParseUriA(&state, _url.c_str()) != URI_SUCCESS) {
    return false;
  }
  if (!uri.hostText.first) {
    return false;
  }

  {
    auto conn_type = get_connection_type(uri);
    if (!conn_type)
      return false;
    _connection_type = *conn_type;
  }

  auto port = get_port(uri, _connection_type);
  if (!port)
    return false;

  add_header(header::types::e_Connection, "close"); //
  _host = std::string_view(uri.hostText.first, std::distance(uri.hostText.first, uri.hostText.afterLast));
  _path = std::string_view(uri.hostText.afterLast);
  _total_size += header::to_string(header::types::e_Host).size() + _host.size() + e_header_add_size;
  _total_size += to_string(_type).size() + _path.size() + header::to_string(_version).size() + e_rn_size
                 + e_2_spoce_size;

  //  return true;

  if (uri.hostData.ip4 || uri.hostData.ip6) {
    if (uri.hostData.ip4) {
      _server_address = {proto::ip::v4::address(uri.hostData.ip4->data), *port};
    } else {
      _server_address = {proto::ip::v6::address(uri.hostData.ip6->data), *port};
    }
    create_stream();

  } else {
    _resolver.resolve({_host.begin(), _host.end()},
                      proto::ip::address::version::e_v4,
                      [port = *port, this](bro::net::proto::ip::address const &addr,
                                           std::string const & /*hostname*/,
                                           char const *err) {
                        if (err) {
                          // handle error

                        } else {
                          _server_address = proto::ip::full_address(addr, port);
                          create_stream();
                        }
                      });
  }
  uriFreeUriMembersA(&uri);
  return true;
}

} // namespace bro::net::http

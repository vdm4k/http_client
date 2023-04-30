#pragma once
#include <http_client/header.h>
#include <http_client/status.h>
#include <http_client/response.h>
#include <llhttp.h>
#include <protocols/ip/full_address.h>
#include <string_view>
#include <string>
#include <vector>
#include <dns/resolver.h>
#include <network/stream/factory.h>
#include <span>

namespace bro::net::http {

enum class connection_type { e_http, e_https };

struct result {
  using cbt = std::function<void(response &&resp, char const *const error, std::any user_data)>;
  cbt _cb;
  std::any _data;
};

class request {
public:
  enum class type {
    e_GET, ///< The GET method is used to retrieve information from the given server using a given URI. Requests using GET should only retrieve data and should have no other effect on the data.
    e_HEAD, ///< Same as GET, but transfers the status line and header section only.
    e_POST, ///< A POST request is used to send data to the server, for example, customer information, file upload, etc. using HTML forms.
    e_PUT,  ///< Replaces all current representations of the target resource with the uploaded content.
    e_DELETE,  ///< Removes all current representations of the target resource given by a URI.
    e_CONNECT, ///< Establishes a tunnel to the server identified by a given URI.
    e_OPTIONS, ///< Describes the communication options for the target resource.
    e_TRACE,   ///< Performs a message loop-back test along the path to the target resource.
    e_Unknown_Type
  };

  request(type tp, std::string url, result const &result, header::version ver = header::version::e_1_1)
    : _type(tp)
    , _url(std::move(url))
    , _version(ver)
    , _result(result)
    , _resolver({}) {}

  static std::string_view to_string(type tp);
  static type to_type(std::string_view const &tp);

  void add_header(header::types type, std::string const &value);
  void add_header_v(header::types type, std::string_view const &value);
  void add_body(header::types type, std::string const &value);
  void add_body_v(header::types type, std::string_view const &value);
  bool send();
  void proceed();

private:
  bool create_stream();
  void resolve_host();
  void send_data();

  static int handle_on_message_complete(llhttp_t *h);
  static int on_status(llhttp_t *parser, char const *at, size_t length);
  static int on_body(llhttp_t *parser, char const *at, size_t length);
  static int on_version(llhttp_t *parser, char const *at, size_t length);
  static int on_header_field(llhttp_t *parser, char const *at, size_t length);
  static int on_header_value(llhttp_t *parser, char const *at, size_t length);

  type _type{};
  std::string _url;
  header::version _version{header::version::e_1_1};
  result _result;

  size_t _total_size = 0;
  std::vector<std::pair<header::types, std::string>> _headers;
  std::string _body;
  std::string_view _body_ex;
  dns::resolver _resolver;
  bro::net::ev::factory _factory;
  strm::stream_ptr _send_stream;
  proto::ip::full_address _server_address;
  connection_type _connection_type{connection_type::e_https};
  std::string_view _path;
  std::string_view _host;
  std::string _response_str;
  bool _ready_send_data = false;

  llhttp_t parser;
  llhttp_settings_t settings{};
  std::string _data;
};

} // namespace bro::net::http

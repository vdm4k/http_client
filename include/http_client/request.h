#pragma once
#include <string_view>
#include <string>
#include <vector>
#include <llhttp.h>
#include <protocols/ip/full_address.h>
#include <dns/resolver.h>
#include <network/stream/factory.h>
#include <http_client/header.h>
#include <http_client/status.h>
#include <http_client/response.h>
#include <http_client/zlib_helper.h>

namespace bro::net::http {

enum class connection_type { e_http, e_https };

struct result {
  using cbt = std::function<void(response &&resp, char const *const error, std::any user_data)>;
  cbt _cb;
  std::any _data;
};

class request {
public:
  enum class state { e_idle, e_active };

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
  static std::string_view to_string(type tp);
  static type to_type(std::string_view const &tp);

  request();
  bool send(type tp, std::string url, result const &result, header::version ver = header::version::e_1_1);
  void add_header(std::string_view const &type, std::string_view const &value);
  void add_header(std::string const &type, std::string const &value);
  void add_body(std::string const &value);
  void add_body(std::string_view const &value);
  void proceed();
  bool is_active() { return _state == state::e_active; }

private:
  bool create_stream();
  void resolve_host();
  void generate_message();
  bool parse_uri();
  void init_parser();
  void set_error(char const *error);
  void cleanup();

  static int handle_on_message_complete(llhttp_t *h);
  static int on_status(llhttp_t *parser, char const *at, size_t length);
  static int on_body(llhttp_t *parser, char const *at, size_t length);
  static int on_version(llhttp_t *parser, char const *at, size_t length);
  static int on_header_field(llhttp_t *parser, char const *at, size_t length);
  static int on_header_value(llhttp_t *parser, char const *at, size_t length);
  static void decoded_data(Bytef *data, size_t lenght, std::any user_data, char const *error);

  type _type{};
  std::string _url;
  header::version _version{header::version::e_1_1};
  result _result;

  size_t _total_size = 0;
  std::vector<std::pair<std::string_view, std::string_view>> _headers_v;
  std::vector<std::pair<std::string, std::string>> _headers_s;
  std::string _body_s;
  std::string_view _body_v;
  dns::resolver _resolver;
  bro::net::ev::factory _factory;
  strm::stream_ptr _send_stream;
  proto::ip::full_address _server_address;
  connection_type _connection_type{connection_type::e_https};
  std::string_view _path;
  std::string_view _host;

  state _state{state::e_idle};
  response _resp;
  llhttp_t _parser;
  llhttp_settings_t _settings{};
  zlib::stream _decoder;
};

} // namespace bro::net::http

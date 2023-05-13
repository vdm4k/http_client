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
#include <http_client/version.h>
#include <http_client/response.h>
#include <http_client/zlib_helper.h>

namespace bro::net::http {

/** @defgroup http_client_request http_client_request
 *  @{
 */

/**
 * \brief header types
 */
class request {
public:
  /**
  * \brief header types
  */
  struct result {
    /**
   *  \brief A type alias for a callback function that returns a result or error
   *
   * This function will be called with filled resp or filled error.
   * Pointers no error is a pointer on static buffer - no need to call free
   */
    using cbt = std::function<void(response &&resp, char const *const error, std::any user_data)>;
    cbt _cb;        ///< result callback
    std::any _data; ///< user data
  };

  /**
  * \brief specific settings per request
  */
  struct settings {
    header::version _version{header::version::e_1_1}; ///< http version
    bool _support_redirect{true};                     ///< redirect support
    bool _close_connection{true};                     ///< set close connection with server after receive response
    bool _support_gzip{true};                         ///< add header in request - support commpress data (gzip)
  };

  /**
  * \brief request state
  */
  enum class state {
    e_idle,  ///< not active
    e_active ///< active
  };

  /**
  * \brief request type
  */
  enum class type {
    e_GET, ///< The GET method is used to retrieve information from the given server using a given URI. Requests using GET should only retrieve data and should have no other effect on the data.
    e_HEAD, ///< Same as GET, but transfers the status line and header section only.
    e_POST, ///< A POST request is used to send data to the server, for example, customer information, file upload, etc. using HTML forms.
    e_PUT,  ///< Replaces all current representations of the target resource with the uploaded content.
    e_DELETE,      ///< Removes all current representations of the target resource given by a URI.
    e_CONNECT,     ///< Establishes a tunnel to the server identified by a given URI.
    e_OPTIONS,     ///< Describes the communication options for the target resource.
    e_TRACE,       ///< Performs a message loop-back test along the path to the target resource.
    e_Unknown_Type ///< not set or unknown type (internal use only)
  };

  /*! \brief convert request type to string representation
  * \param [in] tp request type
  * \result string representation
  */
  static std::string_view to_string(type tp);

  /*! \brief convert request type as string to type
  * \param [in] tp request type as string
  * \result right enum type or e_Unknown_Type
  */
  static type to_type(std::string_view const &tp);

  /**
   * default ctor
   */
  request();

  /*! \brief send function
   * \param [in] tp request type
   * \param [in] url specific url
   * \param [in] res request result
   * \param [in] set settings per request (optional)
   * \result if request initialization complete successfuly
   */
  bool send(type tp, std::string url, result const &res, settings *set = nullptr);

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
   */
  void add_body(std::string const &body);

  /*! \brief add body
   * \param [in] body body value
   *
   * \note optimization if we use same headers ( not generate them )
   */
  void add_body(std::string_view const &body);

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

  state _state{state::e_idle};                                           ///< current state
  type _type{};                                                          ///< request type
  std::string _url;                                                      ///< url for request
  result _result;                                                        ///< request result
  size_t _total_size = 0;                                                ///< total size of request message
  std::vector<std::pair<std::string_view, std::string_view>> _headers_v; ///< headers to set in request
  std::vector<std::pair<std::string, std::string>> _headers_s;           ///< headers to set in request
  std::string _body_s;                                                   ///< body to set in request
  std::string_view _body_v;                                              ///< body to set in request as string view
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
  settings _settings;                                                    ///< settings per request
};

} // namespace bro::net::http

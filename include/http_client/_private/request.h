#include <llhttp.h>
#include <dns/resolver.h>
#include <uriparser/Uri.h>
#include <protocols/ip/full_address.h>
#include <network/stream/factory.h>
#include <http_client/request.h>
#include <http_client/common.h>
#include <http_client/zlib_helper.h>

namespace bro::net::http::client::_private {

/**
 * \brief request implementation
 */
class request {
public:

    /**
  * \brief request state
  */
    enum class state : uint8_t {
        e_idle,  ///< not active
        e_active ///< active
    };

    /**
   * default ctor
   */
    request(std::shared_ptr<bro::strm::factory> const & factory, std::shared_ptr<dns::resolver> const & resolver)  :
        _factory(factory),
        _resolver(resolver)
    {}

   /*! \brief send function
   * \param [in] tp request type
   * \param [in] url specific url
   * \param [in] res request result
   * \param [in] set settings per request (optional)
   * \result if request initialization complete successfuly
   */
    bool send(client::request::type tp, std::string const &url, client::request::result const &res, client::request::config *conf = nullptr);

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

    /*! \brief proceed events for request(receive new data, send data). need only if it's in active state
   */
    void proceed_events();

    /*! \brief proceed logic of request function(lazy cleanup). need only if it's in active state
   */
    void proceed_logic();

    /*! \brief check is active
   * \result if request in active state return true
   */
    bool is_active() { return _state == state::e_active; }

private:

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
  * \brief cb for zlib stream
  */
    static void decoded_data(Bytef *data, size_t lenght, std::any user_data, char const *error);

    /**
  * \brief compress send body
  */
    bool compress_body();

    state _state{state::e_idle};                                           ///< current state
    connection_type _connection_type{connection_type::e_https};            ///< connection type
    client::request::type _type{};                                         ///< request type
    bool _do_cleanup = false;                                              ///< lazy cleanup to prevent erase while using something
    bool _do_reset_send_stream = false;                                    ///< lazy erase stream to prevent erase while using something
    bool _compress_body = false;                                           ///< compress request body
    size_t _total_size = 0;                                                ///< total size of request message
    std::shared_ptr<bro::strm::factory> _factory;                          ///< send stream factory
    std::shared_ptr<dns::resolver> _resolver;                              ///< dns resolver for url
    client::request::config _config;                                       ///< settings per request
    strm::stream_ptr _send_stream;                                         ///< connection to server
    std::string_view _body_v;                                              ///< body to set in request as string view
    std::string_view _path;                                                ///< url path
    std::string_view _host;                                                ///< url host
    std::vector<std::pair<std::string_view, std::string_view>> _headers_v; ///< headers to set in request
    std::vector<std::pair<std::string, std::string>> _headers_s;           ///< headers to set in request
    std::string _url;                                                      ///< url for request
    std::string _body_s;                                                   ///< body to set in request
    proto::ip::full_address _server_address;                               ///< server address
    client::request::result _result;                                       ///< request result
    response _response;                                                    ///< response
    llhttp_t _parser;                                                      ///< http parser
    zlib::stream _zstream;                                                 ///< zlib decoder
    llhttp_settings_t _parser_settings{};                                  ///< settings for parser http
};

} // namespace bro::net::http::client::_private

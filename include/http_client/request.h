#pragma once
#include <any>
#include <functional>
#include <memory>
#include <stream/factory.h>
#include <dns/resolver.h>
#include <http_client/common.h>
#include <http_client/version.h>
#include <http_client/response.h>

namespace bro::net::http::client {

/** @defgroup http_client_request http_client_request
 *  @{
 */

namespace _private {
class request;
} // namespace _private
/**
 * \brief header types
 */
class request {
public:

    /**
  * \brief specific configuration per request
  */
    struct config {
        header::version _version{header::version::e_1_1}; ///< http version
        bool _support_redirect{true};                     ///< redirect support
        bool _close_connection{true};                     ///< set close connection with server after receive response
        bool _support_gzip{true};                         ///< add header in request - support commpress data (gzip)
        bool _try_decompress_response{true};              ///< if server compress body with gzip we will decompress it. If have problems with decompress -> may disable it
        size_t *_processed_events{nullptr};               ///< if set will increment it if receive some data
    };

    /**
  * \brief request type
  */
    enum class type : uint8_t {
        e_GET,          ///< The GET method is used to retrieve information from the given server using a given URI. Requests using GET should only retrieve data and should have no other effect on the data.
        e_HEAD,         ///< Same as GET, but transfers the status line and header section only.
        e_POST,         ///< A POST request is used to send data to the server, for example, customer information, file upload, etc. using HTML forms.
        e_PUT,          ///< Replaces all current representations of the target resource with the uploaded content.
        e_DELETE,       ///< Removes all current representations of the target resource given by a URI.
        e_CONNECT,      ///< Establishes a tunnel to the server identified by a given URI.
        e_OPTIONS,      ///< Describes the communication options for the target resource.
        e_TRACE,        ///< Performs a message loop-back test along the path to the target resource.
        e_Unknown_Type  ///< not set or unknown type (internal use only)
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
  * \brief result type
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
   * default ctor
   */
    request();

    /**
   * ctor with external factory and external dns resolver
   */
    request(std::shared_ptr<bro::strm::factory> const &factory, std::shared_ptr<dns::resolver> const & resolver);

    /**
   * move ctor
   */
    request(request &&req);

    /**
   * move assign operator
   */
    request& operator=(request &&req);

    /**
   * deleted assign constructor
   */
    request(request const &) = delete;

    /**
   * deleted assign operator
   */
    request& operator=(request const &) = delete;

    /**
   * default dtor
   */
    ~request();

    /*! \brief send function
   * \param [in] tp request type
   * \param [in] url specific url
   * \param [in] res request result
   * \param [in] conf configuration per request (optional)
   * \result if request initialization complete successfuly
   */
    bool send(type tp, std::string const &url, result &&res, config *conf = nullptr);

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
   * \param [in] content_type type of body content
   * \param [in] do_compress do compress
   */
    void add_body(std::string const &body, std::string const &content_type, bool do_compress = false);

    /*! \brief add body
   * \param [in] body body value
   * \param [in] content_type type of body content
   * \param [in] do_compress do compress
   *
   * \note optimization if we use same headers ( not generate them )
   */
    void add_body(std::string_view const &body, std::string const &content_type, bool do_compress = false);

    /*! \brief proceed request function. need only if it's in active state     
   */
    void proceed();

   /*! \brief get current state
   */
    request_state get_state() const;

  /*! \brief reset internal data. do not modify connection state
   */
    void soft_reset();

   /*! \brief reset internal data. do not modify connection state
   */
    void reset();

private:
    std::unique_ptr<_private::request> _request;   ///< pimpl here
};

} // namespace bro::net::http::client

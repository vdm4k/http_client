#pragma once
#include <any>
#include <functional>
#include <memory>
#include "http_client/version.h"
#include "http_client/response.h"

namespace bro::net::http::client {

/** @defgroup http_client_request http_client_request
 *  @{
 */

class request_impl;

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
    };

    /**
  * \brief request type
  */
    enum class type {
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

    request(request &&req);

    request& operator=(request &&req);

    request(request const &) = delete;

    request& operator=(request const &) = delete;

    /**
   * default dtor
   */
    ~request();

    /*! \brief send function
   * \param [in] tp request type
   * \param [in] url specific url
   * \param [in] res request result
   * \param [in] set settings per request (optional)
   * \result if request initialization complete successfuly
   */
    bool send(type tp, std::string url, result const &res, config *conf = nullptr);

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
   * \param [in] do_compress do compress
   */
    void add_body(std::string const &body, std::string const &content_type, bool do_compress = false);

    /*! \brief add body
   * \param [in] body body value
   * \param [in] do_compress do compress
   *
   * \note optimization if we use same headers ( not generate them )
   */
    void add_body(std::string_view const &body, std::string const &content_type, bool do_compress = false);

    /*! \brief proceed request function. need only if it's in active state
   */
    void proceed();

    /*! \brief check is active
   * \result if request in active state return true
   */
    bool is_active();

private:
    std::unique_ptr<request_impl> _request;
};

} // namespace bro::net::http::client

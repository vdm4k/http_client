#pragma once
#include <string>
#include <vector>
#include <http_client/header.h>
#include <http_client/status.h>
#include <http_client/version.h>

namespace bro::net::http::client {
/** @addtogroup http_client_request
 *  @{
 */

namespace _private {
class request;
} // namespace _private

/*!\ response from server
 */
class response {
public:
    /**
 * \brief header description
 */
    struct header_data {
        std::string _type;  ///<  header type
        std::string _value; ///< header value
    };

    /*! \brief get status code
  * \result status code
  */
    status::code get_status_code() const;

    /*! \brief get verson of http
  * \result version
  */
    header::version get_version() const;

    /*! \brief get body
  * \result body
  */
    std::string_view get_body() const;

    /*! \brief get headers
  * \result reference on headers
  */
    std::vector<header_data> const &get_headers() const;

private:
    friend class _private::request;

    bool _is_gzip_encoded{false};                                 ///< is commpressed by gzip
    status::code _status_code{status::code::e_Unknown_Code};      ///< status code
    header::version _version{header::version::e_Unknown_Version}; ///< http version
    std::vector<header_data> _headers;                            ///< array of headers
    std::string _body;                                            ///< body ( uncompressed if gzip )
};
} // namespace bro::net::http::client

#pragma once
#include <string_view>

namespace bro::net::http::header {
/** @defgroup http_version http_version
 *  @{
 */

/**
 * \brief http version
 */
enum class version {
    e_1_0,            ///< http version 1.0
    e_1_1,            ///< http version 1.1
    e_Unknown_Version ///< http version not set or unknown
};

/*! \brief convert http version to string representation
  * \param [in] ver http version
  * \result string representation
  */
std::string_view to_string(version ver);

/*! \brief convert http version as string to enum
  * \param [in] tp type as string
  * \result right enum type or e_Unknown_Header_Type
  */
version to_version(std::string_view const &tp);

}; // namespace bro::net::http::header

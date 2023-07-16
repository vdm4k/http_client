#pragma once
#include <string_view>

namespace bro::net::http::status {
/** @defgroup http_status http_status
 *  @{
 */

/**
 * \brief all response codes
 * \note see here https://www.iana.org/assignments/http-status-codes/http-status-codes.xhtml
 */
enum class code {
    // Informational codes
    e_Continue = 100,            ///< Continue
    e_Switching_Protocols = 101, ///< Switching_Protocols
    e_Processing = 102,          ///< Processing
    e_Early_Hints = 103,         ///< Early_Hints
    // Unassigned = 104-199,

    // Success codes
    e_OK = 200,                            ///< OK
    e_Created = 201,                       ///< Created
    e_Accepted = 202,                      ///< Accepted
    e_Non_Authoritative_Information = 203, ///< Non_Authoritative_Information
    e_No_Content = 204,                    ///< No_Content
    e_Reset_Content = 205,                 ///< Reset_Content
    e_Partial_Content = 206,               ///< Partial_Content
    e_Multi_Status = 207,                  ///< Multi_Status
    e_Already_Reported = 208,              ///< Already_Reported
    // Unassigned = 209-225,
    e_IM_Used = 226, ///< IM_Used
    // Unassigned = 227-299,

    // Redirection codes
    e_Multiple_Choices = 300,  ///< Multiple_Choices
    e_Moved_Permanently = 301, ///< Moved_Permanently
    e_Found = 302,             ///< Found
    e_See_Other = 303,         ///< See_Other
    e_Not_Modified = 304,      ///< Not_Modified
    e_Use_Proxy = 305,         ///< Use_Proxy
    // Unused = 306,
    e_Temporary_Redirect = 307, ///< Temporary_Redirect
    e_Permanent_Redirect = 308, ///< Permanent_Redirect
    // Unassigned = 309-399,

    //Client Errors
    e_Bad_Request = 400,                   ///< Bad_Request
    e_Unauthorized = 401,                  ///< Unauthorized
    e_Payment_Required = 402,              ///< Payment_Required
    e_Forbidden = 403,                     ///< Forbidden
    e_Not_Found = 404,                     ///< Not_Found
    e_Method_Not_Allowed = 405,            ///< Method_Not_Allowed
    e_Not_Acceptable = 406,                ///< Not_Acceptable
    e_Proxy_Authentication_Required = 407, ///< Proxy_Authentication_Required
    e_Request_Timeout = 408,               ///< Request_Timeout
    e_Conflict = 409,                      ///< Conflict
    e_Gone = 410,                          ///< Gone =
    e_Length_Required = 411,               ///< Length_Required
    e_Precondition_Failed = 412,           ///< Precondition_Failed
    e_Content_Too_Large = 413,             ///< Content_Too_Large
    e_URI_Too_Long = 414,                  ///< URI_Too_Long
    e_Unsupported_Media_Type = 415,        ///< Unsupported_Media_Type
    e_Range_Not_Satisfiable = 416,         ///< Range_Not_Satisfiable
    e_Expectation_Failed = 417,            ///< Expectation_Failed
    // Unused = 418,
    // Unassigned = 419 - 420,
    e_Misdirected_Request = 421,   ///< Misdirected_Request
    e_Unprocessable_Content = 422, ///< Unprocessable_Content
    e_Locked = 423,                ///< Locked
    e_Failed_Dependency = 424,     ///< Failed_Dependency
    e_Too_Early = 425,             ///< Too_Early
    e_Upgrade_Required = 426,      ///< Upgrade_Required
    // Unassigned = 427,
    e_Precondition_Required = 428, ///< Precondition_Required
    e_Too_Many_Requests = 429,     ///< Too_Many_Requests
    // Unassigned = 430,
    e_Request_Header_Fields_Too_Large = 431, ///< Request_Header_Fields_Too_Large
    // Unassigned = 432-450,
    e_Unavailable_For_Legal_Reasons = 451, ///< Unavailable_For_Legal_Reasons
    // Unassigned = 452-499,

    // Server Errors
    e_Internal_Server_Error = 500,      ///< Internal_Server_Error
    e_Not_Implemented = 501,            ///< Not_Implemented
    e_Bad_Gateway = 502,                ///< Bad_Gateway
    e_Service_Unavailable = 503,        ///< Service_Unavailable
    e_Gateway_Timeout = 504,            ///< Gateway_Timeout
    e_HTTP_Version_Not_Supported = 505, ///< HTTP_Version_Not_Supported
    e_Variant_Also_Negotiates = 506,    ///< Variant_Also_Negotiates
    e_Insufficient_Storage = 507,       ///< Insufficient_Storage
    e_Loop_Detected = 508,              ///< Loop_Detected
    // Unassigned = 509,
    // Not_Extended_(OBSOLETED) = 510,
    e_Network_Authentication_Required = 511, ///< Network_Authentication_Required
    // Unassigned = 512-599,
    e_Unknown_Code = 1000 ///< Unknown_Code (internal use only)
};

/*! \brief response code to string as number representation (e_OK -> "200")
   * \param [in] cd response code
   * \result string representation of specific code
   */
std::string_view to_string_as_number(code cd);

/*! \brief response code to string representation
   * \param [in] cd response code
   * \result string representation
   */
std::string_view to_string(code cd);

/*! \brief convert response code as string to code enum
   * \param [in] type response code as string
   * \result right enum type or e_Unknown_Code
   */
code to_code(std::string_view type);

} // namespace bro::net::http::status

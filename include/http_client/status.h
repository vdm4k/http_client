#pragma once
#include <string_view>

namespace bro::net::http::status {
// https://www.iana.org/assignments/http-status-codes/http-status-codes.xhtml
enum class code {
  // Informational codes
  e_Continue = 100,
  e_Switching_Protocols = 101,
  e_Processing = 102,
  e_Early_Hints = 103,
  // Unassigned = 104-199,

  // Success codes
  e_OK = 200,
  e_Created = 201,
  e_Accepted = 202,
  e_Non_Authoritative_Information = 203,
  e_No_Content = 204,
  e_Reset_Content = 205,
  e_Partial_Content = 206,
  e_Multi_Status = 207,
  e_Already_Reported = 208,
  // Unassigned = 209-225,
  e_IM_Used = 226,
  // Unassigned = 227-299,

  // Redirection codes
  e_Multiple_Choices = 300,
  e_Moved_Permanently = 301,
  e_Found = 302,
  e_See_Other = 303,
  e_Not_Modified = 304,
  e_Use_Proxy = 305,
  // Unused = 306,
  e_Temporary_Redirect = 307,
  e_Permanent_Redirect = 308,
  // Unassigned = 309-399,

  //Client Error
  e_Bad_Request = 400,
  e_Unauthorized = 401,
  e_Payment_Required = 402,
  e_Forbidden = 403,
  e_Not_Found = 404,
  e_Method_Not_Allowed = 405,
  e_Not_Acceptable = 406,
  e_Proxy_Authentication_Required = 407,
  e_Request_Timeout = 408,
  e_Conflict = 409,
  e_Gone = 410,
  e_Length_Required = 411,
  e_Precondition_Failed = 412,
  e_Content_Too_Large = 413,
  e_URI_Too_Long = 414,
  e_Unsupported_Media_Type = 415,
  e_Range_Not_Satisfiable = 416,
  e_Expectation_Failed = 417,
  // Unused = 418,
  // Unassigned = 419 - 420,
  e_Misdirected_Request = 421,
  e_Unprocessable_Content = 422,
  e_Locked = 423,
  e_Failed_Dependency = 424,
  e_Too_Early = 425,
  e_Upgrade_Required = 426,
  // Unassigned = 427,
  e_Precondition_Required = 428,
  e_Too_Many_Requests = 429,
  // Unassigned = 430,
  e_Request_Header_Fields_Too_Large = 431,
  // Unassigned = 432-450,
  e_Unavailable_For_Legal_Reasons = 451,
  // Unassigned = 452-499,

  // Server Error
  e_Internal_Server_Error = 500,
  e_Not_Implemented = 501,
  e_Bad_Gateway = 502,
  e_Service_Unavailable = 503,
  e_Gateway_Timeout = 504,
  e_HTTP_Version_Not_Supported = 505,
  e_Variant_Also_Negotiates = 506,
  e_Insufficient_Storage = 507,
  e_Loop_Detected = 508,
  // Unassigned = 509,
  // Not_Extended_(OBSOLETED) = 510,
  e_Network_Authentication_Required = 511,
  // Unassigned = 512-599,
  e_Unknown_Code = 1000
};

std::string_view to_string(code type);
code to_code(std::string_view type);

} // namespace bro::net::http::status

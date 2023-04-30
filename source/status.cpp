#include <http_client/status.h>
#include <unordered_map>

namespace bro::net::http::status {

std::string_view to_string(code c) {
  switch (c) {
  case code::e_Continue:
    return "Continue";
  case code::e_Switching_Protocols:
    return "Switching Protocols";
  case code::e_Processing:
    return "Processing";
  case code::e_Early_Hints:
    return "Early Hints";
  case code::e_OK:
    return "OK";
  case code::e_Created:
    return "Created";
  case code::e_Accepted:
    return "Accepted";
  case code::e_Non_Authoritative_Information:
    return "Non-Authoritative Information";
  case code::e_No_Content:
    return "No Content";
  case code::e_Reset_Content:
    return "Reset Content";
  case code::e_Partial_Content:
    return "Partial Content";
  case code::e_Multi_Status:
    return "Multi-Status";
  case code::e_Already_Reported:
    return "Already Reported";
  case code::e_IM_Used:
    return "IM Used";
  case code::e_Multiple_Choices:
    return "Multiple Choices";
  case code::e_Moved_Permanently:
    return "Moved Permanently";
  case code::e_Found:
    return "Found";
  case code::e_See_Other:
    return "See Other";
  case code::e_Not_Modified:
    return "Not Modified";
  case code::e_Use_Proxy:
    return "Use Proxy";
  case code::e_Temporary_Redirect:
    return "Temporary Redirect";
  case code::e_Permanent_Redirect:
    return "Permanent Redirect";
  case code::e_Bad_Request:
    return "Bad Request";
  case code::e_Unauthorized:
    return "Unauthorized";
  case code::e_Payment_Required:
    return "Payment Required";
  case code::e_Forbidden:
    return "Forbidden";
  case code::e_Not_Found:
    return "Not Found";
  case code::e_Method_Not_Allowed:
    return "Method Not Allowed";
  case code::e_Not_Acceptable:
    return "Not Acceptable";
  case code::e_Proxy_Authentication_Required:
    return "Proxy Authentication Required";
  case code::e_Request_Timeout:
    return "Request Timeout";
  case code::e_Conflict:
    return "Conflict";
  case code::e_Gone:
    return "Gone";
  case code::e_Length_Required:
    return "Length Required";
  case code::e_Precondition_Failed:
    return "Precondition Failed";
  case code::e_Content_Too_Large:
    return "Content Too Large";
  case code::e_URI_Too_Long:
    return "URI Too Long";
  case code::e_Unsupported_Media_Type:
    return "Unsupported Media Type";
  case code::e_Range_Not_Satisfiable:
    return "Range Not Satisfiable";
  case code::e_Expectation_Failed:
    return "Expectation Failed";
  case code::e_Misdirected_Request:
    return "Misdirected Request";
  case code::e_Unprocessable_Content:
    return "Unprocessable Content";
  case code::e_Locked:
    return "Locked";
  case code::e_Failed_Dependency:
    return "Failed Dependency";
  case code::e_Too_Early:
    return "Too Early";
  case code::e_Upgrade_Required:
    return "Upgrade Required";
  case code::e_Precondition_Required:
    return "Precondition Required";
  case code::e_Too_Many_Requests:
    return "Too Many Requests";
  case code::e_Request_Header_Fields_Too_Large:
    return "Request Header Fields Too Large";
  case code::e_Unavailable_For_Legal_Reasons:
    return "Unavailable For Legal Reasons";
  case code::e_Internal_Server_Error:
    return "Internal Server Error";
  case code::e_Not_Implemented:
    return "Not Implemented";
  case code::e_Bad_Gateway:
    return "Bad Gateway";
  case code::e_Service_Unavailable:
    return "Service Unavailable";
  case code::e_Gateway_Timeout:
    return "Gateway Timeout";
  case code::e_HTTP_Version_Not_Supported:
    return "HTTP Version Not Supported";
  case code::e_Variant_Also_Negotiates:
    return "Variant Also Negotiates";
  case code::e_Insufficient_Storage:
    return "Insufficient Storage";
  case code::e_Loop_Detected:
    return "Loop Detected";
  case code::e_Network_Authentication_Required:
    return "Network Authentication Required";
  case code::e_Unknown_Code:
    return "Unknown Status Code";
  }
  return "Unknown Status Code";
}

code to_code(std::string_view type) {
  static std::unordered_map<std::string_view, code>
    s_umap{{to_string(code::e_Continue), code::e_Continue},
           {to_string(code::e_Switching_Protocols), code::e_Switching_Protocols},
           {to_string(code::e_Processing), code::e_Processing},
           {to_string(code::e_Early_Hints), code::e_Early_Hints},
           {to_string(code::e_OK), code::e_OK},
           {to_string(code::e_Created), code::e_Created},
           {to_string(code::e_Accepted), code::e_Accepted},
           {to_string(code::e_Non_Authoritative_Information), code::e_Non_Authoritative_Information},
           {to_string(code::e_No_Content), code::e_No_Content},
           {to_string(code::e_Reset_Content), code::e_Reset_Content},
           {to_string(code::e_Partial_Content), code::e_Partial_Content},
           {to_string(code::e_Multi_Status), code::e_Multi_Status},
           {to_string(code::e_Already_Reported), code::e_Already_Reported},
           {to_string(code::e_IM_Used), code::e_IM_Used},
           {to_string(code::e_Multiple_Choices), code::e_Multiple_Choices},
           {to_string(code::e_Moved_Permanently), code::e_Moved_Permanently},
           {to_string(code::e_Found), code::e_Found},
           {to_string(code::e_See_Other), code::e_See_Other},
           {to_string(code::e_Not_Modified), code::e_Not_Modified},
           {to_string(code::e_Use_Proxy), code::e_Use_Proxy},
           {to_string(code::e_Temporary_Redirect), code::e_Temporary_Redirect},
           {to_string(code::e_Permanent_Redirect), code::e_Permanent_Redirect},
           {to_string(code::e_Bad_Request), code::e_Bad_Request},
           {to_string(code::e_Unauthorized), code::e_Unauthorized},
           {to_string(code::e_Payment_Required), code::e_Payment_Required},
           {to_string(code::e_Forbidden), code::e_Forbidden},
           {to_string(code::e_Not_Found), code::e_Not_Found},
           {to_string(code::e_Method_Not_Allowed), code::e_Method_Not_Allowed},
           {to_string(code::e_Not_Acceptable), code::e_Not_Acceptable},
           {to_string(code::e_Proxy_Authentication_Required), code::e_Proxy_Authentication_Required},
           {to_string(code::e_Request_Timeout), code::e_Request_Timeout},
           {to_string(code::e_Conflict), code::e_Conflict},
           {to_string(code::e_Gone), code::e_Gone},
           {to_string(code::e_Length_Required), code::e_Length_Required},
           {to_string(code::e_Precondition_Failed), code::e_Precondition_Failed},
           {to_string(code::e_Content_Too_Large), code::e_Content_Too_Large},
           {to_string(code::e_URI_Too_Long), code::e_URI_Too_Long},
           {to_string(code::e_Unsupported_Media_Type), code::e_Unsupported_Media_Type},
           {to_string(code::e_Range_Not_Satisfiable), code::e_Range_Not_Satisfiable},
           {to_string(code::e_Expectation_Failed), code::e_Expectation_Failed},
           {to_string(code::e_Misdirected_Request), code::e_Misdirected_Request},
           {to_string(code::e_Unprocessable_Content), code::e_Unprocessable_Content},
           {to_string(code::e_Locked), code::e_Locked},
           {to_string(code::e_Failed_Dependency), code::e_Failed_Dependency},
           {to_string(code::e_Too_Early), code::e_Too_Early},
           {to_string(code::e_Upgrade_Required), code::e_Upgrade_Required},
           {to_string(code::e_Precondition_Required), code::e_Precondition_Required},
           {to_string(code::e_Too_Many_Requests), code::e_Too_Many_Requests},
           {to_string(code::e_Request_Header_Fields_Too_Large), code::e_Request_Header_Fields_Too_Large},
           {to_string(code::e_Unavailable_For_Legal_Reasons), code::e_Unavailable_For_Legal_Reasons},
           {to_string(code::e_Internal_Server_Error), code::e_Internal_Server_Error},
           {to_string(code::e_Not_Implemented), code::e_Not_Implemented},
           {to_string(code::e_Bad_Gateway), code::e_Bad_Gateway},
           {to_string(code::e_Service_Unavailable), code::e_Service_Unavailable},
           {to_string(code::e_Gateway_Timeout), code::e_Gateway_Timeout},
           {to_string(code::e_HTTP_Version_Not_Supported), code::e_HTTP_Version_Not_Supported},
           {to_string(code::e_Variant_Also_Negotiates), code::e_Variant_Also_Negotiates},
           {to_string(code::e_Insufficient_Storage), code::e_Insufficient_Storage},
           {to_string(code::e_Loop_Detected), code::e_Loop_Detected},
           {to_string(code::e_Network_Authentication_Required), code::e_Network_Authentication_Required}};
  if (auto it = s_umap.find(type); it != s_umap.end())
    return it->second;
  return code::e_Unknown_Code;
}

} // namespace bro::net::http::status

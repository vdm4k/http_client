#pragma once
#include "protocols/ip/full_address.h"
#include <string_view>
#include <string>
#include <vector>
#include <dns/resolver.h>
#include <network/stream/factory.h>

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

};

std::string_view to_str_view(code type);

} // namespace bro::net::http::status

namespace bro::net::http::header {

enum class types {
  e_A_IM,
  e_Accept,
  e_Accept_Additions,
  e_Accept_CH,
  e_Accept_Charset,
  e_Accept_Datetime,
  e_Accept_Encoding,
  e_Accept_Features,
  e_Accept_Language,
  e_Accept_Patch,
  e_Accept_Post,
  e_Accept_Ranges,
  e_Access_Control,
  e_Access_Control_Allow_Credentials,
  e_Access_Control_Allow_Headers,
  e_Access_Control_Allow_Methods,
  e_Access_Control_Allow_Origin,
  e_Access_Control_Expose_Headers,
  e_Access_Control_Max_Age,
  e_Access_Control_Request_Headers,
  e_Access_Control_Request_Method,
  e_Age,
  e_Allow,
  e_ALPN,
  e_Alt_Svc,
  e_Alt_Used,
  e_Alternates,
  e_AMP_Cache_Transform,
  e_Apply_To_Redirect_Ref,
  e_Authentication_Control,
  e_Authentication_Info,
  e_Authorization,
  e_C_Ext,
  e_C_Man,
  e_C_Opt,
  e_C_PEP,
  e_C_PEP_Info,
  e_Cache_Control,
  e_Cache_Status,
  e_Cal_Managed_ID,
  e_CalDAV_Timezones,
  e_Capsule_Protocol,
  e_CDN_Cache_Control,
  e_CDN_Loop,
  e_Cert_Not_After,
  e_Cert_Not_Before,
  e_Clear_Site_Data,
  e_Client_Cert,
  e_Client_Cert_Chain,
  e_Close,
  e_Configuration_Context,
  e_Connection,
  e_Content_Base,
  e_Content_Disposition,
  e_Content_Encoding,
  e_Content_ID,
  e_Content_Language,
  e_Content_Length,
  e_Content_Location,
  e_Content_MD5,
  e_Content_Range,
  e_Content_Script_Type,
  e_Content_Security_Policy,
  e_Content_Security_Policy_Report_Only,
  e_Content_Style_Type,
  e_Content_Type,
  e_Content_Version,
  e_Cookie,
  e_Cookie2,
  e_Cross_Origin_Embedder_Policy,
  e_Cross_Origin_Embedder_Policy_Report_Only,
  e_Cross_Origin_Opener_Policy,
  e_Cross_Origin_Opener_Policy_Report_Only,
  e_Cross_Origin_Resource_Policy,
  e_DASL,
  e_Date,
  e_DAV,
  e_Default_Style,
  e_Delta_Base,
  e_Depth,
  e_Derived_From,
  e_Destination,
  e_Differential_ID,
  e_Digest,
  e_Early_Data,
  e_EDIINT_Features,
  e_ETag,
  e_Expect,
  e_Expect_CT,
  e_Expires,
  e_Ext,
  e_Forwarded,
  e_From,
  e_GetProfile,
  e_Hobareg,
  e_Host,
  e_HTTP2_Settings,
  e_If,
  e_If_Match,
  e_If_Modified_Since,
  e_If_None_Match,
  e_If_Range,
  e_If_Schedule_Tag_Match,
  e_If_Unmodified_Since,
  e_IM,
  e_Include_Referred_Token_Binding_ID,
  e_Isolation,
  e_Keep_Alive,
  e_Label,
  e_Last_Event_ID,
  e_Last_Modified,
  e_Link,
  e_Location,
  e_Lock_Token,
  e_Man,
  e_Max_Forwards,
  e_Memento_Datetime,
  e_Meter,
  e_Method_Check,
  e_Method_Check_Expires,
  e_MIME_Version,
  e_Negotiate,
  e_OData_EntityId,
  e_OData_Isolation,
  e_OData_MaxVersion,
  e_OData_Version,
  e_Opt,
  e_Optional_WWW_Authenticate,
  e_Ordering_Type,
  e_Origin,
  e_Origin_Agent_Cluster,
  e_OSCORE,
  e_OSLC_Core_Version,
  e_Overwrite,
  e_P3P,
  e_PEP,
  e_Pep_Info,
  e_PICS_Label,
  e_Ping_From,
  e_Ping_To,
  e_Position,
  e_Pragma,
  e_Prefer,
  e_Preference_Applied,
  e_Priority,
  e_ProfileObject,
  e_Protocol,
  e_Protocol_Info,
  e_Protocol_Query,
  e_Protocol_Request,
  e_Proxy_Authenticate,
  e_Proxy_Authentication_Info,
  e_Proxy_Authorization,
  e_Proxy_Features,
  e_Proxy_Instruction,
  e_Proxy_Status,
  e_Public,
  e_Public_Key_Pins,
  e_Public_Key_Pins_Report_Only,
  e_Range,
  e_Redirect_Ref,
  e_Referer,
  e_Referer_Root,
  e_Refresh,
  e_Repeatability_Client_ID,
  e_Repeatability_First_Sent,
  e_Repeatability_Request_ID,
  e_Repeatability_Result,
  e_Replay_Nonce,
  e_Retry_After,
  e_Safe,
  e_Schedule_Reply,
  e_Schedule_Tag,
  e_Sec_GPC,
  e_Sec_Purpose,
  e_Sec_Token_Binding,
  e_Sec_WebSocket_Accept,
  e_Sec_WebSocket_Extensions,
  e_Sec_WebSocket_Key,
  e_Sec_WebSocket_Protocol,
  e_Sec_WebSocket_Version,
  e_Security_Scheme,
  e_Server,
  e_Server_Timing,
  e_Set_Cookie,
  e_Set_Cookie2,
  e_SetProfile,
  e_SLUG,
  e_SoapAction,
  e_Status_URI,
  e_Strict_Transport_Security,
  e_Sunset,
  e_Surrogate_Capability,
  e_Surrogate_Control,
  e_TCN,
  e_TE,
  e_Timeout,
  e_Timing_Allow_Origin,
  e_Topic,
  e_Traceparent,
  e_Tracestate,
  e_Trailer,
  e_Transfer_Encoding,
  e_TTL,
  e_Upgrade,
  e_Urgency,
  e_URI,
  e_User_Agent,
  e_Variant_Vary,
  e_Vary,
  e_Via,
  e_Want_Digest,
  e_Warning,
  e_WWW_Authenticate,
  e_X_Content_Type_Options,
  e_X_Frame_Options,
  e_Star //*
};

std::string_view to_string(types type);

}; // namespace bro::net::http::header

namespace bro::net::http {

enum class connection_type { e_http, e_https };

using result_cbt = std::function<void(std::string const &response, char const *const error, std::any user_data)>;

class request {
public:
  enum class version {
    e_1_0,
    e_1_1,
  };
  enum class type {
    e_GET, ///< The GET method is used to retrieve information from the given server using a given URI. Requests using GET should only retrieve data and should have no other effect on the data.
    e_HEAD, ///< Same as GET, but transfers the status line and header section only.
    e_POST, ///< A POST request is used to send data to the server, for example, customer information, file upload, etc. using HTML forms.
    e_PUT,  ///< Replaces all current representations of the target resource with the uploaded content.
    e_DELETE,  ///< Removes all current representations of the target resource given by a URI.
    e_CONNECT, ///< Establishes a tunnel to the server identified by a given URI.
    e_OPTIONS, ///< Describes the communication options for the target resource.
    e_TRACE,   ///< Performs a message loop-back test along the path to the target resource.
  };

  request(type tp, std::string url, result_cbt const &result, version ver = version::e_1_1)
    : _type(tp)
    , _url(std::move(url))
    , _version(ver)
    , _result(result) {}

  static std::string_view to_string(type t);
  static std::string_view to_string(version ver);

  void add_header(header::types type, std::string const &value);
  void add_header_ex(header::types type, std::string_view const &value);
  void add_body(header::types type, std::string const &value);
  void add_body_ex(header::types type, std::string_view const &value);
  bool send();
  void proceed();

private:
  bool create_stream();
  void resolve_host();
  void send_data();

  type _type{};
  std::string _url;
  version _version{version::e_1_1};
  result_cbt _result;

  size_t _total_size = 0;
  std::vector<std::pair<header::types, std::string>> _headers;
  std::string _body;
  std::string_view _body_ex;
  dns::resolver _resolver;
  bro::net::ev::factory _factory;
  strm::stream_ptr _send_stream;
  proto::ip::full_address _server_address;
  connection_type _connection_type{connection_type::e_https};
  std::string_view _path;
  std::string_view _host;
  std::string _response;
  bool _ready_send_data = false;
};

} // namespace bro::net::http

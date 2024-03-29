#include <unordered_map>
#include <http_client/header.h>

namespace bro::net::http::header {

std::string_view to_string(types type) {
  switch (type) {
  case types::e_A_IM:
    return "A-IM";
  case types::e_Accept:
    return "Accept";
  case types::e_Accept_Additions:
    return "Accept-Additions";
  case types::e_Accept_CH:
    return "Accept-CH";
  case types::e_Accept_Charset:
    return "Accept-Charset";
  case types::e_Accept_Datetime:
    return "Accept-Datetime";
  case types::e_Accept_Encoding:
    return "Accept-Encoding";
  case types::e_Accept_Features:
    return "Accept-Features";
  case types::e_Accept_Language:
    return "Accept-Language";
  case types::e_Accept_Patch:
    return "Accept-Patch";
  case types::e_Accept_Post:
    return "Accept-Post";
  case types::e_Accept_Ranges:
    return "Accept-Ranges";
  case types::e_Access_Control:
    return "Access-Control";
  case types::e_Access_Control_Allow_Credentials:
    return "Access-Control-Allow-Credentials";
  case types::e_Access_Control_Allow_Headers:
    return "Access-Control-Allow-Headers";
  case types::e_Access_Control_Allow_Methods:
    return "Access-Control-Allow-Methods";
  case types::e_Access_Control_Allow_Origin:
    return "Access-Control-Allow-Origin";
  case types::e_Access_Control_Expose_Headers:
    return "Access-Control-Expose-Headers";
  case types::e_Access_Control_Max_Age:
    return "Access-Control-Max-Age";
  case types::e_Access_Control_Request_Headers:
    return "Access-Control-Request-Headers";
  case types::e_Access_Control_Request_Method:
    return "Access-Control-Request-Method";
  case types::e_Age:
    return "Age";
  case types::e_Allow:
    return "Allow";
  case types::e_ALPN:
    return "ALPN";
  case types::e_Alt_Svc:
    return "Alt-Svc";
  case types::e_Alt_Used:
    return "Alt-Used";
  case types::e_Alternates:
    return "Alternates";
  case types::e_AMP_Cache_Transform:
    return "AMP-Cache-Transform";
  case types::e_Apply_To_Redirect_Ref:
    return "Apply-To-Redirect-Ref";
  case types::e_Authentication_Control:
    return "Authentication-Control";
  case types::e_Authentication_Info:
    return "Authentication-Info";
  case types::e_Authorization:
    return "Authorization";
  case types::e_C_Ext:
    return "C-Ext";
  case types::e_C_Man:
    return "C-Man";
  case types::e_C_Opt:
    return "C-Opt";
  case types::e_C_PEP:
    return "C-PEP";
  case types::e_C_PEP_Info:
    return "C-PEP-Info";
  case types::e_Cache_Control:
    return "Cache-Control";
  case types::e_Cache_Status:
    return "Cache-Status";
  case types::e_Cal_Managed_ID:
    return "Cal-Managed-ID";
  case types::e_CalDAV_Timezones:
    return "CalDAV-Timezones";
  case types::e_Capsule_Protocol:
    return "Capsule-Protocol";
  case types::e_CDN_Cache_Control:
    return "CDN-Cache-Control";
  case types::e_CDN_Loop:
    return "CDN-Loop";
  case types::e_Cert_Not_After:
    return "Cert-Not-After";
  case types::e_Cert_Not_Before:
    return "Cert-Not-Before";
  case types::e_Clear_Site_Data:
    return "Clear-Site-Data";
  case types::e_Client_Cert:
    return "Client-Cert";
  case types::e_Client_Cert_Chain:
    return "Client-Cert-Chain";
  case types::e_Close:
    return "Close";
  case types::e_Configuration_Context:
    return "Configuration-Context";
  case types::e_Connection:
    return "Connection";
  case types::e_Content_Base:
    return "Content-Base";
  case types::e_Content_Disposition:
    return "Content-Disposition";
  case types::e_Content_Encoding:
    return "Content-Encoding";
  case types::e_Content_ID:
    return "Content-ID";
  case types::e_Content_Language:
    return "Content-Language";
  case types::e_Content_Length:
    return "Content-Length";
  case types::e_Content_Location:
    return "Content-Location";
  case types::e_Content_MD5:
    return "Content-MD5";
  case types::e_Content_Range:
    return "Content-Range";
  case types::e_Content_Script_Type:
    return "Content-Script-Type";
  case types::e_Content_Security_Policy:
    return "Content-Security-Policy";
  case types::e_Content_Security_Policy_Report_Only:
    return "Content-Security-Policy-Report-Only";
  case types::e_Content_Style_Type:
    return "Content-Style-Type";
  case types::e_Content_Type:
    return "Content-Type";
  case types::e_Content_Version:
    return "Content-Version";
  case types::e_Cookie:
    return "Cookie";
  case types::e_Cookie2:
    return "Cookie2";
  case types::e_Cross_Origin_Embedder_Policy:
    return "Cross-Origin-Embedder-Policy";
  case types::e_Cross_Origin_Embedder_Policy_Report_Only:
    return "Cross-Origin-Embedder-Policy-Report-Only";
  case types::e_Cross_Origin_Opener_Policy:
    return "Cross-Origin-Opener-Policy";
  case types::e_Cross_Origin_Opener_Policy_Report_Only:
    return "Cross-Origin-Opener-Policy-Report-Only";
  case types::e_Cross_Origin_Resource_Policy:
    return "Cross-Origin-Resource-Policy";
  case types::e_DASL:
    return "DASL";
  case types::e_Date:
    return "Date";
  case types::e_DAV:
    return "DAV";
  case types::e_Default_Style:
    return "Default-Style";
  case types::e_Delta_Base:
    return "Delta-Base";
  case types::e_Depth:
    return "Depth";
  case types::e_Derived_From:
    return "Derived-From";
  case types::e_Destination:
    return "Destination";
  case types::e_Differential_ID:
    return "Differential-ID";
  case types::e_Digest:
    return "Digest";
  case types::e_Early_Data:
    return "Early-Data";
  case types::e_EDIINT_Features:
    return "EDIINT-Features";
  case types::e_ETag:
    return "ETag";
  case types::e_Expect:
    return "Expect";
  case types::e_Expect_CT:
    return "Expect-CT";
  case types::e_Expires:
    return "Expires";
  case types::e_Ext:
    return "Ext";
  case types::e_Forwarded:
    return "Forwarded";
  case types::e_From:
    return "From";
  case types::e_GetProfile:
    return "GetProfile";
  case types::e_Hobareg:
    return "Hobareg";
  case types::e_Host:
    return "Host";
  case types::e_HTTP2_Settings:
    return "HTTP2-Settings";
  case types::e_If:
    return "If";
  case types::e_If_Match:
    return "If-Match";
  case types::e_If_Modified_Since:
    return "If-Modified-Since";
  case types::e_If_None_Match:
    return "If-None-Match";
  case types::e_If_Range:
    return "If-Range";
  case types::e_If_Schedule_Tag_Match:
    return "If-Schedule-Tag-Match";
  case types::e_If_Unmodified_Since:
    return "If-Unmodified-Since";
  case types::e_IM:
    return "IM";
  case types::e_Include_Referred_Token_Binding_ID:
    return "Include-Referred-Token-Binding-ID";
  case types::e_Isolation:
    return "Isolation";
  case types::e_Keep_Alive:
    return "Keep-Alive";
  case types::e_Label:
    return "Label";
  case types::e_Last_Event_ID:
    return "Last-Event-ID";
  case types::e_Last_Modified:
    return "Last-Modified";
  case types::e_Link:
    return "Link";
  case types::e_Location:
    return "Location";
  case types::e_Lock_Token:
    return "Lock-Token";
  case types::e_Man:
    return "Man";
  case types::e_Max_Forwards:
    return "Max-Forwards";
  case types::e_Memento_Datetime:
    return "Memento-Datetime";
  case types::e_Meter:
    return "Meter";
  case types::e_Method_Check:
    return "Method-Check";
  case types::e_Method_Check_Expires:
    return "Method-Check-Expires";
  case types::e_MIME_Version:
    return "MIME-Version";
  case types::e_Negotiate:
    return "Negotiate";
  case types::e_OData_EntityId:
    return "OData-EntityId";
  case types::e_OData_Isolation:
    return "OData-Isolation";
  case types::e_OData_MaxVersion:
    return "OData-MaxVersion";
  case types::e_OData_Version:
    return "OData-Version";
  case types::e_Opt:
    return "Opt";
  case types::e_Optional_WWW_Authenticate:
    return "Optional-WWW-Authenticate";
  case types::e_Ordering_Type:
    return "Ordering-Type";
  case types::e_Origin:
    return "Origin";
  case types::e_Origin_Agent_Cluster:
    return "Origin-Agent-Cluster";
  case types::e_OSCORE:
    return "OSCORE";
  case types::e_OSLC_Core_Version:
    return "OSLC-Core-Version";
  case types::e_Overwrite:
    return "Overwrite";
  case types::e_P3P:
    return "P3P";
  case types::e_PEP:
    return "PEP";
  case types::e_Pep_Info:
    return "Pep-Info";
  case types::e_PICS_Label:
    return "PICS-Label";
  case types::e_Ping_From:
    return "Ping-From";
  case types::e_Ping_To:
    return "Ping-To";
  case types::e_Position:
    return "Position";
  case types::e_Pragma:
    return "Pragma";
  case types::e_Prefer:
    return "Prefer";
  case types::e_Preference_Applied:
    return "Preference-Applied";
  case types::e_Priority:
    return "Priority";
  case types::e_ProfileObject:
    return "ProfileObject";
  case types::e_Protocol:
    return "Protocol";
  case types::e_Protocol_Info:
    return "Protocol-Info";
  case types::e_Protocol_Query:
    return "Protocol-Query";
  case types::e_Protocol_Request:
    return "Protocol-Request";
  case types::e_Proxy_Authenticate:
    return "Proxy-Authenticate";
  case types::e_Proxy_Authentication_Info:
    return "Proxy-Authentication-Info";
  case types::e_Proxy_Authorization:
    return "Proxy-Authorization";
  case types::e_Proxy_Features:
    return "Proxy-Features";
  case types::e_Proxy_Instruction:
    return "Proxy-Instruction";
  case types::e_Proxy_Status:
    return "Proxy-Status";
  case types::e_Public:
    return "Public";
  case types::e_Public_Key_Pins:
    return "Public-Key-Pins";
  case types::e_Public_Key_Pins_Report_Only:
    return "Public-Key-Pins-Report-Only";
  case types::e_Range:
    return "Range";
  case types::e_Redirect_Ref:
    return "Redirect-Ref";
  case types::e_Referer:
    return "Referer";
  case types::e_Referer_Root:
    return "Referer-Root";
  case types::e_Refresh:
    return "Refresh";
  case types::e_Repeatability_Client_ID:
    return "Repeatability-Client-ID";
  case types::e_Repeatability_First_Sent:
    return "Repeatability-First-Sent";
  case types::e_Repeatability_Request_ID:
    return "Repeatability-Request-ID";
  case types::e_Repeatability_Result:
    return "Repeatability-Result";
  case types::e_Replay_Nonce:
    return "Replay-Nonce";
  case types::e_Retry_After:
    return "Retry-After";
  case types::e_Safe:
    return "Safe";
  case types::e_Schedule_Reply:
    return "Schedule-Reply";
  case types::e_Schedule_Tag:
    return "Schedule-Tag";
  case types::e_Sec_GPC:
    return "Sec-GPC";
  case types::e_Sec_Purpose:
    return "Sec-Purpose";
  case types::e_Sec_Token_Binding:
    return "Sec-Token-Binding";
  case types::e_Sec_WebSocket_Accept:
    return "Sec-WebSocket-Accept";
  case types::e_Sec_WebSocket_Extensions:
    return "Sec-WebSocket-Extensions";
  case types::e_Sec_WebSocket_Key:
    return "Sec-WebSocket-Key";
  case types::e_Sec_WebSocket_Protocol:
    return "Sec-WebSocket-Protocol";
  case types::e_Sec_WebSocket_Version:
    return "Sec-WebSocket-Version";
  case types::e_Security_Scheme:
    return "Security-Scheme";
  case types::e_Server:
    return "Server";
  case types::e_Server_Timing:
    return "Server-Timing";
  case types::e_Set_Cookie:
    return "Set-Cookie";
  case types::e_Set_Cookie2:
    return "Set-Cookie2";
  case types::e_SetProfile:
    return "SetProfile";
  case types::e_SLUG:
    return "SLUG";
  case types::e_SoapAction:
    return "SoapAction";
  case types::e_Status_URI:
    return "Status-URI";
  case types::e_Strict_Transport_Security:
    return "Strict-Transport-Security";
  case types::e_Sunset:
    return "Sunset";
  case types::e_Surrogate_Capability:
    return "Surrogate-Capability";
  case types::e_Surrogate_Control:
    return "Surrogate-Control";
  case types::e_TCN:
    return "TCN";
  case types::e_TE:
    return "TE";
  case types::e_Timeout:
    return "Timeout";
  case types::e_Timing_Allow_Origin:
    return "Timing-Allow-Origin";
  case types::e_Topic:
    return "Topic";
  case types::e_Traceparent:
    return "Traceparent";
  case types::e_Tracestate:
    return "Tracestate";
  case types::e_Trailer:
    return "Trailer";
  case types::e_Transfer_Encoding:
    return "Transfer-Encoding";
  case types::e_TTL:
    return "TTL";
  case types::e_Upgrade:
    return "Upgrade";
  case types::e_Urgency:
    return "Urgency";
  case types::e_URI:
    return "URI";
  case types::e_User_Agent:
    return "User-Agent";
  case types::e_Variant_Vary:
    return "Variant-Vary";
  case types::e_Vary:
    return "Vary";
  case types::e_Via:
    return "Via";
  case types::e_Want_Digest:
    return "Want-Digest";
  case types::e_Warning:
    return "Warning";
  case types::e_WWW_Authenticate:
    return "WWW-Authenticate";
  case types::e_X_Content_Type_Options:
    return "X-Content-Type-Options";
  case types::e_X_Frame_Options:
    return "X-Frame-Options";
  case types::e_Star:
    return "*";
  case types::e_Unknown_Header_Type:
    return "unknown-header-type";
  }
  return "unknown-header-type";
}

types to_type(std::string_view type) {
  static std::unordered_map<std::string_view, types> s_umap{
    {to_string(types::e_A_IM), types::e_A_IM},
    {to_string(types::e_Accept), types::e_Accept},
    {to_string(types::e_Accept_Additions), types::e_Accept_Additions},
    {to_string(types::e_Accept_CH), types::e_Accept_CH},
    {to_string(types::e_Accept_Charset), types::e_Accept_Charset},
    {to_string(types::e_Accept_Datetime), types::e_Accept_Datetime},
    {to_string(types::e_Accept_Encoding), types::e_Accept_Encoding},
    {to_string(types::e_Accept_Features), types::e_Accept_Features},
    {to_string(types::e_Accept_Language), types::e_Accept_Language},
    {to_string(types::e_Accept_Patch), types::e_Accept_Patch},
    {to_string(types::e_Accept_Post), types::e_Accept_Post},
    {to_string(types::e_Accept_Ranges), types::e_Accept_Ranges},
    {to_string(types::e_Access_Control), types::e_Access_Control},
    {to_string(types::e_Access_Control_Allow_Credentials), types::e_Access_Control_Allow_Credentials},
    {to_string(types::e_Access_Control_Allow_Headers), types::e_Access_Control_Allow_Headers},
    {to_string(types::e_Access_Control_Allow_Methods), types::e_Access_Control_Allow_Methods},
    {to_string(types::e_Access_Control_Allow_Origin), types::e_Access_Control_Allow_Origin},
    {to_string(types::e_Access_Control_Expose_Headers), types::e_Access_Control_Expose_Headers},
    {to_string(types::e_Access_Control_Max_Age), types::e_Access_Control_Max_Age},
    {to_string(types::e_Access_Control_Request_Headers), types::e_Access_Control_Request_Headers},
    {to_string(types::e_Access_Control_Request_Method), types::e_Access_Control_Request_Method},
    {to_string(types::e_Age), types::e_Age},
    {to_string(types::e_Allow), types::e_Allow},
    {to_string(types::e_ALPN), types::e_ALPN},
    {to_string(types::e_Alt_Svc), types::e_Alt_Svc},
    {to_string(types::e_Alt_Used), types::e_Alt_Used},
    {to_string(types::e_Alternates), types::e_Alternates},
    {to_string(types::e_AMP_Cache_Transform), types::e_AMP_Cache_Transform},
    {to_string(types::e_Apply_To_Redirect_Ref), types::e_Apply_To_Redirect_Ref},
    {to_string(types::e_Authentication_Control), types::e_Authentication_Control},
    {to_string(types::e_Authentication_Info), types::e_Authentication_Info},
    {to_string(types::e_Authorization), types::e_Authorization},
    {to_string(types::e_C_Ext), types::e_C_Ext},
    {to_string(types::e_C_Man), types::e_C_Man},
    {to_string(types::e_C_Opt), types::e_C_Opt},
    {to_string(types::e_C_PEP), types::e_C_PEP},
    {to_string(types::e_C_PEP_Info), types::e_C_PEP_Info},
    {to_string(types::e_Cache_Control), types::e_Cache_Control},
    {to_string(types::e_Cache_Status), types::e_Cache_Status},
    {to_string(types::e_Cal_Managed_ID), types::e_Cal_Managed_ID},
    {to_string(types::e_CalDAV_Timezones), types::e_CalDAV_Timezones},
    {to_string(types::e_Capsule_Protocol), types::e_Capsule_Protocol},
    {to_string(types::e_CDN_Cache_Control), types::e_CDN_Cache_Control},
    {to_string(types::e_CDN_Loop), types::e_CDN_Loop},
    {to_string(types::e_Cert_Not_After), types::e_Cert_Not_After},
    {to_string(types::e_Cert_Not_Before), types::e_Cert_Not_Before},
    {to_string(types::e_Clear_Site_Data), types::e_Clear_Site_Data},
    {to_string(types::e_Client_Cert), types::e_Client_Cert},
    {to_string(types::e_Client_Cert_Chain), types::e_Client_Cert_Chain},
    {to_string(types::e_Close), types::e_Close},
    {to_string(types::e_Configuration_Context), types::e_Configuration_Context},
    {to_string(types::e_Connection), types::e_Connection},
    {to_string(types::e_Content_Base), types::e_Content_Base},
    {to_string(types::e_Content_Disposition), types::e_Content_Disposition},
    {to_string(types::e_Content_Encoding), types::e_Content_Encoding},
    {to_string(types::e_Content_ID), types::e_Content_ID},
    {to_string(types::e_Content_Language), types::e_Content_Language},
    {to_string(types::e_Content_Length), types::e_Content_Length},
    {to_string(types::e_Content_Location), types::e_Content_Location},
    {to_string(types::e_Content_MD5), types::e_Content_MD5},
    {to_string(types::e_Content_Range), types::e_Content_Range},
    {to_string(types::e_Content_Script_Type), types::e_Content_Script_Type},
    {to_string(types::e_Content_Security_Policy), types::e_Content_Security_Policy},
    {to_string(types::e_Content_Security_Policy_Report_Only), types::e_Content_Security_Policy_Report_Only},
    {to_string(types::e_Content_Style_Type), types::e_Content_Style_Type},
    {to_string(types::e_Content_Type), types::e_Content_Type},
    {to_string(types::e_Content_Version), types::e_Content_Version},
    {to_string(types::e_Cookie), types::e_Cookie},
    {to_string(types::e_Cookie2), types::e_Cookie2},
    {to_string(types::e_Cross_Origin_Embedder_Policy), types::e_Cross_Origin_Embedder_Policy},
    {to_string(types::e_Cross_Origin_Embedder_Policy_Report_Only), types::e_Cross_Origin_Embedder_Policy_Report_Only},
    {to_string(types::e_Cross_Origin_Opener_Policy), types::e_Cross_Origin_Opener_Policy},
    {to_string(types::e_Cross_Origin_Opener_Policy_Report_Only), types::e_Cross_Origin_Opener_Policy_Report_Only},
    {to_string(types::e_Cross_Origin_Resource_Policy), types::e_Cross_Origin_Resource_Policy},
    {to_string(types::e_DASL), types::e_DASL},
    {to_string(types::e_Date), types::e_Date},
    {to_string(types::e_DAV), types::e_DAV},
    {to_string(types::e_Default_Style), types::e_Default_Style},
    {to_string(types::e_Delta_Base), types::e_Delta_Base},
    {to_string(types::e_Depth), types::e_Depth},
    {to_string(types::e_Derived_From), types::e_Derived_From},
    {to_string(types::e_Destination), types::e_Destination},
    {to_string(types::e_Differential_ID), types::e_Differential_ID},
    {to_string(types::e_Digest), types::e_Digest},
    {to_string(types::e_Early_Data), types::e_Early_Data},
    {to_string(types::e_EDIINT_Features), types::e_EDIINT_Features},
    {to_string(types::e_ETag), types::e_ETag},
    {to_string(types::e_Expect), types::e_Expect},
    {to_string(types::e_Expect_CT), types::e_Expect_CT},
    {to_string(types::e_Expires), types::e_Expires},
    {to_string(types::e_Ext), types::e_Ext},
    {to_string(types::e_Forwarded), types::e_Forwarded},
    {to_string(types::e_From), types::e_From},
    {to_string(types::e_GetProfile), types::e_GetProfile},
    {to_string(types::e_Hobareg), types::e_Hobareg},
    {to_string(types::e_Host), types::e_Host},
    {to_string(types::e_HTTP2_Settings), types::e_HTTP2_Settings},
    {to_string(types::e_If), types::e_If},
    {to_string(types::e_If_Match), types::e_If_Match},
    {to_string(types::e_If_Modified_Since), types::e_If_Modified_Since},
    {to_string(types::e_If_None_Match), types::e_If_None_Match},
    {to_string(types::e_If_Range), types::e_If_Range},
    {to_string(types::e_If_Schedule_Tag_Match), types::e_If_Schedule_Tag_Match},
    {to_string(types::e_If_Unmodified_Since), types::e_If_Unmodified_Since},
    {to_string(types::e_IM), types::e_IM},
    {to_string(types::e_Include_Referred_Token_Binding_ID), types::e_Include_Referred_Token_Binding_ID},
    {to_string(types::e_Isolation), types::e_Isolation},
    {to_string(types::e_Keep_Alive), types::e_Keep_Alive},
    {to_string(types::e_Label), types::e_Label},
    {to_string(types::e_Last_Event_ID), types::e_Last_Event_ID},
    {to_string(types::e_Last_Modified), types::e_Last_Modified},
    {to_string(types::e_Link), types::e_Link},
    {to_string(types::e_Location), types::e_Location},
    {to_string(types::e_Lock_Token), types::e_Lock_Token},
    {to_string(types::e_Man), types::e_Man},
    {to_string(types::e_Max_Forwards), types::e_Max_Forwards},
    {to_string(types::e_Memento_Datetime), types::e_Memento_Datetime},
    {to_string(types::e_Meter), types::e_Meter},
    {to_string(types::e_Method_Check), types::e_Method_Check},
    {to_string(types::e_Method_Check_Expires), types::e_Method_Check_Expires},
    {to_string(types::e_MIME_Version), types::e_MIME_Version},
    {to_string(types::e_Negotiate), types::e_Negotiate},
    {to_string(types::e_OData_EntityId), types::e_OData_EntityId},
    {to_string(types::e_OData_Isolation), types::e_OData_Isolation},
    {to_string(types::e_OData_MaxVersion), types::e_OData_MaxVersion},
    {to_string(types::e_OData_Version), types::e_OData_Version},
    {to_string(types::e_Opt), types::e_Opt},
    {to_string(types::e_Optional_WWW_Authenticate), types::e_Optional_WWW_Authenticate},
    {to_string(types::e_Ordering_Type), types::e_Ordering_Type},
    {to_string(types::e_Origin), types::e_Origin},
    {to_string(types::e_Origin_Agent_Cluster), types::e_Origin_Agent_Cluster},
    {to_string(types::e_OSCORE), types::e_OSCORE},
    {to_string(types::e_OSLC_Core_Version), types::e_OSLC_Core_Version},
    {to_string(types::e_Overwrite), types::e_Overwrite},
    {to_string(types::e_P3P), types::e_P3P},
    {to_string(types::e_PEP), types::e_PEP},
    {to_string(types::e_Pep_Info), types::e_Pep_Info},
    {to_string(types::e_PICS_Label), types::e_PICS_Label},
    {to_string(types::e_Ping_From), types::e_Ping_From},
    {to_string(types::e_Ping_To), types::e_Ping_To},
    {to_string(types::e_Position), types::e_Position},
    {to_string(types::e_Pragma), types::e_Pragma},
    {to_string(types::e_Prefer), types::e_Prefer},
    {to_string(types::e_Preference_Applied), types::e_Preference_Applied},
    {to_string(types::e_Priority), types::e_Priority},
    {to_string(types::e_ProfileObject), types::e_ProfileObject},
    {to_string(types::e_Protocol), types::e_Protocol},
    {to_string(types::e_Protocol_Info), types::e_Protocol_Info},
    {to_string(types::e_Protocol_Query), types::e_Protocol_Query},
    {to_string(types::e_Protocol_Request), types::e_Protocol_Request},
    {to_string(types::e_Proxy_Authenticate), types::e_Proxy_Authenticate},
    {to_string(types::e_Proxy_Authentication_Info), types::e_Proxy_Authentication_Info},
    {to_string(types::e_Proxy_Authorization), types::e_Proxy_Authorization},
    {to_string(types::e_Proxy_Features), types::e_Proxy_Features},
    {to_string(types::e_Proxy_Instruction), types::e_Proxy_Instruction},
    {to_string(types::e_Proxy_Status), types::e_Proxy_Status},
    {to_string(types::e_Public), types::e_Public},
    {to_string(types::e_Public_Key_Pins), types::e_Public_Key_Pins},
    {to_string(types::e_Public_Key_Pins_Report_Only), types::e_Public_Key_Pins_Report_Only},
    {to_string(types::e_Range), types::e_Range},
    {to_string(types::e_Redirect_Ref), types::e_Redirect_Ref},
    {to_string(types::e_Referer), types::e_Referer},
    {to_string(types::e_Referer_Root), types::e_Referer_Root},
    {to_string(types::e_Refresh), types::e_Refresh},
    {to_string(types::e_Repeatability_Client_ID), types::e_Repeatability_Client_ID},
    {to_string(types::e_Repeatability_First_Sent), types::e_Repeatability_First_Sent},
    {to_string(types::e_Repeatability_Request_ID), types::e_Repeatability_Request_ID},
    {to_string(types::e_Repeatability_Result), types::e_Repeatability_Result},
    {to_string(types::e_Replay_Nonce), types::e_Replay_Nonce},
    {to_string(types::e_Retry_After), types::e_Retry_After},
    {to_string(types::e_Safe), types::e_Safe},
    {to_string(types::e_Schedule_Reply), types::e_Schedule_Reply},
    {to_string(types::e_Schedule_Tag), types::e_Schedule_Tag},
    {to_string(types::e_Sec_GPC), types::e_Sec_GPC},
    {to_string(types::e_Sec_Purpose), types::e_Sec_Purpose},
    {to_string(types::e_Sec_Token_Binding), types::e_Sec_Token_Binding},
    {to_string(types::e_Sec_WebSocket_Accept), types::e_Sec_WebSocket_Accept},
    {to_string(types::e_Sec_WebSocket_Extensions), types::e_Sec_WebSocket_Extensions},
    {to_string(types::e_Sec_WebSocket_Key), types::e_Sec_WebSocket_Key},
    {to_string(types::e_Sec_WebSocket_Protocol), types::e_Sec_WebSocket_Protocol},
    {to_string(types::e_Sec_WebSocket_Version), types::e_Sec_WebSocket_Version},
    {to_string(types::e_Security_Scheme), types::e_Security_Scheme},
    {to_string(types::e_Server), types::e_Server},
    {to_string(types::e_Server_Timing), types::e_Server_Timing},
    {to_string(types::e_Set_Cookie), types::e_Set_Cookie},
    {to_string(types::e_Set_Cookie2), types::e_Set_Cookie2},
    {to_string(types::e_SetProfile), types::e_SetProfile},
    {to_string(types::e_SLUG), types::e_SLUG},
    {to_string(types::e_SoapAction), types::e_SoapAction},
    {to_string(types::e_Status_URI), types::e_Status_URI},
    {to_string(types::e_Strict_Transport_Security), types::e_Strict_Transport_Security},
    {to_string(types::e_Sunset), types::e_Sunset},
    {to_string(types::e_Surrogate_Capability), types::e_Surrogate_Capability},
    {to_string(types::e_Surrogate_Control), types::e_Surrogate_Control},
    {to_string(types::e_TCN), types::e_TCN},
    {to_string(types::e_TE), types::e_TE},
    {to_string(types::e_Timeout), types::e_Timeout},
    {to_string(types::e_Timing_Allow_Origin), types::e_Timing_Allow_Origin},
    {to_string(types::e_Topic), types::e_Topic},
    {to_string(types::e_Traceparent), types::e_Traceparent},
    {to_string(types::e_Tracestate), types::e_Tracestate},
    {to_string(types::e_Trailer), types::e_Trailer},
    {to_string(types::e_Transfer_Encoding), types::e_Transfer_Encoding},
    {to_string(types::e_TTL), types::e_TTL},
    {to_string(types::e_Upgrade), types::e_Upgrade},
    {to_string(types::e_Urgency), types::e_Urgency},
    {to_string(types::e_URI), types::e_URI},
    {to_string(types::e_User_Agent), types::e_User_Agent},
    {to_string(types::e_Variant_Vary), types::e_Variant_Vary},
    {to_string(types::e_Vary), types::e_Vary},
    {to_string(types::e_Via), types::e_Via},
    {to_string(types::e_Want_Digest), types::e_Want_Digest},
    {to_string(types::e_Warning), types::e_Warning},
    {to_string(types::e_WWW_Authenticate), types::e_WWW_Authenticate},
    {to_string(types::e_X_Content_Type_Options), types::e_X_Content_Type_Options},
    {to_string(types::e_X_Frame_Options), types::e_X_Frame_Options},
    {to_string(types::e_Star), types::e_Star}};
  if (auto it = s_umap.find(type); it != s_umap.end())
    return it->second;
  return types::e_Unknown_Header_Type;
}

} // namespace bro::net::http::header

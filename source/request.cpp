#include "fmt/core.h"
#include "network/common/buffer.h"
#include <http_client/request.h>
#include <uriparser/Uri.h>
#include <charconv>
#include <network/tcp/ssl/send/settings.h>
#include <iostream>

namespace bro::net::http::status {

std::string_view to_str_view(code c) {
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
  }
  return "unknown-status-code";
}

} // namespace bro::net::http::status
namespace bro::net::http {

std::string_view request::to_string(type t) {
  switch (t) {
  case type::e_GET:
    return "GET";
  case type::e_HEAD:
    return "HEAD";
  case type::e_POST:
    return "POST";
  case type::e_PUT:
    return "PUT";
  case type::e_DELETE:
    return "DELETE";
  case type::e_CONNECT:
    return "CONNECT";
  case type::e_OPTIONS:
    return "OPTIONS";
  case type::e_TRACE:
    return "TRACE";
  }
  return "";
}

std::string_view request::to_string(version ver) {
  switch (ver) {
  case version::e_1_0:
    return "HTTP/1.0";
  case version::e_1_1:
    return "HTTP/1.1";
  }
  return "";
}

} // namespace bro::net::http

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
  }
  return "unknown-header-type";
}

} // namespace bro::net::http::header

namespace bro::net::http {

enum {
  e_rn_size = 2,
  e_semicolon_space_size = 2,
  e_header_add_size = e_semicolon_space_size + e_rn_size,
  e_2_spoce_size = 2
};

void request::add_header(header::types type, std::string const &value) {
  _total_size += value.size() + header::to_string(type).size() + e_header_add_size;
  _headers.push_back({type, value});
}

std::optional<connection_type> get_connection_type(UriUriA const &uri) {
  if (!uri.scheme.first) {
    return std::nullopt;
  }

  std::string_view sheme(uri.scheme.first, std::distance(uri.scheme.first, uri.scheme.afterLast));
  if (sheme == "http") {
    return connection_type::e_http;
  } else if (sheme == "https") {
    return connection_type::e_https;
  }
  return std::nullopt;
}

std::optional<uint16_t> get_port(UriUriA const &uri, connection_type conn_type) {
  std::optional<uint16_t> port;
  if (uri.portText.first) {
    auto [ptr, ec]{std::from_chars(uri.portText.first, uri.portText.afterLast, *port)};
    if (ec != std::errc()) {
      return std::nullopt;
    }
  }

  return port ? *port : conn_type == connection_type::e_http ? 80 : 443;
}

void request::resolve_host() {}

bool request::create_stream() {
  switch (_connection_type) {
  case connection_type::e_http: {
    bro::net::tcp::send::settings send_set;
    send_set._peer_addr = _server_address;
    _send_stream = _factory.create_stream(&send_set);
    break;
  }
  case connection_type::e_https: {
    bro::net::tcp::ssl::send::settings send_set;
    send_set._peer_addr = _server_address;
    _send_stream = _factory.create_stream(&send_set);
    break;
  }
  default:
    // handle error
    return false;
  }
  if (!_send_stream->is_active()) {
    // handle error
    return false;
  }

  _factory.bind(_send_stream);
  _send_stream->set_state_changed_cb(
    [&](strm::stream *strm, std::any) {
      switch (strm->get_state()) {
      case strm::stream::state::e_established:
        send_data();
        //_ready_send_data = true;
        break;
      case strm::stream::state::e_wait:
        return;
        break;
      default:
        _result(_response, "brum", this); // just for test
        // handle error
        break;
      }
    },
    nullptr);
  _send_stream->set_received_data_cb(
    [&](strm::stream *strm, std::any) {
      char buffer[2000] = {};
      auto res = strm->receive((std::byte *) buffer, 2000);
      if (res > 0)
        _response.append(buffer, res);
    },
    nullptr);

  //  _send_stream->send((std::byte *) buffer, _total_size + 2);
  return true;
}

void request::send_data() {
  int total = _total_size + e_rn_size;
  std::byte buffer[total];
  //  memset(buffer, 0, total);

  //  GET /all/ HTTP/1.1
  //  Host: mobile-review.com
  //  Connection: keep-alive
  //  Upgrade-Insecure-Requests: 1
  //  User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36
  //  Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
  // Accept-Encoding: gzip, deflate
  // Accept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7

  //  _total_size += header::to_string(header::types::e_Host).size() + host.size() + 4;      // host size
  //  _total_size += to_string(_type).size() + path.size() + to_string(_version).size() + 4; // path size

  auto res = fmt::format_to_n((char *) buffer, total, "{} {} {}\r\n", to_string(_type), _path, to_string(_version));
  res.out = fmt::format_to_n(res.out, total, "{}: {}\r\n", header::to_string(header::types::e_Host), _host).out;
  for (auto const &hdr : _headers) {
    res.out = fmt::format_to_n(res.out, total, "{}: {}\r\n", header::to_string(hdr.first), hdr.second).out;
  }
  res.out = fmt::format_to_n(res.out, total, "\r\n").out;
  _send_stream->send(buffer, total);
}

void request::proceed() {
  _resolver.proceed();
  _factory.proceed();
  if (_ready_send_data) {
    send_data();
    _ready_send_data = false;
  }
}

bool request::send() {
  UriParserStateA state;
  UriUriA uri;
  state.uri = &uri;
  if (uriParseUriA(&state, _url.c_str()) != URI_SUCCESS) {
    return false;
  }
  if (!uri.hostText.first) {
    return false;
  }

  {
    auto conn_type = get_connection_type(uri);
    if (!conn_type)
      return false;
    _connection_type = *conn_type;
  }

  auto port = get_port(uri, _connection_type);
  if (!port)
    return false;

  add_header(header::types::e_Connection, "close"); //
  _host = std::string_view(uri.hostText.first, std::distance(uri.hostText.first, uri.hostText.afterLast));
  _path = std::string_view(uri.hostText.afterLast);
  _total_size += header::to_string(header::types::e_Host).size() + _host.size() + e_header_add_size;
  _total_size += to_string(_type).size() + _path.size() + to_string(_version).size() + e_rn_size + e_2_spoce_size;

  //  return true;

  if (uri.hostData.ip4 || uri.hostData.ip6) {
    if (uri.hostData.ip4) {
      _server_address = {proto::ip::v4::address(uri.hostData.ip4->data), *port};
    } else {
      _server_address = {proto::ip::v6::address(uri.hostData.ip6->data), *port};
    }
    create_stream();

  } else {
    _resolver.resolve({_host.begin(), _host.end()},
                      proto::ip::address::version::e_v4,
                      [port = *port, this](bro::net::proto::ip::address const &addr,
                                           std::string const & /*hostname*/,
                                           char const *err) {
                        if (err) {
                          // handle error

                        } else {
                          _server_address = proto::ip::full_address(addr, port);
                          create_stream();
                        }
                      });
  }
  uriFreeUriMembersA(&uri);
  return true;
}

} // namespace bro::net::http

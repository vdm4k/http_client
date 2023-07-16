#pragma once
#include <string_view>

namespace bro::net::http::header {

/** @defgroup http_header_types http_header_types
 *  @{
 */

/**
 * \brief header types
 */
enum class types {
    e_A_IM,                                     ///< A_IM
    e_Accept,                                   ///< Accept
    e_Accept_Additions,                         ///< Accept_Additions
    e_Accept_CH,                                ///< Accept_CH
    e_Accept_Charset,                           ///< Accept_Charset
    e_Accept_Datetime,                          ///< Accept_Datetime
    e_Accept_Encoding,                          ///< Accept_Encoding
    e_Accept_Features,                          ///< Accept_Features
    e_Accept_Language,                          ///< Accept_Language
    e_Accept_Patch,                             ///< Accept_Patch
    e_Accept_Post,                              ///< Accept_Post
    e_Accept_Ranges,                            ///< Accept_Ranges
    e_Access_Control,                           ///< Access_Control
    e_Access_Control_Allow_Credentials,         ///< Access_Control_Allow_Credentials
    e_Access_Control_Allow_Headers,             ///< Access_Control_Allow_Headers
    e_Access_Control_Allow_Methods,             ///< Access_Control_Allow_Methods
    e_Access_Control_Allow_Origin,              ///< Access_Control_Allow_Origin
    e_Access_Control_Expose_Headers,            ///< Access_Control_Expose_Headers
    e_Access_Control_Max_Age,                   ///< Access_Control_Max_Age
    e_Access_Control_Request_Headers,           ///< Access_Control_Request_Headers
    e_Access_Control_Request_Method,            ///< Access_Control_Request_Method
    e_Age,                                      ///< Age
    e_Allow,                                    ///< Allow
    e_ALPN,                                     ///< ALPN
    e_Alt_Svc,                                  ///< Alt_Svc
    e_Alt_Used,                                 ///< Alt_Used
    e_Alternates,                               ///< Alternates
    e_AMP_Cache_Transform,                      ///< AMP_Cache_Transform
    e_Apply_To_Redirect_Ref,                    ///< Apply_To_Redirect_Ref
    e_Authentication_Control,                   ///< Authentication_Control
    e_Authentication_Info,                      ///< Authentication_Info
    e_Authorization,                            ///< Authorization
    e_C_Ext,                                    ///< C_Ext
    e_C_Man,                                    ///< C_Man
    e_C_Opt,                                    ///< C_Opt
    e_C_PEP,                                    ///< C_PEP
    e_C_PEP_Info,                               ///< C_PEP_Info
    e_Cache_Control,                            ///< Cache_Control
    e_Cache_Status,                             ///< Cache_Status
    e_Cal_Managed_ID,                           ///< Cal_Managed_ID
    e_CalDAV_Timezones,                         ///< CalDAV_Timezones
    e_Capsule_Protocol,                         ///< Capsule_Protocol
    e_CDN_Cache_Control,                        ///< CDN_Cache_Control
    e_CDN_Loop,                                 ///< CDN_Loop
    e_Cert_Not_After,                           ///< Cert_Not_After
    e_Cert_Not_Before,                          ///< Cert_Not_Before
    e_Clear_Site_Data,                          ///< Clear_Site_Data
    e_Client_Cert,                              ///< Client_Cert
    e_Client_Cert_Chain,                        ///< Client_Cert_Chain
    e_Close,                                    ///< Close
    e_Configuration_Context,                    ///< Configuration_Context
    e_Connection,                               ///< Connection ( possible values close or keep-alive ) by default close in http 1.0 keep-alive in http 1.1
    e_Content_Base,                             ///< Content_Base
    e_Content_Disposition,                      ///< Content_Disposition
    e_Content_Encoding,                         ///< Content_Encoding
    e_Content_ID,                               ///< Content_ID
    e_Content_Language,                         ///< Content_Language
    e_Content_Length,                           ///< Content_Length
    e_Content_Location,                         ///< Content_Location
    e_Content_MD5,                              ///< Content_MD5
    e_Content_Range,                            ///< Content_Range
    e_Content_Script_Type,                      ///< Content_Script_Type
    e_Content_Security_Policy,                  ///< Content_Security_Policy
    e_Content_Security_Policy_Report_Only,      ///< Content_Security_Policy_Report_Only
    e_Content_Style_Type,                       ///< Content_Style_Type
    e_Content_Type,                             ///< Content_Type
    e_Content_Version,                          ///< Content_Version
    e_Cookie,                                   ///< Cookie
    e_Cookie2,                                  ///< Cookie2
    e_Cross_Origin_Embedder_Policy,             ///< Cross_Origin_Embedder_Policy
    e_Cross_Origin_Embedder_Policy_Report_Only, ///< Cross_Origin_Embedder_Policy_Report_Only
    e_Cross_Origin_Opener_Policy,               ///< Cross_Origin_Opener_Policy
    e_Cross_Origin_Opener_Policy_Report_Only,   ///< Cross_Origin_Opener_Policy_Report_Only
    e_Cross_Origin_Resource_Policy,             ///< Cross_Origin_Resource_Policy
    e_DASL,                                     ///< DASL
    e_Date,                                     ///< Date
    e_DAV,                                      ///< DAV
    e_Default_Style,                            ///< Default_Style
    e_Delta_Base,                               ///< Delta_Base
    e_Depth,                                    ///< Depth
    e_Derived_From,                             ///< Derived_From
    e_Destination,                              ///< Destination
    e_Differential_ID,                          ///< Differential_ID
    e_Digest,                                   ///< Digest
    e_Early_Data,                               ///< Early_Data
    e_EDIINT_Features,                          ///< EDIINT_Features
    e_ETag,                                     ///< ETag
    e_Expect,                                   ///< Expect
    e_Expect_CT,                                ///< Expect_CT
    e_Expires,                                  ///< Expires
    e_Ext,                                      ///< Ext
    e_Forwarded,                                ///< Forwarded
    e_From,                                     ///< From
    e_GetProfile,                               ///< GetProfile
    e_Hobareg,                                  ///< Hobareg
    e_Host,                                     ///< Host
    e_HTTP2_Settings,                           ///< HTTP2_Settings
    e_If,                                       ///< If
    e_If_Match,                                 ///< If_Match
    e_If_Modified_Since,                        ///< If_Modified_Since
    e_If_None_Match,                            ///< If_None_Match
    e_If_Range,                                 ///< If_Range
    e_If_Schedule_Tag_Match,                    ///< If_Schedule_Tag_Match
    e_If_Unmodified_Since,                      ///< If_Unmodified_Since
    e_IM,                                       ///< IM
    e_Include_Referred_Token_Binding_ID,        ///< Include_Referred_Token_Binding_ID
    e_Isolation,                                ///< Isolation
    e_Keep_Alive,                               ///< Keep_Alive ( how the connection may be used to set a timeout and a maximum amount of requests ) can be timeout=5 (5 sec) | max=1000 (1000 max number or requests)
    e_Label,                                    ///< Label
    e_Last_Event_ID,                            ///< Last_Event_ID
    e_Last_Modified,                            ///< Last_Modified
    e_Link,                                     ///< Link
    e_Location,                                 ///< Location
    e_Lock_Token,                               ///< Lock_Token
    e_Man,                                      ///< Man
    e_Max_Forwards,                             ///< Max_Forwards
    e_Memento_Datetime,                         ///< Memento_Datetime
    e_Meter,                                    ///< Meter
    e_Method_Check,                             ///< Method_Check
    e_Method_Check_Expires,                     ///< Method_Check_Expires
    e_MIME_Version,                             ///< MIME_Version
    e_Negotiate,                                ///< Negotiate
    e_OData_EntityId,                           ///< OData_EntityId
    e_OData_Isolation,                          ///< OData_Isolation
    e_OData_MaxVersion,                         ///< OData_MaxVersion
    e_OData_Version,                            ///< OData_Version
    e_Opt,                                      ///< Opt
    e_Optional_WWW_Authenticate,                ///< Optional_WWW_Authenticate
    e_Ordering_Type,                            ///< Ordering_Type
    e_Origin,                                   ///< Origin
    e_Origin_Agent_Cluster,                     ///< Origin_Agent_Cluster
    e_OSCORE,                                   ///< OSCORE
    e_OSLC_Core_Version,                        ///< OSLC_Core_Version
    e_Overwrite,                                ///< Overwrite
    e_P3P,                                      ///< P3P
    e_PEP,                                      ///< PEP
    e_Pep_Info,                                 ///< Pep_Info
    e_PICS_Label,                               ///< PICS_Label
    e_Ping_From,                                ///< Ping_From
    e_Ping_To,                                  ///< Ping_To
    e_Position,                                 ///< Position
    e_Pragma,                                   ///< Pragma
    e_Prefer,                                   ///< Prefer
    e_Preference_Applied,                       ///< Preference_Applied
    e_Priority,                                 ///< Priority
    e_ProfileObject,                            ///< ProfileObject
    e_Protocol,                                 ///< Protocol
    e_Protocol_Info,                            ///< Protocol_Info
    e_Protocol_Query,                           ///< Protocol_Query
    e_Protocol_Request,                         ///< Protocol_Request
    e_Proxy_Authenticate,                       ///< Proxy_Authenticate
    e_Proxy_Authentication_Info,                ///< Proxy_Authentication_Info
    e_Proxy_Authorization,                      ///< Proxy_Authorization
    e_Proxy_Features,                           ///< Proxy_Features
    e_Proxy_Instruction,                        ///< Proxy_Instruction
    e_Proxy_Status,                             ///< Proxy_Status
    e_Public,                                   ///< Public
    e_Public_Key_Pins,                          ///< Public_Key_Pins
    e_Public_Key_Pins_Report_Only,              ///< Public_Key_Pins_Report_Only
    e_Range,                                    ///< Range
    e_Redirect_Ref,                             ///< Redirect_Ref
    e_Referer,                                  ///< Referer
    e_Referer_Root,                             ///< Referer_Root
    e_Refresh,                                  ///< Refresh
    e_Repeatability_Client_ID,                  ///< Repeatability_Client_ID
    e_Repeatability_First_Sent,                 ///< Repeatability_First_Sent
    e_Repeatability_Request_ID,                 ///< Repeatability_Request_ID
    e_Repeatability_Result,                     ///< Repeatability_Result
    e_Replay_Nonce,                             ///< Replay_Nonce
    e_Retry_After,                              ///< Retry_After
    e_Safe,                                     ///< Safe
    e_Schedule_Reply,                           ///< Schedule_Reply
    e_Schedule_Tag,                             ///< Schedule_Tag
    e_Sec_GPC,                                  ///< Sec_GPC
    e_Sec_Purpose,                              ///< Sec_Purpose
    e_Sec_Token_Binding,                        ///< Sec_Token_Binding
    e_Sec_WebSocket_Accept,                     ///< Sec_WebSocket_Accept
    e_Sec_WebSocket_Extensions,                 ///< Sec_WebSocket_Extensions
    e_Sec_WebSocket_Key,                        ///< Sec_WebSocket_Key
    e_Sec_WebSocket_Protocol,                   ///< Sec_WebSocket_Protocol
    e_Sec_WebSocket_Version,                    ///< Sec_WebSocket_Version
    e_Security_Scheme,                          ///< Security_Scheme
    e_Server,                                   ///< Server
    e_Server_Timing,                            ///< Server_Timing
    e_Set_Cookie,                               ///< Set_Cookie
    e_Set_Cookie2,                              ///< Set_Cookie2
    e_SetProfile,                               ///< SetProfile
    e_SLUG,                                     ///< SLUG
    e_SoapAction,                               ///< SoapAction
    e_Status_URI,                               ///< Status_URI
    e_Strict_Transport_Security,                ///< Strict_Transport_Security
    e_Sunset,                                   ///< Sunset
    e_Surrogate_Capability,                     ///< Surrogate_Capability
    e_Surrogate_Control,                        ///< Surrogate_Control
    e_TCN,                                      ///< TCN
    e_TE,                                       ///< TE
    e_Timeout,                                  ///< Timeout
    e_Timing_Allow_Origin,                      ///< Timing_Allow_Origin
    e_Topic,                                    ///< Topic
    e_Traceparent,                              ///< Traceparent
    e_Tracestate,                               ///< Tracestate
    e_Trailer,                                  ///< Trailer
    e_Transfer_Encoding,                        ///< Transfer_Encoding
    e_TTL,                                      ///< TTL
    e_Upgrade,                                  ///< Upgrade
    e_Urgency,                                  ///< Urgency
    e_URI,                                      ///< URI
    e_User_Agent,                               ///< User_Agent
    e_Variant_Vary,                             ///< Variant_Vary
    e_Vary,                                     ///< Vary
    e_Via,                                      ///< Via
    e_Want_Digest,                              ///< Want_Digest
    e_Warning,                                  ///< Warning
    e_WWW_Authenticate,                         ///< WWW_Authenticate
    e_X_Content_Type_Options,                   ///< X_Content_Type_Options
    e_X_Frame_Options,                          ///< X_Frame_Options
    e_Star,                                     ///< Star, //
    e_Unknown_Header_Type                       ///< Unknown_Header_Type
};

/*! \brief convert header type to string representation
  * \param [in] type header type
  * \result string representation
  */
std::string_view to_string(types type);

/*! \brief convert header type as string to type
  * \param [in] type as string
  * \result right enum type or e_Unknown_Header_Type
  */
types to_type(std::string_view type);

}; // namespace bro::net::http::header

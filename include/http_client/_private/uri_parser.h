#pragma once
#include <string>
#include <uriparser/Uri.h>

namespace bro::net::http::client::_private {
/**
 * \brief wrapper for uri parser lib
 */
struct uri_parser {
    /**
   * default ctor
   */
    uri_parser() { _state.uri = &_uri; }

    /**
   * default dtor
   */
    ~uri_parser() { uriFreeUriMembersA(&_uri); }

    /*! \brief parse url
  * \param [in] url request type
  * \result uri parse result
  */
    int parse(std::string const &url) { return uriParseUriA(&_state, url.c_str()); }

    /*! \brief get url
  * \result uri
  */
    UriUriA const &get_uri() const { return _uri; }

    /*! \brief error to string converter
  * \result string representation of error
  */
    static char const *uri_parser_error_to_string(int error) {
        switch (error) {
        case URI_SUCCESS:
            return "Uri parse error: success";
        case URI_ERROR_SYNTAX:
            return "Uri parse error: Parsed text violates expected format";
        case URI_ERROR_NULL:
            return "Uri parse error: One of the params passed was NULL although it mustn't be";
        case URI_ERROR_MALLOC:
            return "Uri parse error: Requested memory could not be allocated";
        case URI_ERROR_OUTPUT_TOO_LARGE:
            return "Uri parse error: Some output is to large for the receiving buffer";
        case URI_ERROR_NOT_IMPLEMENTED:
            return "Uri parse error: The called function is not implemented yet";
        case URI_ERROR_RANGE_INVALID:
            return "Uri parse error: The parameters passed contained invalid ranges";
        case URI_ERROR_MEMORY_MANAGER_INCOMPLETE:
            return "Uri parse error: The UriMemoryManager passed does not implement all needed functions";
        case URI_ERROR_ADDBASE_REL_BASE:
            return "Uri parse error: Given base is not absolute";
        case URI_ERROR_REMOVEBASE_REL_BASE:
            return "Uri parse error: Given base is not absolute";
        case URI_ERROR_REMOVEBASE_REL_SOURCE:
            return "Uri parse error: Given base is not absolute";
        case URI_ERROR_MEMORY_MANAGER_FAULTY:
            return "Uri parse error: The UriMemoryManager given did not pass the test suite";
        }
        return "unknown error";
    }

private:
    UriUriA _uri{};           ///< uri
    UriParserStateA _state{}; ///< state
};

} // namespace bro::net::http::client::_private

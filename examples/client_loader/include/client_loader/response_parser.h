#pragma once
#include <quill/Logger.h>
#include <llhttp.h>
#include <http_client/zlib_helper.h>

namespace bro::net::http::client::loader {

class response_parser {
public:

    enum class status {
        e_continue,
        e_error,
        e_finish,
    };

    explicit response_parser(quill::Logger* logger) ;
    status process_data(std::byte *buffer, size_t buffer_size);
private:

    static int on_url(llhttp_t *parser, char const *at, size_t length) ;
    static int on_method(llhttp_t *parser, char const *at, size_t length);
    static int on_body(llhttp_t *parser, char const *at, size_t length);
    static int on_version(llhttp_t *parser, char const *at, size_t length);
    static int on_header_field(llhttp_t *parser, char const *at, size_t length);
    static int on_header_value(llhttp_t *parser, char const *at, size_t length);
    static int handle_on_message_complete(llhttp_t *h);

    status _status{status::e_continue};
    quill::Logger* _logger = nullptr;
    llhttp_t _parser;                                                      ///< http parser
    llhttp_settings_t _parser_settings{};                                  ///< settings for parser http
};

} // namespace bro::net::http::client::loader

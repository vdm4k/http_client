#include <client_loader/response_parser.h>
#include <quill/detail/LogMacros.h>

namespace bro::net::http::client::loader {

response_parser::response_parser(quill::Logger* logger) : _logger(logger)  {
    /* Initialize user callbacks and settings */
    llhttp_settings_init(&_parser_settings);
    /* Set user callback */
    _parser_settings.on_message_complete = handle_on_message_complete;
    _parser_settings.on_url = on_url;
    _parser_settings.on_method = on_method;
    _parser_settings.on_body = on_body;
    _parser_settings.on_version = on_version;
    _parser_settings.on_header_field = on_header_field;
    _parser_settings.on_header_value = on_header_value;
    llhttp_init(&_parser, HTTP_REQUEST, &_parser_settings);
    _parser.data = this;
    _status = status::e_continue;
}

response_parser::status response_parser::process_data(std::byte *buffer, size_t buffer_size) {
    auto err = llhttp_execute(&_parser, (char *)buffer, buffer_size);
    if (err != HPE_OK) {
        LOG_ERROR(_logger, "Parse error: %s %s", llhttp_errno_name(err), _parser.reason);
        _status = status::e_error;
    }
    return _status;
}

int response_parser::on_url(llhttp_t *parser, char const *at, size_t length) {
    return 0;
}

int response_parser::on_method(llhttp_t *parser, char const *at, size_t length) {
    return 0;
}


int response_parser::on_body(llhttp_t *parser, char const *at, size_t length) {
    return 0;
}

int response_parser::on_version(llhttp_t *parser, char const *at, size_t length) {
    return 0;
}

int response_parser::on_header_field(llhttp_t *parser, char const *at, size_t length) {
    return 0;
}

int response_parser::on_header_value(llhttp_t *parser, char const *at, size_t length) {
    return 0;
}

int response_parser::handle_on_message_complete(llhttp_t *h) {
    if (!h->data)
        return -1;
    response_parser *resp = (response_parser *)h->data;
    resp->_status = status::e_finish;
    return 0;
}

} // namespace bro::net::http::client::loader

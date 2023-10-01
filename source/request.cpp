#include <http_client/request.h>
#include <http_client/_private/request.h>


namespace bro::net::http::client {

request::request() : request(std::make_shared<bro::net::ev::factory>(), std::make_shared<dns::resolver>(net::dns::resolver::config{}))  {
}

request::request(std::shared_ptr<bro::strm::factory> const &factory, std::shared_ptr<dns::resolver> const & resolver) :
    _request(std::make_unique<_private::request>(factory, resolver)) {}

request::request(request &&req) : _request(std::move(req._request)) {}

request& request::operator=(request &&req) {
    std::swap(_request, req._request);
    return *this;
}

request::~request() {}

bool request::send(type tp, std::string const & url, result &&res, config *conf) {
    return _request->send(tp, url, std::move(res), conf);
}

void request::add_header(std::string_view const &type, std::string_view const &value)  {
    _request->add_header(type, value);
}

void request::add_header(std::string const &type, std::string const &value)  {
    _request->add_header(type, value);
}

void request::add_body(std::string const &body, std::string const &content_type, bool do_compress) {
    _request->add_body(body, content_type, do_compress);
}

void request::add_body(std::string_view const &body, std::string const &content_type, bool do_compress) {
    _request->add_body(body, content_type, do_compress);
}

void request::proceed()  {
    _request->proceed_logic();
    _request->proceed_events();
}

request_state request::get_state() const {
    return _request->get_state();
}

void request::soft_reset() {
    _request->soft_reset();
}

void request::reset() {
    _request->reset();
}

std::string_view request::to_string(type tp) {
    switch (tp) {
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
    case type::e_Unknown_Type:
        return "Unknown Type";
    }
    return "";
}

request::type request::to_type(std::string_view const &tp) {
    static std::unordered_map<std::string_view, type> umap{
                                                           {to_string(type::e_GET), type::e_GET},
                                                           {to_string(type::e_HEAD), type::e_HEAD},
                                                           {to_string(type::e_POST), type::e_POST},
                                                           {to_string(type::e_PUT), type::e_PUT},
                                                           {to_string(type::e_DELETE), type::e_DELETE},
                                                           {to_string(type::e_CONNECT), type::e_CONNECT},
                                                           {to_string(type::e_OPTIONS), type::e_OPTIONS},
                                                           {to_string(type::e_TRACE), type::e_TRACE},
                                                           };
    if (auto it = umap.find(tp); it != umap.end())
        return it->second;
    return request::type::e_Unknown_Type;
}

} // namespace bro::net::http::client

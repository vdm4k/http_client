#include <http_client/response.h>

namespace bro::net::http {

status::code response::get_status_code() const {
  return _status_code;
}

header::version response::get_version() const {
  return _version;
}

std::string_view response::get_body() const {
  return _body;
}

std::vector<response::header_data> const &response::get_headers() const {
  return _headers;
}
} // namespace bro::net::http

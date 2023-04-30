#include <http_client/response.h>

namespace bro::net::http {

status::code response::get_status_code() const {
  return _status_code;
}

header::version response::get_version() const {
  return _version;
}

std::string_view response::get_body() const {
  if (_body.size)
    return {_full_message.data() + _body.start, _body.size};
  return {};
}

std::string const &response::get_full_message() const noexcept {
  return _full_message;
}

std::vector<response::header_data> response::get_headers() const {
  std::vector<header_data> res;
  for (auto const &h : _headers)
    res.push_back({h._type, {_full_message.data() + h.start, h.size}});
  return res;
}
} // namespace bro::net::http

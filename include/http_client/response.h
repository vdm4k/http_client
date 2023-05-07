#pragma once
#include <http_client/header.h>
#include <http_client/status.h>
#include <optional>
#include <string>
#include <vector>

namespace bro::net::http {

class response {
public:
  struct header_data {
    std::string _type;
    std::string _value;
  };

  status::code get_status_code() const;
  header::version get_version() const;
  std::string_view get_body() const;
  std::vector<header_data> const &get_headers() const;

private:
  friend class request;
  status::code _status_code{status::code::e_Unknown_Code};
  header::version _version{header::version::e_Unknown_Version};
  std::vector<header_data> _headers;
  std::string _body;
  bool _is_gzip_encoded{false};
};
} // namespace bro::net::http

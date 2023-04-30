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
    header::types _type;
    std::string_view _value;
  };

  status::code get_status_code() const;
  header::version get_version() const;
  std::string_view get_body() const;
  std::vector<header_data> get_headers() const;
  std::string const &get_full_message() const noexcept;

private:
  friend class request;
  struct data {
    size_t start = 0;
    size_t size = 0;
  };

  struct headers : data {
    header::types _type;
  };

  status::code _status_code{status::code::e_Unknown_Code};
  header::version _version{header::version::e_Unknown_Version};
  std::vector<headers> _headers;
  data _body;
  std::string _full_message;
};
} // namespace bro::net::http

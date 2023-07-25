#include "fmt/color.h"
#include "fmt/core.h"
#include <http_client/request.h>
#include <CLI/CLI.hpp>

void print_response(bro::net::http::client::response &resp) {
  fmt::print(fg(fmt::color::dark_green) | fmt::emphasis::bold,"---------- HTTP Response ----------\n");
  fmt::print("statuc code: {}\n", bro::net::http::status::to_string(resp.get_status_code()));
  fmt::print("version: {}\n", bro::net::http::header::to_string(resp.get_version()));
  fmt::print("---------- headers ----------\n");
  for (auto const &hdr : resp.get_headers()) {
      fmt::print("{}: {}\n", hdr._type, hdr._value);
  }
  fmt::print("---------- body ----------\n{}\n", resp.get_body());
}

int main(int argc, char **argv) {
  CLI::App app{"http_client"};
  std::string url{};
  app.add_option("-u,--url", url, "full url");
  CLI11_PARSE(app, argc, argv);

  bool receive{false};

  bro::net::http::client::request request;
  request.send(bro::net::http::client::request::type::e_GET,
               url,
               {._cb =
                  [&receive](bro::net::http::client::response &&resp, char const *const error, std::any /*user_data*/) {
                    receive = true;
                    if (error) {
                      fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold,"request failed with error {}\n", error);
                    } else {
                      print_response(resp);
                    }
                  },
                ._data = nullptr});
  while (!receive)
    request.proceed();
  return 0;
}

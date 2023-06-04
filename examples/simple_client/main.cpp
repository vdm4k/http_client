#include <http_client/request.h>
#include <CLI/CLI.hpp>
#include <iostream>

void print_response(bro::net::http::client::response &resp) {
  std::cout << "---------- HTTP Response ----------\n";
  std::cout << "statuc code: " << bro::net::http::status::to_string(resp.get_status_code()) << "\n";
  std::cout << "version: " << bro::net::http::header::to_string(resp.get_version()) << "\n";
  std::cout << "-------headers-------\n";
  for (auto const &hdr : resp.get_headers()) {
    std::cout << hdr._type << ": " << hdr._value << "\n";
  }
  std::cout << "-------body-------\n";
  std::cout << resp.get_body() << "\n";
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
                      std::cout << error << std::endl;
                    } else {
                      print_response(resp);
                    }
                  },
                ._data = nullptr});
  while (!receive)
    request.proceed();
  return 0;
}

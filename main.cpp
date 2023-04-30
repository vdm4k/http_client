#include <http_client/request.h>
#include <iostream>

void print_response(bro::net::http::response &resp) {
  std::cout << "---------- HTTP Response ----------\n";
  std::cout << "statuc code: " << bro::net::http::status::to_string(resp.get_status_code()) << "\n";
  std::cout << "version: " << bro::net::http::header::to_string(resp.get_version()) << "\n";
  auto headers = resp.get_headers();
  for (auto const hdr : headers) {
    std::cout << "header: " << bro::net::http::header::to_string(hdr._type) << ": " << hdr._value << "\n";
  }
  std::cout << "body: " << resp.get_body() << "\n";
  std::cout << "\n\n---------- Debug output ----------\n";
  std::cout << "full message is: " << resp.get_full_message() << "\n";
}

int main(int /*argc*/, char ** /*argv*/) {
  bool receive{false};

  bro::net::http::request request(bro::net::http::request::type::e_GET,
                                  "https://mobile-review.com/all/",
                                  {._cb =
                                     [&receive](bro::net::http::response &&resp,
                                                char const *const error,
                                                std::any /*user_data*/) {
                                       receive = true;
                                       if (error) {
                                         std::cout << "failed with error - " << error << std::endl;
                                       } else {
                                         print_response(resp);
                                       }
                                     },
                                   ._data = nullptr});
  request.add_header(
    bro::net::http::header::types::e_Accept_Encoding,
    "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/"
    "signed-exchange;v=b3;q=0.7");

  request.send();
  while (!receive)
    request.proceed();
  return 0;
}

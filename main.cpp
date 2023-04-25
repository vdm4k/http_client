#include <http_client/request.h>
#include <iostream>

int main(int argc, char **argv) {
  bool receive{false};
  bro::net::http::request request(bro::net::http::request::type::e_GET,
                                  "https://mobile-review.com/all/",
                                  // "https://mobile-review.com/all/",
                                  [&receive](std::string const &response, char const *const error, std::any user_data) {
                                    receive = true;
                                    std::cout << response << std::endl;
                                  });
  request.add_header(
    bro::net::http::header::types::e_Accept_Encoding,
    "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/"
    "signed-exchange;v=b3;q=0.7");

  request.send();
  while (!receive)
    request.proceed();
  return 0;
}

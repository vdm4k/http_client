#include <http_client/request.h>
#include <gtest/gtest.h>

namespace bro::net::http::client::test {

TEST(url_tests, host_not_set) {
  request req;
  bool error_set = false;
  req.send(request::type::e_GET,
               "http:",
               {._cb =
                  [&](response &&resp, char const *const error, std::any /*user_data*/) {
                    if (error)
                      error_set = true;
                  },
                ._data = nullptr});
  EXPECT_TRUE(error_set);
}

TEST(url_tests, sytnax_error) {
  request req;
  bool error_set = false;
  req.send(request::type::e_GET,
               "http: blah",
               {._cb =
                  [&](response &&resp, char const *const error, std::any /*user_data*/) {
                    if (error)
                      error_set = true;
                  },
                ._data = nullptr});
  EXPECT_TRUE(error_set);
}

TEST(url_tests, url_without_scheme) {
  request req;
  bool error_set = false;
  req.send(request::type::e_GET,
               "//www.google.com",
               {._cb =
                  [&](response &&resp, char const *const error, std::any /*user_data*/) {
                    if (error)
                      error_set = true;
                  },
                ._data = nullptr});
  EXPECT_TRUE(error_set);
}

TEST(url_tests, url_without_port) {
  request req;
  bool error_set = false;
  req.send(request::type::e_GET,
               "https://127.0.0.1:/",
               {._cb =
                  [&](response &&resp, char const *const error, std::any /*user_data*/) {
                    if (error)
                      error_set = true;
                  },
                ._data = nullptr});
  EXPECT_TRUE(error_set);
}

} // bro::net::http::client::test

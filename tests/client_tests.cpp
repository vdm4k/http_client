#include <http_client/request.h>
#include <gtest/gtest.h>

TEST(url_tests, host_not_set) {
  bro::net::http::request request;
  bool error_set = false;
  request.send(bro::net::http::request::type::e_GET,
               "http:",
               {._cb =
                  [&](bro::net::http::response &&resp, char const *const error, std::any /*user_data*/) {
                    if (error)
                      error_set = true;
                  },
                ._data = nullptr});
  EXPECT_TRUE(error_set);
}

TEST(url_tests, sytnax_error) {
  bro::net::http::request request;
  bool error_set = false;
  request.send(bro::net::http::request::type::e_GET,
               "http: blah",
               {._cb =
                  [&](bro::net::http::response &&resp, char const *const error, std::any /*user_data*/) {
                    if (error)
                      error_set = true;
                  },
                ._data = nullptr});
  EXPECT_TRUE(error_set);
}

TEST(url_tests, url_without_scheme) {
  bro::net::http::request request;
  bool error_set = false;
  request.send(bro::net::http::request::type::e_GET,
               "//www.google.com",
               {._cb =
                  [&](bro::net::http::response &&resp, char const *const error, std::any /*user_data*/) {
                    if (error)
                      error_set = true;
                  },
                ._data = nullptr});
  EXPECT_TRUE(error_set);
}

TEST(url_tests, url_without_port) {
  bro::net::http::request request;
  bool error_set = false;
  request.send(bro::net::http::request::type::e_GET,
               "https://127.0.0.1:/",
               {._cb =
                  [&](bro::net::http::response &&resp, char const *const error, std::any /*user_data*/) {
                    if (error)
                      error_set = true;
                  },
                ._data = nullptr});
  EXPECT_TRUE(error_set);
}

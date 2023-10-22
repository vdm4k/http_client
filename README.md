# HTTP client library

Http client library is a library that can be used to write http client ( examples are included )
Main focus was on performance and test writing
Depends on hardware and protocol you can have tens or hundreds of thousands RPS(requests per second) per thread (handler)
Writing tests also easy. You can use gtest/gmock and implement your own factory that will create streams
This streams can simulate different problems on network and you can check how your software handle it

## ***Build***

You need cmake

1. mkdir build
2. cd build
3. cmake ../
   1. cmake build options 
      1. (optional) build examples *-DWITH_EXAMPLES=ON* 
      2. (optional) with sanitazer support *-DWITH_SANITIZER=ON* 
4. make 

## Design

HTTP client library is asynchronous library. It can be used in different scenarion multi-thread(loader app) or single thread (simple client)
It uses libev to check socket state. c-ares with wrapper to resolve dns and other libraries.

## Examples

### minimal application (can be found in simple client folder)

```cpp
   // create flag for callback
   bool receive{false};

   bro::net::http::client::request request;
   request.send(bro::net::http::client::request::type::e_GET, // request type
               url,                                           // url (ex. https://localhost:8080/my_handler 
               {._cb =                                        // function to handle response or error
                  [&receive](bro::net::http::client::response &&resp, char const *const error, std::any /*user_data*/) {
                    receive = true;
                    if (error) {                              // colour only error
                      fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold,"request failed with error {}\n", error);
                    } else {
                      print_response(resp);                   // some function that will process response
                    }
                  },
                ._data = nullptr});                           // we can pass pointer on some user data per request. don't need it for example

   while (!receive)                                           // wait response or error
     request.proceed();                                       // send request in same thread => need proceed logic periodically
```

### Loader

This application can be used to test performance of some server.
It creates many loaders(threads) 
Every loader has its own pool of requests
We can set how many online connections per thread we want to see
Also we can create new stream for request or reuse active stream
In config.yaml we can set different types of parameters.

# Prerequisites

cmake, gcc, libssl-dev must be already installed
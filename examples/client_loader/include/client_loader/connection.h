#pragma once
#include "http_client/request.h"
#include "client_loader/response_parser.h"
#include <stream/stream.h>

namespace bro::net::http::client::loader {

class loader;

struct connection {
    loader * _loader = nullptr;
    request _request;
    client::loader::response_parser _transaction;
};

} // namespace bro::net::http::client::loader

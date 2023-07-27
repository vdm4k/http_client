#pragma once
#include "protocols/ip/address.h"
#include <string>
#include <vector>
#include <list>
#include <optional>
#include <chrono>
#include <quill/LogLevel.h>
#include <http_client/request.h>

namespace bro::net::http::client::loader::config {

struct request {
    std::string _path;
    std::string _body;
    std::string _body_type;
    bro::net::http::client::request::type _type;
};

struct loaders {
    size_t _total{0};
    size_t _online_connections{0};
    std::list<size_t> _core_ids;
    std::string _prefix_name;
    std::chrono::microseconds _sleep{100};
    size_t _call_sleep_on_n_empty_loop_in_a_row = 1000;
};

struct logger {
    quill::LogLevel _level{quill::LogLevel::Critical};
    std::optional<size_t> _core;
    std::string _file_name;
    std::string _thread_name;
};

struct client_loader {
    std::list<request> _requests;
    loaders _loaders;
    std::string _server_host_name;
    bro::net::proto::ip::address _server_address;
    bro::net::proto::ip::address::version _ver{bro::net::proto::ip::address::version::e_v4};
    logger _logger;
    bool _online_statistics = false;
    bool _reuse_connections = false;
    std::optional<std::chrono::seconds> _test_time;
    std::optional<size_t> _service_core_id;
};

std::optional<client_loader> parse(std::string const &path);

} // namespace bro::net::http::client::loader::config

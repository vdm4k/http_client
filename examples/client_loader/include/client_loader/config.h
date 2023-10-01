#pragma once
#include "http_client/common.h"
#include <string>
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
    std::chrono::microseconds _sleep{500};
    size_t _call_sleep_on_n_empty_loop_in_a_row = 10;
    std::chrono::milliseconds _flush_statistic{1000};
    bool _reuse_connections = true;
    bool _print_stat_per_loader = false;
};

struct logger {
    quill::LogLevel _level{quill::LogLevel::Critical};
    std::optional<size_t> _core_id;
    std::string _file_name;
    std::string _thread_name;
    std::string _logger_name;
};

struct server {
    std::string _hostname;
    bool _use_ipv6{false};
    bro::net::http::client::connection_type _connection_type{bro::net::http::client::connection_type::e_http};
    std::optional<uint16_t> _port;
};

struct client_loader {
    std::list<request> _requests;
    loaders _loaders;
    logger _logger;
    server _server;
    bool _online_statistics = false;
    std::optional<std::chrono::seconds> _test_time;
};

std::optional<client_loader> parse(std::string const &path);

} // namespace bro::net::http::client::loader::config

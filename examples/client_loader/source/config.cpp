#include <fstream>
#include <fmt/color.h>
#include <fmt/core.h>
#include <yaml-cpp/yaml.h>
#include <client_loader/config.h>

namespace YAML {

template<>
struct convert<bro::net::http::client::loader::config::request> {
    static Node encode(const bro::net::http::client::loader::config::request& rhs) {
        Node node;
        return node;
    }

    static bool decode(const Node& node, bro::net::http::client::loader::config::request& rhs) {

        if (node["path"]) {
            rhs._path = node["path"].as<std::string>();
        }

        if (node["method"]) {
            rhs._type =  bro::net::http::client::request::to_type(node["method"].as<std::string>());
        }

        if (node["body"]) {
            rhs._body = node["body"].as<std::string>();
        }
        return true;
    }
};


template<>
struct convert<bro::net::http::client::loader::config::loaders> {
    static Node encode(const bro::net::http::client::loader::config::loaders& rhs) {
        Node node;
        return node;
    }

    static bool decode(const Node& node, bro::net::http::client::loader::config::loaders& rhs) {

        if (node["prefix_name"]) {
            rhs._prefix_name = node["prefix_name"].as<std::string>();
        }

        if (node["total"]) {
            rhs._total = node["total"].as<size_t>();
        }

        if (node["total"]) {
            rhs._total = node["total"].as<size_t>();
        }

        if (node["online_connections"]) {
            rhs._online_connections = node["online_connections"].as<size_t>();
        }

        if (node["sleep"]) {
            rhs._sleep = std::chrono::microseconds(node["sleep"].as<size_t>());
        }

        if (node["call_sleep_on_n_empty_loop_in_a_row"]) {
            rhs._call_sleep_on_n_empty_loop_in_a_row = node["call_sleep_on_n_empty_loop_in_a_row"].as<size_t>();
        }

        if (node["flush_statistic"]) {
            rhs._flush_statistic = std::chrono::milliseconds(node["flush_statistic"].as<size_t>());
        }

        if (node["core_ids"]) {
            auto core_ids = node["core_ids"];
            for(auto it = core_ids.begin(); it != core_ids.end(); ++it) {
                rhs._core_ids.push_back(it->as<size_t>());
            }
        }
        return true;
    }
};


template<>
struct convert<bro::net::http::client::loader::config::logger> {
    static Node encode(const bro::net::http::client::loader::config::logger& rhs) {
        Node node;
        return node;
    }

    static bool decode(const Node& node, bro::net::http::client::loader::config::logger& rhs) {

        if (node["thread_name"]) {
            rhs._thread_name = node["thread_name"].as<std::string>();
        }

        if (node["logger_name"]) {
            rhs._logger_name = node["logger_name"].as<std::string>();
        }

        if (node["core_id"]) {
            rhs._core_id = node["core_id"].as<size_t>();
        }

        if (node["level"]) {
            rhs._level = quill::loglevel_from_string(node["level"].as<std::string>());
        }
        return true;
    }
};


template<>
struct convert<bro::net::http::client::loader::config::server> {
    static Node encode(const bro::net::http::client::loader::config::server& rhs) {
        Node node;
        return node;
    }

    static bool decode(const Node& node, bro::net::http::client::loader::config::server& rhs) {

        if (node["hostname"]) {
            rhs._hostname = node["hostname"].as<std::string>();
        }

        if (node["use_ipv6"]) {
            rhs._use_ipv6 = node["use_ipv6"].as<bool>();
        }

        if (node["port"]) {
            rhs._port = node["port"].as<uint16_t>();
        }

        if (node["reuse_connections"]) {
            rhs._reuse_connections = node["reuse_connections"].as<bool>();
        }

        if (node["connection_type"]) {
            auto connection_type(node["connection_type"].as<std::string>());
            if(connection_type == "http") {
                rhs._connection_type = bro::net::http::client::connection_type::e_http;
            } else if (connection_type == "https") {
                rhs._connection_type = bro::net::http::client::connection_type::e_https;
            } else {
                fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold,"incorrect connection_type {}, must be http or https \n", connection_type);
                return false;
            }            
        }
        return true;
    }
};

template<>
struct convert<bro::net::http::client::loader::config::client_loader> {
    static Node encode(const bro::net::http::client::loader::config::client_loader& rhs) {
        Node node;
        return node;
    }

    static bool decode(const Node& node, bro::net::http::client::loader::config::client_loader& rhs) {

        if (node["test_time"])
            rhs._test_time = std::chrono::seconds(node["test_time"].as<size_t>());

        if (node["online_statistics"])
            rhs._online_statistics = node["online_statistics"].as<bool>();

        if (node["server"])
            rhs._server = node["server"].as<bro::net::http::client::loader::config::server>();
        else {
            fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold,"section server must be set\n");
            return false;
        }

        if (node["logger"])
            rhs._logger = node["logger"].as<bro::net::http::client::loader::config::logger>();

        if (node["loaders"])
            rhs._loaders = node["loaders"].as<bro::net::http::client::loader::config::loaders>();

        if (node["requests"]) {
            auto req = node["requests"];
            for(auto it = req.begin(); it != req.end(); ++it) {
                rhs._requests.push_back(it->as<bro::net::http::client::loader::config::request>());
            }
        }

        return true;
    }
};

} // namespace YAML

namespace bro::net::http::client::loader::config {

std::optional<client_loader> parse(std::string const &config_path) {
    std::ifstream fin(config_path);
    if(!fin.is_open()) {
        fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold,"couldn't open config file, path to file {}\n", config_path);
        return {};
    }

    YAML::Node doc;
    try {
        doc = YAML::Load(fin);
    } catch (YAML::ParserException &error) {
        fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold,"config file {}, parsing error {}\n", config_path, error.msg);
        return {};
    }

    return doc.as<config::client_loader>();
}

} // namespace bro::net::http::client::loader::config

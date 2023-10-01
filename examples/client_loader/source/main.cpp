#include <thread>
#include <dns/resolver.h>
#include <fmt/color.h>
#include <CLI/CLI.hpp>
#include <protocols/ip/address.h>
#include <client_loader/config.h>
#include <client_loader/manager.h>
#include <network/platforms/system.h>

std::optional<bro::net::proto::ip::address> resolve_host(std::string const & host_name, bro::net::proto::ip::address::version proto_ver) {

    std::optional<bro::net::proto::ip::address> server_addr;
    bool resolved{false};
    bro::net::dns::resolver resolver({});
    resolver.resolve(host_name, proto_ver,
                     [&server_addr, &resolved](bro::net::proto::ip::address const &addr, std::string const &hostname, char const *err) {
                         resolved = true;
                         if (err) {
                             fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold,"dns request failed with error {}\n", err);
                         } else {
                             server_addr = addr;
                         }
                     });

    while(!resolved) {
        resolver.proceed();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return server_addr;
}

int main(int argc, char **argv) {
    CLI::App app{"http_client_loader"};
    std::string config_path{"/home/vdm4k/projects/http_client/examples/client_loader/config/config.yaml"};
    app.add_option("-c,--config", config_path, "path to config file");
    CLI11_PARSE(app, argc, argv);

    bro::net::disable_sig_pipe();

    auto config = bro::net::http::client::loader::config::parse(config_path);
    if(!config) 
        return -1;

    auto server_addr = resolve_host(config->_server._hostname, config->_server._use_ipv6 ? bro::net::proto::ip::address::version::e_v6 : bro::net::proto::ip::address::version::e_v4);
    if(!server_addr) 
        return -1;

    switch (config->_server._connection_type) {
    case bro::net::http::client::connection_type::e_http: {
        uint16_t port = config->_server._port ? *config->_server._port : 80;
        config->_server._hostname = "http://" + server_addr->to_string() + ":" + std::to_string(port);
        break;
    }
    case bro::net::http::client::connection_type::e_https: {
        uint16_t port = config->_server._port ? *config->_server._port : 443;
        config->_server._hostname = "https://" + server_addr->to_string() + ":" + std::to_string(port);
        break;
    }
    default:
        break;
    }

    bro::net::http::client::loader::manager manager(*config);
    manager.serve();
    return 0;
}

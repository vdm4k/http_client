#pragma once
#include <client_loader/config.h>
#include <client_loader/loader.h>

namespace bro::net::http::client::loader {

class manager {
public:

    manager(config::client_loader & config);
    void serve();

private:

    void init_logger();
    void start_loaders();
    void init_loaders();

    std::vector<std::unique_ptr<loader>> _loaders;
    quill::Logger* _logger = nullptr;
    config::client_loader _config;
};

} // namespace bro::net::http::client::loader

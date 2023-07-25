#pragma once
#include <network/tcp/ssl/send/settings.h>
#include <quill/Logger.h>
#include <system/thread/thread.h>
#include <http_client/common.h>
#include <network/stream/factory.h>
#include <client_loader/config.h>
#include <client_loader/connection.h>

namespace bro::net::http::client::loader {

class loader {
public:


    loader(config::loaders const& conf, std::list<request> const &requests, quill::Logger* logger, std::shared_ptr<bro::strm::settings> settings);

private:

    void pre_start();
    void post_end();
    bool serve();
    void logic_proceed();
//    void process_new_stream(strm::stream_ptr &&stream);
//    bool compress_body(response & resp);
//    static void parse_result_cb(request &req, std::any user_data, char const *error);

    config::loaders _config;
    quill::Logger* _logger = nullptr;
    std::list<std::unique_ptr<request>> _free_connections;
    std::list<std::unique_ptr<request>> _active_connections;
//    std::unordered_map<bro::strm::stream*, std::unique_ptr<connection>> _streams;
//    std::unordered_map<bro::strm::stream*, std::unique_ptr<connection>> _streams;
//    std::set<connection *> _failed_connections;
    std::shared_ptr<bro::strm::factory> _factory;
    std::shared_ptr<dns::resolver> _resolver;
    system::thread::thread _thread;
    std::list<request> _requests;
    size_t _processed_data{0};
};

} // namespace bro::net::http::client::loader::config

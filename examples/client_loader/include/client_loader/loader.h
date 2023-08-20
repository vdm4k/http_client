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

    struct statistic {
        size_t _success_requests = 0;
        size_t _failed_requests = 0;
        std::chrono::duration<double> _total_time{0};
        std::chrono::duration<double> _max_time{0};
        system::thread::statistic _statistic;
    };


    loader(config::loaders const& conf, std::list<config::request> const &requests, std::string const &url_prefix, quill::Logger* logger);
    ~loader();

    statistic get_statistic() noexcept;
    std::string const & get_name() noexcept;

private:

    struct stat {
        std::chrono::steady_clock::time_point _start{};
    };

    struct node {
        stat _stat;
        std::unique_ptr<request> _request;
    };

    void pre_start();
    void post_end();
    bool serve();
    void logic_proceed();
    void send_request(std::list<node>::iterator it, config::request const & conf);
    void flush_statistic();
    void copy_statistic(statistic *to, statistic *from) noexcept;
    void check_statistic();
    void activate_new_connnection();
//    void process_new_stream(strm::stream_ptr &&stream);
//    bool compress_body(response & resp);
//    static void parse_result_cb(request &req, std::any user_data, char const *error);

    statistic _prev_statistic;
    statistic _actual_statistic;
    size_t _processed_events{0};
    config::loaders _config;
    quill::Logger* _logger = nullptr;
    std::list<node> _free_connections;
    std::list<node> _active_connections;
//    std::unordered_map<bro::strm::stream*, std::unique_ptr<connection>> _streams;
//    std::unordered_map<bro::strm::stream*, std::unique_ptr<connection>> _streams;
//    std::set<connection *> _failed_connections;
    std::string _url_prefix;
    std::shared_ptr<bro::strm::factory> _factory;
    std::shared_ptr<dns::resolver> _resolver;
    system::thread::thread _thread;
    std::list<config::request> _request_configs;
    std::chrono::steady_clock::time_point _last_flush;
    std::atomic_bool _write_stat{false}; ///< flag for write statistic
};

} // namespace bro::net::http::client::loader::config

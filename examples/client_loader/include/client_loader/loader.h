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

        statistic& merge(statistic const & lval) {
            _success_requests += lval._success_requests;
            _failed_requests += lval._failed_requests;
            _activated_connections += lval._activated_connections;
            _reuse_connections += lval._reuse_connections;

            _max_request_time = std::max(_max_request_time, lval._max_request_time);
            _statistic._loops = std::max(_statistic._loops, lval._statistic._loops);
            _statistic._max_main_function_time = std::max(_statistic._max_main_function_time, lval._statistic._max_main_function_time);
            _statistic._max_logic_function_time = std::max(_statistic._max_logic_function_time, lval._statistic._max_logic_function_time);
            _statistic._busy_time = std::max(_statistic._busy_time, lval._statistic._busy_time);
            _statistic._empty_loops = std::max(_statistic._empty_loops, lval._statistic._empty_loops);
            return *this;
        }

        size_t _success_requests = 0;
        size_t _failed_requests = 0;
        size_t _activated_connections = 0;
        size_t _reuse_connections = 0;
        std::chrono::duration<double> _max_request_time{0};
        system::thread::statistic _statistic;
    };


    loader(config::loaders const& conf, std::list<config::request> const &requests, std::string const &url_prefix, quill::Logger* logger);
    ~loader();
    /**
     * \brief Get actual statistic
     * 
     * \return statistic 
     */
    statistic get_statistic() noexcept;

    /**
     * \brief Get the name of loader
     * 
     * \return std::string const& 
     */
    std::string const & get_name() noexcept;

private:

    struct node {
        std::chrono::steady_clock::time_point _start_request{};
        std::unique_ptr<request> _request;
    };

    void pre_start();
    void post_end();
    bool serve();
    void logic_proceed();
    bool send_request(std::list<node>::iterator it, config::request const & conf);
    void flush_statistic();
    void copy_statistic(statistic *to, statistic *from) noexcept;
    void check_statistic();
    void activate_new_connnection();

    statistic _prev_statistic;
    statistic _actual_statistic;
    size_t _processed_events{0};
    config::loaders _config;
    quill::Logger* _logger = nullptr;
    std::list<node> _free_connections;
    std::list<node> _active_connections;
    std::string _url_prefix;
    std::shared_ptr<bro::strm::factory> _factory;
    std::shared_ptr<dns::resolver> _resolver;
    system::thread::thread _thread;
    std::list<config::request> _request_configs;
    std::chrono::steady_clock::time_point _last_flush;
    std::atomic_bool _write_stat{false}; ///< flag for write statistic
};

} // namespace bro::net::http::client::loader::config

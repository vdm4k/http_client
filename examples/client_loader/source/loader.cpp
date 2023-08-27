#include <client_loader/loader.h>
#include <quill/detail/LogMacros.h>

namespace bro::net::http::client::loader {

loader::loader(config::loaders const & conf, const std::list<config::request> &requests, const std::string &url_prefix, quill::Logger* logger)  :
    _config(conf),
    _request_configs(requests),
    _logger(logger),
    _url_prefix(url_prefix),
    _factory(std::make_shared<bro::net::ev::factory>()),
    _resolver(std::make_shared<dns::resolver>(dns::resolver::config{})) {

    for(size_t i = 0; i < _config._online_connections; ++i) {
        _free_connections.push_back({{}, std::make_unique<request>(_factory, _resolver)});
    }

    _last_flush = std::chrono::steady_clock::now();
    system::thread::config config;
    config._sleep = _config._sleep;
    config._flush_statistic = _config._flush_statistic;
    config._call_sleep_on_n_empty_loop_in_a_row = conf._call_sleep_on_n_empty_loop_in_a_row;

    _thread.run_with_logic_pre_post(system::thread::callable(&loader::serve, this),
                                    system::thread::callable(&loader::logic_proceed, this),
                                    system::thread::callable(&loader::pre_start, this),
                                    system::thread::callable(&loader::post_end, this),
                                    &config);
}

loader::~loader() {
    _thread.stop();
}

std::string const & loader::get_name() noexcept {
    return _thread.get_name();
}

bool loader::serve() {
    //    _resolver->proceed();
    _factory->proceed();
    size_t processed_events = _processed_events;
    _processed_events = 0;
    return processed_events;
}

void loader::logic_proceed() {
    activate_new_connnection();
    check_statistic();
}

void loader::pre_start() {
    _thread.set_name(_config._prefix_name);
    if(!_config._core_ids.empty()) {
        _thread.set_affinity({_config._core_ids.front()});
    }

    LOG_INFO(_logger, "Start loader {}", _thread.get_name());
}

void loader::post_end() {
    LOG_INFO(_logger, "Stop loader {}", _thread.get_name());
}

void loader::send_request(std::list<node>::iterator it, config::request const & conf) {
    auto &req_node = *it;

    req_node._request->proceed(); // just cleanup if needed
    if(!conf._body.empty() && !conf._body_type.empty())
        req_node._request->add_body(conf._body, conf._body_type);

    req_node._stat._start = std::chrono::steady_clock::now();
    request::config req_conf;
    req_conf._processed_events = &_processed_events;
    req_node._request->send(conf._type,
                            _url_prefix + conf._path,
                            {._cb =
                             [&](bro::net::http::client::response &&resp, char const *const error, std::any user_data) {
                                 auto it_con = std::any_cast<std::list<node>::iterator>(user_data);
                                 if (error) {
                                     _actual_statistic._failed_requests++;
                                     LOG_ERROR(_logger, "request failed with error {}\n", error);
                                 } else {
                                     std::chrono::duration<double> diff = std::chrono::steady_clock::now() - it_con->_stat._start;
                                     _actual_statistic._success_requests++;
                                     _actual_statistic._max_time = std::max(_actual_statistic._max_time, diff);
                                     _actual_statistic._total_time += diff;
                                 }
                                 _free_connections.splice(_free_connections.end(), _active_connections, it_con);
                             },
                             ._data = it},
                            &req_conf
                            );
}

void loader::activate_new_connnection() {
    if(_free_connections.empty())
        return;

    auto it_con = _free_connections.begin();
    auto it_req = _request_configs.begin();
    _active_connections.splice(_active_connections.begin(), _free_connections, it_con);
    send_request(_active_connections.begin(), *it_req);
    _request_configs.splice(_request_configs.end(), _request_configs, it_req);
}

void loader::check_statistic() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = now - _last_flush;
    auto sec_past = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    if(sec_past >= _config._flush_statistic) {
        _last_flush = now;
        flush_statistic();
    }
}

void loader::flush_statistic() {
    _actual_statistic._statistic = _thread.get_statistic();
    copy_statistic(&_prev_statistic, &_actual_statistic);
    _actual_statistic = {};
}

void loader::copy_statistic(statistic *to, statistic *from) noexcept {
    bool expect{false};
    while (!_write_stat.compare_exchange_strong(expect, true))
        ;
    memcpy(to, from, sizeof(statistic));
    _write_stat.store(false, std::memory_order_release);
}

loader::statistic loader::get_statistic() noexcept {
    statistic stat;
    copy_statistic(&stat, &_prev_statistic);
    return stat;
}

} // namespace bro::net::http::client::loader

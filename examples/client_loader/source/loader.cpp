#include <client_loader/loader.h>
#include <quill/detail/LogMacros.h>

namespace bro::net::http::client::loader {

loader::loader(config::loaders const & conf, const std::list<config::request> &requests, quill::Logger* logger, std::shared_ptr<bro::strm::settings> settings)  :
    _config(conf),
    _request_configs(requests),
    _logger(logger),
    _factory(std::make_shared<bro::net::ev::factory>()),
    _resolver(std::make_shared<dns::resolver>(dns::resolver::config{})) {

    for(size_t i = 0; i < _config._online_connections; ++i) {
        _free_connections.push_back({{}, std::make_unique<request>(_factory, _resolver)});
    }

    system::thread::config config;
    config._sleep = _config._sleep;
    config._call_sleep_on_n_empty_loop_in_a_row = conf._call_sleep_on_n_empty_loop_in_a_row;

    _thread.run_with_logic_pre_post(system::thread::callable(&loader::serve, this),
                                    system::thread::callable(&loader::logic_proceed, this),
                                    system::thread::callable(&loader::pre_start, this),
                                    system::thread::callable(&loader::post_end, this),
                                    &config);
}

bool loader::serve() {
//    _resolver->proceed();
    _factory->proceed();
    bool had_new_streams = _processed_data > 0;
    _processed_data = 0;
    return had_new_streams;
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
    if(!conf._body.empty() && !conf._body_type.empty())
        req_node._request->add_body(conf._body, conf._body_type);

    req_node._stat._start = std::chrono::steady_clock::now();
    req_node._request->send(conf._type, _url_prefix + conf._path, {._cb =
        [&](bro::net::http::client::response &&resp, char const *const error, std::any user_data) {
            auto it_con = std::any_cast<std::list<node>::iterator>(user_data);
            if (error) {
                _total_stat._failed_requests++;
                LOG_ERROR(_logger, "request failed with error {}\n", error);
            } else {
                std::chrono::duration<double> diff = std::chrono::steady_clock::now() - it_con->_stat._start;
                _total_stat._success_requests++;
                _total_stat._max_time = std::max(_total_stat._max_time, diff);
                _total_stat._total_time += diff;
            }
            _free_connections.splice(_free_connections.end(), _active_connections, it_con);
        },
        ._data = it});
}

void loader::logic_proceed() {
    if(!_free_connections.empty()) {
        auto it_con = _free_connections.begin();
        auto it_req = _request_configs.begin();
        _active_connections.splice(_active_connections.begin(), _free_connections, it_con);
        send_request(_active_connections.begin(), *it_req);
        _request_configs.splice(_request_configs.end(), _request_configs, it_req);
    }
}

} // namespace bro::net::http::client::loader

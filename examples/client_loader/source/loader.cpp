#include <client_loader/loader.h>
#include <quill/detail/LogMacros.h>

namespace bro::net::http::client::loader {

loader::loader(config::loaders const & conf, std::list<request> const &requests, quill::Logger* logger, std::shared_ptr<bro::strm::settings> settings)  :
    _config(conf),
    _requests(requests),
    _logger(logger),
    _factory(std::make_shared<bro::strm::factory>()),
    _resolver(std::make_shared<dns::resolver>()) {

    for(size_t i = 0; i < _config._online_connections; ++i) {
        _free_connections.push_back(std::make_unique<request>(_factory, _resolver));
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
    _resolver->proceed();
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

void loader::logic_proceed() {
    if(!_free_connections.empty()) {
        auto it = _free_connections.begin();
//        (*it)->
        _active_connections.splice(_active_connections.end(), _free_connections, it);
    }
//    if(_has_new_stream.load(std::memory_order_acquire)) {
//        std::lock_guard lg(_guard);
//        while(!_new_streams.empty()) {
//            process_new_stream(std::move(_new_streams.front()));
//            _new_streams.pop_front();
//        }
//        _has_new_stream.store(false, std::memory_order_release);
//    }
//    if(!_failed_connections.empty()) {
//        for(auto fc : _failed_connections) {
//            _streams.erase(fc->_stream.get());
//        }
//        _failed_connections.clear();
//    }
}

} // namespace bro::net::http::client::loader

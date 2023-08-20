#include "quill/Quill.h"
#include <client_loader/manager.h>

namespace bro::net::http::client::loader {

static std::atomic_bool s_active{true};

void stopHandler(int /*dummy*/) {
    s_active.store(false);
}

manager::manager(config::client_loader & config) : _config(config) {
    init_logger();
}

void manager::print_stat() {
    for(auto & loader : _loaders) {
        auto stat = loader->get_statistic();
        LOG_INFO(_logger, "\nStatistics for {}\nsuccess requests {}\nfailed requests {}\nmax time {}\nloops {}\nmax main function {}\nmax logic function {}"
                          "\nbusy time {}\nempty loops {}\n", loader->get_name(),
                 stat._success_requests, stat._failed_requests, stat._max_time, stat._statistic._loops, stat._statistic._max_main_function_time,
                 stat._statistic._max_logic_function_time, stat._statistic._busy_time, stat._statistic._empty_loops);
    }
}

void manager::serve() {
    signal(SIGINT, stopHandler);
    LOG_INFO(_logger, "Start manager for loaders");
    init_loaders();
    auto start = std::chrono::steady_clock::now();
    auto sleep_time = std::chrono::milliseconds(1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));


    while(s_active.load(std::memory_order_acquire)) {
        std::this_thread::sleep_for(sleep_time);
        print_stat();
        if(_config._test_time) {
            std::chrono::duration<double> diff = std::chrono::steady_clock::now() - start;
            auto past = std::chrono::duration_cast<std::chrono::seconds>(diff);
            if(past > *_config._test_time)
                break;
        }
    }
    _loaders.clear();
}

void manager::init_logger() {
    quill::Config q_cfg;
    q_cfg.enable_console_colours = true;
    q_cfg.backend_thread_empty_all_queues_before_exit = true;
    if(!_config._logger._thread_name.empty())
        q_cfg.backend_thread_name = _config._logger._thread_name;
    if(_config._logger._core)
        q_cfg.backend_thread_cpu_affinity = *_config._logger._core;

    if(!_config._logger._file_name.empty()) {
        auto file_handler = quill::file_handler(_config._logger._file_name, "w", quill::FilenameAppend::DateTime);
        q_cfg.default_handlers.push_back(std::move(file_handler));
    }


    quill::configure(q_cfg);
    quill::start();

    _logger = quill::create_logger("client_loader_logger");
    _logger->set_log_level(_config._logger._level);
}

void manager::init_loaders() {
    auto & cores = _config._loaders._core_ids;
    for(size_t i = 0; i < _config._loaders._total; ++i) {
        auto loder_conf = _config._loaders;
        if(!cores.empty()) {
            loder_conf._core_ids = {cores.front()};
            cores.splice(cores.end(), cores, cores.begin());
        }

        if(loder_conf._prefix_name.empty())
            loder_conf._prefix_name = "loader_";
        loder_conf._prefix_name += std::to_string(i);
        _loaders.emplace_back(std::make_unique<loader>(loder_conf, _config._requests, _config._server_host_name, _logger));
    }
}

} // namespace bro::net::http::client::loader

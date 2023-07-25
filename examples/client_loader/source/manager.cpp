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

void manager::serve() {
    signal(SIGINT, stopHandler);
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

        if(_config._loaders._prefix_name.empty())
            _config._loaders._prefix_name = "loader_";
        _config._loaders._prefix_name += std::to_string(i);
//        _loaders.emplace_back(std::make_unique<loader>(loder_conf, _logger));
    }
}


} // namespace bro::net::http::client::loader

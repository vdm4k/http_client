#pragma once
#include <stdint.h>>

namespace bro::net::http::client {

/**
 * \brief default sizes
 */
enum {
    e_eol_size = 2,                                         ///< end of line size
    e_sem_n_space_size = 2,                                 ///< after : always have a space -> 2 symbols
    e_header_add_size = e_sem_n_space_size + e_eol_size,    ///< header consist of : and eol -> 4 symbols
    e_2_spoce_size = 2,                                     ///< just size of 2 spaces )
    e_max_msg_size = 8192                                   ///< typical message size of lower than that (to allocate on stack)
};

/**
  * \brief connection type
  */
enum class connection_type : uint8_t {
    e_http, ///< http
    e_https ///< https
};

/**
  * \brief request state
  */
enum class request_state : uint8_t {
    e_idle,    ///< in idle state
    e_failed,  ///< is failed
    e_active   ///< in active state
};

} // namespace bro::net::http::client

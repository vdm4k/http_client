#pragma once
#include <any>
#include <cstddef>
#include <zlib.h>

namespace bro::zlib {
/** @defgroup zlib_streamer zlib_streamer
 *  @{
 */

/**
 * \brief small wrapper for zlib. simple to ruese
 */
class stream {
public:
  /**
 * \brief steam type
 */
  enum class type {
    e_compressor,   ///< compressor
    e_decompressor, ///< decompressor
    e_not_set       ///< type not set ( undefined )
  };

  /**
  * \brief A type alias for a callback function that returns a result.
  *
  * This function will be called with filled data and set lenght ( > 0) or filled error.
  * For pointers no need to call free
  */
  using result_fun_t = void (*)(Bytef *data, size_t lenght, std::any user_data, char const *error);

  /*! \brief init stream function
   * \param [in] tp stream type
   * \param [in] result_f (optional) if set will call it if error happen
   * \param [in] user_data user data pass to result_f
   * \result true if init success.
   *
   * \note if stream is init we call cleanup for it
   */
  bool init(type tp, result_fun_t result_f = nullptr, std::any user_data = nullptr);

  /*! \brief cleanup stream
   */
  void cleanup();

  /*! \brief process data depends on what stream type is
   * \param [in] data pointer on data to process
   * \param [in] lenght data lenght
   * \param [in] user_data user data pass to result_f
   * \param [in] result_f will call with filled error or data with data size
   * \param [in] last_chunk actual only for compressor mode. need set for last call
   * \result true if all good, false otherwise
   */
  bool process(Bytef *data, size_t lenght, std::any user_data, result_fun_t result_f, bool last_chunk = false);

private:
  z_stream _stream;            ///< current zstream
  type _type{type::e_not_set}; ///< current stream type
};

}; // namespace bro::zlib

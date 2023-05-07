#pragma once
#include <any>
#include <cstddef>
#include <zlib.h>

namespace bro::zlib {

class stream {
public:
  enum class type { e_compressor, e_decompressor, e_not_set };
  using result_fun_t = void (*)(Bytef *data, size_t lenght, std::any user_data, char const *error);

  bool init(type tp, result_fun_t result_f = nullptr, std::any user_data = nullptr);
  void cleanup();
  bool process(Bytef *data, size_t lenght, std::any user_data, result_fun_t result_f, bool last_chunk = false);

private:
  z_stream _stream;
  type _type{type::e_not_set};
};

}; // namespace bro::zlib

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <limits>
#include <http_client/zlib_helper.h>

namespace bro::zlib {

enum {
  e_simple_gzip_header = 16,
  e_default_memlevel = 8,
  e_enable_zlib_gzib_auto_header_detection = 32,
  e_max_buffer = std::numeric_limits<uint16_t>::max()
};

stream::~stream() {
  cleanup();
}

void stream::cleanup() {
  switch (_type) {
  case type::e_compressor: {
    deflateEnd(&_stream);
    break;
  }
  case type::e_decompressor: {
    inflateEnd(&_stream);
    break;
  }
  case type::e_not_set:
    break;
  }
  _type = type::e_not_set;
}

bool stream::init(stream::type tp, result_fun_t result_f, std::any user_data) {
  if (tp != type::e_not_set)
    cleanup();

  memset(&_stream, 0, sizeof(z_stream));
  int result{Z_OK};
  switch (tp) {
  case type::e_compressor: {
    result = deflateInit2(&_stream,
                          Z_DEFAULT_COMPRESSION,
                          Z_DEFLATED,
                          MAX_WBITS + e_simple_gzip_header, // produce a gzip header + trailer
                          e_default_memlevel,
                          Z_DEFAULT_STRATEGY);
    break;
  }
  case type::e_decompressor: {
    result = inflateInit2(&_stream, MAX_WBITS + e_enable_zlib_gzib_auto_header_detection);
    break;
  }
  case type::e_not_set:
    if (result_f)
      result_f(nullptr, 0, user_data, "Need to specify stream type");
    return false;
  }
  if (result != Z_OK && result_f)
    result_f(nullptr, 0, user_data, zError(result));
  if (result != Z_OK)
    return false;

  _type = tp;
  return true;
}

bool stream::process(Bytef *data, size_t lenght, std::any user_data, stream::result_fun_t result_f, bool last_chunk) {
  constexpr size_t max_avail_in = (std::numeric_limits<decltype(z_stream_s::avail_in)>::max)();
  int rc{Z_OK};

  switch (_type) {
  case type::e_compressor: {
    do {
      _stream.avail_in = std::min(lenght, max_avail_in);
      _stream.next_in = data;
      // shift on next chunk
      lenght -= _stream.avail_in;
      data += _stream.avail_in;
      bool finished = (last_chunk && lenght == 0);
      Bytef out_buf[e_max_buffer];

      do {
        _stream.avail_out = e_max_buffer;
        _stream.next_out = out_buf;
        rc = deflate(&_stream, finished ? Z_FINISH : Z_NO_FLUSH);
        if (rc != Z_OK && (!finished || rc != Z_STREAM_END)) {
          result_f(nullptr, 0, user_data, zError(rc));
          cleanup();
          return false;
        }
        int const bytes = e_max_buffer - _stream.avail_out;
        result_f(out_buf, bytes, user_data, nullptr);

      } while (_stream.avail_out == 0);

    } while (lenght > 0);
    break;
  }
  case type::e_decompressor: {
    do {
      _stream.avail_in = std::min(lenght, max_avail_in);
      _stream.next_in = data;
      // shift on next chunk
      lenght -= _stream.avail_in;
      data += _stream.avail_in;

      Bytef out_buf[e_max_buffer];
      while (_stream.avail_in > 0) {
        _stream.avail_out = e_max_buffer;
        _stream.next_out = out_buf;
        rc = inflate(&_stream, Z_NO_FLUSH);
        if (rc != Z_OK && rc != Z_STREAM_END) {
          result_f(nullptr, 0, user_data, zError(rc));
          cleanup();
          return false;
        }
        int const bytes = e_max_buffer - _stream.avail_out;
        result_f(out_buf, bytes, user_data, nullptr);
      }
    } while (lenght > 0);
    break;
  }
  case type::e_not_set: {
    result_f(nullptr, 0, user_data, "stream type not set");
    return false;
  }
  }
  return true;
}
} // namespace bro::zlib

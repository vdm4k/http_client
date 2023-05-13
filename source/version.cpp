#include <http_client/version.h>
#include <unordered_map>

namespace bro::net::http::header {

std::string_view to_string(version ver) {
  switch (ver) {
  case version::e_1_0:
    return "HTTP/1.0";
  case version::e_1_1:
    return "HTTP/1.1";
  case version::e_Unknown_Version:
    return "Unknown Version";
  }
  return "";
}

version to_version(std::string_view const &tp) {
  static std::unordered_map<std::string_view, version> umap{{"1.0", version::e_1_0}, {"1.1", version::e_1_1}};
  if (auto it = umap.find(tp); it != umap.end())
    return it->second;
  return version::e_Unknown_Version;
}

} // namespace bro::net::http::header

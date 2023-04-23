#include <network/tcp/ssl/listen/settings.h>
#include <network/tcp/ssl/listen/statistic.h>
#include <network/tcp/ssl/send/settings.h>
#include <network/tcp/ssl/send/statistic.h>
#include <network/stream/factory.h>
#include <network/platforms/system.h>
#include <fmt/format.h>

#include <atomic>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <csignal>
#include <http_client/request.h>
#include <uriparser/Uri.h>

#include "CLI/CLI.hpp"

bool print_debug_info = false;

using namespace bro::net;
using namespace bro::strm;

struct per_thread_data {
  std::thread _thread;
  tcp::ssl::send::statistic _stat;
};

void received_data_cb(stream *stream, std::any data_com) {
  const size_t data_size = 1500;
  std::byte data[data_size];
  ssize_t size = stream->receive(data, data_size);
  if (size > 0) {
    if (print_debug_info)
      std::cout << "receive message - " << std::string((char *) data, size) << std::endl;
    std::string *res = std::any_cast<std::string *>(data_com);
    (*res) += std::string((char *) data, size);
  }
}

void state_changed_cb(stream *stream, std::any data_com) {
  if (!stream->is_active()) {
    std::cout << "state_changed_cb " << stream->get_state() << ", " << stream->get_error_description() << std::endl;
    auto *need_to_handle = std::any_cast<std::unordered_set<bro::strm::stream *> *>(data_com);
    need_to_handle->insert(stream);
  } else
    std::cout << "state_changed_cb " << stream->get_state() << std::endl;
}

std::string get() {
  //  char name[0x64];
  //  fmt::format_to_n(name, 0x64, "{}s", type);
  //  std::cout << name << std::endl;

  std::stringstream ss;

  //  auto path = request.path().empty() ? "/" : request.path();
  std::string path = "/all/";
  ss << bro::net::http::request::to_string(bro::net::http::request::type::e_GET) << ' ' << path << " HTTP/1.1\n";
  //  ss << Headers::HOST << ": " << request.uri().host() << "\n";
  ss << bro::net::http::header::to_string(bro::net::http::header::types::e_Host) << ": "
     << "mobile-review.com"
     << "\n";
  //  ss << Headers::CONTENT_LENGTH << ": " << request.data().size() << "\n";
  ss << bro::net::http::header::to_string(bro::net::http::header::types::e_Connection) << ": close\n";
  //  ss << Headers::ACCEPT_ENCODING << ": identity\n";
  ss << bro::net::http::header::to_string(bro::net::http::header::types::e_Accept_Encoding)
     << ": "
        "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/"
        "signed-exchange;v=b3;q=0.7\n";
  //  for (auto header : request.headers()) {
  //    ss << header.first << ": " << header.second << "\n";
  //  }
  ss << "\n";
  //  ss << request.data();
  return ss.str();

  //  GET /all/ HTTP/1.1
  //  Host: mobile-review.com
  //  Connection: keep-alive
  //  Upgrade-Insecure-Requests: 1
  //  User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36
  //  Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
  // Accept-Encoding: gzip, deflate
  // Accept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7
}

void thread_fun(proto::ip::address const &server_addr,
                uint16_t server_port,
                std::atomic_bool &work,
                size_t connections_per_thread,
                size_t /*thread_number*/,
                tcp::ssl::send::statistic &stat) {
  tcp::ssl::send::settings settings;
  ev::factory manager;
  settings._peer_addr = {server_addr, server_port};
  auto g = get();
  std::vector<std::byte> initial_data((std::byte *) g.data(), (std::byte *) g.data() + g.size());
  //  fillTestData(thread_number, initial_data, data_size);
  std::unordered_set<stream *> need_to_handle;
  std::unordered_map<stream *, stream_ptr> stream_pool;
  std::string res;

  while (work.load(std::memory_order_acquire)) {
    if (stream_pool.size() < connections_per_thread) {
      auto new_stream = manager.create_stream(&settings);
      if (new_stream->is_active()) {
        manager.bind(new_stream);
        new_stream->set_received_data_cb(::received_data_cb, &res);
        new_stream->set_state_changed_cb(::state_changed_cb, &need_to_handle);
        new_stream->send(initial_data.data(), initial_data.size());
        stream_pool[new_stream.get()] = std::move(new_stream);
      } else {
        std::cout << "couldn't create stream cause " << new_stream->get_error_description() << std::endl;
      }
    }

    if (!need_to_handle.empty()) {
      auto it = need_to_handle.begin();
      if (!(*it)->is_active()) {
        stat += *static_cast<tcp::ssl::send::statistic const *>((*it)->get_statistic());
        stream_pool.erase((*it));
        need_to_handle.erase(it);
        break;
      }
    }

    manager.proceed();
  }

  std::cout << res << std::endl;

  for (auto &strm : stream_pool) {
    stat += *static_cast<tcp::ssl::send::statistic const *>(strm.first->get_statistic());
  }
}

#define RANGE(x) (int) ((x).afterLast - (x).first), ((x).first)

int main(int argc, char **argv) {
  CLI::App app{"ssl_server"};
  std::string server_address_string{"91.208.42.3"};
  uint16_t server_port = 443;
  //  uint16_t server_port = 80;
  size_t test_time = 5; // in seconds
  std::string certificate_path{"/home/vdm4k/projects/network/app/tcp_ssl_server/certificate.pem"};
  std::string key_path{"/home/vdm4k/projects/network/app/tcp_ssl_server/key.pem"};
  int threads_count = 1;
  int connections_per_thread = 1;

  auto type = "brum";
  char name[0x64];
  fmt::format_to_n(name, 0x64, "{}s", type);

  UriParserStateA state;
  UriUriA uri;
  state.uri = &uri;
  uriParseUriA(&state, "https://mobile-review.com/all/");
  std::cout << std::string(uri.scheme.first, uri.scheme.afterLast) << std::endl;
  std::cout << std::string(uri.hostText.first, uri.hostText.afterLast) << std::endl;
  std::cout << std::string(uri.pathHead->text.first, uri.pathHead->text.afterLast) << std::endl;
  uriFreeUriMembersA(&uri);

  bro::net::http::request request(bro::net::http::request::type::e_GET, "https://mobile-review.com/all/");
  request.add_header(
    bro::net::http::header::types::e_Accept_Encoding,
    "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/"
    "signed-exchange;v=b3;q=0.7");

  request.send();
  return 0;

  // http::get("http://www.google.com");

  // Serialize a request to a string
  //  std::stringstream ss;
  //  auto path = request.path().empty() ? "/" : request.path();
  //  ss << str(request.method()) << ' ' << path << " HTTP/1.1\n";
  //  ss << Headers::HOST << ": " << request.uri().host() << "\n";
  //  ss << Headers::CONTENT_LENGTH << ": " << request.data().size() << "\n";
  //  ss << Headers::CONNECTION << ": close\n";
  //  ss << Headers::ACCEPT_ENCODING << ": identity\n";
  //  for(auto header : request.headers()) {
  //    ss << header.first << ": " << header.second << "\n";
  //  }
  //  ss << "\n";
  //  ss << request.data();
  //  return ss.str();

  //  app.add_option("-a,--address", server_address_s, "server address")->required();
  //  app.add_option("-p,--port", server_port, "server port")->required();
  app.add_option("-l,--log", print_debug_info, "print debug info");
  //  app.add_option("-d,--data", data_size, "send data size")->type_size(1, std::numeric_limits<std::uint16_t>::max());
  app.add_option("-t,--test_time", test_time, "test time in seconds");
  app.add_option("-c,--certificate_path", certificate_path, "certificate path");
  app.add_option("-k,--key_path", key_path, "key path");
  CLI11_PARSE(app, argc, argv);

  disable_sig_pipe();

  proto::ip::address server_address(server_address_string);
  if (server_address.get_version() == proto::ip::address::version::e_none) {
    std::cerr << "incorrect address - " << server_address << std::endl;
    return -1;
  }

  std::cout << "client start" << std::endl;
  std::atomic_bool work(true);
  std::vector<per_thread_data> worker_pool;
  worker_pool.reserve(threads_count);
  for (size_t i = 0; i < threads_count; ++i) {
    worker_pool.emplace_back();
    auto &last = worker_pool.back();
    last._thread = std::thread(thread_fun,
                               server_address,
                               server_port,
                               std::ref(work),
                               connections_per_thread,
                               i,
                               std::ref(worker_pool.back()._stat));
  }

  std::this_thread::sleep_for(std::chrono::seconds(test_time));
  work = false;
  tcp::ssl::send::statistic stat;
  for (auto &wrk : worker_pool) {
    wrk._thread.join();
    stat += wrk._stat;
  }

  std::cout << "client stoped" << std::endl;
  std::cout << "success_send_data - " << stat._success_send_data << std::endl;
  std::cout << "retry_send_data - " << stat._retry_send_data << std::endl;
  std::cout << "failed_send_data - " << stat._failed_send_data << std::endl;
  std::cout << "success_recv_data - " << stat._success_recv_data << std::endl;
  std::cout << "retry_recv_data - " << stat._retry_recv_data << std::endl;
  std::cout << "failed_recv_data - " << stat._failed_recv_data << std::endl;
  return 0;
}

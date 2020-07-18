#ifndef OTUS_ASYNC_HPP
#define OTUS_ASYNC_HPP

#include <memory>
#include <mutex>
#include <unordered_map>

#include "parser.hpp"
#include "wrapper.hpp"

namespace otus::async {
  using handle_t = std::shared_ptr<otus::Wrapper>;

  handle_t connect(size_t bulk);
  void receive(handle_t handle, const char *data, size_t size);
  void disconnect(handle_t handle);
}

#endif

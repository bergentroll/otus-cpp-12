#include <string>

#include "async.h"

#include "tee_buffer.hpp"

using namespace std;
using namespace otus;

namespace async {
  handle_t connect(std::size_t bulk) {
    auto handle { make_shared<Wrapper>(bulk) };
    return handle;
  }

  void receive(handle_t handle, const char *data, std::size_t size) {
    if (!handle) throw invalid_argument("invalidated context recieved");
    handle->receive(string(data, size));
  }

  void disconnect(handle_t handle) {
    handle.reset();
  }
}

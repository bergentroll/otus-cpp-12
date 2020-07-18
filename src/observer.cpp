#include "observer.hpp"

using namespace otus;

void Observable::notify() {
  auto iter = subscribers.begin();
  while(iter != subscribers.end()) {
    auto ptr = iter->lock();
    if (ptr) {
      ptr->update();
      ++iter;
    } else {
      subscribers.erase(iter++);
    }
  }
}

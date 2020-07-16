#ifndef OTUS_OBSERVER_HPP
#define OTUS_OBSERVER_HPP

#include <memory>
#include <list>

#include <iostream>

namespace otus {
  class Observer {
  public:
    virtual ~Observer() = default;

    virtual void update() = 0;
  };

  class Observable {
  public:
    void subscribe(const std::shared_ptr<Observer>& observer) {
      subscribers.emplace_back(observer);
    }

    void notify() {
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

  private:
    std::list<std::weak_ptr<Observer>> subscribers { };
  };
}

#endif

#ifndef OTUS_OBSERVER_HPP
#define OTUS_OBSERVER_HPP

#include <memory>
#include <list>

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

    void notify();

  private:
    std::list<std::weak_ptr<Observer>> subscribers { };
  };
}

#endif

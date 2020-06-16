#ifndef OTUS_OBSERVER_HPP
#define OTUS_OBSERVER_HPP

#include <memory>

namespace otus {
  class Observer {
  public:
    virtual ~Observer() = default;

    virtual void update() = 0;
  };

  class Observable {
  public:
    virtual ~Observable() = default;

    virtual void subscribe(const std::shared_ptr<Observer>& observer) = 0;
  };
}

#endif

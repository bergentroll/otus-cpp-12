#ifndef OTUS_WORKER
#define OTUS_WORKER

#include <atomic>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <shared_mutex>
#include <sstream>
#include <string>
#include <thread>

namespace otus {
  // TODO Conditional variable.
  class Worker {
  public:
    using QueueType = std::queue<std::string>;

    Worker(
        std::string const &name,
        std::shared_mutex &mutex,
        QueueType &queue,
        std::atomic_bool const &endFlag):
      thread([this]() { run(); }),
      name(name),
      mutex(mutex),
      queue(queue),
      endFlag(endFlag) { }

    virtual ~Worker() { thread.join(); }

  protected:
    std::thread thread;
    std::string name;
    std::shared_mutex &mutex;
    QueueType &queue;
    std::atomic_bool const &endFlag;

    void run() {
      while (!endFlag) {
        bool hasWork { };
        if (mutex.try_lock()) {
          if (!queue.empty()) {
            hasWork = true;
            execCritical();
            queue.pop();
          }
          mutex.unlock();
          if (hasWork) execPostCritical();
        }
      }
    }

    virtual void execCritical() = 0;

    virtual void execPostCritical() { }

  };

  class WorkerStdout: public Worker {
    using Worker::Worker;

    void execCritical() override {
      std::cout << queue.front();
    }
  };

  class WorkerFile: public Worker {
    using Worker::Worker;

    std::string buf { };
    static inline std::atomic_uint index { 1 };

    void execCritical() override {
      buf = queue.front();
    }

    void execPostCritical() override {
      std::ofstream file { generateFileName(), std::ios_base::app };
      file << buf;
      file.close();
    }

    std::string generateFileName() {
      auto now {
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())
      };

      std::stringstream path { };
      path
        << "bulk"
        << std::to_string(now)
        << '-'
        << std::setfill('0') << std::setw(5) << std::to_string(index)
        << ".log";
      ++index;
      return path.str();
    }
  };
}

#endif

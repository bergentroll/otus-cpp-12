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
    Worker(
        std::shared_mutex &mutex,
        std::queue<std::string> &queue,
        std::atomic_bool const &endFlag):
        mutex(mutex), queue(queue), endFlag(endFlag) {
      std::thread thread { [this]() { run(); } };
      thread.detach();
    }

    virtual ~Worker() { };

  protected:
    virtual void execute() = 0;
    std::shared_mutex &mutex;
    std::queue<std::string> &queue;
    std::atomic_bool const &endFlag;

    void run() {
      while (!endFlag) {
        if (mutex.try_lock()) {
          if (!queue.empty()) {
            execute();
          }
          mutex.unlock();
        }
      }
    }
  };

  class WorkerStdout: public Worker {
  protected:
    virtual void execute() {
      std::cout << queue.front();
      queue.pop();
    }

  private:
    using Worker::Worker;
  };

  class WorkerFile: public Worker {
  protected:
    virtual void execute() {
      nextFile();
      file << queue.front();
      file.close();
      queue.pop();
    }

  private:
    using Worker::Worker;

    static inline std::ofstream file { };
    static inline uint16_t index { 1 };

    void nextFile() {
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
      file = std::ofstream(path.str(), std::ios_base::app);
      ++index;
    }
  };
}

#endif

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
    using QueueType = std::queue<std::pair<std::string, unsigned>>;

    Worker(
        std::string const &name,
        std::shared_mutex &mutex,
        QueueType &queue,
        std::atomic_bool const &endFlag):
        name(name), mutex(mutex), queue(queue), endFlag(endFlag) {
      std::thread thread { [this]() { run(); } };
      thread.detach();
    }

    virtual ~Worker() {
      std::unique_lock lock { mutex };
      std::cerr
        << "Thread "
        << name
        << ": "
        << blocksCounter
        << " blocks, "
        << commandsCounter
        << " commands."
        << std::endl;
    };

  protected:
    std::string name;
    std::shared_mutex &mutex;
    QueueType &queue;
    std::atomic_bool const &endFlag;
    unsigned blocksCounter { };
    unsigned commandsCounter { };

    virtual void run() = 0;
  };

  class WorkerStdout: public Worker {
  protected:
    virtual void run() {
      while (!endFlag) {
        if (mutex.try_lock()) {
          if (!queue.empty()) {
            std::cout << queue.front().first;
            ++blocksCounter;
            commandsCounter += queue.front().second;
            queue.pop();
          }
          mutex.unlock();
        }
      }
    }

  private:
    using Worker::Worker;
  };

  class WorkerFile: public Worker {
  protected:
    virtual void run() {
      while (!endFlag) {
        std::string buf { };
        if (mutex.try_lock()) {
          if (!queue.empty()) {
            buf = queue.front().first;
            commandsCounter += queue.front().second;
            queue.pop();
            mutex.unlock();

            std::ofstream file { generateFileName(), std::ios_base::app };
            file << buf;
            file.close();
            ++blocksCounter;
          } else {
            mutex.unlock();
          }
        }
      }
    }

  private:
    using Worker::Worker;

    static inline std::atomic_uint index { 1 };

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

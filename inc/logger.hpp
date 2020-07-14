#ifndef OTUS_LOGGER_HPP
#define OTUS_LOGGER_HPP

#include <atomic>
#include <shared_mutex>
#include <queue>

#include "worker.hpp"

namespace otus {
  class Logger {
  public:
    Logger() {
      workers.emplace_back( new WorkerStdout(
            "log",
            mutex,
            stdoutInputQueue,
            done));

      for (int i { 1 }; i < 3; ++i)
        workers.emplace_back(new WorkerFile(
              "file" + std::to_string(i),
              mutex,
              fileInputQueue,
              done));
    }

    ~Logger() {
      while (true) {
        std::shared_lock lock { mutex };
        if (fileInputQueue.empty() && stdoutInputQueue.empty()) break;
      }
      done = true;
    }

    void print(std::string const &block, unsigned blockSize) {
      {
        std::unique_lock lock { mutex };
        stdoutInputQueue.push(std::make_pair(block, blockSize));
        fileInputQueue.push(std::make_pair(block, blockSize));
      }
    }

    std::shared_mutex & getMutex() { return mutex; }

  private:
    std::atomic_bool done { };
    // TODO DRY
    Worker::QueueType stdoutInputQueue { };
    Worker::QueueType fileInputQueue { };
    std::vector<std::unique_ptr<Worker>> workers { };
    // TODO Separate mutexes.
    std::shared_mutex mutex { };
  };
}

#endif

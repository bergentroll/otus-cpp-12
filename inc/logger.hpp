#ifndef OTUS_LOGGER_HPP
#define OTUS_LOGGER_HPP

#include <atomic>
#include <shared_mutex>
#include <queue>

#include "worker.hpp"

namespace otus {
  class ILogger {
  public:
    virtual ~ILogger() { };
    virtual void print(std::string const &) = 0;
  };

  // TODO Testing
  class Logger: public ILogger {
  public:
    Logger() {
      workers.emplace_back( new WorkerStdout(
            "log",
            stdstreamMutex,
            stdoutInputQueue,
            done));

      for (int i { 1 }; i < 3; ++i)
        workers.emplace_back(new WorkerFile(
              "file" + std::to_string(i),
              fileMutex,
              fileInputQueue,
              done));
    }

    ~Logger() override {
      while (true) {
        std::shared_lock lock { stdstreamMutex };
        if (stdoutInputQueue.empty()) break;
      }
      while (true) {
        std::shared_lock lock { fileMutex };
        if (fileInputQueue.empty()) break;
      }
      done = true;
    }

    void print(std::string const &block) override {
      {
        std::unique_lock lock { stdstreamMutex };
        stdoutInputQueue.push(block);
      }
      {
        std::unique_lock lock { fileMutex };
        fileInputQueue.push(block);
      }
    }

  private:
    std::atomic_bool done { };
    std::vector<std::unique_ptr<Worker>> workers { };
    Worker::QueueType stdoutInputQueue { };
    Worker::QueueType fileInputQueue { };
    std::shared_mutex stdstreamMutex { };
    std::shared_mutex fileMutex { };
    std::string mainStatistics { };
  };
}

#endif

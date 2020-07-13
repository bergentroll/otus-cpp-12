#ifndef OTUS_TEE_BUFFER_HPP
#define OTUS_TEE_BUFFER_HPP

#include <atomic>
#include <shared_mutex>
#include <queue>

#include "observer.hpp"
#include "worker.hpp"

namespace otus {
  class TeeBuffer: public std::stringbuf, public Observer {
  public:
    TeeBuffer() {
      workers.emplace_back(new WorkerStdout(mutex, stdoutInputQueue, done));
      workers.emplace_back(new WorkerFile(mutex, fileInputQueue, done));
      workers.emplace_back(new WorkerFile(mutex, fileInputQueue, done));
    }

    ~TeeBuffer() override {
      while (true) {
        std::shared_lock lock { mutex };
        if (fileInputQueue.empty() && stdoutInputQueue.empty()) break;
      }
      done = true;
    }

    int sync() override {
      {
        std::unique_lock lock { mutex };
        stdoutInputQueue.push(str());
        fileInputQueue.push(str());
      }
      str("");
      return 0;
    }

    void update() override { }

  private:
    std::atomic_bool done { };
    // TODO DRY
    std::queue<std::string> stdoutInputQueue { };
    std::queue<std::string> fileInputQueue { };
    std::vector<std::unique_ptr<Worker>> workers { };
    std::shared_mutex mutex { };
  };
}

#endif

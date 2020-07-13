#ifndef OTUS_TEE_BUFFER_HPP
#define OTUS_TEE_BUFFER_HPP

#include <atomic>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <shared_mutex>
#include <queue>
#include <sstream>
#include <thread>

#include "observer.hpp"

namespace otus {
  class TeeBuffer: public std::stringbuf, public Observer {
  public:
    TeeBuffer() {
      fileThreads.emplace_back(std::thread([this]() { fileWorker(); }));
      fileThreads.emplace_back(std::thread([this]() { fileWorker(); }));
    }

    ~TeeBuffer() override {
      while (true) {
        std::shared_lock lock { mutex };
        if (fileInputQueue.empty() && stdoutInputQueue.empty()) break;
      }
      done = true;
      logThread.join();
      for (auto &thread: fileThreads) {
        thread.join();
      }
      closeFile();
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

    void update() override {
      closeFile();
      ++index;
      nextFile();
    }

  private:
    std::ofstream file { };
    std::atomic<uint16_t> index { };
    std::atomic_bool done { };
    // TODO DRY
    std::queue<std::string> stdoutInputQueue { };
    std::queue<std::string> fileInputQueue { };
    std::thread logThread { [this]() { logWorker(); } };
    std::vector<std::thread> fileThreads { };
    // TODO Shared mutex
    std::shared_mutex mutex { };

    void logWorker() {
      while (!done) {
        if (mutex.try_lock()) {
          if (!stdoutInputQueue.empty()) {
            std::cout << stdoutInputQueue.front();
            stdoutInputQueue.pop();
          }
          mutex.unlock();
        }
      }
    }

    // TODO DRY
    void fileWorker() {
      while (!done) {
        if (mutex.try_lock()) {
          if (!fileInputQueue.empty()) {
            file << fileInputQueue.front();
            fileInputQueue.pop();
          }
          mutex.unlock();
        }
      }
    }

    // FIXME Critical
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
      {
        std::unique_lock lock { mutex };
        file = std::ofstream(path.str(), std::ios_base::app);
      }
    }

    void closeFile() {
      sync();
      file.close();
    }
  };
}

#endif

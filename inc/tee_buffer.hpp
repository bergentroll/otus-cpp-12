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
      logThread.detach();
      for (auto &thread: fileThreads) {
        thread.detach();
      }
    }

    ~TeeBuffer() override {
      while (true) {
        std::shared_lock lock { mutex };
        if (fileInputQueue.empty() && stdoutInputQueue.empty()) break;
      }
      done = true;
      file.close();
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
    std::ofstream file { };
    uint16_t index { 1 };
    std::atomic_bool done { };
    // TODO DRY
    std::queue<std::string> stdoutInputQueue { };
    std::queue<std::string> fileInputQueue { };
    std::thread logThread { [this]() { logWorker(); } };
    std::vector<std::thread> fileThreads { };
    std::shared_mutex mutex { };

    // TODO Conditional variable.
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
            nextFile();
            file << fileInputQueue.front();
            file.close();
            fileInputQueue.pop();
          }
          mutex.unlock();
        }
      }
    }

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

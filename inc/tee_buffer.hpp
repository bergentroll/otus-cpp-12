#ifndef OTUS_TEE_BUFFER_HPP
#define OTUS_TEE_BUFFER_HPP

#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>

#include "observer.hpp"

namespace otus {
  // TODO: Testing.
  class TeeBuffer: public std::stringbuf, public Observer {
  public:
    ~TeeBuffer() override {
      closeFile();
    }

    int sync() override {
      std::thread log { [this]() { std::cout << str(); } };
      std::thread file1 { [this]() { file << str(); } };
      log.join();
      file1.join();
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
    }

    void closeFile() {
      sync();
      file.close();
    }
  };
}

#endif

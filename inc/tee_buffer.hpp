#ifndef OTUS_TEE_BUFFER_HPP
#define OTUS_TEE_BUFFER_HPP

#include <atomic>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "observer.hpp"

namespace otus {
  // TODO: Testing.
  class TeeBuffer: public std::stringbuf, public Observer {
  public:
    ~TeeBuffer() override { closeFile(); }

    int sync() override {
      std::cout << str();
      file << str();
      str("");
      return 0;
    }

    void update() override {
      closeFile();
      nextFile();
    }

  private:
    std::ofstream file { };
    static inline std::atomic_uint index { 1 };

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
      ++index;
      file = std::ofstream(path.str(), std::ios_base::app);
    }

    void closeFile() {
      sync();
      file.close();
    }
  };
}

#endif

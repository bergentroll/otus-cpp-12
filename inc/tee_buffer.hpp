#ifndef OTUS_TEE_BUFFER_HPP
#define OTUS_TEE_BUFFER_HPP

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "observer.hpp"

namespace otus {
  // TODO: Testing.
  class TeeBuffer: public std::stringbuf, public Observer {
  public:
    ~TeeBuffer() override {
      closeFile();
    }

    TeeBuffer() {
      nextFile();
    }

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
    std::ofstream file;

    void nextFile() {
      auto now {
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())
      };

      std::filesystem::path const path = "bulk" + std::to_string(now) + ".log";
      // When things happen to quick, existing file will be appended.
      file = std::ofstream(path, std::ios_base::app);
    }

    void closeFile() {
      sync();
      file.close();
    }
  };
}

#endif

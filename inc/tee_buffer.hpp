#ifndef OTUS_TEE_BUFFER_HPP
#define OTUS_TEE_BUFFER_HPP

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "observer.hpp"

namespace otus {
  class TeeBuffer: public std::stringbuf, public Observer {
  public:
    ~TeeBuffer() override {
      sync();
      file.close();
    }
  
    TeeBuffer() {
      auto now {
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())
      };
  
      std::filesystem::path const path = "bulk" + std::to_string(now) + ".log";
      file = std::ofstream(path);
    }
  
    int sync() override {
      std::cout << str();
      file << str();
      str("");
      return 0;
    }

    void update() override { }
  
  private:
    std::ofstream file;
  };
}

#endif

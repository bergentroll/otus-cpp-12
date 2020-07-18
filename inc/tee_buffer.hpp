#ifndef OTUS_TEE_BUFFER_HPP
#define OTUS_TEE_BUFFER_HPP

#include <atomic>
#include <fstream>
#include <sstream>

#include "observer.hpp"

namespace otus {
  // TODO: Testing.
  class TeeBuffer: public std::stringbuf, public Observer {
  public:
    ~TeeBuffer() override { closeFile(); }

    int sync() override;

    void update() override {
      closeFile();
      nextFile();
    }

  private:
    std::ofstream file { };
    static inline std::atomic_uint index { 1 };

    void nextFile();

    void closeFile() {
      sync();
      file.close();
    }
  };
}

#endif

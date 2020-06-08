#ifndef TEE_BUFFER_HPP
#define TEE_BUFFER_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

class TeeBuffer: public std::stringbuf{
public:
  ~TeeBuffer() override {
    sync();
    file.close();
  }

  TeeBuffer() {
    std::filesystem::path const path = "file.txt";
    file = std::ofstream(path);
  }

  int sync() override {
    std::cout << str();
    file << str();
    str("");
    return 0;
  }

private:
  std::ofstream file;
};

#endif

#ifndef TEE_BUFFER_HPP
#define TEE_BUFFER_HPP

#include <iostream>
#include <sstream>

class TeeBuffer: public std::stringbuf{
public:
  int sync() override {
    std::cout << '!' << str().c_str();
    str("");
    return 0;
  }
};

#endif

#include <chrono>
#include <iomanip>
#include <iostream>

#include "tee_buffer.hpp"

using namespace std;
using namespace otus;

int TeeBuffer::sync() {
  cout << str();
  file << str();
  str("");
  return 0;
}

void TeeBuffer::nextFile() {
  auto now {
    chrono::system_clock::to_time_t(chrono::system_clock::now())
  };

  stringstream path { };
  path
    << "bulk"
    << to_string(now)
    << '-'
    << setfill('0') << setw(5) << to_string(index)
    << ".log";
  ++index;
  file = ofstream(path.str(), ios_base::app);
}

#include "wrapper.hpp"

using namespace std;
using namespace otus;


void Wrapper::asyncCall(string const &input) {
  ss << input;
  string buf { };
  while (ss) {
    ss >> buf;
    if (!buf.empty()) parser << buf;
    buf.clear();
  }
  ss.clear();
}

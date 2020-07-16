#include <gtest/gtest.h>
#include <limits>

#include "parser.hpp"

using namespace std;
using namespace otus;

TEST(bulk, plain) {
  stringstream ss { };
  Parser p { 2, ss };

  p << "cmd1" << "cmd2";
  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
  ss.str("");

  p << "cmd1" << "cmd2";
  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
}

TEST(bulk, block) {
  stringstream ss { };
  Parser p { 2, ss };

  p << "cmd1" << "cmd2";
  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
  ss.str("");

  p << "{" << "cmd1" << "cmd2" << "cmd3" << "}";
  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2, cmd3\n");
  ss.str("");

  p << "cmd1" << "cmd2";
  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
}

TEST(bulk, nested_blocks) {
  stringstream ss { };
  Parser p { 2, ss };

  p << "cmd1" << "cmd2";
  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
  ss.str("");

  p << "{" << "cmd1" << "{" << "cmd2" << "}" << "cmd3" << "}";
  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2, cmd3\n");
  ss.str("");

  p
    << "{" << "cmd1"
    << "{" << "cmd2"
    << "{" << "cmd3" << "}" << "}"
    << "cmd4" << "}";
  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2, cmd3, cmd4\n");
  ss.str("");

  p << "cmd1" << "cmd2";
  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
}

TEST(bulk, invalid_token) {
  stringstream ss { };
  Parser p { 2, ss };

  p << "cmd1" << "{" << "cmd2" << "}";
  EXPECT_THROW(p << "}", Parser::InvalidToken);
}

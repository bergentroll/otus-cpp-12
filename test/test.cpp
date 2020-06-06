#include <gtest/gtest.h>

#include "parser.hpp"

using namespace std;

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
  EXPECT_EQ(p.getState(), State::plain);
  ss.str("");

  p << "{" << "cmd1";
  EXPECT_EQ(p.getState(), State::block);
  p  << "{" << "cmd2";
  EXPECT_EQ(p.getState(), State::nested);
  p << "{" << "cmd3" << "}" << "}";
  EXPECT_EQ(p.getState(), State::block);
  p << "cmd4" << "}";
  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2, cmd3, cmd4\n");
  ss.str("");

  p << "cmd1" << "cmd2";
  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

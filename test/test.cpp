#include <gtest/gtest.h>
#include <limits>

#include "parser.hpp"
#include "get_arg.hpp"

using namespace std;
using namespace otus;

//TEST(bulk, plain) {
//  stringstream ss { };
//  Parser p { 2, ss };
//
//  p << "cmd1" << "cmd2";
//  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
//  ss.str("");
//
//  p << "cmd1" << "cmd2";
//  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
//}
//
//TEST(bulk, block) {
//  stringstream ss { };
//  Parser p { 2, ss };
//
//  p << "cmd1" << "cmd2";
//  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
//  ss.str("");
//
//  p << "{" << "cmd1" << "cmd2" << "cmd3" << "}";
//  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2, cmd3\n");
//  ss.str("");
//
//  p << "cmd1" << "cmd2";
//  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
//}
//
//TEST(bulk, nested_blocks) {
//  stringstream ss { };
//  Parser p { 2, ss };
//
//  p << "cmd1" << "cmd2";
//  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
//  ss.str("");
//
//  p << "{" << "cmd1" << "{" << "cmd2" << "}" << "cmd3" << "}";
//  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2, cmd3\n");
//  ss.str("");
//
//  p
//    << "{" << "cmd1"
//    << "{" << "cmd2"
//    << "{" << "cmd3" << "}" << "}"
//    << "cmd4" << "}";
//  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2, cmd3, cmd4\n");
//  ss.str("");
//
//  p << "cmd1" << "cmd2";
//  EXPECT_EQ(ss.str(), "bulk: cmd1, cmd2\n");
//}
//
//TEST(bulk, invalid_token) {
//  stringstream ss { };
//  Parser p { 2, ss };
//
//  p << "cmd1" << "{" << "cmd2" << "}";
//  EXPECT_THROW(p << "}", Parser::InvalidToken);
//}
//
//TEST(get_arg, valid) {
//  char const *v[] { "bulk", "345" };
//  EXPECT_EQ(345, get_arg(2, v));
//}
//
//TEST(get_arg, invalid_argc) {
//  char const *v1[] { "bulk" };
//  EXPECT_THROW(get_arg(1, v1), InvalidArgument);
//
//  char const *v2[] { "bulk", "345", "-h" };
//  EXPECT_THROW(get_arg(3, v2), InvalidArgument);
//}
//
//TEST(get_arg, invalid_arg) {
//  char const *v[] { "bulk", "a345" };
//  EXPECT_THROW(get_arg(2, v), InvalidArgument);
//
//  v[1] = "3a45";
//  EXPECT_THROW(get_arg(2, v), InvalidArgument);
//
//  v[1] = "345a";
//  EXPECT_THROW(get_arg(2, v), InvalidArgument);
//}
//
//TEST(get_arg, out_of_range) {
//  char const *v[] { "bulk", "-345" };
//  EXPECT_THROW(get_arg(2, v), InvalidArgument);
//
//  v[1] = to_string(static_cast<long>(numeric_limits<int>::max()) + 1).c_str();
//  EXPECT_THROW(get_arg(2, v), InvalidArgument);
//}

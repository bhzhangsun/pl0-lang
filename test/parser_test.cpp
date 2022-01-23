/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2022-01-04 09:35:17
 * @LastEditTime: 2022-01-22 19:18:55
 * @Description: Parser 单元测试
 */

#define CATCH_CONFIG_MAIN
#include "parser.h"

#include <fstream>
#include <iostream>
#include <string>

#include "catch.hpp"
#include "lexer.h"

int test_parser() {
  Lexer lex("../resource/square.pl0");
  lex.Tokenization();
  Parser parser(lex);
  parser.Parsing();

  return 0;
}

int test_imcomplate_parser() {
  Lexer lex("const x");
  lex.Tokenization();
  Parser parser(lex);
  parser.Parsing();

  return 0;
}

TEST_CASE("Parser", "[short]") {
  REQUIRE(test_parser() == 0);
  REQUIRE(test_imcomplate_parser() == 0);
}
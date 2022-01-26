/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2022-01-04 09:35:17
 * @LastEditTime: 2022-01-26 21:31:30
 * @Description: Parser 单元测试
 */

#define CATCH_CONFIG_MAIN
#include "parser.h"

#include <array>
#include <fstream>
#include <iostream>
#include <string>

#include "catch.hpp"
#include "lexer.h"

int test_parser() {
  std::array<std::string, 3> sources = {"../resource/square.pl0",
                                        "../resource/prime.pl0",
                                        "../resource/operation.pl0"};

  for (auto file : sources) {
    Lexer lex("../resource/square.pl0");
    REQUIRE(lex.Tokenization() == true);
    Parser parser(lex);
    REQUIRE(parser.Parsing() == true);
  }
  return 0;
}

int test_imcomplate_parser() {
  Lexer lex("const x", Lexer::MODE_CONTENT);
  REQUIRE(lex.Tokenization() == true);
  Parser parser(lex);
  REQUIRE(parser.Parsing() == false);
  return 0;
}

TEST_CASE("Parser", "[short]") {
  REQUIRE(test_parser() == 0);
  REQUIRE(test_imcomplate_parser() == 0);
}
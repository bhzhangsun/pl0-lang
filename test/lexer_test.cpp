/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-12 22:48:12
 * @LastEditTime: 2022-01-23 18:52:25
 * @Description: Lexer 单元测试
 */
#define CATCH_CONFIG_MAIN
#include "lexer.h"

#include <fstream>
#include <iostream>
#include <string>

#include "catch.hpp"

int test_lexer_constructor() {
  Lexer lex("../resource/nonexist_file.pl0");
  lex.Tokenization();
  std::list<Token> stream = lex.token_stream();
  return stream.size();
}

int test_lexer() {
  Lexer lex("../resource/square.pl0");
  lex.Tokenization();
  std::list<Token> stream = lex.token_stream();
  return stream.size();
}

int test_content_lexer() {
  Lexer lex("faaa = 3.14f", Lexer::MODE_CONTENT);
  lex.Tokenization();
  std::list<Token> stream = lex.token_stream();
  return stream.size();
}

int test_content_reapet_error() {
  Lexer lex("faaa = 3.14ffff", Lexer::MODE_CONTENT);
  lex.Tokenization();
  std::list<Token> stream = lex.token_stream();
  return stream.size();
}

TEST_CASE("Lexer", "[short]") {
  REQUIRE(test_lexer_constructor() == 0);
  REQUIRE(test_lexer() == 41);
  REQUIRE(test_content_lexer() == 2);
  REQUIRE(test_content_reapet_error() == 2);
}
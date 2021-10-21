/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-12 22:48:12
 * @LastEditTime: 2021-10-14 21:48:43
 * @Description: Lexer 单元测试
 */
#define CATCH_CONFIG_MAIN
#include "lexer.h"

#include <fstream>
#include <iostream>
#include <string>

#include "catch.hpp"

int test() {
  Lexer lex("../test/testdata.pl0");
  std::regex num_pattern("^([1-9]\\d*|0)(.\\d\\d*)?$");

  //   std::cout << "start!" << std::endl;
  //   std::string content;
  //   std::fstream fin;
  //   fin.open("../test/testdata.pl0", std::ios::in);
  //   fin >> content;
  //   std::cout << "end: " << content << std::endl;
  return 15;
}

TEST_CASE("Lexer tokenrization", "[short]") {
  //   raw_stream->getline(content, 100);
  //   std::cout << content << std::endl;
  //   Lexer lex("test/testdata.pl0");
  REQUIRE(test() == 15);
}
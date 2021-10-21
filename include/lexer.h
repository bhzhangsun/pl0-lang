/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-12 07:59:47
 * @LastEditTime: 2021-10-19 23:18:08
 * @Description: Lexer词法分析器
 */
#if !defined(__PL0_LEXER_H__)
#define __PL0_LEXER_H__

#include <fstream>
#include <istream>
#include <list>
#include <memory>
#include <strstream>

#include "token.h"

class Lexer {
 private:
  std::shared_ptr<std::istream> raw_stream_;
  std::list<Token> token_stream_;
  std::size_t num_line_ = 0;

 public:
  Lexer(const std::string &filename)
      : raw_stream_(new std::ifstream(filename, std::ios::in)) {
    Tokenization();
  };
  //   Lexer(std::ifstream &in) : raw_stream(&in) { tokenization(); }
  //   Lexer(std::strstream &in) : raw_stream(new std::strstream(in)) {
  //     tokenization();
  //   }

  ~Lexer() = default;

  void Tokenization();
};

#endif  // __PL0_LEXER_H__

/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-12 07:59:47
 * @LastEditTime: 2021-10-21 22:53:13
 * @Description: Lexer词法分析器
 */
#if !defined(__PL0_LEXER_H__)
#define __PL0_LEXER_H__

#include <fstream>
#include <istream>
#include <list>
#include <memory>
#include <sstream>

#include "token.h"

class Lexer {
 private:
  std::shared_ptr<std::istream> raw_stream_;
  std::list<Token> token_stream_;
  std::size_t num_line_ = 0;

 public:
  // 构造函数方式，内部类型
  typedef enum { MODE_CONTENT = 0, MODE_FILENAME } constructor_mode;

 public:
  Lexer(const std::string &str, constructor_mode type = MODE_FILENAME) {
    if (type == MODE_CONTENT) {
      std::string content = str;
      raw_stream_.reset(new std::istringstream(content));
    } else {
      raw_stream_.reset(new std::ifstream(str, std::ios::in));
    }
    Tokenization();
  };
  Lexer(std::string &in) : raw_stream_() { Tokenization(); }

  ~Lexer() = default;
  std::list<Token> &token_stream() { return token_stream_; }

  void Tokenization();
};

#endif  // __PL0_LEXER_H__

/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-12 07:59:47
 * @LastEditTime: 2022-01-26 08:30:29
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
 public:
  // 构造函数方式，内部类型
  typedef enum { MODE_CONTENT = 0, MODE_FILENAME } constructor_mode;
  typedef std::list<Token> token_buffer;
  typedef typename std::list<Token>::iterator token_iterator;

 private:
  std::shared_ptr<std::istream> raw_stream_;
  token_buffer token_stream_;
  std::size_t num_line_ = 1;

  token_iterator itor_;

 public:
  Lexer(const std::string &str, constructor_mode type = MODE_FILENAME) {
    if (type == MODE_CONTENT) {
      std::string content = str;
      raw_stream_.reset(new std::istringstream(content));
    } else {
      raw_stream_.reset(new std::ifstream(str, std::ios::in));
    }
    itor_ = token_stream_.begin();
  };
  Lexer(std::string &in) : raw_stream_() { itor_ = token_stream_.begin(); }

  ~Lexer() = default;
  token_buffer &token_stream() { return token_stream_; }

  bool Tokenization();

  token_iterator next();
  token_iterator peek() { return itor_; }
  bool eof(token_iterator itor) { return itor == token_stream_.end(); }
};

#endif  // __PL0_LEXER_H__

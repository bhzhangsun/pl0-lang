/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-12 07:59:47
 * @LastEditTime: 2022-01-26 08:30:40
 * @Description: Lexer解析器的实现
 */
#include "lexer.h"

#include <cctype>
#include <iostream>
#include <regex>

#include "error.h"
#include "number_recognizer.h"
#include "symbol_recognizer.h"
#include "token.h"
#include "word_recognizer.h"

bool Lexer::Tokenization() {
  bool succ = true;
  while (raw_stream_->peek() != EOF) {
    try {
      char c = raw_stream_->peek();
      NumberRecognizer number(raw_stream_);
      SymbolRecognizer symbol(raw_stream_);
      WordRecognizer word(raw_stream_);

      // white char
      if (c == ' ' || c == '\t' || c == '\r') {
        raw_stream_->get();
      } else if (c == '\n') {  // 换行
        raw_stream_->get();
        num_line_++;
      } else if (number.Match()) {
        token_stream_.push_back(number.Consumer(num_line_));
      } else if (word.Match()) {
        token_stream_.push_back(word.Consumer(num_line_));
      } else if (symbol.Match()) {
        token_stream_.push_back(symbol.Consumer(num_line_));
      } else {
        raw_stream_->get();
        throw Error(ERRNO::ERRNO_UNRECOGNIZE_TOKEN, num_line_,
                    std::string(1, c));
      }
    } catch (const Error& e) {
      std::cerr << e.what() << std::endl;
      succ = false;
    }
  }
  itor_ = token_stream_.begin();
  return succ;
}

/**
 * @description: next迭代器函数
 * @param {*}
 * @return {*} 返回当前位置的迭代器，将迭代器移到下一位置
 */

Lexer::token_iterator Lexer::next() {
  if (itor_ == token_stream_.end()) {
    return token_stream_.end();
  }
  return itor_++;
};
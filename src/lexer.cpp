/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-12 07:59:47
 * @LastEditTime: 2021-12-30 08:42:09
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

void Lexer::Tokenization() {
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
        throw Error(ERRNO::ERRNO_EXECPTION_TOKEN, num_line_, std::string(1, c));
      }
    } catch (const Error& e) {
      std::cerr << e.what() << std::endl;
    }
  }
}
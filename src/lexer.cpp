/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-12 07:59:47
 * @LastEditTime: 2021-10-19 23:30:22
 * @Description: Lexer解析器的实现
 */
#include "lexer.h"

#include <cctype>
#include <iostream>
#include <regex>

#include "number_recognizer.h"
#include "symbol_recognizer.h"
#include "token.h"
#include "word_recognizer.h"

void Lexer::Tokenization() {
  std::cout << "start !!" << std::endl;
  while (raw_stream_->peek() != EOF) {
    try {
      char c = raw_stream_->peek();
      NumberRecognizer number(raw_stream_);
      SymbolRecognizer symbol(raw_stream_);
      WordRecognizer word(raw_stream_);

      std::cout << "curr char:" << int(c) << std::endl;
      if (c == ' ' || c == '\t' || c == '\r') {
        raw_stream_->get();
      } else if (c == '\n') {
        raw_stream_->get();
        num_line_++;
      } else if (number.Match()) {
        token_stream_.push_back(number.Consumer(num_line_));
      } else if (word.Match()) {
        token_stream_.push_back(word.Consumer(num_line_));
      } else if (symbol.Match()) {
        token_stream_.push_back(symbol.Consumer(num_line_));
      } else {
        std::cout << "未识别的字符：" << c << std::endl;
        raw_stream_->get();
      }
    } catch (const std::string& str) {
      std::cerr << str << '\n';
    }
  }
}
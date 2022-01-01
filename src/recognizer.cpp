/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-19 22:50:20
 * @LastEditTime: 2021-12-30 08:57:46
 * @Description:
 */

#include <cctype>
#include <iostream>
#include <regex>

#include "error.h"
#include "number_recognizer.h"
#include "symbol_recognizer.h"
#include "word_recognizer.h"

/**
 * @description: 工具函数 是否空白
 * @param {char} c
 * @return 是否空白
 */
bool iswhite(const char c) {
  if (c == ' ' || c == '\t' || c == '\r') {
    return true;
  } else {
    return false;
  }
}

/**
 * @description: 工具函数 是否是tag
 * @param {char} c
 * @return 是否是tag
 */
bool istag(const char c) {
  if (c == ',' || c == '.' || c == ';' || c == '?' || c == '!' || c == '#' ||
      c == '(' || c == ')' || c == '*' || c == '/' || c == '+' || c == '-' ||
      c == '<' || c == '>' || c == ':' || c == '=') {
    return true;
  } else {
    return false;
  }
}

/**
 * @description: 数字识别器
 * @param {*}
 * @return 识别到的token，贪心匹配
 */
bool NumberRecognizer::Match() { return isdigit(raw_stream_->peek()); }
Token NumberRecognizer::Consumer() { return Consumer(0); }
Token NumberRecognizer::Consumer(size_t line) {
  std::regex num_pattern("^([1-9]\\d*|0)(.\\d\\d*)?$");
  std::string str = "";
  char c = raw_stream_->peek();

  do {
    str += c;
    raw_stream_->get();
    c = raw_stream_->peek();
  } while (c == '.' ||
           (!iswhite(c) && c != '\n' && !istag(c) && !raw_stream_->eof()));

  if (std::regex_match(str, num_pattern)) {
    return Token(str, line);
  }

  throw Error(ERRNO::ERRNO_EXECPTION_NUMBER, line, str);
}

/**
 * @description: 符号识别器
 * @return: 识别到的token, 最短匹配
 */

bool SymbolRecognizer::Match() { return istag(raw_stream_->peek()); }
Token SymbolRecognizer::Consumer() { return Consumer(0); }
Token SymbolRecognizer::Consumer(size_t line) {
  std::regex symbol_pattern("^[,.;\\?!#\\(\\)*/+-]|[<>]=?|:=$");
  std::string str = "";
  char c = raw_stream_->peek();

  switch (c) {
    case ',':
    case '.':
    case ';':
    case '?':
    case '!':
    case '#':
    case '(':
    case ')':
    case '*':
    case '/':
    case '+':
    case '-':
    case '=':
      str += c;
      raw_stream_->get();
      break;
    case '<':
    case '>':
      str += c;
      raw_stream_->get();
      c = raw_stream_->peek();

      if (c == '=') {
        str += c;
        raw_stream_->get();
      }
      break;
    case ':':
      str += c;
      raw_stream_->get();
      c = raw_stream_->peek();

      if (c == '=') {
        str += c;
        raw_stream_->get();
        break;
      }
      // else fall into default
    default:
      throw Error(ERRNO::ERRNO_EXECPTION_SYMBOL, line, str);
  }
  return Token(str, line);
}

/**
 * @description: 单词识别器
 * @param {*}
 * @return 识别到的token，贪心匹配
 */
bool WordRecognizer::Match() { return isalpha(raw_stream_->peek()); }
Token WordRecognizer::Consumer() { return Consumer(0); }
Token WordRecognizer::Consumer(size_t line) {
  std::regex word_pattern("^[A-Za-z_][A-Za-z0-9_]*$");
  std::string str = "";
  char c = raw_stream_->peek();

  do {
    str += c;
    raw_stream_->get();
    c = raw_stream_->peek();
  } while (!iswhite(c) && c != '\n' && !istag(c) && !raw_stream_->eof());

  if (std::regex_match(str, word_pattern)) {
    return Token(str, line);
  }
  throw Error(ERRNO::ERRNO_EXECPTION_WORD, line, str);
}

/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-21 00:09:40
 * @LastEditTime: 2021-12-13 22:40:27
 * @Description:
 */

#include "token.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <unordered_map>

void Token::GetType() {
  std::array<std::string, Tag::STATIC_NUM> idx2value = {
      ",",   ".",  ";",    "?",     "!",   ":=",        "+",    "-",
      "*",   "/",  "(",    ")",     "=",   "#",         "<",    "<=",
      ">",   ">=", "odd",  "const", "var", "procedure", "call", "begin",
      "end", "if", "then", "while", "do"};
  std::unordered_map<std::string, size_t> value2idx;
  std::string lower_value = value;
  for (int i = 0; i < Tag::STATIC_NUM; i++) {
    value2idx[idx2value[i]] = i;
  }

  std::transform(value.begin(), value.end(), lower_value.begin(), ::tolower);
  if (value2idx.count(lower_value) > 0) {
    type = Tag(value2idx[lower_value]);
  } else {
    type = isdigit(value[0]) ? Tag::NUMBER : Tag::IDENTIFIER;
  }
}
Token::operator bool() { return value != ""; }

std::ostream &operator<<(std::ostream &out, const Token &t) {
  out << "[" << t.line << "," << t.value << "," << t.type << "]" << std::endl;
  return out;
}
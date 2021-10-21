/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-21 00:09:40
 * @LastEditTime: 2021-10-21 09:00:59
 * @Description:
 */

#include "token.h"

#include <array>
#include <cctype>
#include <unordered_map>

void Token::GetType() {
  std::array<std::string, Tag::STATIC_NUM> idx2value = {
      ",",     ".",   ";",   ":=",    "+",     "-",         "*",
      "/",     "(",   ")",   "=",     "#",     "<",         "<=",
      ">",     ">=",  "odd", "const", "var",   "procedure", "call",
      "begin", "end", "if",  "then",  "while", "do"};
  std::unordered_map<std::string, size_t> value2idx;

  for (int i = 0; i < Tag::STATIC_NUM; i++) {
    value2idx[idx2value[i]] = i;
  }
  if (value2idx.count(value) > 0) {
    type = Tag(value2idx[value]);
  } else {
    type = isdigit(value[0]) ? Tag::NUMBER : Tag::IDENTIFIER;
  }
}
/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-12 07:59:47
 * @LastEditTime: 2021-12-13 22:38:26
 * @Description: Token类，表示词法单元
 */
#if !defined(__PL0_TOKEN_H__)
#define __PL0_TOKEN_H__

#include <iostream>
#include <string>

/**
 * @description: Tag 枚举类型，词元的类型
 */
enum Tag {
  PESUDO = -1,       // 伪标签
  SYMBOL_COMMA = 0,  // ,
  SYMBOL_DOT,        // .
  SYMBOL_SEMI,       // ;
  SYMBOL_QUES,       // ?  输入
  SYMBOL_EXCL,       // !  输出
  SYMBOL_BECOME,     // :=
  SYMBOL_ADD,        // +
  SYMBOL_SUB,        // -
  SYMBOL_MUL,        // *
  SYMBOL_DIV,        // /
  SYMBOL_LPARENT,    // (
  SYMBOL_RPARENT,    // )
  SYMBOL_EQ,         // =
  SYMBOL_NEQ,        // #
  SYMBOL_LE,         // <
  SYMBOL_LEQ,        // <=
  SYMBOL_GT,         // >
  SYMBOL_GEQ,        // >=
  SYMBOL_ODD,        // odd
  KEYWORD_CONST,     // const
  KEYWORD_VAR,       // var
  KEYWORD_PROC,      // procedure
  KEYWORD_CALL,      // call
  KEYWORD_BEGIN,     // begin
  KEYWORD_END,       // end
  KEYWORD_IF,        // if
  KEYWORD_THEN,      // then
  KEYWORD_WHILE,     // while
  KEYWORD_DO,        // do
  STATIC_NUM,        // 静态符号个数
  NUMBER,            // 数字
  IDENTIFIER,        // 标识符
};

/**
 * @description: Token类，基础词法单元
 */
struct Token {
  std::string value;
  Tag type;
  size_t line;

  Token(const std::string &str) : value(str), line(0) { GetType(); };
  Token(const std::string &str, size_t ln) : value(str), line(ln) {
    GetType();
  };
  ~Token() = default;
  operator bool();
  friend std::ostream &operator<<(std::ostream &out, const Token &t);

 private:
  void GetType();
};

#endif  // __PL0_TOKEN_H__

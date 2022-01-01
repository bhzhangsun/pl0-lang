/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-22 08:21:23
 * @LastEditTime: 2021-12-09 22:21:32
 * @Description:
 */

#if !defined(__PL0_PARSER_H__)
#define __PL0_PARSER_H__

#include <list>
#include <memory>

#include "expr_ast.h"
#include "token.h"

class Parser {
 public:
  typedef std::list<Token> token_buffer;
  typedef std::list<Token>::iterator token_iterator;

 private:
  token_buffer token_stream_;
  token_iterator current_;
  std::shared_ptr<ExprAst> root_;

  std::shared_ptr<ExprAst> ParseBlock();
  std::shared_ptr<ExprAst> ParseConstDefine();
  std::shared_ptr<ExprAst> ParseVarDefine();
  std::shared_ptr<ExprAst> ParseFuncDefine();
  std::shared_ptr<ExprAst> ParseStatment();

 public:
  Parser(const std::list<Token>& tokens)
      : token_stream_(tokens), current_(token_stream_.begin()) {}
  void Parsing();
};

#endif  // __PL0_PARSER_H__

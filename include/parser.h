/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-22 08:21:23
 * @LastEditTime: 2022-03-14 21:33:59
 * @Description:
 */

#if !defined(__PL0_PARSER_H__)
#define __PL0_PARSER_H__

#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "error.h"
#include "expr_ast.h"
#include "lexer.h"

class Parser {
 public:
  typedef Lexer::token_buffer token_buffer;
  typedef Lexer::token_iterator token_iterator;

 private:
  std::shared_ptr<ExprAst> root_;
  Lexer& lex_;
  std::list<Error> errors_;

 public:
  Parser(Lexer& lex) : lex_(lex) {}
  bool Parsing();
  bool Codegen();

 private:
  std::shared_ptr<ExprAst> ParseBlock(bool global = false);
  std::shared_ptr<ExprAst> ParseFuncDefine();
  std::shared_ptr<ExprAst> ParseStatment();
  std::shared_ptr<ExprAst> ParseAssignment();
  std::shared_ptr<ExprAst> ParseFuncCall();
  std::shared_ptr<ExprAst> ParseScan();
  std::shared_ptr<ExprAst> ParsePrint();
  std::shared_ptr<ExprAst> ParseScope();
  std::shared_ptr<ExprAst> ParseIf();
  std::shared_ptr<ExprAst> ParseWhile();
  std::shared_ptr<ExprAst> ParseCondition();
  std::shared_ptr<ExprAst> ParseExprssion();
  std::shared_ptr<ExprAst> ParseTerm();
  std::shared_ptr<ExprAst> ParseFactor();

  std::map<std::string, double> ParseConstDefine();
  std::set<std::string> ParseVarDefine();

  void NoAvailableToken();
};

#endif  // __PL0_PARSER_H__

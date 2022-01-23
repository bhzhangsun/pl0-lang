/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-12-08 09:18:11
 * @LastEditTime: 2022-01-23 09:57:58
 * @Description: Parser 语法解析器
 */

#include "parser.h"

#include <iostream>
#include <memory>

#include "error.h"
#include "expr_ast.h"
#include "lexer.h"
#include "token.h"

bool Parser::Parsing() {
  root_ = ParseBlock();

  Lexer::token_iterator current_ = lex_.next();
  if (lex_.eof(current_) || current_->type != Tag::SYMBOL_DOT ||  // .
      !lex_.eof(lex_.peek())) {                                   // EOF
    errors_.push_back(Error(ERRNO::ERRNO_NOTFOND_MODULE_END, current_->line,
                            lex_.peek()->value));
  }

  if (errors_.size() == 0) {
    return true;
  }

  for (Error e : errors_) {
    std::cerr << e << std::endl;
  }
  return false;
}

/**
 * @description: Block解析器，递归下降解析
 * @param {*}
 * @return {*} AST节点指针
 */
std::shared_ptr<ExprAst> Parser::ParseBlock() {
  std::unique_ptr<CommonAst> block_ptr =
      std::make_unique<CommonAst>(Tag::PESUDO);
  // 处每次以;作为一个单元外，若开始的符号是DOT则认为是程序结尾

  Lexer::token_iterator current_ = lex_.peek();
  if (!lex_.eof(current_) && current_->type == Tag::KEYWORD_CONST) {
    block_ptr->AddChild(ParseConstDefine());
  }
  current_ = lex_.peek();
  if (!lex_.eof(current_) && current_->type == Tag::KEYWORD_VAR) {
    block_ptr->AddChild(ParseVarDefine());
  }

  while (!lex_.eof(lex_.peek()) && lex_.peek()->type == Tag::KEYWORD_PROC) {
    block_ptr->AddChild(ParseFuncDefine());
  }

  block_ptr->AddChild(ParseStatment());

  return block_ptr;
}

/**
 * @description: 常量定义解析
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseConstDefine() {
  std::unique_ptr<CommonAst> const_ptr =
      std::make_unique<CommonAst>(Tag::KEYWORD_CONST);
  Lexer::token_iterator current_ = lex_.next();  // const
  bool first = true;
  do {
    // 消除分割符,
    if (!first && !lex_.eof(lex_.peek()) &&
        lex_.peek()->type == Tag::SYMBOL_COMMA) {  //,
      current_ = lex_.next();
    }
    first = false;

    auto ident = lex_.next();
    auto eq = lex_.next();
    auto val = lex_.next();
    if (lex_.eof(ident) || ident->type != Tag::IDENTIFIER) {
      errors_.push_back(
          Error(ERRNO::ERRNO_UNRECOGNIZE_IDENTITY, ident->line, ident->value));
    } else if (lex_.eof(eq) || eq->type != Tag::SYMBOL_EQ) {
      errors_.push_back(Error(ERRNO::ERRNO_REQUIRE_EQ, eq->line, eq->value));
    } else if (lex_.eof(val) || val->type != Tag::NUMBER) {
      errors_.push_back(
          Error(ERRNO::ERRNO_REQUIRE_NUMBER, ident->line, eq->value));
    } else {
      std::unique_ptr<CommonAst> temp_ptr =
          std::make_unique<CommonAst>(Tag::SYMBOL_EQ);
      temp_ptr->AddChild(std::make_unique<ValueAst>(*ident));
      temp_ptr->AddChild(std::make_unique<ValueAst>(*val));
      const_ptr->AddChild(std::move(temp_ptr));
    }

  } while (!lex_.eof(lex_.peek()) && lex_.peek()->type != Tag::SYMBOL_SEMI);

  lex_.next();  // 吃掉;
  return const_ptr;
}

/**
 * @description: 解析变量定义
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseVarDefine() {
  std::unique_ptr<CommonAst> var_ptr =
      std::make_unique<CommonAst>(Tag::KEYWORD_VAR);
  Lexer::token_iterator current_ = lex_.next();  // const
  bool first = true;
  do {
    // 消除分割符,
    if (!first && !lex_.eof(lex_.peek()) &&
        lex_.peek()->type == Tag::SYMBOL_COMMA) {  //,
      current_ = lex_.next();
    }
    first = false;

    auto ident = lex_.next();
    if (lex_.eof(ident) || ident->type != Tag::IDENTIFIER) {
      errors_.push_back(
          Error(ERRNO::ERRNO_REQUIRE_IDENTITY, ident->line, ident->value));
    } else {
      var_ptr->AddChild(std::make_unique<ValueAst>(*ident));
    }
  } while (!lex_.eof(lex_.peek()) && lex_.peek()->type != Tag::SYMBOL_SEMI);
  lex_.next();
  return var_ptr;
}

/**
 * @description: ParseFuncDefine
 * @param {*}
 * @return {*} 返回AST节点
 */
std::shared_ptr<ExprAst> Parser::ParseFuncDefine() {
  std::unique_ptr<CommonAst> func_ptr =
      std::make_unique<CommonAst>(Tag::KEYWORD_PROC);
  Lexer::token_iterator current_ = lex_.next();  // procedure

  auto ident = lex_.next();
  auto comma = lex_.next();
  std::shared_ptr<ExprAst> block_ptr = ParseBlock();
  auto comma1 = lex_.next();

  if (lex_.eof(ident) || ident->type != Tag::IDENTIFIER) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_IDENTITY, ident->line, ident->value));
  } else if (lex_.eof(comma) || comma->type != Tag::SYMBOL_COMMA) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_COMMA, comma->line, comma->value));
  } else if (lex_.eof(comma1) || comma1->type != Tag::SYMBOL_COMMA) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_COMMA, comma1->line, comma1->value));
  } else {
    func_ptr->AddChild(std::make_unique<ValueAst>(*ident));
    func_ptr->AddChild(block_ptr);
  }

  return func_ptr;
}

/**
 * @description: ParseStatment
 * @param {*}
 * @return {*} 返回AST节点
 */
std::shared_ptr<ExprAst> Parser::ParseStatment() {
  Lexer::token_iterator current_ = lex_.peek();
  if (lex_.eof(current_)) {
    return nullptr;
  }

  switch (current_->type) {
    case Tag::IDENTIFIER:
      return ParseAssignment();
    case Tag::KEYWORD_CALL:
      return ParseFuncCall();
    case Tag::SYMBOL_QUES:  // ?
      return ParseScan();
    case Tag::SYMBOL_EXCL:  // !
      return ParsePrint();
    case Tag::KEYWORD_BEGIN:
      return ParseScope();
    case Tag::KEYWORD_IF:
      return ParseIf();
    case Tag::KEYWORD_WHILE:
      return ParseWhile();
    default:
      errors_.push_back(Error(ERRNO::ERRNO_UNEXPECTED_SYMBOL_TYPE,
                              current_->line, current_->value));
  }
  return nullptr;
}

/**
 * @description: ParseAssignment
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseAssignment() {
  std::unique_ptr<CommonAst> assign_ptr =
      std::make_unique<CommonAst>(Tag::SYMBOL_BECOME);

  auto ident = lex_.next();
  auto become = lex_.next();
  std::shared_ptr<ExprAst> expr = ParseExprssion();  // 解析表达式

  if (lex_.eof(ident) || ident->type != Tag::IDENTIFIER) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_IDENTITY, ident->line, ident->value));
  } else if (lex_.eof(become) || become->type != Tag::SYMBOL_BECOME) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_BECOME, ident->line, ident->value));
  } else {
    assign_ptr->AddChild(std::make_unique<ValueAst>(*ident));
    assign_ptr->AddChild(expr);
  }
  return assign_ptr;
}

/**
 * @description: ParseFuncCall
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseFuncCall() {
  std::unique_ptr<CommonAst> call_ptr =
      std::make_unique<CommonAst>(Tag::KEYWORD_CALL);
  Lexer::token_iterator current_ = lex_.next();  // call

  auto ident = lex_.next();

  if (lex_.eof(ident) || ident->type != Tag::IDENTIFIER) {
    errors_.push_back(Error(ERRNO_REQUIRE_IDENTITY, ident->line, ident->value));
  } else {
    call_ptr->AddChild(std::make_unique<ValueAst>(*ident));
  }

  return call_ptr;
}

/**
 * @description: ParseScan
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseScan() {
  std::unique_ptr<CommonAst> in_ptr =
      std::make_unique<CommonAst>(Tag::SYMBOL_QUES);
  Lexer::token_iterator current_ = lex_.next();  // ?

  auto ident = lex_.next();

  if (lex_.eof(ident) || ident->type != Tag::IDENTIFIER) {
    errors_.push_back(Error(ERRNO_REQUIRE_IDENTITY, ident->line, ident->value));
  } else {
    in_ptr->AddChild(std::make_unique<ValueAst>(*ident));
  }
  return in_ptr;
}

/**
 * @description: ParsePrint
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParsePrint() {
  std::unique_ptr<CommonAst> out_ptr =
      std::make_unique<CommonAst>(Tag::SYMBOL_EXCL);
  Lexer::token_iterator current_ = lex_.next();  // !

  out_ptr->AddChild(ParseExprssion());
  return out_ptr;
}

/**
 * @description: ParseScope
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseScope() {
  std::unique_ptr<CommonAst> scope_ptr =
      std::make_unique<CommonAst>(Tag::KEYWORD_BEGIN);
  Lexer::token_iterator current_ = lex_.next();  // begin

  bool first = true;
  do {
    if (!first) {
      if (lex_.eof(lex_.peek()) || lex_.peek()->type != Tag::SYMBOL_SEMI) {
        errors_.push_back(
            Error(ERRNO_REQUIRE_SEMI, lex_.peek()->line, lex_.peek()->value));
      }
      lex_.next();
    }
    first = false;

    scope_ptr->AddChild(ParseStatment());
  } while (!lex_.eof(lex_.peek()) && lex_.peek()->type != Tag::KEYWORD_END);

  return scope_ptr;
}

/**
 * @description: ParseIf
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseIf() {
  std::unique_ptr<CommonAst> if_ptr =
      std::make_unique<CommonAst>(Tag::KEYWORD_IF);
  Lexer::token_iterator current_ = lex_.next();  // if

  if_ptr->AddChild(ParseCondition());
  auto then = lex_.next();
  if (lex_.eof(then) || then->type != KEYWORD_THEN) {  // then
    // errors_.push_back(Error());
  }
  if_ptr->AddChild(ParseStatment());
  return if_ptr;
}

/**
 * @description: ParseWhile
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseWhile() {
  std::unique_ptr<CommonAst> while_ptr =
      std::make_unique<CommonAst>(Tag::KEYWORD_WHILE);
  Lexer::token_iterator current_ = lex_.next();  // while

  while_ptr->AddChild(ParseCondition());
  auto then = lex_.next();
  if (lex_.eof(then) || then->type != KEYWORD_DO) {  // do
    // errors_.push_back(Error());
  }
  while_ptr->AddChild(ParseStatment());
  return while_ptr;
}

/**
 * @description: ParseCondition
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseCondition() {
  std::unique_ptr<CommonAst> cond_ptr = std::make_unique<CommonAst>();
  Lexer::token_iterator current_ = lex_.peek();  // while

  if (!lex_.eof(current_) && current_->type == Tag::SYMBOL_ODD) {
    cond_ptr->set_token_type(SYMBOL_ODD);
    cond_ptr->AddChild(ParseExprssion());

  } else {
    cond_ptr->AddChild(ParseExprssion());
    auto op = lex_.next();
    cond_ptr->set_token_type(op->type);
    cond_ptr->AddChild(ParseExprssion());
  }
  return cond_ptr;
}

/**
 * @description: ParseExprssion
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseExprssion() {
  std::shared_ptr<ExprAst> root_ptr = nullptr;
  bool first = true;
  do {
    auto op = lex_.peek();
    if (!lex_.eof(op) &&
        (op->type == Tag::SYMBOL_ADD || op->type == Tag::SYMBOL_SUB)) {
      std::unique_ptr<CommonAst> temp_ptr =
          std::make_unique<CommonAst>(op->type);

      if (first) {
        temp_ptr->AddChild(std::make_unique<ValueAst>(Token("0")));
      } else {
        temp_ptr->AddChild(root_ptr);
      }
      lex_.next();  // +/-
      temp_ptr->AddChild(ParseTerm());
      root_ptr = std::move(temp_ptr);
    } else {
      root_ptr = ParseTerm();
    }

  } while (!lex_.eof(lex_.peek()) && (lex_.peek()->type == Tag::SYMBOL_ADD ||
                                      lex_.peek()->type == Tag::SYMBOL_SUB));
  return root_ptr;
}

/**
 * @description: ParseTerm
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseTerm() {
  std::shared_ptr<ExprAst> root_ptr = nullptr;
  bool first = true;
  do {
    if (first) {
      root_ptr = ParseFactor();
      first = false;
    } else {
      auto op = lex_.next();
      std::unique_ptr<CommonAst> temp_ptr =
          std::make_unique<CommonAst>(op->type);
      temp_ptr->AddChild(root_ptr);
      temp_ptr->AddChild(ParseFactor());
      root_ptr = std::move(temp_ptr);
    }

  } while (!lex_.eof(lex_.peek()) && (lex_.peek()->type == Tag::SYMBOL_MUL ||
                                      lex_.peek()->type == Tag::SYMBOL_DIV));
  return root_ptr;
}

/**
 * @description: ParseFactor
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseFactor() {
  Lexer::token_iterator current_ = lex_.peek();

  if (lex_.eof(current_)) {
    // errors_.push_back(Error()) // 意外的文件尾
    return nullptr;
  }

  switch (current_->type) {
    case Tag::IDENTIFIER:
      return std::make_unique<ValueAst>(*lex_.next());
    case Tag::NUMBER:
      return std::make_unique<ValueAst>(*lex_.next());
    case Tag::SYMBOL_LPARENT: {
      lex_.next();  // (
      std::shared_ptr<ExprAst> root_ptr = ParseExprssion();
      lex_.next();
      return root_ptr;
    }
    default:
      errors_.push_back(
          Error(ERRNO_UNRECOGNIZE_TOKEN, current_->line, current_->value));
  };
  return nullptr;
}
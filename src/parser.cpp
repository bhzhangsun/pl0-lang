/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-12-08 09:18:11
 * @LastEditTime: 2022-03-14 21:34:51
 * @Description: Parser 语法解析器
 */

#include "parser.h"

#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "compiler.h"
#include "error.h"
#include "expr_ast.h"
#include "lexer.h"
#include "logger.hpp"
#include "token.h"

bool Parser::Parsing() {
  root_ = ParseBlock(true);
  Lexer::token_iterator current = lex_.next();

  if (lex_.eof(current)) {
    errors_.push_back(
        Error(ERRNO::ERRNO_NOTFOND_MODULE_END, (--current)->line, "EOF"));
  } else if (current->type != Tag::SYMBOL_DOT ||  // .
             !lex_.eof(lex_.peek())) {            // EOF
    errors_.push_back(
        Error(ERRNO::ERRNO_NOTFOND_MODULE_END, current->line, current->value));
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
std::shared_ptr<ExprAst> Parser::ParseBlock(bool global) {
  std::unique_ptr<PesudoExpr> block_ptr = std::make_unique<PesudoExpr>();

  // 处每次以;作为一个单元外，若开始的符号是DOT则认为是程序结尾
  std::map<std::string, double> consts;
  Lexer::token_iterator current = lex_.peek();
  if (!lex_.eof(current) && current->type == Tag::KEYWORD_CONST) {
    consts = ParseConstDefine();
  }
  current = lex_.peek();
  std::set<std::string> vars;
  if (!lex_.eof(current) && current->type == Tag::KEYWORD_VAR) {
    vars = ParseVarDefine();
  }

  if (global) {
    std::shared_ptr<GlobalVariableExpr> global_ptr =
        std::make_shared<GlobalVariableExpr>();
    for (auto it : consts) {
      global_ptr->AddGlobalVariable(it.first, it.second, true);
    }
    for (auto it : vars) {
      global_ptr->AddGlobalVariable(it, 0, false);
    }
    if (!global_ptr->empty()) block_ptr->AddChild(global_ptr);
  } else {
    std::shared_ptr<LocalVariableExpr> local_ptr =
        std::make_shared<LocalVariableExpr>();
    for (auto it : consts) {
      local_ptr->AddLocalVariable(it.first, it.second, true);
    }
    for (auto it : vars) {
      local_ptr->AddLocalVariable(it, 0, false);
    }
    if (!local_ptr->empty()) block_ptr->AddChild(local_ptr);
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
std::map<std::string, double> Parser::ParseConstDefine() {
  std::map<std::string, double> consts;
  Lexer::token_iterator current = lex_.next();  // const
  bool first = true;
  do {
    // 消除分割符,
    if (!first && !lex_.eof(lex_.peek()) &&
        lex_.peek()->type == Tag::SYMBOL_COMMA) {  //,
      current = lex_.next();
    }
    first = false;

    auto ident = lex_.next();
    auto eq = lex_.next();
    auto val = lex_.next();
    if (lex_.eof(ident) || lex_.eof(eq) || lex_.eof(val)) {
      NoAvailableToken();
    } else if (ident->type != Tag::IDENTIFIER) {
      errors_.push_back(
          Error(ERRNO::ERRNO_UNRECOGNIZE_IDENTITY, ident->line, ident->value));
    } else if (eq->type != Tag::SYMBOL_EQ) {
      errors_.push_back(Error(ERRNO::ERRNO_REQUIRE_EQ, eq->line, eq->value));
    } else if (val->type != Tag::NUMBER) {
      errors_.push_back(
          Error(ERRNO::ERRNO_REQUIRE_NUMBER, ident->line, eq->value));
    } else {
      consts[ident->value] = std::stod(val->value);
    }

  } while (!lex_.eof(lex_.peek()) && lex_.peek()->type != Tag::SYMBOL_SEMI);

  if (lex_.eof(lex_.peek())) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_SEMI, (--lex_.peek())->line, "EOF"));
  }
  lex_.next();  // 吃掉;
  return consts;
}

/**
 * @description: 解析变量定义
 * @param {*}
 * @return {*}
 */
std::set<std::string> Parser::ParseVarDefine() {
  std::set<std::string> vars;
  Lexer::token_iterator current = lex_.next();  // var
  bool first = true;
  do {
    // 消除分割符,
    if (!first && !lex_.eof(lex_.peek()) &&
        lex_.peek()->type == Tag::SYMBOL_COMMA) {  //,
      current = lex_.next();
    }
    first = false;

    auto ident = lex_.next();
    if (lex_.eof(ident)) {
      NoAvailableToken();
    } else if (ident->type != Tag::IDENTIFIER) {
      errors_.push_back(
          Error(ERRNO::ERRNO_REQUIRE_IDENTITY, ident->line, ident->value));
    } else {
      vars.insert(ident->value);
    }
  } while (!lex_.eof(lex_.peek()) && lex_.peek()->type != Tag::SYMBOL_SEMI);
  if (lex_.eof(lex_.peek())) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_SEMI, (--lex_.peek())->line, "EOF"));
  }
  lex_.next();
  return vars;
}

/**
 * @description: ParseFuncDefine
 * @param {*}
 * @return {*} 返回AST节点
 */
std::shared_ptr<ExprAst> Parser::ParseFuncDefine() {
  std::unique_ptr<FunctionExpr> func_ptr = nullptr;
  Lexer::token_iterator current = lex_.next();  // procedure

  auto ident = lex_.next();
  auto semi = lex_.next();
  std::shared_ptr<ExprAst> block_ptr = ParseBlock(false);

  auto semi1 = lex_.next();

  if (lex_.eof(ident) || lex_.eof(semi) || lex_.eof(semi1)) {
    NoAvailableToken();
  } else if (ident->type != Tag::IDENTIFIER) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_IDENTITY, ident->line, ident->value));
  } else if (semi->type != Tag::SYMBOL_SEMI) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_SEMI, semi->line, semi->value));
  } else if (semi1->type != Tag::SYMBOL_SEMI) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_SEMI, semi1->line, semi1->value));
  } else {
    func_ptr = std::make_unique<FunctionExpr>(ident->value, block_ptr);
  }

  return func_ptr;
}

/**
 * @description: ParseStatment
 * @param {*}
 * @return {*} 返回AST节点
 */
std::shared_ptr<ExprAst> Parser::ParseStatment() {
  Lexer::token_iterator current = lex_.peek();
  if (lex_.eof(current)) {
    return nullptr;
  }

  switch (current->type) {
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
                              current->line, current->value));
  }
  return nullptr;
}

/**
 * @description: ParseAssignment
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseAssignment() {
  std::unique_ptr<BinaryOpExpr> assign_ptr = nullptr;

  auto ident = lex_.next();
  auto become = lex_.next();
  std::shared_ptr<ExprAst> expr = ParseExprssion();  // 解析表达式

  if (lex_.eof(ident) || lex_.eof(become)) {
    NoAvailableToken();
  } else if (ident->type != Tag::IDENTIFIER) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_IDENTITY, ident->line, ident->value));
  } else if (become->type != Tag::SYMBOL_BECOME) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_BECOME, ident->line, ident->value));
  } else {
    std::shared_ptr<NamedExpr> named_ptr =
        std::make_shared<NamedExpr>(ident->value);
    assign_ptr =
        std::make_unique<BinaryOpExpr>(Tag::SYMBOL_BECOME, named_ptr, expr);
  }
  return assign_ptr;
}

/**
 * @description: ParseFuncCall
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseFuncCall() {
  std::unique_ptr<CallExpr> call_ptr = nullptr;
  Lexer::token_iterator current = lex_.next();  // call

  auto ident = lex_.next();

  if (lex_.eof(ident)) {
    NoAvailableToken();
  } else if (ident->type != Tag::IDENTIFIER) {
    errors_.push_back(Error(ERRNO_REQUIRE_IDENTITY, ident->line, ident->value));
  } else {
    call_ptr = std::make_unique<CallExpr>(ident->value);
  }

  return call_ptr;
}

/**
 * @description: ParseScan
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseScan() {
  std::unique_ptr<ScanExpr> in_ptr = nullptr;

  Lexer::token_iterator current = lex_.next();  // ?

  auto ident = lex_.next();
  if (lex_.eof(ident)) {
    NoAvailableToken();
  } else if (ident->type != Tag::IDENTIFIER) {
    errors_.push_back(Error(ERRNO_REQUIRE_IDENTITY, ident->line, ident->value));
  } else {
    in_ptr =
        std::make_unique<ScanExpr>(std::make_unique<NamedExpr>(ident->value));
  }
  return in_ptr;
}

/**
 * @description: ParsePrint
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParsePrint() {
  Lexer::token_iterator current = lex_.next();  // !
  std::unique_ptr<PrintExpr> out_ptr =
      std::make_unique<PrintExpr>(ParseExprssion());

  return out_ptr;
}

/**
 * @description: ParseScope
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseScope() {
  std::unique_ptr<PesudoExpr> scope_ptr = std::make_unique<PesudoExpr>();
  Lexer::token_iterator current = lex_.next();  // begin

  bool first = true;
  do {
    if (!first) {
      if (lex_.eof(lex_.peek())) {
        NoAvailableToken();
      } else if (lex_.peek()->type != Tag::SYMBOL_SEMI) {
        errors_.push_back(
            Error(ERRNO_REQUIRE_SEMI, lex_.peek()->line, lex_.peek()->value));
      }
      lex_.next();
    }
    first = false;

    scope_ptr->AddChild(ParseStatment());
  } while (!lex_.eof(lex_.peek()) && lex_.peek()->type != Tag::KEYWORD_END);
  if (lex_.eof(lex_.peek())) {
    errors_.push_back(
        Error(ERRNO::ERRNO_REQUIRE_END, (--lex_.peek())->line, "EOF"));
  }
  lex_.next();  // eof or end
  return scope_ptr;
}

/**
 * @description: ParseIf
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseIf() {
  std::unique_ptr<IfExpr> if_ptr = nullptr;

  Lexer::token_iterator current = lex_.next();  // if

  std::shared_ptr<ExprAst> cond_ptr = ParseCondition();
  auto then = lex_.next();
  if (lex_.eof(then)) {
    NoAvailableToken();
  } else if (then->type != KEYWORD_THEN) {  // then
    errors_.push_back(Error(ERRNO_REQUIRE_THEN, then->line, then->value));
  }
  std::shared_ptr<ExprAst> body_ptr = ParseStatment();
  if_ptr = std::make_unique<IfExpr>(cond_ptr, body_ptr);
  return if_ptr;
}

/**
 * @description: ParseWhile
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseWhile() {
  Lexer::token_iterator current = lex_.next();  // while

  std::shared_ptr<ExprAst> cond_ptr = ParseCondition();
  auto doo = lex_.next();
  if (lex_.eof(doo)) {
    NoAvailableToken();
  } else if (doo->type != KEYWORD_DO) {  // do
    errors_.push_back(Error(ERRNO_REQUIRE_DO, doo->line, doo->value));
  }
  std::shared_ptr<ExprAst> body_ptr = ParseStatment();

  std::unique_ptr<WhileExpr> while_ptr =
      std::make_unique<WhileExpr>(cond_ptr, body_ptr);
  return while_ptr;
}

/**
 * @description: ParseCondition
 * @param {*}
 * @return {*}
 */
std::shared_ptr<ExprAst> Parser::ParseCondition() {
  std::unique_ptr<ExprAst> cond_ptr = nullptr;
  Lexer::token_iterator current = lex_.peek();  // odd | expr cond expr

  if (!lex_.eof(current) && current->type == Tag::SYMBOL_ODD) {
    lex_.next();
    std::shared_ptr<ExprAst> expr_ptr = ParseExprssion();
    cond_ptr = std::make_unique<OddExpr>(expr_ptr);
  } else {
    std::shared_ptr<ExprAst> lhs_ptr = ParseExprssion();
    auto op = lex_.next();
    std::shared_ptr<ExprAst> rhs_ptr = ParseExprssion();
    cond_ptr = std::make_unique<BinaryOpExpr>(op->type, lhs_ptr, rhs_ptr);
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
      std::unique_ptr<BinaryOpExpr> temp_ptr = nullptr;
      std::shared_ptr<ExprAst> lhs_ptr = nullptr;
      std::shared_ptr<ExprAst> rhs_ptr = nullptr;

      if (first) {
        lhs_ptr = std::make_unique<NumberExpr>(0);
      } else {
        lhs_ptr = root_ptr;
      }

      lex_.next();  // +/-
      rhs_ptr = ParseTerm();
      temp_ptr = std::make_unique<BinaryOpExpr>(op->type, lhs_ptr, rhs_ptr);
      root_ptr = std::move(temp_ptr);
    } else {
      root_ptr = ParseTerm();
    }
    first = false;

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
    } else {
      auto op = lex_.next();
      std::shared_ptr<ExprAst> rhs_ptr = ParseFactor();
      std::unique_ptr<BinaryOpExpr> temp_ptr =
          std::make_unique<BinaryOpExpr>(op->type, root_ptr, rhs_ptr);
      root_ptr = std::move(temp_ptr);
    }
    first = false;
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
  Lexer::token_iterator current = lex_.peek();

  if (lex_.eof(current)) {
    NoAvailableToken();  // 意外的文件尾
    return nullptr;
  }

  switch (current->type) {
    case Tag::IDENTIFIER:
      return std::make_unique<NamedExpr>(lex_.next()->value);
    case Tag::NUMBER:
      return std::make_unique<NumberExpr>(lex_.next()->value);
    case Tag::SYMBOL_LPARENT: {
      lex_.next();  // (
      std::shared_ptr<ExprAst> root_ptr = ParseExprssion();
      lex_.next();
      return root_ptr;
    }
    default:
      errors_.push_back(
          Error(ERRNO_UNRECOGNIZE_TOKEN, current->line, current->value));
  };
  return nullptr;
}

void Parser::NoAvailableToken() {
  token_iterator tail = --(lex_.token_stream().end());
  errors_.push_back(Error(ERRNO::ERRNO_NO_AVAILABLE_TOKEN, tail->line, "EOF"));
}

bool Parser::Codegen() {
  try {
    llvm::FunctionType *FT =
        llvm::FunctionType::get(llvm::Type::getVoidTy(*TheContext), false);
    llvm::Function *F = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, "module_entry", TheModule.get());
    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", F);
    Builder->SetInsertPoint(BB);
    root_->Codegen();
    Builder->CreateRet(nullptr);
  } catch (const std::exception &e) {
    return false;
  }
  return true;
}
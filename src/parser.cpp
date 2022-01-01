/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-12-08 09:18:11
 * @LastEditTime: 2021-12-13 22:44:27
 * @Description: Parser 语法解析器
 */

#include "parser.h"

#include <iostream>
#include <memory>

#include "token.h"

void Parser::Parsing() {
  root_ = ParseBlock();
  if (current_ == token_stream_.end() || current_->type != Tag::SYMBOL_DOT) {
    std::cout << current_->line << ": module end not found!" << std::endl;
  }
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
  //   while (current_ != token_stream_.end() && current_->type !=
  //   Tag::SYMBOL_DOT) {
  if (current_->type == Tag::KEYWORD_CONST) {
    block_ptr->AddChild(ParseConstDefine());
  }
  if (current_->type == Tag::KEYWORD_VAR) {
    block_ptr->AddChild(ParseVarDefine());
  }
  while (current_->type == Tag::KEYWORD_PROC) {
    block_ptr->AddChild(ParseFuncDefine());
  }
  block_ptr->AddChild(ParseStatment());
  //   }
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
  current_++;
  do {
    auto ident = current_++;
    auto eq = current_++;
    auto val = current_++;
    if (ident->type != Tag::IDENTIFIER) {
      std::cout << ident->line << ": " << ident->value << "应该为标识符;";
      continue;
    }
    if (eq->type != Tag::SYMBOL_EQ) {
      std::cout << ident->line << ": " << eq->value << "应该为=;";
      continue;
    }
    if (val->type != Tag::NUMBER) {
      std::cout << ident->line << ": " << eq->value << "应该为数字字面值";
      continue;
    }
    std::unique_ptr<CommonAst> temp_ptr =
        std::make_unique<CommonAst>(Tag::SYMBOL_EQ);
    temp_ptr->AddChild(std::make_unique<ValueAst>(*ident));
    temp_ptr->AddChild(std::make_unique<ValueAst>(*val));
    const_ptr->AddChild(std::move(temp_ptr));

    // 消除分割符,
    if (current_->type == Tag::SYMBOL_COMMA) {
      current_++;
    }
  } while (current_->type != Tag::SYMBOL_SEMI);
  current_++;
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
  do {
    auto ident = current_++;
    if (ident->type != Tag::IDENTIFIER) {
      std::cout << ident->line << ": " << ident->value << "应该为标识符;";
      continue;
    }
    var_ptr->AddChild(std::make_unique<ValueAst>(*ident));
    if (current_->type == Tag::SYMBOL_COMMA) {
      current_++;
    }
  } while (current_->type != Tag::SYMBOL_SEMI);
  current_++;
  return var_ptr;
}

std::shared_ptr<ExprAst> Parser::ParseFuncDefine() {
  std::unique_ptr<CommonAst> func_ptr =
      std::make_unique<CommonAst>(Tag::KEYWORD_PROC);
  current_++;
  do {
    auto ident = current_++;
    auto comma = current_++;
    std::shared_ptr<ExprAst> block_ptr = ParseBlock();
    auto comma1 = current_++;

    if (ident->type != Tag::IDENTIFIER) {
      std::cout << ident->line << ": " << ident->value << "应该为标识符;";
      continue;
    }
    if (comma->type != Tag::SYMBOL_COMMA) {
      std::cout << comma->line << ": " << comma->value << "应该为;";
      continue;
    }
    if (comma1->type != Tag::SYMBOL_COMMA) {
      std::cout << comma1->line << ": " << comma1->value << "应该为;";
      continue;
    }
    func_ptr->AddChild(std::make_unique<ValueAst>(*ident));
    func_ptr->AddChild(block_ptr);

  } while (current_->type != Tag::SYMBOL_SEMI);
  current_++;

  return func_ptr;
}

std::shared_ptr<ExprAst> Parser::ParseStatment() {
  std::unique_ptr<CommonAst> stat_ptr =
      std::make_unique<CommonAst>(Tag::PESUDO);

  switch (current_->type) {
    case Tag::IDENTIFIER:
    case Tag::KEYWORD_CALL:
    case Tag::SYMBOL_QUES:  // ?
    case Tag::SYMBOL_EXCL:  // !
    case Tag::KEYWORD_BEGIN:
    case Tag::KEYWORD_IF:
    case Tag::KEYWORD_WHILE:
    default:
      std::cout << "error:"
                << "出现意外的符号类型" << current_->type << std::endl;
  }
  return stat_ptr;
}

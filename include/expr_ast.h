/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-22 08:34:07
 * @LastEditTime: 2021-12-13 21:24:49
 * @Description: ExprAst.h 定义最小语义单元结构，他是组成Ast的结点
 */

#if !defined(__PL0_EXPR_Ast_H__)
#define __PL0_EXPR_Ast_H__

#include <iostream>
#include <memory>
#include <vector>

#include "token.h"

struct ExprAst {
  virtual ~ExprAst() {}
};

class ValueAst : public ExprAst {
 private:
  Token value_;

 public:
  ValueAst(const Token& t) : value_(t){};
};

class CommonAst : public ExprAst {
 private:
  Tag token_type_;
  std::vector<std::shared_ptr<ExprAst>> children_;
  // op [src, dst]/[1st, 2rd]
  // call arg
  // if cond body
  // procedure name body
  // block op1, op2, op3, ...

 public:
  CommonAst(Tag type = PESUDO) : token_type_(type){};

  void AddChild(const std::shared_ptr<ExprAst>& ptr);

  // friend std::ostream &operator<<(std::ostream &out, const CommonAst &ast);
};

#endif  // __PL0_EXPR_Ast_H__

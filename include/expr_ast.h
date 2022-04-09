/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-22 08:34:07
 * @LastEditTime: 2022-03-14 21:37:04
 * @Description: ExprAst.h 定义最小语义单元结构，他是组成Ast的结点
 */

#if !defined(__PL0_EXPR_AST_H__)
#define __PL0_EXPR_AST_H__

#include <llvm/IR/Value.h>
#include <llvm/Support/ExtensibleRTTI.h>

#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "token.h"

struct ExprAst : public llvm::RTTIExtends<ExprAst, llvm::RTTIRoot> {
  virtual ~ExprAst() {}

 public:
  static char ID;
  virtual llvm::Value *Codegen(bool isLhs = false) const = 0;
};

class NumberExpr : public llvm::RTTIExtends<NumberExpr, ExprAst> {
 private:
  double value_;

 public:
  static char ID;
  NumberExpr(double val) : value_(val){};
  NumberExpr(const std::string &val) : value_(std::stod(val)){};
  llvm::Value *Codegen(bool isLhs = false) const override;
};

class NamedExpr : public llvm::RTTIExtends<NamedExpr, ExprAst> {
 private:
  std::string name_;

 public:
  static char ID;
  NamedExpr(const std::string &val) : name_(val){};
  llvm::Value *Codegen(bool isLhs = false) const override;
};

// class ConstExpr : public llvm::RTTIExtends<ConstExpr, ExprAst> {
//  private:
//   std::map<std::string, double> consts_;

//  public:
//   static char ID;
//   void AddConstant(const std::string &key, double value);
//   llvm::Value *Codegen(bool isLhs = false) const override;
// };

// class VariableExpr : public llvm::RTTIExtends<VariableExpr, ExprAst> {
//  private:
//   std::set<std::string> vars_;

//  public:
//   static char ID;
//   void AddVariant(const std::string &key);
//   llvm::Value *Codegen(bool isLhs = false) const override;
// };

class GlobalVariableExpr
    : public llvm::RTTIExtends<GlobalVariableExpr, ExprAst> {
 private:
  std::map<std::string, std::pair<double, bool>> vars_;

 public:
  static char ID;
  void AddGlobalVariable(const std::string &key, double value, bool isConst);
  llvm::Value *Codegen(bool isLhs = false) const override;
  bool empty() { return vars_.empty(); }
};

class LocalVariableExpr : public llvm::RTTIExtends<LocalVariableExpr, ExprAst> {
 private:
  std::map<std::string, std::pair<double, bool>> vars_;

 public:
  static char ID;
  void AddLocalVariable(const std::string &key, double value, bool isConst);
  llvm::Value *Codegen(bool isLhs = false) const override;
  bool empty() { return vars_.empty(); }
};

class BinaryOpExpr : public llvm::RTTIExtends<BinaryOpExpr, ExprAst> {
 private:
  Tag op_type_;  // + - * / = # < <= > >= :=
  std::shared_ptr<ExprAst> lhs_;
  std::shared_ptr<ExprAst> rhs_;

 public:
  static char ID;
  BinaryOpExpr(Tag type, const std::shared_ptr<ExprAst> &lhs,
               const std::shared_ptr<ExprAst> &rhs)
      : op_type_(type), lhs_(lhs), rhs_(rhs) {}
  llvm::Value *Codegen(bool isLhs = false) const override;
};

class UnaryOpExpr : public llvm::RTTIExtends<UnaryOpExpr, ExprAst> {
 private:
  Tag op_type_;  // odd call ? !
  std::shared_ptr<ExprAst> value_;

 public:
  static char ID;
  UnaryOpExpr(Tag type, const std::shared_ptr<ExprAst> &val)
      : op_type_(type), value_(val) {}
  llvm::Value *Codegen(bool isLhs = false) const override;
};

class PesudoExpr : public llvm::RTTIExtends<PesudoExpr, ExprAst> {
 private:
  std::vector<std::shared_ptr<ExprAst>> children_;

 public:
  static char ID;
  void AddChild(const std::shared_ptr<ExprAst> &child);
  llvm::Value *Codegen(bool isLhs = false) const override;
};

class FunctionExpr : public llvm::RTTIExtends<FunctionExpr, ExprAst> {
 private:
  std::string name_;
  std::shared_ptr<ExprAst> body_;

 public:
  static char ID;
  FunctionExpr(const std::string &name, const std::shared_ptr<ExprAst> &body)
      : name_(name), body_(body) {}
  llvm::Value *Codegen(bool isLhs = false) const override;
};

class CallExpr : public llvm::RTTIExtends<CallExpr, ExprAst> {
 private:
  std::string name_;

 public:
  static char ID;
  CallExpr(const std::string &name) : name_(name) {}
  llvm::Value *Codegen(bool isLhs = false) const override;
};

class OddExpr : public llvm::RTTIExtends<OddExpr, ExprAst> {
 private:
  std::shared_ptr<ExprAst> expr_;

 public:
  static char ID;
  OddExpr(const std::shared_ptr<ExprAst> &expr) : expr_(expr) {}
  llvm::Value *Codegen(bool isLhs = false) const override;
};

class ScanExpr : public llvm::RTTIExtends<ScanExpr, ExprAst> {
 private:
  std::shared_ptr<ExprAst> var_;

 public:
  static char ID;
  ScanExpr(const std::shared_ptr<ExprAst> &var) : var_(var) {}
  llvm::Value *Codegen(bool isLhs = false) const override;
};

class PrintExpr : public llvm::RTTIExtends<PrintExpr, ExprAst> {
 private:
  std::shared_ptr<ExprAst> expr_;

 public:
  static char ID;
  PrintExpr(const std::shared_ptr<ExprAst> &expr) : expr_(expr) {}
  llvm::Value *Codegen(bool isLhs = false) const override;
};

class IfExpr : public llvm::RTTIExtends<IfExpr, ExprAst> {
 private:
  std::shared_ptr<ExprAst> cond_;
  std::shared_ptr<ExprAst> body_;

 public:
  static char ID;
  IfExpr(const std::shared_ptr<ExprAst> &cond,
         const std::shared_ptr<ExprAst> &body)
      : cond_(cond), body_(body) {}
  llvm::Value *Codegen(bool isLhs = false) const override;
};

class WhileExpr : public llvm::RTTIExtends<WhileExpr, ExprAst> {
 private:
  std::shared_ptr<ExprAst> cond_;
  std::shared_ptr<ExprAst> body_;

 public:
  static char ID;
  WhileExpr(const std::shared_ptr<ExprAst> &cond,
            const std::shared_ptr<ExprAst> &body)
      : cond_(cond), body_(body) {}
  llvm::Value *Codegen(bool isLhs = false) const override;
};

#endif  // __PL0_EXPR_AST_H__

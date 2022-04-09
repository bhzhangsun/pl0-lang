/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-12-09 22:18:54
 * @LastEditTime: 2022-03-16 23:05:50
 * @Description: Expr_Ast实现
 */

#include "expr_ast.h"

#include <llvm-c/Core.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/Casting.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

#include "compiler.h"
#include "error.h"

char ExprAst::ID = 0;
char NumberExpr::ID = 0;
char NamedExpr::ID = 0;
// char ConstExpr::ID = 0;
// char VariableExpr::ID = 0;
char GlobalVariableExpr::ID = 0;
char LocalVariableExpr::ID = 0;
char BinaryOpExpr::ID = 0;
char UnaryOpExpr::ID = 0;
char PesudoExpr::ID = 0;
char FunctionExpr::ID = 0;
char CallExpr::ID = 0;
char OddExpr::ID = 0;
char ScanExpr::ID = 0;
char PrintExpr::ID = 0;
char IfExpr::ID = 0;
char WhileExpr::ID = 0;

llvm::Value *NumberExpr::Codegen(bool isLhs) const {
  assert(!isLhs);
  return llvm::ConstantFP::get(*TheContext, llvm::APFloat(value_));
}

llvm::Value *NamedExpr::Codegen(bool isLhs) const {
  // Look this variable up in the function.
  llvm::Value *V = nullptr;
  if (LocalValues.count(name_) > 0) {
    V = LocalValues[name_];
  }
  if (V == nullptr) {
    V = TheModule->getGlobalVariable(name_);
    // V = GlobalValues[name_];
    // llvm::outs() << V << "\n";
  }
  if (V != nullptr && !isLhs) {
    V = Builder->CreateLoad(V);
  }
  if (V == nullptr) {
    llvm::errs() << Error(ERRNO_NOTFOUND_DEFINED_SYMBOL, name_);
  }
  return V;
}

// llvm::Value *ConstExpr::Codegen(bool isLhs) const {
//   assert(!isLhs);
//   for (auto child : consts_) {
//     if (ConstantValues.count(child.first) > 0 ||
//         MutableValues.count(child.first) > 0) {
//       llvm::errs() << Error(ERRNO_DEFINED_SYMBOL_CONFLICT, child.first);
//     }
//     ConstantValues[child.first] =
//         llvm::ConstantFP::get(*TheContext, llvm::APFloat(child.second));
//   }
//   return nullptr;
// }

// llvm::Value *VariableExpr::Codegen(bool isLhs) const {
//   assert(!isLhs);
//   for (auto var : vars_) {
//     if (MutableValues.count(var) > 0 || ConstantValues.count(var) > 0) {
//       llvm::errs() << Error(ERRNO_DEFINED_SYMBOL_CONFLICT, var);
//     }

//     MutableValues[var] = Builder->CreateAlloca(
//         llvm::Type::getDoubleTy(*TheContext), nullptr, var);
//   }
//   return nullptr;
// }

void GlobalVariableExpr::AddGlobalVariable(const std::string &key, double value,
                                           bool isConst) {
  vars_[key] = std::make_pair(value, isConst);
}

llvm::Value *GlobalVariableExpr::Codegen(bool isLhs) const {
  assert(!isLhs);
  llvm::Value *resptr = nullptr;
  for (auto var : vars_) {
    // resptr = new llvm::GlobalVariable(
    //     llvm::Type::getDoubleTy(*TheContext), var.second.second,
    //     llvm::GlobalVariable::LinkageTypes::ExternalLinkage,
    //     llvm::ConstantFP::get(*TheContext, llvm::APFloat(var.second.first)),
    //     var.first);
    auto varptr = llvm::cast<llvm::GlobalVariable>(TheModule->getOrInsertGlobal(
        var.first, llvm::Type::getDoubleTy(*TheContext)));
    varptr->setConstant(var.second.second);
    varptr->setLinkage(llvm::GlobalVariable::LinkageTypes::ExternalLinkage);
    varptr->setInitializer(
        llvm::ConstantFP::get(*TheContext, llvm::APFloat(var.second.first)));
    GlobalValues[var.first] = varptr;
    resptr = varptr;
  }
  return resptr;
}

void LocalVariableExpr::AddLocalVariable(const std::string &key, double value,
                                         bool isConst) {
  vars_[key] = std::make_pair(value, isConst);
}

llvm::Value *LocalVariableExpr::Codegen(bool isLhs) const {
  assert(!isLhs);
  llvm::Value *resptr = nullptr;
  for (auto var : vars_) {
    auto instptr = Builder->CreateAlloca(llvm::Type::getDoubleTy(*TheContext),
                                         nullptr, var.first);
    LocalValues[var.first] = instptr;
    resptr = Builder->CreateStore(
        llvm::ConstantFP::get(*TheContext, llvm::APFloat(var.second.first)),
        instptr);
  }
  return resptr;
}

llvm::Value *UnaryOpExpr::Codegen(bool isLhs) const { return nullptr; }

llvm::Value *BinaryOpExpr::Codegen(bool isLhs) const {
  assert(!isLhs);

  llvm::Value *L = lhs_->Codegen(op_type_ == Tag::SYMBOL_BECOME);
  llvm::Value *R = rhs_->Codegen();

  switch (op_type_) {
    case Tag::SYMBOL_ADD:
      return Builder->CreateFAdd(L, R, "faddtmp");
    case Tag::SYMBOL_SUB:
      return Builder->CreateFSub(L, R, "fsubtmp");
    case Tag::SYMBOL_MUL:
      return Builder->CreateFMul(L, R, "fmultmp");
    case Tag::SYMBOL_DIV:
      return Builder->CreateFDiv(L, R, "fdivtmp");
    case Tag::SYMBOL_EQ:
      return Builder->CreateFCmpUEQ(L, R, "fcmpeqtmp");
    case Tag::SYMBOL_NEQ:
      return Builder->CreateFCmpUNE(L, R, "fcmpnetmp");
    case Tag::SYMBOL_LE:
      return Builder->CreateFCmpULT(L, R, "fcmplttmp");
    case Tag::SYMBOL_LEQ:
      return Builder->CreateFCmpULE(L, R, "fcmpletmp");
    case Tag::SYMBOL_GT:
      return Builder->CreateFCmpUGT(L, R, "fcmpgttmp");
    case Tag::SYMBOL_GEQ:
      return Builder->CreateFCmpUGE(L, R, "fcmpfetmp");
    case Tag::SYMBOL_BECOME:
      return Builder->CreateStore(R, L);
    default:
      return nullptr;
  }
  return nullptr;
}

llvm::Value *PesudoExpr::Codegen(bool isLhs) const {
  assert(!isLhs);
  for (auto child : children_) {
    child->Codegen();
  }
  return nullptr;
}

llvm::Value *FunctionExpr::Codegen(bool isLhs) const {
  assert(!isLhs);

  // 保存上下文相关变量
  llvm::BasicBlock *_BB = Builder->GetInsertBlock();
  // std::map<std::string, llvm::Value *> Consts = ConstantValues;
  // std::map<std::string, llvm::AllocaInst *> Mutables = MutableValues;

  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::FunctionType::getVoidTy(*TheContext), false);
  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, name_, TheModule.get());
  // Create a new basic block to start insertion into.
  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(*TheContext, name_ + " entry", F);

  Builder->SetInsertPoint(BB);
  body_->Codegen();
  Builder->CreateRet(nullptr);
  llvm::verifyFunction(*F);

  // Print out all of the generated code.
  // TheModule->print(llvm::errs(), nullptr);
  // Optimize the function.
  TheFPM->run(*F);

  // 恢复上下文相关变量
  // ConstantValues = Consts;
  // MutableValues = Mutables;
  LocalValues.clear();
  Builder->SetInsertPoint(_BB);
  return F;
}

llvm::Value *CallExpr::Codegen(bool isLhs) const {
  assert(!isLhs);

  llvm::Function *F = TheModule->getFunction(name_);
  if (F == nullptr) {
    llvm::errs() << Error(ERRNO_NOTFOUND_DEFINED_SYMBOL, name_);
    return nullptr;
  }
  return Builder->CreateCall(F);
}

llvm::Value *OddExpr::Codegen(bool isLhs) const {
  assert(!isLhs);

  llvm::Value *V = expr_->Codegen();
  V = Builder->CreateFPToSI(V, llvm::Type::getInt32Ty(*TheContext), "f2itmp");
  llvm::Value *Rem = Builder->CreateSRem(
      V, llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 2, true)),
      "remtmp");

  return Builder->CreateICmpNE(
      Rem, llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0, true)),
      "nezero");
}

llvm::Value *ScanExpr::Codegen(bool isLhs) const {
  llvm::Function *F = TheModule->getFunction("scand");
  llvm::Value *LhsPtr = var_->Codegen(true);
  std::vector<llvm::Value *> Args;
  Args.push_back(LhsPtr);
  llvm::Value *V = Builder->CreateCall(F, Args);
  return V;
}

llvm::Value *PrintExpr::Codegen(bool isLhs) const {
  llvm::Function *F = TheModule->getFunction("printd");
  llvm::Value *Rhs = expr_->Codegen();
  std::vector<llvm::Value *> Args;
  Args.push_back(Rhs);
  llvm::Value *V = Builder->CreateCall(F, Args);
  return V;
}

llvm::Value *IfExpr::Codegen(bool isLhs) const {
  assert(!isLhs);

  llvm::Value *Cond = cond_->Codegen();
  llvm::Function *F = Builder->GetInsertBlock()->getParent();
  llvm::BasicBlock *TBB = llvm::BasicBlock::Create(*TheContext, "then", F);
  llvm::BasicBlock *MBB = llvm::BasicBlock::Create(*TheContext, "ifend", F);
  Builder->CreateCondBr(Cond, TBB, MBB);

  Builder->SetInsertPoint(TBB);
  body_->Codegen();
  Builder->CreateBr(MBB);

  Builder->SetInsertPoint(MBB);
  return nullptr;
}

llvm::Value *WhileExpr::Codegen(bool isLhs) const {
  assert(!isLhs);

  llvm::Function *F = Builder->GetInsertBlock()->getParent();
  llvm::BasicBlock *CondBB =
      llvm::BasicBlock::Create(*TheContext, "loopcond", F);
  llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(*TheContext, "then", F);
  llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(*TheContext, "ifend", F);

  Builder->CreateBr(CondBB);
  Builder->SetInsertPoint(CondBB);
  llvm::Value *Cond = cond_->Codegen();
  Builder->CreateCondBr(Cond, LoopBB, AfterBB);

  Builder->SetInsertPoint(LoopBB);
  body_->Codegen();
  Builder->CreateBr(CondBB);

  Builder->SetInsertPoint(AfterBB);
  return nullptr;
}

// void ConstExpr::AddConstant(const std::string &key, double value) {
//   if (consts_.count(key) > 0) {
//     llvm::errs() << Error(ERRNO_DEFINED_SYMBOL_CONFLICT, key);
//   } else {
//     consts_[key] = value;
//   }
// }

// void VariableExpr::AddVariant(const std::string &key) {
//   if (vars_.count(key) > 0) {
//     llvm::errs() << Error(ERRNO_DEFINED_SYMBOL_CONFLICT, key);
//   } else {
//     vars_.insert(key);
//   }
// }

void PesudoExpr::AddChild(const std::shared_ptr<ExprAst> &child) {
  if (child != nullptr) {
    children_.push_back(child);
  }
}
/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2022-02-02 10:08:19
 * @LastEditTime: 2022-02-11 07:06:21
 * @Description: Compiler
 */

#include "compiler.h"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>

#include <cstdio>
#include <memory>

#include "pl0_jit.h"

std::unique_ptr<llvm::LLVMContext> TheContext;
std::unique_ptr<llvm::Module> TheModule;
std::unique_ptr<llvm::IRBuilder<>> Builder;
std::unique_ptr<llvm::orc::Pl0JIT> TheJIT;
std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;
std::map<std::string, llvm::Value *> ConstantValues;
std::map<std::string, llvm::AllocaInst *> MutableValues;

void InitializeEntryModuleAndPassManager() {
  // Open a new context and module.
  TheContext = std::make_unique<llvm::LLVMContext>();
  TheModule = std::make_unique<llvm::Module>("pl0 main module", *TheContext);

  // Create a new builder for the module.
  Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getVoidTy(*TheContext), false);
  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "module_entry", TheModule.get());
  // Create a new basic block to start insertion into.
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", F);
  Builder->SetInsertPoint(BB);

  // scand
  std::vector<llvm::Type *> ArgsType;
  ArgsType.push_back(llvm::Type::getDoublePtrTy(*TheContext));
  FT = llvm::FunctionType::get(llvm::Type::getVoidTy(*TheContext), ArgsType,
                               false);
  F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "scand",
                             *TheModule);

  // printd
  ArgsType.clear();
  ArgsType.push_back(llvm::Type::getDoubleTy(*TheContext));
  FT = llvm::FunctionType::get(llvm::Type::getVoidTy(*TheContext), ArgsType,
                               false);
  F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "printd",
                             *TheModule);

  // PassManager
  // Create a new pass manager attached to it.
  TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());

  // Do simple "peephole" optimizations and bit-twiddling optzns.
  TheFPM->add(llvm::createInstructionCombiningPass());
  // Reassociate expressions.
  TheFPM->add(llvm::createReassociatePass());
  // Eliminate Common SubExpressions.
  TheFPM->add(llvm::createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  TheFPM->add(llvm::createCFGSimplificationPass());

  TheFPM->doInitialization();
}

void Release() {
  TheContext.release();
  TheModule.release();
  Builder.release();
  TheJIT.release();
  TheFPM.release();
  ConstantValues.clear();
  MutableValues.clear();
}

/// scand - scand that takes a double
extern "C" DLLEXPORT void scand(double *X) { fscanf(stdin, "%lf", X); }

/// printd - printf that takes a double prints it as "%f\n"
extern "C" DLLEXPORT void printd(double X) { fprintf(stdout, "%lf\n", X); }

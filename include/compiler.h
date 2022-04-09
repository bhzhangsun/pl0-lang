/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2022-02-02 09:42:22
 * @LastEditTime: 2022-03-14 22:33:03
 * @Description: Compiler
 */

#if !defined(__PL0_COMPILER_H__)
#define __PL0_COMPILER_H__

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include <map>
#include <memory>
#include <string>

#include "pl0_jit.h"

extern std::unique_ptr<llvm::LLVMContext> TheContext;
extern std::unique_ptr<llvm::Module> TheModule;
extern std::unique_ptr<llvm::orc::Pl0JIT> TheJIT;
extern std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;
extern std::unique_ptr<llvm::IRBuilder<>> Builder;
extern std::map<std::string, llvm::Value *> GlobalValues;
extern std::map<std::string, llvm::AllocaInst *> LocalValues;

void InitializeEntryModuleAndPassManager();
void Release();

//===----------------------------------------------------------------------===//
// "Library" functions that can be "extern'd" from user code.
//===----------------------------------------------------------------------===//

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

/// putchard - putchar that takes a double and returns 0.
extern "C" DLLEXPORT void scand(double *X);

/// printd - printf that takes a double prints it as "%f\n", returning 0.
extern "C" DLLEXPORT void printd(double X);

#endif  // __PL0_COMPILER_H__

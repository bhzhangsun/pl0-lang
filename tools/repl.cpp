/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2022-02-12 11:16:14
 * @LastEditTime: 2022-03-17 09:13:29
 * @Description: REPL交互JIT解释器
 */

#include <llvm/Support/TargetSelect.h>

#include <iostream>
#include <memory>

#include "compiler.h"
#include "lexer.h"
#include "parser.h"

int main() {
  //   bool first = true;
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
  TheJIT = std::make_unique<llvm::orc::Pl0JIT>();
  llvm::DataLayout DL = TheJIT->getTargetMachine().createDataLayout();

  InitializeEntryModuleAndPassManager();
  TheModule->setDataLayout(DL);

  while (true) {
    std::string source;
    std::cout << ">>> ";
    std::cout.flush();
    while (true) {
      std::string line;
      std::getline(std::cin, line);
      source += "\n" + line;
      if (line.length() > 0 && line[line.length() - 1] == '.') {
        break;
      }
      std::cout << "... ";
      std::cout.flush();
    }

    Lexer lex(source, Lexer::MODE_CONTENT);
    if (!lex.Tokenization()) {
      llvm::outs() << "lexer error";
      continue;
    }

    Parser parser(lex);
    if (!parser.Parsing()) {
      llvm::outs() << "parser error";
      continue;
    }

    parser.Codegen();

    // Print out all of the generated code.
    TheModule->print(llvm::errs(), nullptr);
    auto H = TheJIT->addModule(std::move(TheModule));
    auto ExprSymbol = TheJIT->findSymbol("module_entry");
    assert(ExprSymbol && "Function not found");

    void (*FP)() = (void (*)())(intptr_t)cantFail(ExprSymbol.getAddress());
    FP();

    // TheJIT->removeModule(H);
  }

  return 0;
}
/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2022-02-12 11:31:53
 * @LastEditTime: 2022-03-23 08:39:13
 * @Description: Compiler
 */

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include <iostream>
#include <memory>

#include "compiler.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[]) {
  std::string outfile = "";
  std::string source = "";

  int idx = 1;
  if (argc > idx + 1 && std::string(argv[idx]) == "-o") {
    outfile = argv[idx + 1];
    idx = 3;
  }
  if (argc > idx + 1 && std::string(argv[idx]) == "-c") {
    source = argv[idx + 1];
  } else if (argc > idx) {
    source = argv[idx];
  } else {
    llvm::errs() << "usage: cc [-o <outfile.o>] [-c] source\n";
    return 0;
  }

  if (outfile == "") {
    outfile = "out.o";
  }

  auto inptr = std::make_shared<std::ifstream>(source, std::ios::in);
  if (inptr->fail()) {
    llvm::errs() << "can not open this file：" << source << "\n";
    return 0;
  }
  Lexer lex(inptr);
  if (!lex.Tokenization()) {
    return 0;
  }
  Parser parser(lex);
  if (!parser.Parsing()) {
    return 0;
  }

  InitializeEntryModuleAndPassManager();
  parser.Codegen();

  // Initialize the target registry etc.
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  TheModule->setTargetTriple(TargetTriple);

  std::string err;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, err);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!Target) {
    llvm::errs() << err;
    return 0;
  }

  auto CPU = "generic";
  auto Feature = "";

  llvm::TargetOptions opt;
  auto RM = llvm::Optional<llvm::Reloc::Model>();
  auto TheTargetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Feature, opt, RM);
  TheModule->setDataLayout(TheTargetMachine->createDataLayout());

  std::error_code EC;
  llvm::raw_fd_ostream dest(outfile, EC, llvm::sys::fs::OF_None);

  if (EC) {
    llvm::errs() << "Could not open file: " << EC.message();
    return 1;
  }

  llvm::legacy::PassManager pass;
  auto FileType = llvm::CGFT_ObjectFile;

  if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    llvm::errs() << "TheTargetMachine can't emit a file of this type";
    return 1;
  }

  pass.run(*TheModule);
  dest.flush();
  dest.close();

  llvm::outs() << "Wrote " << outfile << "\n";
  return 0;
}
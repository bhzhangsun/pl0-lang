/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2022-01-04 09:35:17
 * @LastEditTime: 2022-02-11 07:06:40
 * @Description: Parser 单元测试
 */

#define CATCH_CONFIG_MAIN

#include "parser.h"

#include <llvm/Support/Error.h>
#include <llvm/Support/TargetSelect.h>

#include <array>
#include <fstream>
#include <iostream>
#include <string>

#include "catch.hpp"
#include "compiler.h"
#include "lexer.h"

int test_parser() {
  std::array<std::string, 3> sources = {"../resource/square.pl0",
                                        "../resource/prime.pl0",
                                        "../resource/operation.pl0"};

  // for (auto file : sources) {
  Lexer lex("../resource/operation.pl0");
  REQUIRE(lex.Tokenization() == true);
  Parser parser(lex);
  REQUIRE(parser.Parsing() == true);
  // }
  return 0;
}

int test_imcomplate_parser() {
  Lexer lex("const x", Lexer::MODE_CONTENT);
  REQUIRE(lex.Tokenization() == true);
  Parser parser(lex);
  REQUIRE(parser.Parsing() == false);
  return 0;
}

int test_irgen_expr() {
  Lexer lex("const x = 8;var y; y := x .", Lexer::MODE_CONTENT);
  REQUIRE(lex.Tokenization() == true);
  Parser parser(lex);
  REQUIRE(parser.Parsing() == true);
  InitializeEntryModuleAndPassManager();

  parser.Codegen();
  for (auto named : ConstantValues) {
    std::cout << "first: " << named.first << ", second:" << named.second
              << std::endl;
  }
  for (auto named : MutableValues) {
    std::cout << "first: " << named.first << ", second:" << named.second
              << std::endl;
  }
  // Print out all of the generated code.
  TheModule->print(llvm::errs(), nullptr);
  return 0;
}

int test_irgen_function() {
  Lexer lex(
      "const x = 8;var y; procedure func; const x = 9;var y; y := x;  y := x .",
      Lexer::MODE_CONTENT);
  REQUIRE(lex.Tokenization() == true);
  Parser parser(lex);
  REQUIRE(parser.Parsing() == true);
  InitializeEntryModuleAndPassManager();

  parser.Codegen();
  // Print out all of the generated code.
  TheModule->print(llvm::errs(), nullptr);
  return 0;
}

int test_irgen_flow() {
  Lexer lex(
      "const x = 8;var y; begin ?y; while y > 0 do begin y := y - 1; !y end "
      "end .",
      Lexer::MODE_CONTENT);
  REQUIRE(lex.Tokenization() == true);
  Parser parser(lex);
  REQUIRE(parser.Parsing() == true);
  InitializeEntryModuleAndPassManager();

  parser.Codegen();
  // Print out all of the generated code.
  TheModule->print(llvm::errs(), nullptr);
  return 0;
}
// function
// "const x = 8;var y; procedure func; const x = 9;var y; begin y := x; "
// "!y end; begin  y := x; ?y; !y+x end .",

// scand printd + - () * /
// "const x = 8;var y; begin y := x; ?y; !-0.8+y; !y+x; !-0.8-0.2; !-0.8*2; "
// "!-0.8/2; !-3+4*2; !(-3+4)*2 end ."

// if > < = # <= >=
// "const x = 8;var y; begin !0; if 0 * 1.1 = 0 then !1; if 1.1 > 1 "
// "then !2; if 1.1 < 1 then !3; if 1.1 # 1 then !4; if 1.1 >= 1 then !5; "
// "if 1.1 <= 1 then !6 end ."

int test_irgen_jit() {
  Lexer lex(
      "const x = 8;var y,z,j; begin ?y; while odd y do begin j := 3; !3+3 + x; "
      "y := y - 1; !y end end .",
      Lexer::MODE_CONTENT);
  REQUIRE(lex.Tokenization() == true);
  Parser parser(lex);
  REQUIRE(parser.Parsing() == true);

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
  InitializeEntryModuleAndPassManager();
  TheJIT = std::make_unique<llvm::orc::Pl0JIT>();
  TheModule->setDataLayout(TheJIT->getTargetMachine().createDataLayout());

  parser.Codegen();
  Builder->CreateRet(nullptr);
  // Print out all of the generated code.
  TheModule->print(llvm::errs(), nullptr);

  // JIT the module containing the anonymous expression, keeping a handle so
  // we can free it later.
  auto H = TheJIT->addModule(std::move(TheModule));

  // Search the JIT for the module_entry symbol.
  auto ExprSymbol = TheJIT->findSymbol("module_entry");
  assert(ExprSymbol && "Function not found");

  // Get the symbol's address and cast it to the right type (takes no
  // arguments, returns a double) so we can call it as a native function.
  void (*FP)() = (void (*)())(intptr_t)cantFail(ExprSymbol.getAddress());
  FP();
  llvm::outs() << "Evaluated module entry complate!\n";

  // ExprSymbol = TheJIT->findSymbol("printd");
  // void (*Printd)(double) =
  //     (void (*)(double))(intptr_t)cantFail(ExprSymbol.getAddress());

  // ExprSymbol = TheJIT->findSymbol("scand");
  // void (*Scand)(double*) =
  //     (void (*)(double*))(intptr_t)cantFail(ExprSymbol.getAddress());
  // double lf;
  // Scand(&lf);
  // Printd(lf);
  // ExprSymbol = TheJIT->findSymbol("func");
  // void (*Func)() = (void (*)())(intptr_t)cantFail(ExprSymbol.getAddress());
  // Func();

  // Delete the anonymous expression module from the JIT.
  TheJIT->removeModule(H);

  // InitializeJIT();

  return 0;
}

TEST_CASE("Parser", "[short]") {
  REQUIRE(test_parser() == 0);
  Release();
  REQUIRE(test_imcomplate_parser() == 0);
  Release();
  REQUIRE(test_irgen_expr() == 0);
  Release();
  REQUIRE(test_irgen_function() == 0);
  Release();
  REQUIRE(test_irgen_flow() == 0);
  Release();
  REQUIRE(test_irgen_jit() == 0);
}
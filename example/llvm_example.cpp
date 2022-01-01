/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-11-22 23:40:36
 * @LastEditTime: 2021-11-26 07:46:52
 * @Description: llvm 样例
 */

#include <llvm/ADT/PackedVector.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/ExtensibleRTTI.h>
#include <llvm/Support/FormatVariadic.h>

#include <iostream>

class A : public llvm::RTTIExtends<A, llvm::RTTIRoot> {
 public:
  static char ID;
  A() {}
  virtual void show() { std::cout << "hello A" << std::endl; }
};
char A::ID = 0;

class C : public llvm::RTTIExtends<C, A> {
 public:
  static char ID;
  C() {}
  void show() override { std::cout << "hello C" << std::endl; }
};
char C::ID = 0;

void RTTIExample() {
  A* aptr = new C();
  aptr->show();
  std::cout << "c " << (llvm::isa<C>(aptr) ? "is" : "is not") << " a"
            << std::endl;
  C* cptr = llvm::cast<C>(aptr);
  cptr->show();
  cptr = llvm::dyn_cast<C>(aptr);
  cptr->show();
}

enum State {
  None = 0x0,
  FirstCondition = 0x1,
  SecondCondition = 0x2,
  Both = 0x3
};

void PackedVectorExample() {
  llvm::PackedVector<State, 2> Vec1;
  Vec1.push_back(FirstCondition);

  llvm::PackedVector<State, 2> Vec2;
  Vec2.push_back(SecondCondition);

  Vec1 |= Vec2;
  std::cout << "FirstCondition & SecondCondition = " << Vec1[0]
            << std::endl;  // returns 'Both'.
}

void FormatExample() {
  std::string S;
  // Simple formatting of basic types and implicit string conversion.
  S = llvm::formatv("{0} ({1:P})", 7, 0.35);  // S == "7 (35.00%)"

  // Out-of-order referencing and multi-referencing
  std::cout << llvm::formatv("{0} {2} {1} {0}", 1, "test", 3).str()
            << std::endl;
}

int main() {
  std::cout << "llvm useful api example!" << std::endl;
  RTTIExample();
  FormatExample();
  PackedVectorExample();
  return 0;
}
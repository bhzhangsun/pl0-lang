/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2022-02-27 19:42:33
 * @LastEditTime: 2022-03-15 08:45:46
 * @Description: SymbolTable
 */
#include <llvm/IR/Value.h>

#include <map>
#include <memory>
#include <string>

class SymbolTable {
  std::map<SymbolScope *, std::week_ptr<SymbolScope>> nodes_;
};

struct SymbolScope {
  std::shared_ptr<SymbolScope> parent_;
  std::map<std::string, llvm::Value *> scope_table_;
};

/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-12-09 22:18:54
 * @LastEditTime: 2021-12-12 15:53:01
 * @Description: Expr_Ast实现
 */

#include "expr_ast.h"

#include <iostream>
#include <memory>

void CommonAst::AddChild(const std::shared_ptr<ExprAst>& ptr) {
  children_.push_back(ptr);
}

// std::ostream operator<<(std::ostream out, const CommonAst& ast) {}
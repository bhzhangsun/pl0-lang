/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2022-03-20 09:46:21
 * @LastEditTime: 2022-03-20 10:19:22
 * @Description:
 */

#include <iostream>

extern "C" {
void module_entry();
}

int main() {
  std::cout << "module start..." << std::endl;
  module_entry();
  std::cout << "module stop." << std::endl;
  return 0;
}

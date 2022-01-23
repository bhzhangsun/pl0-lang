/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2022-01-16 09:11:05
 * @LastEditTime: 2022-01-16 09:18:03
 * @Description:
 */

#include <iostream>

int main() {
  int arr[5] = {1, 2, 3, 4, 5};
  int i = 0;
  do {
    std::cout << i << " ";
    i++;

    if (i == 5) {
      continue;
    }
  } while (i < 5);

  std::cout << std::endl;
  i = 0;

  while (i < 5) {
    std::cout << i << " ";
    i++;

    if (i == 5) {
      continue;
    }
  };
  std::cout << std::endl;
  return 0;
}
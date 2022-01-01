/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-11-29 08:24:06
 * @LastEditTime: 2021-11-30 07:37:26
 * @Description:
 */
#include <array>
#include <iostream>
#include <vector>

int main() {
  using std::array;
  using std::cout;
  using std::endl;
  using std::vector;

  vector<int> vec = {3, 4, 5, 10};
  array<int, 10> arr = {2, 4, 6, 8};

  int expr[] = {1, 2, 3};
  auto& [a, b, _] = expr;

  for (int i : arr) {
    cout << i << " ";
  }
  cout << endl;
  return 0;
}
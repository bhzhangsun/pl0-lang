/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-12-28 09:25:08
 * @LastEditTime: 2022-01-23 08:42:00
 * @Description: Error的实现
 */

#include "error.h"

#include <array>
#include <iostream>
#include <string>

const std::array<std::string, ERRNO_SIZE> Error::message_map_ = {
    "",
    "模块结尾未找到",
    "未识别的Token",
    "未识别的数字",
    "未识别的符号",
    "未识别的单词",
    "未识别的标识符",
    "此处需要一个'='",
    "应该为数字字面值",
    "应该为标识符",
    "应该为','",
    "应该为';'",
    "应该为':='",
    "出现意外的符号类型"};

Error Error::Success() { return Error(3); }

const char *Error::what() const noexcept { return message_.c_str(); }

std::ostream &operator<<(std::ostream &out, const Error &e) {
  out << e.what() << std::endl;
  return out;
}
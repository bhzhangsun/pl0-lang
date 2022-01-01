/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-12-28 09:25:08
 * @LastEditTime: 2021-12-30 08:44:25
 * @Description: Error的实现
 */

#include "error.h"

#include <array>
#include <string>

const std::array<std::string, ERRNO_SIZE> Error::message_map_ = {
    "", "未识别的Token", "未识别的数字", "未识别的符号", "未识别的单词"};

Error Error::Success() { return Error(3); }

const char* Error::what() const noexcept { return message_.c_str(); }

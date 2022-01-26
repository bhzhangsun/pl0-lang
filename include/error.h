/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-12-26 17:25:05
 * @LastEditTime: 2022-01-26 21:23:36
 * @Description: Error 类，使用异常抛出
 */

#if !defined(__PL0_ERROR_H__)
#define __PL0_ERROR_H__

#include <array>
#include <string>

enum ERRNO : size_t {
  ERRNO_SUCCESS = 0,
  ERRNO_NOTFOND_MODULE_END,
  ERRNO_UNRECOGNIZE_TOKEN,
  ERRNO_UNRECOGNIZE_NUMBER,
  ERRNO_UNRECOGNIZE_SYMBOL,
  ERRNO_UNRECOGNIZE_WORD,
  ERRNO_UNRECOGNIZE_IDENTITY,
  ERRNO_REQUIRE_EQ,
  ERRNO_REQUIRE_NUMBER,
  ERRNO_REQUIRE_IDENTITY,
  ERRNO_REQUIRE_COMMA,
  ERRNO_REQUIRE_SEMI,
  ERRNO_REQUIRE_BECOME,
  ERRNO_REQUIRE_END,
  ERRNO_REQUIRE_THEN,
  ERRNO_REQUIRE_DO,
  ERRNO_UNEXPECTED_SYMBOL_TYPE,
  ERRNO_NO_AVAILABLE_TOKEN,
  ERRNO_SIZE
};

class Error : public std::exception {
 private:
  size_t errno_;
  std::string message_;
  static const std::array<std::string, ERRNO_SIZE> message_map_;

 public:
  Error(size_t err) : errno_(err) {
    assert(err < ERRNO_SIZE);
    message_ = Error::message_map_[err];
  }
  Error(size_t err, const std::string& message) : errno_(err) {
    message_ = message + " " + Error::message_map_[err];
  }

  // [line: 出错片段] 错误原因
  Error(size_t err, size_t line, const std::string& message) : errno_(err) {
    message_ = "[" + std::to_string(line) + ": " + message + "] " +
               Error::message_map_[err];
  }

  const char* what() const noexcept override;

  static Error Success();
  friend std::ostream& operator<<(std::ostream& out, const Error& e);
};

#endif  // __PL0_ERROR_H__
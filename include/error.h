/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-12-26 17:25:05
 * @LastEditTime: 2021-12-30 09:00:43
 * @Description: Error 类，使用异常抛出
 */

#include <array>
#include <string>

enum ERRNO : size_t {
  ERRNO_SUCCESS = 0,
  ERRNO_EXECPTION_TOKEN,
  ERRNO_EXECPTION_NUMBER,
  ERRNO_EXECPTION_SYMBOL,
  ERRNO_EXECPTION_WORD,
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
};

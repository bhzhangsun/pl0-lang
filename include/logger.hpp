/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2022-01-23 19:07:17
 * @LastEditTime: 2022-01-25 08:58:46
 * @Description: Logger
 */

#if !defined(__PL0_LOGGER_H__)
#define __PL0_LOGGER_H__

#include <iostream>
#include <map>
#include <string>

class Logger {
 public:
  enum LogLevel {
    LOG_LEVEL_RELEASE,
    LOG_LEVEL_DEBUG,
  };

 private:
  int level_;
  std::ostream& out_;

 public:
  Logger(int level = LOG_LEVEL_RELEASE) : out_(std::cout), level_(level){};
  Logger(std::ostream& out, int level = LOG_LEVEL_RELEASE)
      : out_(out), level_(level){};
  void SetLevel(int level) { level_ = level; }

  template <typename T>
  void info(const T& content) {
    if (level_ == LOG_LEVEL_DEBUG) {
      out_ << content << std::endl;
    }
  }

  template <typename T>
  void notice(const T& content) {
    out_ << content << std::endl;
  }

  template <typename T>
  void warning(const T& content) {
    out_ << content << std::endl;
  }

  template <typename T>
  void error(const T& content) {
    out_ << content << std::endl;
  }
};

Logger logger = Logger(
#ifdef DEBUG
    Logger::LOG_LEVEL_DEBUG
#else
    Logger::LOG_LEVEL_RELEASE
#endif
);

#endif  // __PL0_LOGGER_H__

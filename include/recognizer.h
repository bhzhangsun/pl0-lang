/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-19 22:18:10
 * @LastEditTime: 2021-10-19 23:34:52
 * @Description:
 */
#if !defined(__PL0_RECOGNIZER_H__)
#define __PL0_RECOGNIZER_H__

#include <istream>
#include <memory>

#include "token.h"
class Recognizer {
 protected:
  std::shared_ptr<std::istream> raw_stream_;

 public:
  Recognizer(std::shared_ptr<std::istream> stream) : raw_stream_(stream) {}
  virtual ~Recognizer() {}

  virtual bool Match() = 0;
  virtual Token Consumer() = 0;
  virtual Token Consumer(size_t) = 0;
};

#endif  // __PL0_RECOGNIZER_H__

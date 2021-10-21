/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-19 22:27:11
 * @LastEditTime: 2021-10-19 23:35:02
 * @Description:
 */
/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-19 22:26:59
 * @LastEditTime: 2021-10-19 22:31:48
 * @Description:
 */

#if !defined(__PL0_WORD_RECOGNIZER_H__)
#define __PL0_WORD_RECOGNIZER_H__

#include <istream>
#include <memory>

#include "recognizer.h"
#include "token.h"
class WordRecognizer : Recognizer {
 private:
  /* data */
 public:
  WordRecognizer(std::shared_ptr<std::istream> stream) : Recognizer(stream) {}
  ~WordRecognizer() = default;

  bool Match() override;
  Token Consumer() override;
  Token Consumer(size_t) override;
};

#endif  // __PL0_WORD_RECOGNIZER_H__

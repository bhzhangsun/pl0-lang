/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-19 22:27:31
 * @LastEditTime: 2021-10-19 23:34:57
 * @Description:
 */
/*
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-19 22:26:59
 * @LastEditTime: 2021-10-19 22:31:48
 * @Description:
 */

#if !defined(__PL0_SYMBOL_RECOGNIZER_H__)
#define __PL0_SYMBOL_RECOGNIZER_H__

#include <istream>
#include <memory>

#include "recognizer.h"
#include "token.h"
class SymbolRecognizer : Recognizer {
 private:
  /* data */
 public:
  SymbolRecognizer(std::shared_ptr<std::istream> stream) : Recognizer(stream) {}
  ~SymbolRecognizer() = default;

  bool Match() override;
  Token Consumer() override;
  Token Consumer(size_t) override;
};

#endif  // __PL0_SYMBOL_RECOGNIZER_H__

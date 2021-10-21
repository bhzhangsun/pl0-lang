<!--
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-14 22:02:37
 * @LastEditTime: 2021-10-14 23:01:37
 * @Description:
-->

# Lexer

Lexer 用来做 pl0 的词法解析器，它接收一个文件字符流，产出一个 token 流。它的作用就是识别单词。将字符流分成一段段的单词，并为每个单词赋予词法类型，以及源码的相关属性。

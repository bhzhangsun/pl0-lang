<!--
 * @Author: zhangsunbaohong
 * @Email: zhangsunbaohong@163.com
 * @Date: 2021-10-12 07:59:47
 * @LastEditTime: 2022-01-25 09:23:14
 * @Description:
-->

# pl0-lang

这是使用 C++写的 pl0 语言编译器

## 语法

需要注意的是，PL/0 中有些特殊的操作符，它们有一些特殊的语义

1. ! expression 作为输出语句，与 cout 的语义一致
2. ? ident 从标准输入读 cin 语义一致
   [PL/0 语法 Wiki](https://en.wikipedia.org/wiki/PL/0#cite_note-2)

```
program = block "." ;

block = [ "const" ident "=" number {"," ident "=" number} ";"]
        [ "var" ident {"," ident} ";"]
        { "procedure" ident ";" block ";" } statement;

statement = [ ident ":=" expression | "call" ident
              | "?" ident | "!" expression
              | "begin" statement {";" statement } "end"
              | "if" condition "then" statement
              | "while" condition "do" statement ];

condition = "odd" expression |
            expression ("="|"#"|"<"|"<="|">"|">=") expression ;

expression = [ "+"|"-"] term { ("+"|"-") term};

term = factor {("*"|"/") factor};

factor = ident | number | "(" expression ")";
```

## 设计

pl0 是一个类 pascal 语言, 该编译器使用 C++语言完成，从词法解析，语法解析到 AST，直至 JIT 执行结束

### [1. Lexer](./docs/Lexer.md)

## 规范

[使用 Google 代码风格](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#general-naming-rules)

## 用法

本项目使用[vcpkg](https://vcpkg.io/en/index.html)作依赖管理，使用 cmake 进行构建
安装 vcpkg，vscode 配置 CMAKE_TOOLCHAIN_FILE=/install_path/vcpkg/scripts/buildsystems/vcpkg.

- mkdir build
- cd build
- cmake ..
- make
- ./pl0 "filepath"
- "exec the program"
- make clean

## LICENSE

MIT © [duduscript](https://github.com/duduscript)

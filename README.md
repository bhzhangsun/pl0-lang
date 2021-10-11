# pl0-lang
这是使用C++写的pl0语言编译器
## 语法

```
program = block "." .

block = [ "const" ident "=" number {"," ident "=" number} ";"]
        [ "var" ident {"," ident} ";"]
        { "procedure" ident ";" block ";" } statement .

statement = [ ident ":=" expression | "call" ident 
            | "?" ident | "!" expression 
            | "begin" statement {";" statement } "end" 
	          | "if" condition "then" statement 
            | "while" condition "do" statement ].

condition = "odd" expression |
           expression ("="|"#"|"<"|"<="|">"|">=") expression .

expression = [ "+"|"-"] term { ("+"|"-") term}.
term = factor {("*"|"/") factor}.
factor = ident | number | "(" expression ")".
```


## 用法

\- make

\- ./pl0 "filepath"

\- "exec the program"

\- make clean



## LICENSE

MIT © [duduscript](https://github.com/duduscript)



```
// pl0词法
number = 
ident = 
keyword = 
symbol = 

```



```
# 词法解析
# in: stream
# out: list<token>

parser(in:stream) {
	result = list<token>();
	while(in.peek() != EOF) {
		if (match(in, 'alpha')) {
			result.push_back(getWord(in));
		} else if (match(in, 'number')) {
			result.push_back(getNumber(in));
		}
		}
	}
	return result;
}

result = [{value = 'if', type='word'}, {value='23.2', type='number'}]

// 缺少；号属于语法解析是报错
// 9432nhc不属于标识符报错，词法解析报错
// pl0使用括号处理块作用域，将无异议空白字符在词法解析时处理掉，\n处理完后将相关属性放在token里
```



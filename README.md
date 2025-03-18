# 词法分析器的设计与实现

## 设计要求
### 核心要求
设计一个程序，根据选定的源语言的构词规则，从输入文件中识别出该语言所有的合法的单词符号，并以等长的二元组形式输出。程序应该对无法识别的子串做相应处理。程序应将所有结果信息写入输出文件。  

---
### I/O要求
**输入:** 一个纯文本文件，内容是字符串形式的源程序。  
**输出:** 一个纯文本文件，内容是由等长二元组形式的单词符号所构成的串（流）。

## 设计思路
### 源语言描述
本实验的源语言为类C子集，包含以下基本词法单元：

类别|特征|正则式表达
:-:|:-:|:-:
关键字|一个特定的字符串|"if"\|\|"else"\|\|"while"\|\|"for"\|\|"int"\|\|"float"\|\|"return"\|\|"void"
标识符|以字母或下划线开头，后接字母、数字或下划线|[a-zA-Z_][a-zA-Z0-9_]*
数值常量|整数：由数字组成的序列；浮点数：包含小数点|("-"\|\|NULL)[1-9][0-9]\* ((.[0-9]\*) \|\| NULL)
运算符与界符|特定字符（可能为两字符）|"+" \| "-" \| "*" \| "/" \| "=" \| "==" \| ";" \| "," \| "(" \| ")"
注释|行注释以 // 开头至行尾；块注释以 /* 开始，*/ 结束
---
### 词法规则与设计
#### 单词分类与编码
类别|编码|示例
-|-|-
关键字|1|if, int
标识符|2|x, sum
整数|3|42
浮点数|4|3.14
运算符|5|+, ==
界符|6|;, (
错误字符|7|@, #
---
#### 数据结构设计
Token结构体
存储单词类型和词素：
```cpp
struct Token {
    TokenType type;   // 类型编码
    std::string lexeme; // 词素字符串
};
```

符号表
使用哈希表预存关键字和运算符，快速查找：
```cpp
const std::unordered_set<std::string> KEYWORDS = {"if", "else", ...};
const std::unordered_set<std::string> OPERATORS = {"+", "==", ...};
```
---
### 程序设计与实现
#### 模块划分
- **驱动模块（main.cpp）**  
负责文件输入/输出  
调用工作模块进行词法分析

- **工作模块（tokenizer.cpp）**  
清洗源程序（跳过空白、处理注释）  
分割并识别单词符号  
生成Token序列  
---
#### 源代码结构
```paintext
lex_analyzer/
├── include/         // 头文件
├── src/             // 源文件
    ├──main.cpp
    └──tokenizer.cpp
├── IO/             // 测试用例及输出结果
    ├──TestCases/
    └──output/
└── Makefile         // 编译脚本
```
---
#### 核心算法流程
- 初始化：读取源文件内容至字符串
- 逐字符扫描：  
**跳过空白符：** 空格、换行、制表符  
**处理注释：** 根据 // 或 /- 跳过后续内容

- 识别单词：  
**标识符/关键字：** 首字符为字母或下划线，后续字符为字母/数字/下划线  
**数值常量：** 数字开头，可能包含小数点  
**运算符：** 需检查最长匹配（如 == 优先于 =）  
**界符：** 单字符直接匹配  
**错误处理：** 无法识别的字符生成错误Token  

流程图：

```paintext
开始
  ↓
读取源文件 → 失败 → 报错退出
  ↓
初始化扫描位置pos=0
  ↓
循环处理字符：
  跳过空白符 → 处理注释 → 识别单词 → 生成Token
  ↓
直到文件结束
  ↓
输出Token序列到文件
  ↓
结束
```
---
### 测试用例设计
[测试例文件](IO/TestCases/simpleCauculate.txt)  
> 含有浮点数，负数，注释解析

### 优化方案
#### 通过队列实现缓冲区设计
- 设想  
可能会存在多个文件在短时间内需要进行分析，先前的代码每更换一个文件进行词法分析需要重新修改代码并重新编译，改进后将`IO/TestCases/`一整个目录作为输入，获取目录下文件并进行词法分析，并把分析后的token进行输出到`IO/output/`下，名称与源文件相同。
- 主要改动  
使用了`<filesystem>`进行文件夹查看，将文件名称生成队列进行管理
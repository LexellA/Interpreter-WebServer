# 脚本语言设计

## 语法设计

### 变量定义

```
VAR NAME EXP
```
定义变量，变量名为NAME，变量值为一个表达式，仅支持实数和字符串。

### 变量赋值

```
SET NAME EXP
```

将变量NAME的值设置为VALUE。

### 运算符号

```
EQ LS GT LE GE NE
ADD SUB MUL DIV
```


### 表达式
```
OP NAME1 NAME2
OP EXP EXP
OP EXP NAME
OP EXP VALUE
NAME
VALUE
"string"
```
单个变量可以作为表达式

表达式全部使用前缀表达式

### 输入与输出

```
RECV NAME
SEND "string"
SEND EXP
```

RECV用于接收用户输入，SEND用于向用户输出。
输出时输出表达式。

### 条件判断

```
IF EXP
    ...
ENDIF
```

IF语句用于条件判断，EXP为表达式

### 函数

```
FUNC NAME
    ...
ENDFUNC

CALL NAME
```

FUNC用于定义函数，ENDFUNC用于结束函数定义。

函数需要定义在文件的最前面，调用时使用CALL NAME。
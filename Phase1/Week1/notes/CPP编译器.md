# CPP 编译器

## CPP 编译流程

预处理 -> 编译 -> 汇编 -> 链接

### 预处理

处理所有的以 `#` 开头的预处理指令，进行文本层面的替换与扩展

最终得到的 `main.i` 是替换后的纯 C++ 文本

```bash
g++ -E main.cpp -o main.i
```


### 编译

将 C++ 文本编译为相应平台（CPU 可读）的汇编指令

```bash
g++ -S main.i -o main.s
```

### 汇编

使用汇编器将汇编代码转为机器码（不可执行，可能缺少系统库，内存也不是真是内存）

```bash
g++ -c main.s -o main.o
```

### 链接

将多个目标文件以及系统静态库，动态库合并，形成完整地址布局的二进制文件

```bash
g++ main.o -o main
```

## 常用参数

### 输入和输出文件 `-o`，`-c`

- `-o`：指定输出文件名
- `-c`：将指定文件编译为临时机器码，如 `g++ -c main.cpp -o main.o`

注意如果是直接得到可执行程序，不需要加 `-c`

```bash
g++ main.cpp -o main
```

### 指定头文件所在目录 `-I`

一般情况下**编译器会直接在源代码的同一级目录找头文件**，当用 `-I` 指定头文件目录后，编译器就可以在制定目录找头文件，

```bash
g++ -I./include src/main.cpp -o main
```

如果有多个头文件目录（如有第三方库）

```bash
g++ -I./include -I./third_party/spdlog/include src/main.cpp -o main
```

### 指定库文件 `-L`/`-l`

一般情况下，如果要用到链接库，（Linux 系统）会直接到 `/usr/lib` 或 `/lib` 目录下寻找

如果链接库不在这里，就需要手动指定**链接库目录**

```bash
g++ main.o -L./third_paty/lib -o main
```

在 Linux 中，库名一般是 `lib<动态库名>.so`/`lib<静态库名>.a`，在给 `-l` 传参数时，必须省去开头的 `lib` 和结尾的后缀

| 实际库名 | `-l` 参数 | 含义 |
|:---:|:---:|:---:|
| `libm.so`/`libm.a` | `-lm` | 链接数学库 |
| `libpthread.so` | `-lpthread` | 链接 POSIX 线程库 |
| `libssl.so` | `-lssl` | 链接 OpsnSSL 库 |

比如项目的目录为

```text
my_project/
├── lib/
│   └── libcalculator.so  <-- 动态库文件
└── src/
    └── main.cpp          <-- 源码
```

```bash
g++ src/main.cpp -L./lib -lcalcualtor -o main
```

### 指定 C++ 语言标准 `-std=`

`std=c++11` 表示使用 C++11 标准，类似的还有 `c++14`,`c++17`,`c++20`,`c++23`

以上这些都是 ISO C++ 标准，如果要使用 GCC 特有的标准，可以使用 `gnu++17` 等

### 添加警告 `-Wall -Wextra -Wpedantic`

- `-Wall`：即 `Warning All`，开启最常用的基础警告
  -  `if (x == 0)` 写成了 `if (x = 0)` `-Wparentheses`
  -  定义了但从未使用的变量 `-Wunused-variable`
  -  有返回值的函数没写 `return` `-Wreturn-type`
- `-Wextra`：在 `-Wall` 的基础上开启更深层的警告
  - 有符号整型和无符号整型比较 `-Wsign-compare`
  - 函数定义了形参但是未使用 `-Wunused-parameter`
  - `if-else` 分支体为空 `-Wempty-body`
  - 结构体或类初始化时遗漏部分字段 `-Wmissing-field-initializers`
- `-Wpedantic`：严格遵守 ISO C/C++ 标准
  - 使用了变长数组（VLA）
  - 使用了 0 长度的数组
  - 使用了当前标准已废弃的语法

这些只是警告，仍然可以编译（只是在编译时显示警告）

使用 `-Werror` 可以将警告上升为错误（可以用 `echo &?` 查看退出码）

## nm 工具

nm 是 Linux 下一个用来查看二进制文件符号表的命令行工具

可以用来查看在目标文件（.o）、静态库（.a）、动态库（.so）以及可执行程序中的函数和全局变量

当运行 `nm nmTest.o` 时，每一行通常包含三列

```text
地址/偏移量        类型代码  符号（函数名或变量名）
0000000000000000 T        _Z3addii
```

### 类型代码

大写一般表示全局/外部符号，即可以被其他文件或库引用的符号，小写一般表示静态/内部符号，即今在当前文件可以被访问的符号

| 代码 | 全称含义 | 内存区域 | 常见对应的代码 |
|:---:|:---:|:---:|:---:|
| `T`/`t` | Text | 代码段 | 普通函数定义，`T` 为全局函数，`t` 为静态函数 |
| `U` | Undefined | 不占用空间 | 当前文件只调用该符号，但是没有实现，等待链接器在外部补齐 |
| `D`/`d` | Data | 已初始化的数据段（`.data`） | 初始值不为 0 的全局变量或静态变量 |
| `B`/`b` | BSS | 未初始化的数据段（`.bss`） | 未初始化或初始化为 0 的全局变量或 static 变量 | 
| `R`/`r` | Read-Only | 只读数据段（`.rodata`） | `const` 全局变量、字符串字面量 |
| `W`/`w` | Weak | 代码段/数据段 | 常见于 inline 函数或模板实例化，允许多处定义且不报冲突 | 
| `C` | Common | 未初始化公共段 | 未初始化的 C 风格全局变量（在最终链接阶段会被合并并转入 BSS 段） |


```cpp
#include <iostream>

const int g_const_val = 100;     // R : 全局 const 变量 -> 只读数据段 (Read-Only)
int g_init_val = 1;              // D : 已初始化且非零 -> 数据段 (Data)
int g_zero_val = 0;              // B : 初始化为零 -> BSS 段
int g_uninit_val;                // B : 未初始化 -> BSS 段 (或 C)

static void inner_func() {}      // t : static 函数 -> 局部代码段 (小写 t)
void global_func() {}            // T : 普通全局函数 -> 全局代码段 (大写 T)

inline void inline_func() {}     // W : 内联函数 -> 弱代码段 (Weak)

void test() {
    std::cout << "Hi";           // U : std::cout 等外部符号 -> 未定义 (Undefined)
}
```

### 常用参数

- `-C`：还原 C++ 名字（即将列出的符号部分的“乱码”翻译为 C++ 变量名/函数名）
- `-u`：只看未定义符号
- `-g`：只显示全局/外部符号
- `-D`：查看动态符号表，一般用来查看 `.so` 文件
- `-A`：显示所属文件名


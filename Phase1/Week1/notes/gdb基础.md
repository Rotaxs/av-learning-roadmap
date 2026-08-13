# gdb 基础

## 启动 gdb

如果要使用 gdb 调试，在编译源码时最好加上 `-g` 参数

```bash
g++ -g main.cpp -o main
```

这个参数可以使得调试器能够查看源代码，方便我们调试

然后就可以用 gdb 调试了

```bash
gdb main
```

## 使用 gdb

### 断点

**添加断点**

| 类型 | 语法 | 示例 |
|:---:|:---:|:---:|
| 按行号 | `b <line>`/`b <file>:<line>` | `b 5`/`b main:5` |
| 按函数名 | `b <function>`/`b <class>::<function>` | `b main` |
| 按条件 | `b <line/function> if <condition>` | `b 4 if i == 100` |

注意添加断点是断在执行当前语句之前

**查看所有断点**

```bash
info break
```

```text
(gdb) info break
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000000000001195 in func() at gdb_test.cpp:4
2       breakpoint     keep y   0x00000000000011f0 in main() at gdb_test.cpp:11
```

**删除断点**

使用 `d <n>` 删除对应编号的断点，编号可以通过 `info break` 查看

直接输入 `d` 会清空所有断点

**启用/禁用断点**

```bash
diable <n>
enable <n>
```

**观察数据点**

当某个变量或内存地址发生改变时自动暂停程序

```bash
watch variable
```

### 控制运行

使用 `run`/`r` 可以从头开始调试，使用 `kill`/`k` 可以终止当前正在调试的程序

| 指令 | 缩写 | 行为 |
| `next` | `n` | 单步执行（不进入函数） |
| `step` | `s` | 单步执行（进入函数） |
| `continue` | `c` | 恢复程序正常运行，直到遇到下一个断点 |
| `finish` | `f` | 运行至当前函数结束 |
| `until` | `u` | 运行至指定位置（行）/跳出循环 |

> 直接回车默认执行上一条命令

`start` 也可以启动调试，只不过 `start` 会默认在 `main` 函数打一个断点，然后执行 `r`

使用 `list` 可以显示源码的上下文

### 输出变量和内存地址

使用 `print`/`p` 即可输出变量的值

```bash
p val
```

- `p/x val` 按十六进制输出
- `p/t val` 按二进制输出
- `p/c val` 按字符输出

使用 `display` 可以在每次单步执行后自动打印该变量

```bash
display val
```

使用 `info dispaly` 可以查看所有监视项

如果要查看内存，直接用取地址运算符 `&` 即可

```bash
p &val
```

直接查看裸内存

```bash
x/<n><f><u> <pointer>
```

- `<n>` 要查看的单元数量
- `<f>` 格式 `x` 十六进制 `d` 十进制 `s` 字符串 `i` 汇编指令
- `<u>` 单位大小 `b` 1字节 `h` 2字节 `w` 4字节 `g` 8字节

示例

- `x/5dw arr` 查看从数组 `arr` 开头的连续 5 个 4 字节的十进制整数
- `x/4xb &val` 按 1 字节独立打印 `val` 占用的 4 字节
- `x/s std_ptr` 将 `str_ptr` 当作 C 风格字符串打印直到遇到 `\0` 结束

### 查看栈调用

使用 `bt` 可以查看栈调用

```text
(gdb) bt
#0  func () at gdb_test.cpp:6
#1  0x00005555555551f5 in main () at gdb_test.cpp:13
```

- `bt full` 查看完整的调用栈信息
- `bt <N>` 打印最内层的 `N` 个栈帧
- `bt -<N>` 打印最外层的 `N` 个栈帧（和 `main` 靠近的）

## gdb 调试流程

1. 现象：捕获与还原现场，用 `run` 跑出控制台输出
2. 根因：精准定位漏洞源头，使用 `bt` 查调用栈和局部变量，`x/p` 查看内存和指针合法性
3. 修复：编写并应用修复代码
4. 回归：验证生效且无侧效应

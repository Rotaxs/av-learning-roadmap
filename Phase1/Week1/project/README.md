# textstats

## 项目目的

用一个可在一周完成的命令行工具建立 Linux、C++ 多文件编译、CMake、Git、测试和 GDB 的能力基线。重点不是文本算法，而是能独立完成“实现 -> 构建 -> 失败处理 -> 测试 -> 调试 -> 复现”的工程闭环。

## 使用场景

给定本地文本文件，快速输出原始字节数、行数和按空白分隔的单词数。项目仅处理本地普通文件，用于训练基础工具链，不作为通用文本分析器。

## 输入与输出

- 输入：一个命令行位置参数，表示待读取文件的路径。
- stdout：固定、易测试的三项统计，例如 `bytes=12 lines=2 words=3`。
- stderr：参数错误、打开失败或读取失败的可行动错误信息。
- 退出码：成功为 `0`；用非零值表示参数或 I/O 错误，具体约定由实现记录在本文件。
- 统计规则：字节按文件原始字节计；单词按 C locale 空白分隔；行按换行符计，非空文件末尾无换行时补一行。空文件三项均为 `0`。

## 处理流程

```text
argv
  -> 参数数量校验
  -> 打开输入文件
  -> 顺序读取字节
  -> 更新 bytes / lines / words
  -> 检查读取状态
  -> 输出统计或错误
  -> 返回确定退出码
```

P0 最小闭环只要求单次顺序读取；文件流对象拥有文件资源，离开作用域时释放。统计结果由值对象返回，不使用裸 `new`/`delete`。

## 功能需求

### P0 必做

- 接受且仅接受一个文件路径参数。
- 对普通 ASCII 文本输出正确的字节数、行数和单词数。
- 对空文件、无末尾换行、仅空白文件给出符合约定的结果。
- 对缺参数、多参数、文件不存在和读取失败输出错误并返回非零。
- 使用头文件与至少两个 `.cpp` 文件分离 CLI 和统计逻辑。
- 支持 CMake out-of-source build，并通过 GCC、Clang 构建。
- 至少 5 个自动测试，覆盖 3 个正常/边界场景和 2 个错误场景。

### P1 可选

- 增加 `--help`，并为其添加测试。
- 增加多文件逐个统计，但不得改变 P0 单文件接口或挤占测试时间。
- 使用 `std::filesystem::path` 改善路径表达并记录取舍。

### 本周不做

- Unicode 字符、语言学分词、字符编码检测和文本规范化。
- 递归目录、并行统计、`mmap`、大文件性能优化和 GUI。
- 第三方 CLI/测试框架、安装规则、打包、CI 或跨平台兼容层。

## 建议结构

```text
project/
  CMakeLists.txt
  .gitignore
  README.md
  include/
    textstats/
      stats.h
  src/
    main.cpp
    stats.cpp
  tests/
    stats_test.cpp
    fixtures/
```

测试夹具只保留小型、来源明确的自建文本。构建目录放在 `project/` 下但不提交；日志统一放到同级的 `../evidence/`。

## 核心接口与所有权

接口只需表达统计逻辑与失败结果的边界，建议从以下形状开始，允许学习者在实现时基于错误处理方案调整：

```cpp
namespace textstats {

struct Stats {
    std::size_t bytes{};
    std::size_t lines{};
    std::size_t words{};
};

// 方案一：成功返回 Stats，失败抛出带上下文的标准异常。
Stats analyze_file(const std::string& path);

}  // namespace textstats
```

- `main` 拥有参数视图，负责用户可见输出和将失败映射为退出码。
- `analyze_file` 内部的 `std::ifstream` 独占文件资源，由 RAII 自动关闭；本周只需会使用，RAII 原理在第 3 周系统学习。
- `Stats` 按值返回，不暴露流对象、缓冲区或悬空引用。
- 若选择“不抛异常的结果类型”，必须保持错误信息和退出码可测试；不要同时实现两套方案。

## 实现步骤

1. 用单个最小程序验证参数、stdout/stderr 和退出码。
2. 建立 `stats.h`、`stats.cpp`、`main.cpp`，先手工编译对象文件并链接。
3. 定义空文件、末尾换行和空白分词规则，完成 P0 主链路。
4. 写 `CMakeLists.txt`，先构建核心 target，再构建 CLI target。
5. 增加至少 5 个 CTest 测试并确认一次故意失败可被捕获。
6. 用 GCC、Clang 从空目录构建；用 Debug 构建和 GDB 定位一个临时注入缺陷。
7. 更新本文中的真实退出码、命令和限制，执行最终干净复现。

## 构建与运行

```bash
cmake -S . -B build-gcc -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++
cmake --build build-gcc
ctest --test-dir build-gcc --output-on-failure
./build-gcc/textstats tests/fixtures/normal.txt
```

Clang 预期通过独立目录验证：

```bash
cmake -S . -B build-clang -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++
cmake --build build-clang
ctest --test-dir build-clang --output-on-failure
```

若生成器把可执行文件放在其他目录，应以实际路径更新本文，不能保留失效示例。

## 测试方案

| 类别 | 场景 | 关键断言 |
| --- | --- | --- |
| 正常 | 多行 ASCII 文本 | 三项统计与人工计算一致，退出码为 0 |
| 边界 | 空文件 | `0/0/0`，无错误输出 |
| 边界 | 非空且无末尾换行 | 最后一行计入行数 |
| 边界 | 仅空格、tab、换行 | 单词数为 0，其余按规则计数 |
| 错误 | 缺少路径参数 | stderr 有用法提示，退出码非零 |
| 错误 | 文件不存在 | 不输出伪统计，stderr 含路径上下文，退出码非零 |
| 错误 | 不可读路径或读取失败 | 返回失败，已获取资源正常释放 |

测试既可以直接调用统计接口，也应至少有错误路径通过 CLI 验证退出码。测试文件必须小到可人工核算，不能只复制未知内容。

## 验收标准

- GCC 与 Clang 从两个空 build 目录 configure/build 成功，无项目自身警告。
- `ctest` 至少运行 5 个测试且 100% 通过，覆盖不少于 3 个正常/边界和 2 个错误场景。
- README 给出的 GCC 流程从 configure 到测试不超过 3 条命令，命令与实际 target 一致。
- 对一个随机小样例人工核算，bytes、lines、words 三项均一致。
- 缺参数和不存在文件均向 stderr 报错并返回非零，不产生伪统计。
- GDB 证据包含复现、至少 2 层调用栈或明确调用路径、关键变量、根因、修复和回归。
- 构建日志、测试日志和调试记录保存在 `../evidence/`，薄弱项保存在 `../notes/baseline.md`。

## 调试指南

按以下顺序定位，不要在信息不足时反复修改源码：

1. **环境**：记录工作目录、输入路径、工具版本和完整命令。
2. **configure**：确认 CMake 选择的编译器、源码目录与 build 目录。
3. **compile**：从第一条项目源码诊断开始，检查头文件、类型和作用域。
4. **link**：对 undefined reference 检查定义是否存在、签名是否一致、对象文件是否进入 target；必要时用 `nm` 看符号。
5. **test**：用 `ctest --output-on-failure` 获取真实命令和期望/实际差异，单独复现失败用例。
6. **runtime**：使用 Debug 构建，在参数解析与 `analyze_file` 设置断点，查看路径、流状态和统计值；崩溃时先执行 `bt`。
7. **cleanup**：确认错误路径退出码非零、无残留临时资源，并重跑全部测试。

常见错误包括把字节误当字符、漏算无末尾换行的最后一行、在 `while (!eof())` 中多处理一次、头文件放定义造成重复符号、CMake 漏加源码，以及错误路径仍返回 `0`。

## 交付清单

- [ ] `CMakeLists.txt`、`.gitignore`、多文件源码和测试夹具
- [ ] 已替换“预期命令”的准确构建、运行、测试说明
- [ ] 至少 5 个自动测试及最终 `../evidence/ctest.log`
- [ ] GCC、Clang 构建记录 `../evidence/build-gcc.log`、`../evidence/build-clang.log`
- [ ] 工具版本 `../evidence/tool-versions.txt`
- [ ] GDB 定位记录 `../evidence/gdb-session.txt`
- [ ] C++ 自测和薄弱项 `../notes/baseline.md`
- [ ] 可审阅的 Git 历史，且无 build 产物被提交

本项目是阶段 0 的一次性能力基线，不单独成为长期维护仓库。第 3-12 周可将其中经过验证的构建、测试和错误处理习惯合并到未来的并发媒体数据管道；文本统计业务代码无需迁移。

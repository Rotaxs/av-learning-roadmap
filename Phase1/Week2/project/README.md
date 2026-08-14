# av-env-check

## 项目目的

建立阶段 0“基线与环境”的可重复音视频开发环境自检。它不以“软件看起来已安装”为结论，而是依次证明命令可执行、开发库可发现、最小程序可编译链接运行、调试/动态检查有效、合法测试素材可探测，并用确定报告和退出码暴露失败。

本项目是一次性环境基线，不是播放器。后续第 3-12 周可复用其 CMake、测试和 Sanitizer 习惯；第 13 周后的媒体实验可复用自生成素材与环境报告。不会合并 FFmpeg 解码或 SDL 渲染业务代码。

## 使用场景

- 新安装或升级工具链后，判断当前 Linux 主机能否开始 FFmpeg/SDL2/OpenGL 开发。
- 后续实验构建失败时，先区分 CLI、开发包、链接、图形会话、权限和程序代码问题。
- 生成不含敏感抓包的可审阅环境证据。

使用前提：Linux、C++17、CMake、CTest 和至少一个第 1 周已验证编译器。SDL2 是唯一 P0 GUI 路线；Qt 仅在已安装时记录。

## 输入与输出

- 输入：默认检查当前环境；可选参数只用于指定 build/report/assets 路径或测试故障注入，最终接口以实现后 `--help` 为准。
- 环境输入：`PATH`、`PKG_CONFIG_PATH`、CMake package paths、图形会话变量以及当前用户可用权限；报告不得回显不必要的敏感值。
- 素材输入：`project/assets/` 内由 FFmpeg `lavfi` 自生成的短媒体文件。
- stdout：适合人读的固定顺序 check 摘要。
- stderr：参数、命令执行、依赖发现、runtime 和报告写入错误。
- `../evidence/env-report.txt`：每项 `check_id/status/summary` 与最终汇总。
- `../evidence/ffprobe.json`：自生成素材的 stream/format 元数据。
- 退出码契约：`0`=全部必需检查通过（允许文档明确许可的环境型 `SKIP`）；`1`=至少一个必需检查失败；`2`=参数错误或报告生成失败。P1 与 `SKIP` 不得掩盖必需检查的 FAIL。

## 处理流程

```text
CLI / current environment / generated asset
  -> validate arguments and output paths
  -> command existence + real versions
  -> pkg-config/CMake development-library discovery
  -> compile/link/runtime smoke targets
       -> FFmpeg linked-library version
       -> SDL2 window + OpenGL context + GL information
  -> GDB batch probe
  -> ASan/UBSan clean and controlled-fault probes
  -> Wireshark/tshark/dumpcap capability query (no capture)
  -> ffprobe generated asset and validate metadata
  -> aggregate PASS/FAIL/SKIP in stable order
  -> write report -> map to deterministic exit code
  -> clean context/window and temporary directory
```

P0 最小闭环不负责安装软件；它只观察、验证和报告。自动安装可隐藏包管理器失败，也需要额外权限，因此不纳入本周。

## 功能需求

### P0 必做

- 检查 `ffmpeg`、`ffprobe`、`gdb` 及相关诊断命令是否真实可执行并记录实际版本/缺失。
- 通过 CMake config 或明确的 `pkg-config` fallback 发现 FFmpeg 开发库、SDL2 与 OpenGL；不得只检查 CLI。
- 编译、链接并运行 `ffmpeg_smoke`，读取 linked FFmpeg library 版本，不打开或解码媒体。
- 编译、链接并运行 `sdl_gl_smoke`，创建最小 SDL2 window/OpenGL context、读取真实 GL 信息并反向释放资源。
- 用 GDB batch 命中带调试符号的项目函数并得到可识别调用路径。
- 分别验证 ASan、UBSan：clean target 正常退出，隔离 fault target 被对应 sanitizer 诊断且退出行为符合测试约定。
- 检测 Wireshark GUI 命令/版本；调用 `tshark -D` 或 `dumpcap -D` 只查询接口/权限，不启动真实 capture，不保存包内容。
- 用 `lavfi` 自生成 2-3 秒小素材，记录生成命令；用 ffprobe 验证至少 video/audio stream 类型、尺寸、采样率与时长容差。
- 对每项生成稳定的 PASS/FAIL/SKIP、可行动错误和证据；必需检查汇总严格映射为 `0/1/2` 退出码。
- 覆盖正常、边界、错误和清理测试；临时目录、SDL window 与 GL context 在所有路径上可释放。

### P1 可选

- Qt 已安装时只记录 executable/development package 版本，不编写第二套 GUI smoke。
- 增加机器可读 JSON 总报告，但文本报告仍是 P0。
- 在已有且可信的虚拟显示环境中增加 headless SDL/OpenGL 回归。
- 增加 `--only <check-id>`、超时配置或彩色输出；不得改变默认 P0 和退出码。

### 本周不做

- FFmpeg demux/decode/encode API、播放器、音视频同步、shader、纹理上传和媒体渲染。
- 同时维护 SDL2/Qt 两套窗口实现，或引入 GLFW/Vulkan 等额外图形栈。
- 自动提权、自动修改用户组/系统配置、以 root 运行真实抓包、保存 pcap 或任何敏感流量。
- 下载来源/版权不明的媒体，构建 FFmpeg 源码，或测试硬件编解码器。
- CI、容器镜像、跨平台安装器、包管理抽象或完整环境管理框架。

## 建议结构

```text
project/
  CMakeLists.txt
  README.md
  av-env-check.sh
  src/
    ffmpeg_smoke.cpp
    sdl_gl_smoke.cpp
  tests/
    CMakeLists.txt
    sanitizer_clean.cpp
    asan_fault.cpp
    ubsan_fault.cpp
    fixtures/
  assets/
    README.md
    generated-smoke.<actual-extension>
```

build 目录不提交；运行日志、版本、ffprobe 和报告全部放在同级 `../evidence/`。只在实现确实需要时创建文件，不为结构完整制造空文件。

## 核心接口与所有权

使用 Bash 脚本作为主入口 `av-env-check.sh`，并在其中调用编译好的 C++ smoke targets。不要在 Week 2 尝试用 C++ 封装跨平台多进程 runner。

- `av-env-check.sh` 拥有配置和最终状态集合，负责固定执行顺序、报告落盘及退出码映射。
- 在 bash 脚本中直接运行本周已知的短命令并捕获 `$?`，将输出重定向到临时文件；本周不实现通用超时和进程管理器。
- 每个检查返回确定的状态码与可落盘的证据，不保存指向局部 stdout/stderr 或临时路径的引用；bash 侧以命令退出码和临时文件保存每项结果。
- 临时目录由 `av-env-check.sh` 拥有，通过 `trap` 在正常/异常返回时清理；最终证据从临时位置完整写入后再保留。
- `ffmpeg_smoke` 不拥有媒体资源，只查询 linked library version。
- `sdl_gl_smoke` 的取得顺序为 SDL subsystem -> window -> GL context；释放顺序必须为 GL context -> window -> SDL subsystem。每次失败只释放已成功取得的资源。
- 报告生成失败映射为退出码 `2`；每次运行先写带本次日期的完整报告，不得把旧报告当成本次结果。

## 实现步骤

1. 冻结 check IDs、P0/P1、PASS/FAIL/SKIP 和 `0/1/2` 退出码，不依赖当前机器状态硬编码结果。
2. 用 CMake 建立 FFmpeg/SDL2/OpenGL discovery；先让缺依赖的 configure 产生明确错误。
3. 分别完成 FFmpeg 与 SDL2/OpenGL smoke，验证部分初始化失败与反向清理。
4. 使用实际可用 encoder 和 `lavfi` 生成短素材，记录命令并建立 ffprobe 字段/容差检查。
5. 实现固定顺序检查与文本报告；注入必需命令缺失、坏素材和报告不可写。
6. 添加 GDB batch、ASan/UBSan clean/fault 与 CTest 回归；fault targets 必须与正常入口隔离。
7. 从空 build 目录复现，更新真实命令、限制与最终证据；删除构建和临时产物。

## 构建与运行

项目尚未实现，以下全部是**预期命令**，不是当前已可用声明；实现后必须逐条验证并更新准确 target、参数和路径。

```bash
# 预期命令：configure 与构建
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# 预期命令：自动测试
ctest --test-dir build --output-on-failure

# 预期命令：运行环境自检并显式指定报告
./av-env-check.sh --build-dir build --report ../evidence/env-report.txt

# 预期命令：单独运行两个 smoke targets
./build/ffmpeg_smoke
./build/sdl_gl_smoke
```

素材生成命令必须在实现时根据 `ffmpeg -encoders` 的真实结果写入 `assets/README.md`；不得在规格阶段假定某个可选 encoder 已可用。形状应为 `lavfi testsrc + sine -> 明确时长/尺寸/采样率 -> project/assets/generated-smoke.<ext>`。实际命令执行后再保存 ffprobe JSON。

运行图形 smoke 前需有真实图形会话或已记录的虚拟显示；若无会话，程序必须输出可行动错误并按契约 FAIL/SKIP。Wireshark 部分只能查询版本和接口能力，不能运行 capture。

## 测试方案

| 类别 | 场景 | 关键断言 | 资源/证据 |
| --- | --- | --- | --- |
| 正常 | 全部必需依赖可用 | 固定顺序全 PASS，报告存在，退出 `0` | 最终报告完整 |
| 正常 | FFmpeg smoke | compile/link/runtime 成功，版本来自 linked API | 无媒体对象与临时文件 |
| 正常 | SDL2/OpenGL smoke | context 创建、GL 字符串非空、退出 `0` | context -> window -> SDL 反向释放 |
| 正常 | 合法生成素材 | video/audio 字段、尺寸、采样率、时长容差符合约定 | ffprobe JSON 可复核 |
| 边界 | Qt 不存在 | 仅 P1 `SKIP`，不影响 SDL2 P0 | 报告理由明确 |
| 边界 | `dumpcap -D` 权限不足 | 不抓包，记录能力限制；是否阻塞严格按契约 | 无 pcap、无提权进程 |
| 错误 | 必需命令不存在 | 对应 check FAIL，退出 `1`，不得沿用旧版本 | 失败原因明确 |
| 错误 | 开发库发现失败 | configure/report 指明准确 package/module | build 临时目录可删除 |
| 错误 | 无图形会话或 GL context 创建失败 | 明确 FAIL/SKIP，不输出伪 GL 信息 | 已取得 SDL/window 资源释放 |
| 错误 | 损坏/非媒体素材 | ffprobe check FAIL，退出 `1`，无陈旧 metadata | 临时输出删除 |
| 错误 | 报告路径不可写 | stderr 可行动，退出 `2`，旧报告不被伪装为本次结果 | 临时报告删除 |
| 调试 | GDB batch | 命中项目函数并显示符号/调用路径 | 保存 session 文本 |
| 动态检查 | ASan/UBSan clean | 两个 clean case 均无对应 sanitizer error | 正常退出 |
| 动态检查 | 隔离 fault targets | 对应诊断特征出现且状态符合预期 | 不产生/不提交 core dump |

CTest 至少注册 6 个高价值场景，正常/边界不少于 3 个、错误/降级不少于 3 个。对机器相关能力测试应显式说明前置环境，不得把“未运行”写成“通过”。

## 验收标准

- 从空 build 目录按 README configure、build、CTest 成功；至少 6 个测试 100% 通过。
- 必需环境完整时，一次 `av-env-check` 报告包含所有必需 check ID、固定顺序汇总并退出 `0`；任一必需检查故障注入退出 `1`，参数/报告错误退出 `2`。
- FFmpeg、SDL2、OpenGL 均不止有版本字符串：开发库 discovery、compile/link 与 runtime smoke 证据齐全。
- SDL2/OpenGL 成功路径和至少一个部分初始化失败路径均验证资源释放；测试后无窗口、GL context 和临时目录残留。纯 headless 时 runtime 可明确 `SKIP`，但需记录第 17 周前重验。
- GDB batch 能命中带符号函数；ASan 与 UBSan 各有 1 个 clean 和 1 个受控 fault 证据，诊断与退出行为匹配。
- 生成素材时长 2-3 秒，来源仅为记录的 `lavfi` 命令；ffprobe 至少核对 video/audio 类型、尺寸、采样率和时长容差。
- Wireshark GUI 状态及 tshark/dumpcap 查询能力有真实记录，但 evidence 中没有真实 pcap、IP/流量内容或提权抓包。
- `../evidence/` 至少包含 `tool-versions.txt`、`env-report.txt`、`ffprobe.json`、`ctest.log`、`sanitizers.log`、`gdb-session.txt`；真实失败可保留，但不得伪造成功。

## 调试指南

按层定位，避免看到 runtime 失败就反复重装：

1. **入口与环境**：记录工作目录、完整 argv、退出码、`PATH` 中命令位置；不要把全部环境变量写入报告。
2. **命令版本**：直接运行官方 version/help；区分 `not found`、权限和程序自身失败。
3. **开发库 discovery**：查看 CMake configure 与 `pkg-config` 查询；确认头文件、版本和 imported target，而非只看 CLI。
4. **compile/link**：从第一条项目诊断开始，检查 include target、link target 与 ABI/版本；需要时查看 verbose build，不手写猜测库顺序。
5. **runtime loader**：若可链接但启动失败，检查实际加载库和动态加载器错误，记录实际路径，不全局覆盖系统库。
6. **图形会话**：确认 DISPLAY/Wayland 可用性、SDL video driver、window 与 context 分别在哪一步失败；cleanup 日志应显示只释放已取得资源。
7. **外部进程/素材**：单独运行 ffmpeg/ffprobe 的相同 argv，检查 stderr 与退出码；坏素材不得复用旧 JSON。
8. **GDB/Sanitizer**：确认 Debug symbols 与 sanitizer flags 加在正确 target；fault case 同时检查工具特征和非零行为。
9. **报告与清理**：核对汇总状态、报告日期和最终退出码；运行后检查临时目录、窗口和 core dump。

常见错误：把 FFmpeg CLI 当开发库、同时混用系统库和自装库、只编译不运行、在 headless 环境把 SDL 失败误判为缺头文件、GL context 创建失败后漏销毁 window、fault target 进入正常构建入口、只看 stderr 不检查退出码、旧报告被误当本次成功，以及为验证 Wireshark 而进行不必要的真实抓包。

## 交付清单

- [ ] `CMakeLists.txt`、最小 source/tests、主入口脚本和准确 `.gitignore`
- [ ] FFmpeg 与 SDL2/OpenGL compile/link/runtime smoke targets
- [ ] 至少 6 个正常/边界/错误/清理 CTest 场景
- [ ] `assets/README.md`、自生成短素材与可复现 `lavfi` 命令
- [ ] `../evidence/tool-versions.txt` 与最终 `env-report.txt`
- [ ] `../evidence/ffprobe.json` 与元数据容差说明
- [ ] `../evidence/ctest.log`、`sanitizers.log`、`gdb-session.txt`
- [ ] `../notes/environment.md` 的安装来源、限制、未解决项和后续复用说明
- [ ] README 中所有已验证命令与当前 targets 一致；未执行命令仍明确标“预期命令”
- [ ] 无 build/core/临时产物、未知来源素材、真实抓包或敏感信息进入提交

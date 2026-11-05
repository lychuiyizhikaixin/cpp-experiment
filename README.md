# C++车辆指令执行器

当前版本为实验四综合实战，完成TR掉头、动作编排及Normal、Bus、SportsCar三种车型。242项GoogleTest通过，包含实验三原有55项。当前规则和验证方法见文末“实验四综合实战”，前面的章节保留各次实验过程。

## 实验一 高质量编程基础

基于课程初始工程实现 ADAS 车辆执行器。坐标使用 `std::int32_t`，朝向为 N、E、S、W，指令 M 前进一步，L/R 原地左转或右转 90 度。调用方保证参数合法，按课件约束不覆盖非法输入与整数溢出。

## 构建与测试

需要支持 C++17 的编译器及 CMake 3.16 以上版本。GoogleTest 源码已随工程提交，无需另行下载。

```bash
bash script/build_and_run.sh
ctest --test-dir build --output-on-failure
./build/tests/training_main --gtest_filter='ExecutorTest.*'
```

CMake 4 环境为旧版 GoogleTest 指定兼容策略下限；macOS 未显式指定 SDK 时使用当前 Command Line Tools 的 `SDKs/MacOSX.sdk` 别名。构建脚本遇错停止，保留增量构建目录。

## 接口

`Executor::NewExecutor(const Pose& = {0, 0, 'N'})` 创建对象，调用者负责释放，示例测试使用 `std::unique_ptr<Executor>` 管理。`Execute(const std::string&) noexcept` 按字符顺序执行，连续调用保持状态。`Query() const noexcept` 返回完整坐标及朝向。

## 十五项实验用例

前六项沿用课件，后八项补全四种朝向的左右转向，最后一项验证空输入、混合命令、连续调用与重复查询。每项均在测试与累计回归后推送 GitHub；TC02、TC15 复用已有实现直接通过，其余项保留测试失败与功能通过的独立提交。

| 用例 | 场景 | 初态 | 指令 | 终态 | 验收提交 |
| --- | --- | --- | --- | --- | --- |
| TC01 | 初始化指定状态 | (0,0,E) | `无指令` | (0,0,E) | `0a1eb69` |
| TC02 | 默认初始化 | 默认 | `无指令` | (0,0,N) | `07d3328` |
| TC03 | 朝东前进一步 | (0,0,E) | `M` | (1,0,E) | `af8a160` |
| TC04 | 朝西前进一步 | (0,0,W) | `M` | (-1,0,W) | `432c513` |
| TC05 | 朝北前进一步 | (0,0,N) | `M` | (0,1,N) | `56df768` |
| TC06 | 朝南前进一步 | (0,0,S) | `M` | (0,-1,S) | `2690d8d` |
| TC07 | 朝东左转至北 | (2,-3,E) | `L` | (2,-3,N) | `097c96b` |
| TC08 | 朝北左转至西 | (2,-3,N) | `L` | (2,-3,W) | `123c8d9` |
| TC09 | 朝西左转至南 | (2,-3,W) | `L` | (2,-3,S) | `4ee652c` |
| TC10 | 朝南左转至东 | (2,-3,S) | `L` | (2,-3,E) | `2599bd8` |
| TC11 | 朝东右转至南 | (2,-3,E) | `R` | (2,-3,S) | `f769229` |
| TC12 | 朝南右转至西 | (2,-3,S) | `R` | (2,-3,W) | `9a4c3ae` |
| TC13 | 朝西右转至北 | (2,-3,W) | `R` | (2,-3,N) | `7f348c4` |
| TC14 | 朝北右转至东 | (2,-3,N) | `R` | (2,-3,E) | `172294a` |
| TC15 | 混合指令与连续调用 | (0,0,N) | `Execute(""); Execute("MLM"); Execute("MRM")` | (-2,2,N) | `d9a80ff` |

共 15 个 GoogleTest 测试函数；CTest 注册一个汇总可执行目标，该目标运行所有 15 项。回归通过不等于已经测得百分之百代码覆盖率。

## 开发记录

提交使用 `test(TCxx)` 说明新增测试与验证目的，`feat(TCxx)` 说明实现的行为。每一项完成后均执行真实 `git push origin main`，实验报告中的终端截图与验收提交一一对应。环境兼容修复另以 `build` 提交保存，所有阶段均可通过 Git 历史回溯。

## 实验二 面向对象编程

在实验一十五项测试保护下，依次抽取行为函数、封装命令类、引入 ICommand 多态接口，再扩展 F 加速状态。所有阶段分别测试、提交和推送，保留可回溯过程。

### 实验二最终功能与验收

F 切换实例的加速状态；加速 M 调用两次一格 Move，加速 L/R 先 Move 再转向。命令对象无状态，通过 const ICommand 指针统一执行；对象位于栈上，无每字符堆分配。PoseEq.hpp/.cpp 只服务测试，不扩张生产接口。原有15项与新增20项合计35个测试均通过。

| 新增用例 | 输入 | 预期终态 |
| --- | --- | --- |
| F01 FM 东向加速移动 | `FM` | `[2, 0, 'E']` |
| F02 FL 东向先行后左转 | `FL` | `[1, 0, 'N']` |
| F03 FR 东向先行后右转 | `FR` | `[1, 0, 'S']` |
| F04 FFM 取消加速 | `FFM` | `[0, 1, 'N']` |
| F05 FM 西向加速移动 | `FM` | `[0, -3, 'W']` |
| F06 FM 北向加速移动 | `FM` | `[2, -1, 'N']` |
| F07 FM 南向加速移动 | `FM` | `[2, -5, 'S']` |
| F08 FL 北向先行后左转 | `FL` | `[2, -2, 'W']` |
| F09 FL 西向先行后左转 | `FL` | `[1, -3, 'S']` |
| F10 FL 南向先行后左转 | `FL` | `[2, -4, 'E']` |
| F11 FR 南向先行后右转 | `FR` | `[2, -4, 'W']` |
| F12 FR 西向先行后右转 | `FR` | `[1, -3, 'N']` |
| F13 FR 北向先行后右转 | `FR` | `[2, -2, 'E']` |
| F14 F 本身不移动与不转向 | `F` | `[2, -3, 'E']` |
| F15 FFF 奇数次切换后加速 | `FFFM` | `[0, 2, 'N']` |
| F16 FMFM 取消后恢复一步 | `FMFM` | `[0, 3, 'N']` |
| F17 跨调用保持加速与空输入 | `F / M / empty / RM / F / M` | `[3, 3, 'E']` |
| F18 课件完整加速轨迹 | `FMLMLM` | `[-3, 1, 'S']` |
| F19 不同执行器加速状态隔离 | `fast: FM; normal: M` | `fast=(0,2,N); normal=(0,1,N)` |
| F20 FF 取消后转向保持原地 | `FF / L / R` | `[2, -3, 'N']` |

截图中的临时 lab2 命令用于依次运行标准 CMake、GoogleTest、Git 命令；工程可直接用 `bash script/build_and_run.sh` 复现测试。对照 Git 提交前缀 lab2-Sxx 可查看每次抽取、封装、红灯与实现步骤。FFM 在旧版忽略 F 的实现中也会通过，必须与 FM/FL/FR 的失败和状态持续性测试共同判定功能。

## 实验三 解耦 表驱动与函数式编程

接续实验二 35 项测试，依次分离命令和状态、完成对象表驱动及函数式实现，再按红灯和绿灯小步实现 B 倒车指令。提交前缀 lab3-Sxx 记录每步测试与推送。

### 实验三最终结构与行为

`ExecutorImpl` 负责字符遍历和查表；`Command.hpp` 中的无状态函数对象通过 `operator()` 组合基本动作；`PoseHandler` 独占 point、direction、fast、reverse；`Direction.hpp` 使用顺时针 E/S/W/N 顺序；`Point.hpp` 提供坐标值与 `operator+=`。Command 仅依赖 PoseHandler，不依赖 ExecutorImpl。公共 Executor 接口与既有 35 项测试均保持原文件字节不变。

B 切换倒车；倒车 M 后退一格，L 右转、R 左转。B 与 F 叠加时 M 两次单格后退，L/R 先单格后退再反向转向。两种状态分别切换，跨调用保存且实例间隔离。

最终表为静态常量 `unordered_map<char, std::function<void(PoseHandler&)>>`，按值持有无状态函数对象；无引用捕获或 this 捕获。首次初始化可能分配内存，内存耗尽仍受现有 noexcept 接口约束；本实验不宣称所有场景均无分配或无异常终止。函数对象修改接收者状态，是函数式风格分派，不等同于无副作用的纯函数。

| 用例 | 输入 | 主要检查 |
| --- | --- | --- |
| B01 | `BM` | BM 东向后退一格 |
| B02 | `BL` | BL 东向反向右转 |
| B03 | `BR` | BR 东向反向左转 |
| B04 | `BFM` | BFM 加速倒退两格 |
| B05 | `BFL` | BFL 先倒退再右转 |
| B06 | `BFR` | BFR 先倒退再左转 |
| B07 | `BBM` | BBM 取消倒车 |
| B08 | `FBM / BFM` | FB 与 BF 状态正交 |
| B09 | `BM` | BM 西向后退一格 |
| B10 | `BM` | BM 北向后退一格 |
| B11 | `BM` | BM 南向后退一格 |
| B12 | `B / M / empty / L / M / B / M` | 跨调用倒车保持与空输入 |
| B13 | `BFFM` | 取消 F 后仍保持倒车 |
| B14 | `BFBM` | 取消 B 后仍保持加速 |
| B15 | `reverse: BM; normal: M` | 不同实例倒车状态隔离 |
| B16 | `BL / BR across E S W N` | BL 与 BR 四方向转向矩阵 |
| B17 | `BFM across W N S` | BFM 其余三个方向 |
| B18 | `BFL / BFR across E S W N` | BFL 与 BFR 四方向矩阵 |
| B19 | `BFMLMRM` | 混合倒车轨迹与中间状态 |
| B20 | `BLLLL / B / M` | 四次倒车转向与取消恢复 |

保留35项、新增20项，共55个GoogleTest用例。B16/B17/B18在各自测试函数内遍历8/3/8行场景，不将场景行数重复计算成GoogleTest用例数。另有干净构建、CTest、7个头文件自包含检查、短指令串独立参考模型对照与ASan/UBSan检查。可执行 `bash script/build_and_run.sh` 复现正式测试。S09和S11故意保存未实现需求的红灯，其余阶段均通过对应回归。

## 实验四实施计划

以实验三的 55 项测试为回归基线，按小步骤完成指令工厂、八种原子操作、动作编排、TR 掉头及目录重组；然后实现课件作业中的 Bus 和 SportsCar。每步重新构建测试，分别提交并推送。TR 在倒车时忽略，以课件第 39–42 页的示例为准。

## 实验四综合实战

课件任务包括指令工厂、八种原子操作、TR掉头和目录整理，作业扩展Bus与SportsCar。

```cpp
std::unique_ptr<adas::Executor> normal(adas::Executor::NewExecutor({2, -3, 'E'}));
std::unique_ptr<adas::Executor> bus(
    adas::Executor::NewExecutor({2, -3, 'E'}, adas::VehicleType::Bus));
std::unique_ptr<adas::Executor> sports(
    adas::Executor::NewExecutor({2, -3, 'E'}, adas::VehicleType::SportsCar));
sports->Execute("BFL");  // (1, -2, S)
```

### 结构

- `include/Executor.hpp`：公共接口、Pose及VehicleType，保留原工厂入口并新增车型重载。
- `src/executor/`：执行器，每条命令执行时生成动作组再执行。
- `src/command/`：解析TR、注册指令表、普通与车型命令编排。
- `src/action/`：ActionGroup及八种原子操作。
- `src/model/`：Point、Direction和每辆车的PoseHandler状态。
- `src/common/`：Singleton模板，工厂只保存可调用值与只读映射。
- `tests/data/VehicleCases.hpp`：96行车型方向与状态预期，独立于测试逻辑。

### TR与车型规则

TR连续出现才构成掉头，普通状态依次左转、前进一格、左转；F状态在最前面增加一格前进。B状态下忽略整个TR，以课件39至42页的测试与实现为准。解析器用内部Z标记TR，输入的字面Z不构成公开指令。跨两次Execute输入T和R不会拼成TR。未给出不同车型TR差异，因此三种车型共用以上原子规则。

以下动作均按顺序执行。F/B分别切换并跨调用保持，实例间互不影响。Bus坐标为车头，不额外模拟车尾碰撞。

| 车型 | 状态 | M | L | R |
| --- | --- | --- | --- | --- |
| Normal | 普通 | 前进1 | 左转 | 右转 |
| Normal | B | 后退1 | 右转 | 左转 |
| Normal | F | 前进1、前进1 | 前进1、左转 | 前进1、右转 |
| Normal | BF | 后退1、后退1 | 后退1、右转 | 后退1、左转 |
| Bus | 普通 | 前进1 | 前进1、左转 | 前进1、右转 |
| Bus | B | 后退1 | 后退1、右转 | 后退1、左转 |
| Bus | F | 前进1、前进1 | 前进1、前进1、左转 | 前进1、前进1、右转 |
| Bus | BF | 后退1、后退1 | 后退1、后退1、右转 | 后退1、后退1、左转 |
| SportsCar | 普通 | 前进1两次 | 左转、前进1 | 右转、前进1 |
| SportsCar | B | 后退1两次 | 右转、后退1 | 左转、后退1 |
| SportsCar | F | 前进1四次 | 前进1、左转、前进1 | 前进1、右转、前进1 |
| SportsCar | BF | 后退1四次 | 后退1、右转、后退1 | 后退1、左转、后退1 |

### 测试与复现

```bash
bash script/build_and_run.sh
ctest --test-dir build --output-on-failure
bash script/validate_lab4.sh build
```

最后一个脚本除242项正式测试外，还运行独立参考程序。参考程序只使用公开Executor接口，采用字符朝向和switch描述规则，枚举六种完整指令M/L/R/F/B/TR的0至4长度、三种车型与四个初始朝向，共18660组，比较逐前缀及整串调用结果。它不复用生产方向表或动作编排。

在macOS下，脚本从CMakeCache读取实际编译器与SDK路径，显式给独立链接命令传入同一SDK，避免工具默认选到不兼容的新SDK。初次独立链接曾因默认MacOSX27.0.sdk报unknown architecture，使用项目的MacOSX.sdk别名后通过。

另已通过干净Debug构建、10个生产头文件独立包含编译和ASan/UBSan的242项测试。macOS下LeakSanitizer关闭，未宣称完成全部泄漏检查。测试未覆盖非法朝向、坐标溢出、资源耗尽或并发修改同一车辆。

### 提交过程

实验四使用lab4-S01至lab4-S14记录14个步骤，每步分别提交和推送。S05保存TR的3项预期失败，S09保存Bus转弯的2项预期失败，S11保存SportsCar的2项预期失败，相应实现步骤恢复通过。实验三的55项测试在所有阶段始终通过，三个原测试源文件保持字节不变。

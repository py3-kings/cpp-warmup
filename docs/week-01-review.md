# W1 周复盘

任务编号：w1-review-1

项目：cpp-media-player

环境：WSL Ubuntu / C++17 / G++ / CMake / Unix Makefiles

## 1. 本周成果

### 成果一：C++ 工具链与生命周期

实际验证：
- CMake 配置返回码：0
- CMake 构建返回码：0
- CTest：2/2 Passed
- CTest 返回码：0

证据：
docs/evidence/w01/build-ctest.log

限制：
原 build-wsl 构建曾出现 Clock skew 警告。
2026-11-19 在 /tmp 独立干净构建复核：
- CMake 配置返回码：0
- 构建返回码：0
- CTest：2/2 Passed
- Clock skew 警告：0
- 证据：docs/evidence/w01/clock-skew-recheck.log
限制：原 /mnt/d 构建目录的长期稳定性仍待观察。

#### 发布仓库独立复现（2026-11-19）

发布仓库：py3-kings/cpp-warmup

运行环境：
- WSL Ubuntu
- Linux G++ 13.3.0
- C++17
- CMake Debug
- Unix Makefiles

实际验证：
- CMake 配置返回码：0
- 构建返回码：0
- 注册测试数量：3
- toolchain_check：Passed
- lifetime：Passed
- pointers：Passed
- CTest：3/3 Passed
- CTest 返回码：0
- 发布仓库测试整体返回码：0

关键观察：
- 工具链输出 toolchain-ok。
- 生命周期输出包含对象创建、逆序析构与异常处理。
- 指针实验包含 nullptr 边界检查。

证据：
docs/evidence/w01/publish-cmake-ctest.log

限制：
- 本次输出未出现 Clock skew 警告。
- DirGuard 和 FileGuard 已独立测试，
  但未注册到本次 CTest。
- CTest 通过不等于所有边界均有自动断言。

### 成果二：指针与引用

实际验证：
- 编译返回码：0
- 运行返回码：0
- 值传递不改变原变量
- 引用传递修改原变量
- 指针传递修改原变量
- nullptr 检查返回 false
- const 引用读取不改变原变量

证据：
docs/evidence/w01/pointers-build.log
docs/evidence/w01/pointers-run.log

### 成果三：RAII 资源管理

DirGuard：
- 正常路径 PASS
- 打开失败 PASS
- 异常退出 PASS
- 关闭尝试次数 2
- 关闭失败次数 0
- 返回码 0

FileGuard：
- 正常路径 PASS
- 打开失败 PASS
- 异常退出 PASS
- 关闭尝试次数 2
- 关闭失败次数 0
- 返回码 0

证据：
docs/evidence/w01/dir-guard-run.log
docs/evidence/w01/file-guard-run.log

限制：
FileGuard 最终文件为 0 字节；
当前实验未证明文件内容持久化。

## 2. 前置任务验收

已验证：
- 现有 CMake/CTest 两项测试执行成功
- pointers.cpp 独立实验
- dir_guard.cpp 独立实验
- file_guard.cpp 独立实验

缺证据：
- w1-cpp-1 至 w1-cpp-5 与成果的逐项对应记录
- 生命周期测试的详细输出与边界证据
- 原 /mnt/d 构建目录长期稳定性待观察；独立干净构建复核已通过
- 隔天独立变式主动取消，未执行

未完成：
- 周复盘最终验收
- JSON 异地备份
- Git 提交与远端核对

## 3. 本周工时

计划工时：待填写
实际工时：6 小时
编码工时：待填写
调试工时：待填写
测试与记录工时：待填写

说明：
工时与成果完成率分别统计。

## 4. 最大阻塞与根因

最大阻塞：RAII 概念理解不足。

根因：资源所有权、对象生命周期和析构机制等知识点尚未理解透彻；具体薄弱环节待验证。

已观察到的问题：
- CMake 构建存在 Clock skew 警告
- 当前工程目录不是 Git 仓库
- 部分实验尚未注册到 CTest

本周最大阻塞已确认：RAII 理解不足；其他问题属于技术或证据缺口。

## 5. 下周调整

下周计划投入：10 小时

第一个具体动作：阅读 labs/w01/dir_guard.cpp 的构造和析构逻辑，不看教程解释资源获取、异常退出和释放流程。

需要修复的项目：观察原 /mnt/d 增量构建时间戳稳定性，检查 CTest 覆盖范围及剩余证据缺口。

建议推迟的加深范围：FFmpeg/SDL3 深入功能及复杂 RAII 封装；优先完成 W1 基础理解验证。

验收检查命令：./build-wsl/w01_dir_guard_review；对照正常、打开失败和异常路径的输出。

## 6. 核心不变量

本周已通过实验验证：

成功获取的资源由对应 RAII 对象负责释放。
正常作用域退出和异常栈展开均会执行析构函数。

证据：
dir-guard-run.log
file-guard-run.log

边界：
资源打开失败时，构造函数抛出异常；
不产生成功构造的资源所有者对象。

### RAII 理解验收（2026-11-19）

状态：通过

本人独立解释：
- guard 成功构造后管理目录资源。
- 发生异常时，栈展开先执行 guard 的析构函数。
- 资源关闭发生在进入 catch 之前。
- opendir 失败导致构造未完成，不调用该 DirGuard 对象的析构函数。

证据：
docs/evidence/w01/dir-guard-run.log

限制：
隔一天不看复盘的独立变式尚未执行。

## 7. 独立变式

状态：主动取消，作为验收限制。

2026-11-19 RAII 补充口述：

1. 构造和析构职责：部分正确。
   需要区分对象生命周期与资源所有权。

2. 禁止复制：正确。
   防止多个对象错误管理同一资源。

3. 异常释放：正确。
   栈展开时执行已构造局部对象的析构函数。

4. 资源获取失败：需要纠正。
   当前实现抛出 std::system_error；
   是否重试由调用者根据错误原因决定。

结论：
基础理解具备，但资源获取失败的处理原则仍需巩固。

隔天独立变式：
用户明确取消，未执行，不标记通过。

## 8. JSON 备份

导出时间：2026-11-19 03:38:03 UTC

JSON 文件位置：
docs/backups/w01-evidence-20261119-033803-930308.json

项目外备份位置：
/home/sar/backups/cpp-media-player/w01/w01-evidence-20261119-033803-930308.json

备份文件数量：16
备份大小：46270 字节

SHA-256：
23b76c7275a2a0f40de36168c052034c925e27ac087a5f54b6f9e83988bf6b15

完整性检查：
- JSON 解码及文件校验：PASS
- 原文件 SHA-256：PASS
- 备份 SHA-256：PASS
- 两份文件逐字节比较：PASS
- 备份命令返回码：0

限制：
- 当前完成本机项目外备份，异机或云端备份未验证。
- 学习平台原生导入预览未执行。
- 此 JSON 是工程证据快照，不是原生进度导出。
- 后续修改复盘文件不会自动更新此快照。

注意：
CSV 不作为完整恢复备份。

### 最终工程证据 JSON 快照

导出时间（UTC）：2026-11-19T07:40:23.997762+00:00

发布仓库文件：
docs/backups/w01-final-20261119-074023-997762.json

本机项目外副本：
/home/sar/backups/cpp-media-player/w01/w01-final-20261119-074023-997762.json

备份范围：17 个文件，包含最终复盘及发布仓库测试日志。

验证方式：
- JSON 解析及 SHA-256 校验
- 文件内容恢复预览
- 项目内外副本逐字节比较

限制：
- 本备份属于工程证据快照。
- 不等同于学习平台原生进度 JSON。
- 学习平台原生导入预览仍未执行。
- 异机或云端备份仍未验证。

## 9. W1 最终结论

状态：带限制通过。

已完成：
- C++ 核心实验与关键边界路径验证
- 发布仓库 CTest 3/3
- RAII 基础理解检查
- 工时、阻塞与下周调整记录
- Clock skew 独立构建复核
- JSON 工程证据备份与本机项目外校验

验收限制：
- 隔天独立变式主动取消，未执行
- RAII 资源获取失败处理原则需要巩固
- 学习平台原生 JSON 导入预览未执行
- 异机或云端备份未验证
- 工时细分及部分任务编号映射仍缺记录
- DirGuard、FileGuard 尚未注册到发布仓库 CTest

交付收尾：
- 最终工程证据 JSON 已导出并完成本机项目外校验
- 完成 Git 提交、推送与远端哈希核对

Git 提交状态以实际命令输出为准。

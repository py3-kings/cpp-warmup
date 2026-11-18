# W1 构建、运行与资源生命周期

## 1. 工程信息

- 项目根目录：/mnt/d/work/cpp-media-player
- 操作系统：Ubuntu 24.04.5 LTS
- C++ 标准：C++17
- 编译器：G++ 13.3.0
- CMake：3.28.3
- 调试器：GDB 15.1
- 构建生成器：Unix Makefiles

## 2. 源码与程序入口

### 2.1 源码清单

| 源文件                 | CMake 目标      | 注册状态 |
|---                     |  ---            |---       | 
| labs/w01/toolchain.cpp | toolchain_check | 已注册   |
| labs/w01/lifetime.cpp  | lifetime        | 已注册   |
| labs/w01/pointers.cpp  | 无              | 未注册   |
| labs/w01/file_guard.cpp| 无 		   | 未注册   |
| labs/w01/dir_guard.cpp | 无 		   | 未注册   |

当前 CMake 已注册两个测试：
- toolchain_check
- lifetime

以上表示测试已经注册，不表示测试已经运行成功。

pointers.cpp 的 main() 入口尚未核对。

### 2.2 数据目录与输入条件

- 工程根目录：/mnt/d/work/cpp-media-player
- 正常目录路径：data/
- 文件实验路径：data/guard.txt
- 预设失败路径：data/w1cpp4_missing_dir

已检查的实际状态：

- data/ 存在。
- data/guard.txt 存在，大小为 0 字节。
- data/w1cpp4_missing_dir 不存在。

### 2.3 文件覆盖风险

FileGuard 使用 fopen(path, "w") 打开文件。

如果目标文件已经存在，"w" 模式会截断其内容。

因此，运行 file_guard 前必须确认目标文件
没有需要保留的数据，必要时先备份。

FileGuard 和 DirGuard 使用相对路径，
运行时工作目录应设为项目根目录。

当前尚未执行独立构建与测试。

## 3. 环境准备

### 3.1 运行环境

本实验在 WSL Ubuntu 24.04.5 LTS 中执行。

工具链：
- G++ 13.3.0
- CMake 3.28.3
- GDB 15.1
- C++17
- Unix Makefiles

本阶段不依赖 Ninja。

当前 CMake 目标未使用 SDL3 或 FFmpeg。
SDL3 指定前缀尚未找到，不作为本阶段构建的前置条件。

### 3.2 从任意目录进入工程

打开新的 Ubuntu Bash，执行：

```bash
cd /tmp || exit 1
cd /mnt/d/work/cpp-media-player || exit 1
pwd
```

预期工作目录：

/mnt/d/work/cpp-media-player

### 3.3 检查工具链

```bash
command -v g++
g++ --version | head -n 1

command -v cmake
cmake --version | head -n 1

command -v gdb
gdb --version | head -n 1

command -v make
make --version | head -n 1
```

### 3.4 验证结果

TODO：填写本次实际命令、返回码和关键输出。

### 3.5 GitHub 克隆仓库复现

本项目 GitHub 仓库：

https://github.com/py3-kings/cpp-warmup

首次克隆时，在准备存放项目的目录中执行：

```bash
git clone https://github.com/py3-kings/cpp-warmup.git
cd cpp-warmup || exit 1
```

如果已经克隆仓库，则直接进入实际克隆目录。

检查项目根目录：

```bash
pwd
test -f CMakeLists.txt
echo "cmake_file_rc=$?"
```

预期 cmake_file_rc=0。

后续 CMake 命令均以仓库根目录为工作目录。

文档中的 /mnt/d/work/cpp-media-player
是首次实验的实际路径，仅用于保留历史复现证据。

其他读者不需要使用相同的绝对路径。

## 4. 配置、编译与测试

### 4.1 CMake 配置

执行前进入项目根目录：

```bash
cd /mnt/d/work/cpp-media-player || exit 1
```

首次复现时使用不存在的 build-readme 目录。

配置命令：

```bash
cmake -S . -B build-readme \
  -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER=/usr/bin/g++ \
  -DCMAKE_PREFIX_PATH="$HOME/.local/opt/sdl3-3.4.20"
```

本次实际关键输出：

```text
-- The CXX compiler identification is GNU 13.3.0
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features - done
-- Configuring done (1.4s)
-- Generating done (0.1s)
```

实际返回码：0

配置检查：

```text
CMAKE_BUILD_TYPE:STRING=Debug
CMAKE_CXX_COMPILER:STRING=/usr/bin/g++
CMAKE_GENERATOR:INTERNAL=Unix Makefiles
```

结论：Debug 配置成功。

当前 SDL3 指定目录不存在，但现有 CMake
未使用 SDL3，因此不影响这两个目标的配置。

### 4.2 C++ 编译

执行：

```bash
cmake --build build-readme
```

实际关键输出：

```text
[ 50%] Built target toolchain_check
[100%] Built target lifetime
```

实际返回码：0

生成的可执行文件：

- build-readme/toolchain_check
- build-readme/lifetime

两个文件均已验证具有可执行权限。

### 4.3 CTest 测试

查看注册的测试：

```bash
ctest --test-dir build-readme -N
```

实际输出：

```text
Test #1: toolchain_check
Test #2: lifetime

Total Tests: 2
```

实际返回码：0

执行测试：

```bash
ctest --test-dir build-readme --output-on-failure
```

实际关键输出：

```text
1/2 Test #1: toolchain_check ... Passed
2/2 Test #2: lifetime .......... Passed

100% tests passed, 0 tests failed out of 2
```

实际返回码：0

结论：
- 已注册测试数量为 2，满足非零要求。
- 两个测试全部通过。
- file_guard 和 dir_guard 尚未注册，
  本次 CTest 不涵盖这两个实验。

### 4.4 构建和测试证据

- build-readme/build.log
- build-readme/ctest.log

### 4.5 GitHub 发布仓库验证

验证仓库：py3-kings/cpp-warmup

验证目录：build-publish-check

本次验证使用 C++17、G++ 13.3.0、
CMake Debug 和 Unix Makefiles。

配置命令：

```bash
cmake -S . -B build-publish-check \
  -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER=/usr/bin/g++ \
  -DCMAKE_PREFIX_PATH="$HOME/.local/opt/sdl3-3.4.20"
```

配置结果：PASS，返回码 0。

编译命令：

```bash
cmake --build build-publish-check
```

实际编译目标：

- toolchain_check：PASS
- lifetime：PASS
- pointers：PASS

编译返回码：0。

CTest 命令：

```bash
ctest --test-dir build-publish-check \
  --output-on-failure
```

实际测试结果：

```text
1/3 Test #1: toolchain_check ... Passed
2/3 Test #2: lifetime .......... Passed
3/3 Test #3: pointers .......... Passed

100% tests passed, 0 tests failed out of 3
```

CTest 返回码：0。

结论：

GitHub 发布仓库三个构建目标均编译成功，
三个已注册测试全部通过。

首次实验工程的 CTest 结果为 2/2，
发布仓库的 CTest 结果为 3/3。

这是因为发布仓库额外注册了 pointers 测试。

本节记录的是发布仓库的实际验证结果，
不替代首次实验的历史记录。

证据文件：

- build-publish-check/build.log
- build-publish-check/ctest.log

## 5. 程序运行与调试

### 5.1 toolchain_check

执行命令：

```bash
./build-readme/toolchain_check
```

实际输出：

```text
toolchain-ok
```

实际返回码：0

结论：程序运行成功。

### 5.2 lifetime

执行命令：

```bash
./build-readme/lifetime
```

实际输出：

```text
create outer
create inner
destroy inner
outer still alive: outer
create level1
create level2
create level3
destroy level3
destroy level2
destroy level1
create heap
before move: 1
heap_obj owner: 0
another owner: 1
destroy heap
create heap_exception
destroy heap_exception
caught heap exception
create try_outer
create try_inner
destroy try_inner
destroy try_outer
caught forced
destroy outer
```

实际返回码：0

结论：
- 普通局部对象按作用域自动析构。
- 嵌套对象按构造的相反顺序析构。
- unique_ptr 所有权转移符合预期。
- 异常传播时，已成功构造的局部对象正常析构。

### 5.3 GDB 断点调试

执行命令：

```bash
gdb -q -batch \
    -ex "set pagination off" \
    -ex "break labs/w01/lifetime.cpp:15" \
    -ex "run" \
    -ex "bt 4" \
    -ex "print name" \
    --args ./build-readme/lifetime
```

实际断点位置：

labs/w01/lifetime.cpp:15

实际命中函数：

Trace::~Trace()

实际调用栈：

```text
#0 Trace::~Trace()
#1 run_inner_experiment()
#2 main()
```

实际变量：

```text
$1 = "inner"
```

GDB 返回码：0

结论：
GDB 成功命中析构函数断点，
可以查看当前对象及调用关系。

此次调试未继续执行到程序结束。

### 5.4 证据文件

- build-readme/toolchain-run.log
- build-readme/lifetime-run.log
- build-readme/gdb-breakpoint.log

## 6. 资源生命周期

### 6.1 Trace 对象生命周期

对象创建位置：构造函数中

对象析构时机：对象离开作用域时

实际观察到的析构顺序：在这一嵌套作用域实验中，局部对象按照构造完成的相反顺序析构

### 6.2 FileGuard 文件资源管理

资源获取函数：fopen

资源释放函数：fclose

如何防止重复所有权：FileGuard(const FileGuard&) = delete;
                    FileGuard& operator=(const FileGuard&) = delete;
	禁止复制构造；禁止复制赋值；防止通过复制产生多个管理同一文件句柄的对象
文件打开失败时：fopen()失败时返回nullptr，FileGuard检查返回值后抛出std::system_error，构造未完成，因此不会执行该对象的析构函数

异常退出时：析构函数调用fclose()

当前验证状态：已检查源码；本次Cmake/CTest未覆盖FileGuard,尚无本轮独立运行证据

### 6.3 DirGuard 目录资源管理

资源获取函数：opendir

资源释放函数：closedir

如何防止重复所有权：DirGuard(const DirGuard&) = delete;
                    DirGuard& operator=(const DirGuard&)=delete;

目录打开失败时：opendir()打开失败返回nullptr，程序报错std::system_error,对象未完成构造，不执行该对象的析构函数

异常退出时：如果DirGuard已完成构造，异常栈展开时会自动调用~DirGuard，析构函数自动调用closedir（）释放资源
当前验证状态：已检查源码，本次Cmake/CTest未覆盖DirGuard，尚无本轮独立运行证据

### 6.4 RAII 核心原理

RAII 的含义：RAII是资源获取即初始化，对象在初始化过程中获取资源，并负责管理资源，使资源释放与对象生命周期关联
 
为什么能够减少资源泄漏：发生异常栈展开时，需要离开作用域的已成功构造的局部对象会执行析构函数，从而释放其管理的资源
 
与手动 new/delete 的区别：
手动 new/delete	           std::unique_ptr
需要明确执行 delete	   由智能指针负责释放
异常可能导致遗漏 delete	   正常作用域退出或异常栈展开时自动释放
原始指针不表达唯一所有权   表达独占所有权
管理责任由程序员承担	   管理责任封装在对象中

### 6.5 三分钟讲解记录

讲解顺序：

1. 资源如何创建：创建对象的同时创建资源，将资源的释放与对象的生命周期关联
2. 对象如何获得所有权：通过std::make_unique<Trace>()获得对象所有权
3. 正常退出如何释放：通过析构函数正常释放
4. 异常退出如何释放：异常栈展开，通过析构函数释放
5. 如何验证资源释放：例如lifetme.cpp过程中，create正常创建后若没有异常会正常输出destroy，说明资源已经释放，通过GDB查看也能值得在构造对象正常后，能正常进入析构函数

答不清的问题：如何进一步验证File Guard和DirGuard的资源实际关闭成功，而不仅是进入析构函数

## 7. 边界与独立复现

### 7.1 错误工作目录实验

实验目标：

验证读者从其他目录开始时，能按照文档找到工程根目录。

实验前预测：

1. 从 /tmp 检查相对路径 CMakeLists.txt 应失败。
2. 进入正确工程目录后检查应成功。
3. 文档文件应能正常访问。

实际执行命令：

```bash
cd /tmp || exit 1
pwd

test -f CMakeLists.txt
echo "wrong_dir_rc=$?"

cd /mnt/d/work/cpp-media-player || exit 1
pwd

test -f CMakeLists.txt
echo "correct_dir_rc=$?"

test -f docs/build-and-lifetime.md
echo "document_rc=$?"
```

实际关键输出：

```text
/tmp
wrong_dir_rc=1

/mnt/d/work/cpp-media-player
correct_dir_rc=0
document_rc=0
```

实验结论：

错误工作目录下无法通过相对路径找到工程文件。

使用绝对路径进入正确工程目录后，
CMakeLists.txt 和运行文档均可正常访问。

实际结果与预测一致。

### 7.2 独立复现

复现方式：另一位读者在本电脑 WSL Ubuntu 中独立操作。

实际执行日期：2026-11-18

构建目录：build-peer-20261118-191019

实验前预测：
按照文档从正确项目目录开始，
能够成功配置、编译、测试、运行和调试。

实际结果：

| 检查项目 | 实际结果 | 状态 |
|---|---|---|
| CMake 配置 | 返回码 0 | PASS |
| C++ 编译 | 返回码 0 | PASS |
| CTest | 2/2 Passed，返回码 0 | PASS |
| toolchain_check | toolchain-ok，返回码 0 | PASS |
| lifetime | 返回码 0 | PASS |
| GDB 断点 | 命中 Trace::~Trace() | PASS |
| GDB 变量 | name = "inner" | PASS |
| GDB 继续运行 | exited normally，返回码 0 | PASS |

预测与实际差异：
未发现影响复现结果的差异。

独立复现结论：PASS。

证据目录：
build-peer-20261118-191019/

已知限制：
FileGuard 和 DirGuard 尚未纳入 CTest，
本次独立复现不覆盖这两个实验。

## 8. 验收结论

| 验收项目 | 已知实际结果 | 状态 |
|---|---|---|
| Ubuntu 环境 | 24.04.5 LTS | PASS |
| G++ | 13.3.0 | PASS |
| CMake Debug 配置 | 返回码 0 | PASS |
| C++17 编译 | 返回码 0 | PASS |
| CTest | 2/2 Passed | PASS |
| toolchain_check | toolchain-ok，返回码 0 | PASS |
| lifetime | 返回码 0，析构顺序符合预期 | PASS |
| GDB | 命中 Trace::~Trace() | PASS |
| 错误目录恢复 | 返回码 1 → 0 | PASS |
| FileGuard 独立运行 | 本轮没有验证 | NOT TESTED |
| DirGuard 独立运行 | 本轮没有验证 | NOT TESTED |
| 文档独立复现 | 另一位读者完成，CTest 2/2，GDB 正常退出 | PASS |

### 8.1 验收证据

| 证据文件 | 对应实验 |
|---|---|
| build-readme/build.log | CMake 编译 |
| build-readme/ctest.log | CTest 测试 |
| build-readme/toolchain-run.log | 工具链运行 |
| build-readme/lifetime-run.log | 生命周期运行 |
| build-readme/gdb-breakpoint.log | GDB 析构断点 |

以上日志均已检查存在且非空。

### 8.2 已知限制

1. FileGuard 未纳入当前 CMake/CTest 测试。
2. DirGuard 未纳入当前 CMake/CTest 测试。
3. SDL3 指定安装目录不存在，当前 W1 目标未使用 SDL3。
4. 文档尚未完成隔日或他人独立复现。
5. build-readme 是构建输出目录，不应将其作为源码提交。

### 8.3 后续任务

完成独立复现后，填写实际命令、返回码、
关键输出及预测与实际结果的差异。

验收全部通过后，将文档提交至
py3-kings/cpp-warmup 仓库。

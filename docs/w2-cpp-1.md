# W2 · w2-cpp-1：选择 STL 容器

## 1. 任务信息

任务：W2 选择 STL 容器
目标：使用 vector、string、unordered_map 实现文件词频统计器
环境：WSL Ubuntu Bash、C++17、G++、CMake、Unix Makefiles
模式：真实编译与运行
Git 版本：见本报告所在 Git 提交

## 2. 文件与资源

源文件：
- labs/w02/word_count.cpp
- labs/w02/vector_growth.cpp

测试输入：
- data/words.txt
- data/w2-cpp-1-empty.txt
- data/w2-cpp-1-case.txt

测试脚本：
- tests/w02/check_normal.sh
- tests/w02/check_missing.sh

预期结果：
- tests/w02/normal.expected
- tests/w02/empty.expected
- tests/w02/case.expected

资源管理：
- ifstream 管理输入文件资源
- string 保存读取的单词
- unordered_map 保存词频映射
- vector 保存待排序的词频记录

## 3. 正常路径

输入：
red blue red

实际输出：
blue=1
red=2

程序返回码：0
验收：PASS

## 4. 边界与失败路径

空文件：
- 实际标准输出：0 字节
- 实际返回码：0
- 验收：PASS

缺失文件：
- 实际标准输出：0 字节
- 实际返回码：3
- 验收：PASS

## 5. vector 扩容实验

实际结果：
BEFORE_CAPACITY=2
BEFORE_FIRST=10
AFTER_FIRST=10
AFTER_CAPACITY=4
REALLOCATED=1

BUILD_RC=0
RUN_RC=0

结论：
发生内存重新分配后，旧迭代器失效。
实验没有解引用失效迭代器，而是重新获取 begin()。

验收：PASS

## 6. 独立变式

规则：
ASCII 英文字母统一转换为小写后统计。

输入：
Red red

预测：
red=2

归一化前实际输出：
Red=1
red=1

首次实现出现错误：
d=2

修复后实际输出：
red=2

CASE_AFTER_RC=0
REGRESSION_RC=0

预测与最终实际结果一致。
验收：PASS

## 7. CMake 与 CTest

完整构建：
FULL_BUILD_RC=0

完整回归：
7/7 Passed
FULL_CTEST_RC=0

构建日志：
artifacts/w2-full-build.PxsA5S.log

CTest 日志：
artifacts/w2-full-ctest.5O6jl2.log

验收：PASS

## 8. 理解检查

为什么最终排序？
unordered_map 不保证遍历顺序。
复制到 vector 后使用 sort，
可以按照单词字典序获得稳定输出。

边界输入需要保持什么不变量？
输入合法但没有单词时，返回成功和空结果。
文件打开失败时，返回错误码，不能伪装为空文件成功。

## 9. 已知限制

- 缺失文件返回 3，但当前没有 stderr 错误诊断。
- 构建过程中曾出现 Clock skew 时间戳警告。
- 当前文件资源由 main 函数中的 ifstream 管理，
  尚未抽象成独立的可复用文件资源封装类。
- 大小写归一化仅覆盖约定的 ASCII 英文规则。

## 10. 验收结论

正常路径：PASS
边界与失败路径：PASS
vector 扩容实验：PASS
独立变式：PASS
CMake 构建：PASS
CTest 完整回归：PASS

W2 功能与测试验收：PASS
GitHub 交付：见仓库 main 分支提交记录

## 11. 发布仓库中的验收证据

- docs/evidence/w02/full-build.log
- docs/evidence/w02/full-ctest.log

上述日志来自实验工程已完成的完整构建与
7/7 CTest 回归。发布仓库包含额外的 W1
pointers 目标，不将实验工程的 7/7 结果
表述为发布仓库完整测试结果。

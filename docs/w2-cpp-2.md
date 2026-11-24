# W2 · w2-cpp-2 使用智能指针

## 环境

- WSL Ubuntu
- C++17 / Linux G++
- CMake / Unix Makefiles / CTest
- 项目：cpp-media-player
- 发布仓库：py3-kings/cpp-warmup

## 实验目标

1. unique_ptr 唯一所有权
2. unique_ptr 移动语义
3. shared_ptr 共享引用计数
4. FILE* 自定义删除器
5. 文件打开失败与空指针安全
6. FileGuard 独立变式

## 实际结果

来源：学习过程中提交的真实终端输出。

### unique_ptr 生命周期

COMPILE_RC=0
unique_scope=PASS
[DESTROY] count=1

### unique_ptr 移动

unique_move=PASS
[DESTROY] count=2

移动后原智能指针为空，所有权由目标智能指针持有。

### shared_ptr 复制

shared_initial_count=1
shared_copy_count=2
shared_restored_count=1
shared_copy=PASS
[DESTROY] count=3

复制 shared_ptr 增加共享所有者数量，
不会复制被管理的 TrackedObject。

### FILE* 正常路径

[CLOSE] count=1 status=ok
file_normal=PASS

文件内容：
hello smart pointer

FILE_CHECK_RC=0

### 文件打开失败

[EXPECTED OPEN FAILURE]: Not a directory
file_failure=PASS
RUN_RC=0

打开失败后没有产生额外的 fclose 调用。
空资源不应传入 fclose。

### 独立变式

VARIANT_COMPILE_RC=0
[VARIANT_CLOSE] count=1 status=ok
file_guard_variant=PASS
VARIANT_RUN_RC=0
VARIANT_FILE_CHECK_RC=0

## CTest 验收

w2_cpp_2_smart_pointer: Passed
w2_cpp_2_file_guard_variant: Passed

100% tests passed, 0 tests failed out of 2
Total Test time: 0.02 sec
CTEST_RC=0

以上为源工程中两项指定 CTest 的实际结果。
发布仓库本身尚未另行执行构建。
本轮未收到 CONFIGURE_RC、BUILD_RC 的具体输出。

## FileGuard 与 unique_ptr 接口比较

FileGuard：
- 构造函数负责 fopen
- 析构函数负责 fclose
- get() 返回借用的 FILE*
- W1 版本打开失败抛出 system_error
- 禁止复制

unique_ptr<FILE, CloseFile>：
- 调用方执行 fopen
- 自定义删除器执行 fclose
- get() 返回借用的 FILE*
- 打开失败可检查空指针
- 禁止复制，支持移动

## 所有权理解

get():
- 返回被管理的原始指针
- 不转移所有权
- 调用方不能擅自释放资源

release():
- 返回原始指针
- 放弃智能指针对资源的所有权
- 不调用删除器
- 接收者必须负责后续释放或重新托管

reset():
- 释放当前管理的资源
- 非空资源会调用相应删除器
- 可以将智能指针重置为空

## 不变量

1. unique_ptr 管理的同一资源只有一个所有者。
2. 同一资源不能交给两个独立所有者重复释放。
3. 空 FILE* 不允许传入 fclose。
4. 正常路径和失败路径都必须遵守释放规则。
5. 不应仅为方便而把独占资源改为 shared_ptr。

## 资源归属建议

- FILE*：优先 unique_ptr 加 CloseFile。
- FFmpeg AVFrame*：使用匹配的 FFmpeg 释放函数。
- FFmpeg AVFormatContext*：按创建与打开方式匹配释放接口。
- SDL3 窗口：优先明确的独占生命周期管理。
- 确实需要多个所有者共同维持生命周期时才考虑 shared_ptr。

上述媒体资源选择是设计建议，尚未核对媒体工程中的具体调用关系。

## 预测与实际差异

任务预期：
- 唯一资源只析构一次。
- shared_ptr 复制后计数增加。
- 正常打开后自动关闭文件。
- 打开失败不产生无效 fclose。

已观察结果与上述预期一致。
未单独保存用户在独立变式编码前亲自记录的预测。

## TODO

- [x] unique_ptr 生命周期
- [x] unique_ptr 移动语义
- [x] shared_ptr 引用计数
- [x] FILE* 自定义删除器
- [x] 文件正常路径
- [x] 文件失败路径
- [x] FileGuard 独立变式
- [x] 两项指定 CTest 通过
- [x] 记录 get、release、reset 原理
- [ ] 核对实际媒体资源的所有权关系
- [ ] 发布仓库 Git 提交与远程推送

## 已知限制

- 本次没有实际调用 release() 的独立运行实验。
- 本次没有实际调用 reset() 的独立运行实验。
- 尚未执行发布仓库的独立构建。
- 没有提供完整 CTest 回归结果。
- 本次不修改 W1 原有 FileGuard。

#include <cstdio>
#include <iostream>
#include <memory>
#include <utility>

// W2-C++-2: unique_ptr lifetime experiment

struct TrackedObject {
    inline static int destroyed = 0;

    ~TrackedObject() noexcept {
        // TODO 1:
        // 增加 destroyed 计数
	++destroyed;
        // 输出 [DESTROY] count=<当前计数>
	std::cout<<"[DESTROY] count="<<destroyed<<'\n';
    }
};


struct CloseFile {
    inline static int close_count = 0;
    inline static int close_fail_count = 0;

    void operator()(std::FILE* file) const noexcept {
        // TODO 1: 空指针不能调用 fclose
        if (file == nullptr) {
            return;
        }

        // TODO 2: 关闭文件资源
        const int result = std::fclose(file);

        // TODO 3: 记录实际关闭次数
        ++close_count;

        // TODO 4: 检查 fclose 返回码
        if (result != 0) {
            ++close_fail_count;
        }

        // TODO 5: 输出关闭日志
        std::printf(
            "[CLOSE] count=%d status=%s\n",
            close_count,
            result == 0 ? "ok" : "failed"
        );
    }
};


bool test_unique_scope() {
    const int before = TrackedObject::destroyed;

    bool owner_created = false;
    {
        // TODO 2:
        // 使用 std::make_unique 创建 TrackedObject
        // 交给局部 unique_ptr 管理
	auto ptr = std::make_unique<TrackedObject>();
        // TODO 3:
        // 确认智能指针持有非空资源
        // 不允许手动 delete
	owner_created = (ptr != nullptr);
    }

    // TODO 4:
    // 解释为什么离开作用域后析构计数应增加 1
    const bool passed =
        TrackedObject::destroyed == before + 1;

    std::cout
        << "unique_scope="
        << (passed ? "PASS" : "FAIL")
        << '\n';

    return passed;
}

bool test_unique_move() {
    const int before = TrackedObject::destroyed;

    bool owner_transferred = false;

    {
        auto source = std::make_unique<TrackedObject>();

        // TODO 1:
        // 使用 std::move(source)
        // 将所有权转移给 target
        auto target = std::move(source);
        // TODO 2:
        // 检查 source 是否为空
        // 检查 target 是否非空
        // 将检查结果赋给 owner_transferred
	owner_transferred = (source == nullptr) && (target != nullptr);
    }

    const bool destroyed_once =
        TrackedObject::destroyed == before + 1;

    const bool passed =
        owner_transferred && destroyed_once;

    std::cout
        << "unique_move="
        << (passed ? "PASS" : "FAIL")
        << '\n';

    return passed;
}


bool test_shared_copy() {
    const int before = TrackedObject::destroyed;

    bool counts_ok = false;

    {
        // TODO 1: 创建 shared_ptr
        auto first = std::make_shared<TrackedObject>();

        // TODO 2: 初始引用计数应该为 1
        const bool initial_ok =
            first.use_count() == 1;

        std::cout
            << "shared_initial_count="
            << first.use_count()
            << '\n';

        bool copy_ok = false;

        {
            // TODO 3: 复制 shared_ptr
            auto second = first;

            // TODO 4: 两个指针应该共享同一个对象
            copy_ok =
                first.use_count() == 2 &&
                second.use_count() == 2 &&
                first.get() == second.get();

            std::cout
                << "shared_copy_count="
                << first.use_count()
                << '\n';
        }

        // TODO 5: second 销毁后，计数恢复为 1
        const bool restored_ok =
            first.use_count() == 1;

        std::cout
            << "shared_restored_count="
            << first.use_count()
            << '\n';

        counts_ok =
            initial_ok &&
            copy_ok &&
            restored_ok;
    }

    // TODO 6: 最后一个所有者离开作用域，析构一次
    const bool destroyed_once =
        TrackedObject::destroyed == before + 1;

    // TODO 7: 汇总结果
    const bool passed =
        counts_ok && destroyed_once;

    std::cout
        << "shared_copy="
        << (passed ? "PASS" : "FAIL")
        << '\n';

    return passed;
}


bool test_file_normal() {
    const int before = CloseFile::close_count;
    const int before_fail = CloseFile::close_fail_count;

    bool opened = false;
    bool written = false;

    {
        // TODO 6: 使用自定义删除器管理 FILE*
        std::unique_ptr<std::FILE, CloseFile> file(
            std::fopen("artifacts/w2-cpp-2-normal.txt", "w")
        );

        // TODO 7: 检查资源是否成功打开
        opened = (file != nullptr);

        if (opened) {
            // TODO 8: 使用 get() 借用原始指针写入
            written = std::fputs(
                "hello smart pointer\n",
                file.get()
            ) >= 0;
        } else {
            std::perror("[ERROR] fopen");
        }

        // TODO 9:
        // 不手动 fclose
        // 离开作用域后 unique_ptr 自动执行 CloseFile
    }

    // TODO 10: 验证只关闭一次且无关闭失败
    const bool closed_once =
        CloseFile::close_count == before + 1;

    const bool close_ok =
        CloseFile::close_fail_count == before_fail;

    const bool passed =
        opened &&
        written &&
        closed_once &&
        close_ok;

    std::cout
        << "file_normal="
        << (passed ? "PASS" : "FAIL")
        << '\n';

    return passed;
}


bool test_file_failure() {
    // TODO 1: 保存关闭次数，作为测试基准
    const int before = CloseFile::close_count;
    const int before_fail = CloseFile::close_fail_count;

    bool open_failed = false;
    bool empty_safe = false;

    {
        // TODO 2: 使用无效文件路径
        // 正常测试已创建 w2-cpp-2-normal.txt
        // 它是普通文件，不能作为目录
        const char* path =
            "artifacts/w2-cpp-2-normal.txt/invalid";

        // TODO 3: 使用 unique_ptr 管理 fopen 返回值
        std::unique_ptr<std::FILE, CloseFile> file(
            std::fopen(path, "w")
        );

        // TODO 4: 验证打开失败
        open_failed = (file == nullptr);

        if (open_failed) {
            std::perror("[EXPECTED OPEN FAILURE]");
        } else {
            std::cerr
                << "[ERROR] unexpected open success\n";
        }

        // 不调用 fputs
        // 不手动 fclose
    }

    {
        // TODO 5: 构造一个明确的空资源
        std::unique_ptr<std::FILE, CloseFile> empty(nullptr);

        empty_safe = (empty == nullptr);

        // 离开作用域，不应调用 fclose
    }

    // TODO 6: 验证没有发生无效关闭
    const bool no_extra_close =
        CloseFile::close_count == before;

    // TODO 7: 验证没有关闭错误
    const bool no_close_failure =
        CloseFile::close_fail_count == before_fail;

    // TODO 8: 汇总验收结果
    const bool passed =
        open_failed &&
        empty_safe &&
        no_extra_close &&
        no_close_failure;

    std::cout
        << "file_failure="
        << (passed ? "PASS" : "FAIL")
        << '\n';

    return passed;
}


int main() {
    const bool scope_passed = test_unique_scope();
    const bool move_passed = test_unique_move();
    const bool shared_passed = test_shared_copy();
    // TODO 11: 验证 FILE* 自定义删除器
    const bool file_passed = test_file_normal();
    const bool failure_passed = test_file_failure();

    // TODO 12: 所有检查通过才返回 0
    const bool all_passed =
        scope_passed &&
        move_passed &&
        shared_passed &&
        file_passed &&
	failure_passed;
    return all_passed ? 0 : 1;
}

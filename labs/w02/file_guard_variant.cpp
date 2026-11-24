#include <cstdio>
#include <iostream>
#include <memory>

// W2-C++-2: FileGuard independent variant

struct CloseFile {
    inline static int close_count = 0;
    inline static int close_fail_count = 0;

    void operator()(std::FILE* file) const noexcept {
        // TODO 1: 空指针安全检查
        if (file == nullptr) {
            return;
        }

        // TODO 2: 关闭文件
        const int result = std::fclose(file);

        // TODO 3: 统计关闭次数
        ++close_count;

        if (result != 0) {
            ++close_fail_count;
        }

        std::printf(
            "[VARIANT_CLOSE] count=%d status=%s\n",
            close_count,
            result == 0 ? "ok" : "failed"
        );
    }
};

bool test_file_guard_variant() {
    const int before = CloseFile::close_count;
    const int before_fail = CloseFile::close_fail_count;

    bool write_ok = false;

    {
        // TODO 4: 使用 unique_ptr 替换 FileGuard
        std::unique_ptr<std::FILE, CloseFile> file(
            std::fopen(
                "artifacts/w2-cpp-2-variant.txt",
                "w"
            )
        );

        // TODO 5: 检查打开是否成功
        if (!file) {
            std::perror("[VARIANT_OPEN_ERROR]");
            return false;
        }

        // TODO 6: 借用 FILE* 写入文件
        write_ok = std::fputs(
            "written by unique_ptr\n",
            file.get()
        ) >= 0;

        // TODO 7:
        // 不手动 fclose
        // 离开作用域后自动释放
    }

    // TODO 8: 验证恰好关闭一次
    const bool closed_once =
        CloseFile::close_count == before + 1;

    const bool close_ok =
        CloseFile::close_fail_count == before_fail;

    const bool passed =
        write_ok &&
        closed_once &&
        close_ok;

    std::cout
        << "file_guard_variant="
        << (passed ? "PASS" : "FAIL")
        << '\n';

    return passed;
}

int main() {
    // TODO 9: 运行独立变式并返回状态码
    return test_file_guard_variant() ? 0 : 1;
}

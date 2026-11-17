#include <cerrno>
#include <cstdio>
#include <dirent.h>
#include <stdexcept>
#include <string>
#include <system_error>

class DirGuard {
private:
    DIR* dir_ = nullptr;

    inline static int close_count_ = 0;
inline static int close_fail_count_ = 0;

public:
    explicit DirGuard(const char* path) {
        dir_ = ::opendir(path);

        if (dir_ == nullptr) {
            const int error = errno;

            throw std::system_error(
                error,
                std::generic_category(),
                std::string("opendir failed: ") + path
            );
        }

        std::printf("[OPEN DIR] %s\n", path);
    } 

    ~DirGuard() noexcept {
        // TODO 2:
        // 目录句柄有效时调用 closedir
        // 记录关闭次数
        if (dir_ != nullptr) {
            int result = ::closedir(dir_);

            dir_ = nullptr;
            ++close_count_;
if (result != 0) {
    ++close_fail_count_;
}
            std::printf(
                "[CLOSE DIR] count=%d,status=%s\n",
                close_count_,
                result == 0 ? "ok" : "failed"
            );
        }
    }

    DirGuard(const DirGuard&) = delete;
    DirGuard& operator=(const DirGuard&) = delete;

    static int close_count() noexcept {
        return close_count_;
    }
static int close_fail_count() noexcept {
    return close_fail_count_;
}
};

int main() {
    // TODO 3: 正常路径
    std::puts("=== TEST: Normal path ===");

    int before_normal = DirGuard::close_count();
    bool normal_opened = false;

    try {
        {
            DirGuard guard("data/");
            normal_opened = true;
    } // guard 在这里析构
    }
    catch (const std::exception& e) {
        std::fprintf(stderr, "[ERROR] %s\n", e.what());
    }

    bool normal_passed =
        normal_opened &&
        DirGuard::close_count() == before_normal + 1;

    std::printf(
        "Normal path: %s\n",
        normal_passed ? "PASS" : "FAIL"
    );
    // TODO 4: 目录不存在
    std::puts("=== TEST: Open failure ===");

    bool open_failed = false;

    try {
        DirGuard guard("data/w1cpp4_missing_dir");
    }
    catch (const std::system_error& e) {
        open_failed = true;
        std::printf("[EXPECTED ERROR] %s\n", e.what());
    }

    std::printf(
        "Open failure: %s\n",
        open_failed ? "PASS" : "FAIL"
    );
    // TODO 5: 异常退出
    // TODO 5: 异常退出
std::puts("\n=== TEST: Exception path ===");

int before_exception = DirGuard::close_count();
bool exception_caught = false;

try {
    DirGuard guard("data/");

    std::puts("Throwing exception...");

    throw std::logic_error("simulated directory exception");
}
catch (const std::logic_error& e) {
    exception_caught = true;

    std::printf("[CAUGHT] %s\n", e.what());
}
catch (const std::exception& e) {
    std::fprintf(stderr, "[UNEXPECTED ERROR] %s\n", e.what());
}

bool exception_passed =
    exception_caught &&
    DirGuard::close_count() == before_exception + 1;

std::printf(
    "Exception path: %s\n",
    exception_passed ? "PASS" : "FAIL"
);
    // TODO 6: 验证关闭次数
 // TODO 6: 验证关闭次数

bool close_count_passed =
    DirGuard::close_count() == 2;

bool close_status_passed =
    DirGuard::close_fail_count() == 0;

bool all_passed =
    normal_passed &&
    open_failed &&
    exception_passed &&
    close_count_passed &&
    close_status_passed;

std::printf(
    "\nTotal close count: %d\n",
    DirGuard::close_count()
);
std::printf(
    "Close failure count: %d\n",
    DirGuard::close_fail_count()
);
std::printf(
    "=== FINAL RESULT: %s ===\n",
    all_passed ? "PASS" : "FAIL"
);

return all_passed ? 0 : 1;
}

#include <cstdio>
#include <memory>
#include <string>
#include <utility>

// RAII：对象构造时获取资源，析构时自动释放。
// 这个类封装 C 语言的 FILE*，离开作用域时自动 fclose。
class FileGuard {
public:
    explicit FileGuard(const char* path, const char* mode)
        : fp_(std::fopen(path, mode)) {}

    ~FileGuard() {
        if (fp_) std::fclose(fp_);
    }

    // 禁止拷贝（同一个文件句柄不能被两个对象关闭）
    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;

    // 允许移动（所有权转移）
    FileGuard(FileGuard&& other) noexcept
        : fp_(std::exchange(other.fp_, nullptr)) {}

    FileGuard& operator=(FileGuard&& other) noexcept {
        if (this != &other) {
            if (fp_) std::fclose(fp_);
            fp_ = std::exchange(other.fp_, nullptr);
        }
        return *this;
    }

    FILE* get() const { return fp_; }

private:
    FILE* fp_;
};

int main() {
    // 智能指针：离开作用域自动 delete
    auto text = std::make_unique<std::string>("hello RAII");
    std::printf("%s\n", text->c_str());

    FileGuard f("test.txt", "w");
    if (!f.get()) {
        std::puts("open failed");
        return 1;
    }
    std::fputs("written by FileGuard\n", f.get());
    return 0;  // f 在这里自动关闭文件
}
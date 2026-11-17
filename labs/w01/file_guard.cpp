#include <cerrno>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <system_error>

class FileGuard {
private:
    std::FILE* file_ = nullptr;

    inline static int close_count_ = 0;
    
    inline static int close_fail_count_ = 0;
public:
    static int close_fail_count() noexcept{
	return close_fail_count_;
    }
    explicit FileGuard(const char* path) {
        // TODO 1:
        // 使用 fopen(path, "w") 打开文件
        // 失败时抛出异常
	file_= std::fopen(path,"w");
	if(file_ == nullptr){
	    const int error = errno;
	    throw std::system_error(
                error,
		std::generic_category(),
		std::string("fopen failed: ") + path
	    );
    	}
	std::printf("[OPEN] %s\n",path);
    }

    ~FileGuard() noexcept {
        // TODO 2:
        // 如果 file_ 非空：
        // 关闭文件并记录关闭次数
        if (file_ != nullptr){
	    const int result = std::fclose(file_);
	    file_ = nullptr;
	    ++close_count_;

	    if (result != 0) {
    		++close_fail_count_;
	    }	
	    std::printf(
		"[CLOSE] count=%d,status=%s\n",
		close_count_,
		result == 0? "ok":"failed"
	    );
	}
    }

    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;

    std::FILE* get() const noexcept {
        return file_;
    }

    static int close_count() noexcept {
        return close_count_;
    }
};

int main() {
    // TODO 3: 正常作用域退出测试
    std::puts("=== TEST 1: Normal path ===");
    int before = FileGuard::close_count();
    bool write_ok = false;

    try {
	{
	    FileGuard guard("data/guard.txt");
	    write_ok = std::fputs("Hello RAII\n",guard.get()) >=0;
	}
    }
    catch (const std::exception& e){
	std::fprintf(stderr,"[ERROR] %s\n", e.what());
    }
    bool normal_passed = write_ok && FileGuard::close_count() == before + 1;
    std::printf("Normal path: %s\n",normal_passed ? "PASS":"FAIL");
    // TODO 4: 文件打开失败测试
    std::puts("\n=== TEST 2: Open failure ===");
    int before_failure = FileGuard::close_count();
    bool open_failed = false;

    try{
	FileGuard guard("data/w1cpp4_missing_dir/guard.txt");
    }
    catch (const std::system_error& e){
	open_failed = true;
	std::printf("[EXPECTED ERROR] %s\n",e.what());
    }
    bool failure_passed = open_failed && FileGuard::close_count() == before_failure;
    std::printf("Open failure: %s\n",failure_passed ? "PASS" : "FAIL"
    );
    // TODO 5: 中途抛异常测试
    std::puts("\n=== TEST 3: Exception path ===");
    int before_exception = FileGuard::close_count();
    bool exception_caught = false;
    try{
	FileGuard guard("data/guard.txt");
	std::puts("Throwing exception...");
	throw std::runtime_error("simulated exception");
    }
    catch(const std::runtime_error& e){
	exception_caught = true;
	std::printf("[CAUGHT] %s\n",e.what());
    }

    bool exception_passed = exception_caught && FileGuard::close_count() == before_exception + 1;
    std::printf("Exception path: %s\n", exception_passed ? "PASS" :"FAIL");
    // TODO 6: 验证每条路径的关闭次数
    bool close_count_passed = FileGuard::close_count() == 2;
    bool close_status_passed = FileGuard::close_fail_count() == 0;
    bool all_passed = normal_passed && failure_passed && exception_passed && close_count_passed && close_status_passed;
    std::printf("\nTotal close count: %d\n",FileGuard::close_count());
    std::printf("Close failure count: %d\n",FileGuard::close_fail_count());
    std::printf("=== FINAL RESULT: %s ===\n", all_passed ? "PASS" : "FAIL");
    return all_passed ? 0:1;
}

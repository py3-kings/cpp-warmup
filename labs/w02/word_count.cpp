#include <cctype>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

int main(int argc, char** argv) {

    // TODO 1:
    // 检查 argc 是否等于 2。
    // 参数错误时返回 2。
if (argc != 2) {
    return 2;
}
    // TODO 2:
    // 使用 std::ifstream 打开 argv[1]。
std::ifstream input(argv[1]);

// TODO A：检查 input 是否打开失败
// TODO B：打开失败时返回 3
if(!input){
    return 3;
}

    // TODO 3:
    // 使用 unordered_map<string, int> 保存词频。
    // 从文件逐词读取并累加。
std::unordered_map<std::string, int> counts;

for (std::string word; input >> word;) {

 // TODO A：遍历 word 中的每一个字符
    for (char& ch : word) {

        // TODO B：将 ch 转换为小写字母
	ch = std::tolower(static_cast<unsigned char>(ch));
    }

// TODO：将当前 word 对应的计数增加 1
    ++counts[word];


}

// TODO 4：将 unordered_map 复制到 vector
std::vector<std::pair<std::string, int>> sorted(
    counts.begin(),
    counts.end()
);

// TODO A：使用 std::sort 排序
// 在这里补充代码
std::sort(sorted.begin(),sorted.end());

    // TODO 5：正式输出
for (const auto& [word, count] : sorted) {

    // TODO A：使用 std::cout 输出 word=count
    std::cout<< word <<"="<< count <<'\n';
}

// TODO B：正常结束，返回成功状态码
return 0;
}

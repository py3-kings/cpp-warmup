#include <iostream>
#include <vector>

int main() {
    std::vector<int> values;

    values.reserve(2);

    // 先填满当前容量
    while (values.size() < values.capacity()) {
        values.push_back(10);
    }

    const auto old_capacity = values.capacity();
    auto old_it = values.begin();

    std::cout << "BEFORE_CAPACITY="
              << old_capacity << '\n';

    std::cout << "BEFORE_FIRST="
              << *old_it << '\n';

    // TODO 1:
    // 使用一次 push_back 添加整数 20
values.push_back( 20 );
    // TODO 2:
    // 扩容后重新获取 begin() 迭代器
    // 使用新迭代器读取第一个元素
auto new_it = values.begin();
    std::cout << "AFTER_FIRST="
	      << *new_it << '\n';

    std::cout << "AFTER_CAPACITY="
              << values.capacity() << '\n';

    std::cout << "REALLOCATED="
              << (values.capacity() > old_capacity)
              << '\n';

    return 0;
}

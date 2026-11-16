#include <iostream>

// 实验 1：按值传递
void by_value(int n) {
    // TODO 1: 让 n 加 1
     ++n;
    // TODO 2: 输出函数内部的 n
    std::cout<<"inside="<<n<<'\n';
}

//实验 2：引用传递
void by_ref(int& n){
    //TODO 1：将n加1
    ++n;
    //TODO 2: 输出“ref_inside=" 和 n
    std::cout<<"ref_inside="<<n<<'\n';
}

// 实验3,4：指针传递与空指针检查
bool by_ptr(int* n){
    if(n==nullptr){
	std::cout<<"ptr_null"<<'\n';
        return false;
    }
    ++*n;
    std::cout<<"ptr_inside="<<*n<<'\n';
    return true;
}

//实验5 const引用
void inspect(const int& n){
    std::cout<<"inspect="<<n<<'\n';
}

//实验7：按值传递，通过返回值修改变量
int by_value_result(int n){
    ++n;
    return n;
}

int main() {
    int n = 10;

    // TODO 3: 输出调用前的 n
    std::cout<<"before="<< n << '\n';
    // TODO 4: 调用 by_value()
    by_value(n);
    // TODO 5: 输出调用后的 n
    std::cout<<"after="<< n << '\n';
    // 实验2：引用传递
    // TODO 3: 调用by_ref
    by_ref(n);
    std::cout<<"ref_after="<<n<<'\n';
    bool x = by_ptr(&n);
    std::cout<<"ptr_after="<<n<<'\n';
    std::cout<<std::boolalpha<<"ptr_ok="<<x<<'\n';
    bool null_ok = by_ptr(nullptr);
    std::cout<<std::boolalpha<<"ptr_null_ok="<<null_ok<<'\n';
    std::cout<<"final_n="<<n<<'\n';
    inspect(n);
    std::cout<<"after_inspect="<<n<<'\n';
    int variant = 10;
    std::cout<<"variant_before="<<variant<<'\n';
    variant = by_value_result(variant);
    std::cout<<"variant_after="<<variant<<'\n';
    std::cout<<"original_n="<<n<<'\n';
    return 0;
}

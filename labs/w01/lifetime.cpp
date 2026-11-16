#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>
#include <memory>

struct Trace {
    std::string name;

    explicit Trace(std::string n) : name(std::move(n)) {
        std::cout << "create " << name << '\n';
    }

    ~Trace() {
        std::cout << "destroy " << name << '\n';
    }
};

void run_inner_experiment(){
    Trace inner("inner");
}

void run_three_level_experiment(){
    Trace level1("level1");
	{
	    Trace level2("level2");
	    {
		Trace level3("level3");
	    }
 	}
}

void run_heap_experiment(){
    auto heap_obj = std::make_unique<Trace>("heap");
    std::cout<<"before move: "<<static_cast<bool>(heap_obj)<<'\n';
    auto another = std::move(heap_obj);
    std::cout<<"heap_obj owner: " << static_cast<bool>(heap_obj)<<'\n';
    std::cout<<"another owner: "<<static_cast<bool>(another)<<'\n';
}

void run_heap_exception_experiment(){
    auto heap_obj = std::make_unique<Trace>("heap_exception");
    auto another = std::move(heap_obj);
    throw std::runtime_error("heap exception");
}

int main() {
    Trace outer("outer");

    {
        run_inner_experiment();
	std::cout<<"outer still alive: "<< outer.name <<'\n';
    }
    
    run_three_level_experiment();
    run_heap_experiment();
    try {
	run_heap_exception_experiment();
    }catch (const std::exception& e) {
	std::cout<<"caught "<<e.what()<<'\n';
}
    try{
	Trace try_outer("try_outer");
	
	{
	    Trace try_inner("try_inner");
	    throw std::runtime_error("forced");
	}
    }catch(const std::exception& e){
	std::cout<<"caught "<<e.what()<<'\n';
    }

    return 0;
}

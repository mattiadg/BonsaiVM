#include <iostream>

#include "vm.hpp"
#include "../include/object.hpp"

std::string get_string(Value obj) 
{
    return dynamic_cast<B_String*>(std::get<B_Object*>(obj))->value;
}

B_Allocator::~B_Allocator()
{
    for (auto* obj: memory)
    {
        if (obj != nullptr)
        {
            delete obj;
        }
    }
}

B_Allocator::B_Allocator(B_Allocator && other)
: memory{other.memory}
{
    other.memory.clear();
}

B_Object *B_Allocator::alloc(std::string data)
{
    auto* new_obj = new B_String{data};
    memory.push_back(new_obj);
    return new_obj;
}

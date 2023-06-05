#include <iostream>

#include "vm.hpp"
#include "../include/object.hpp"

std::string get_string(Value obj) 
{
    return dynamic_cast<B_String*>(std::get<B_Object*>(obj))->value;
}

std::vector<Value> get_array(Value obj) 
{
    return dynamic_cast<B_Array*>(std::get<B_Object*>(obj))->values;
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

B_Object *B_Allocator::alloc(Value* first, Value* last)
{
    auto* new_obj = new B_Array{first, last};
    memory.push_back(new_obj);
    return new_obj;
}

std::ostream& operator<<(std::ostream& lhs, Value rhs)
{
    if (std::holds_alternative<int64_t>(rhs))
    {
        return lhs << std::get<int64_t>(rhs);
    } else if (std::holds_alternative<_Float64>(rhs))
    {
        return lhs << std::get<_Float64>(rhs);
    } else
    {
        return lhs << "an object";
    }
}
#include "vm.hpp"
#include "../include/object.hpp"

std::string get_string(Value obj) 
{
    return dynamic_cast<B_String*>(std::get<B_Object*>(obj))->value;
}
#include <iostream>

#include "vm.hpp"
#include "../include/object.hpp"

B_HashPair::B_HashPair(Value k, Value v) : key(k), value(v) 
{
    if (std::holds_alternative<int64_t>(k))
    {
        return;
    } if (std::holds_alternative<_Float64>(k))
    {
        return;
    } if (std::holds_alternative<B_Object*>(k))
    {
        if (dynamic_cast<B_String*>(std::get<B_Object*>(k)))
        {
            return;
        } else
        {
            throw invalid_value("Found not hashable value as key");
        }
    } else 
    {
        throw not_implemented("found a type not yet implemented");
    }
}

B_HashMap::B_HashMap(B_HashPair *first, B_HashPair *end)
{
    for (auto* it = first; it < end; ++it)
    {
        std::cout << "key: " << it->key << ", value: " << *it << "\n";
        values.insert_or_assign(it->key, *it);
    }
    set_not_used();
}

size_t VHash::operator()(const Value& v) const
{
    if (auto i64 = std::get_if<int64_t>(&v))
    {
        return std::hash<long>{}(*i64);
    } else if (auto f64 = std::get_if<_Float64>(&v))
    {
        return std::hash<_Float64>{}(*f64);
    } else if (auto* o_str = dynamic_cast<B_String*>(std::get<B_Object*>(v)))
    {
        return std::hash<std::string>{}(o_str->value);
    } 
    else
    {
        return 235482375;
    }
}

bool VEqual::operator()(const Value &l, const Value &r) const
{
    if (std::holds_alternative<int64_t>(l) && std::holds_alternative<int64_t>(r))
    {
        return l == r;
    } else if (std::holds_alternative<_Float64>(l) && std::holds_alternative<_Float64>(r))
    {
        return l == r;
    } else if (auto* lstr = dynamic_cast<B_String*>(std::get<B_Object*>(l)))
    {
        if (auto* rstr = dynamic_cast<B_String*>(std::get<B_Object*>(r)))
        {
            return lstr->value == rstr->value;
        }
    }
    return &l == &r;
}

std::string get_string(Value obj) 
{
    return dynamic_cast<B_String*>(std::get<B_Object*>(obj))->value;
}

std::vector<Value> get_array(Value obj) 
{
    return dynamic_cast<B_Array*>(std::get<B_Object*>(obj))->values;
}

std::unordered_map<Value, B_HashPair, VHash, VEqual> get_hash(Value obj) 
{
    return dynamic_cast<B_HashMap*>(std::get<B_Object*>(obj))->values;
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

B_Object *B_Allocator::alloc(B_HashPair* first, B_HashPair* last)
{
    auto* new_obj = new B_HashMap{first, last};
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
    } else if (auto* obj = std::get<B_Object*>(rhs))
    {
        if (auto str = dynamic_cast<B_String*>(obj))
        {
            return lhs << str->value;
        } else 
        {
        return lhs << "an object";
        }
    }
}

std::ostream& operator<<(std::ostream& lhs, B_HashPair rhs)
{
    return lhs << "key: " << rhs.key << ", value: " << rhs.value;
}
/**
 * Define the object model of BonsaiVM.
 * 
 * There are built-in types that go into the stack and other objects that go into the heap
*/
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <memory>
#include <string>
#include <variant>


class B_Object
{
public:
    virtual ~B_Object() {};
};

class B_String: public B_Object
{
public:
    B_String(std::string s): value{s} {};
    virtual ~B_String() override = default;

    std::string value;
};

typedef std::variant<int64_t, _Float64, bool, B_Object*> Value;

std::string get_string(Value obj);

#endif
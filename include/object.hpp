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
    bool used() {return _used;}
    void set_used() {_used = true;}
    void set_not_used() {_used = false;}
 
private:
    bool _used;
};

using Value = std::variant<int64_t, _Float64, bool, B_Object*>;

class B_String: public B_Object
{
public:
    B_String(std::string s): value(s) {set_not_used();};
    virtual ~B_String() override {};

    std::string value;
};

class B_Array: public B_Object
{
    public:
    B_Array(Value* first, Value* last) : values(first, last)  {};
    virtual ~B_Array() override {};

    std::vector<Value> values;
};

class B_Allocator {
    public:
    B_Allocator() : memory() {}
    ~B_Allocator();

    B_Allocator(const B_Allocator&) = delete;
    B_Allocator(B_Allocator&&);

    B_Object* alloc(std::string data);
    B_Object* alloc(Value* first, Value* last);

    std::vector<B_Object*> memory;
};

std::string get_string(Value obj);
std::vector<Value> get_array(Value obj);

std::ostream& operator<<(std::ostream& lhs, Value rhs);

#endif

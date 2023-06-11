/**
 * Define the object model of BonsaiVM.
 * 
 * There are built-in types that go into the stack and other objects that go into the heap
*/
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
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

struct VHash {
    size_t operator()(const Value& v) const;
};

struct VEqual {
    bool operator()(const Value& l, const Value &r) const;
};

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
    B_Array(Value* first, Value* last) : values(first, last) {set_not_used();};
    virtual ~B_Array() override {};

    std::vector<Value> values;
};

class B_HashPair
{
    public:
    B_HashPair() : key(nullptr), value(nullptr) {};
    B_HashPair(Value k, Value v);

    Value key;
    Value value;
};

class B_HashMap: public B_Object
{
    public:
    B_HashMap(B_HashPair* first, B_HashPair* end);
    virtual ~B_HashMap() override {};

    std::unordered_map<Value, B_HashPair, VHash, VEqual> values;
};

class B_Allocator {
    public:
    B_Allocator() : memory() {}
    ~B_Allocator();

    B_Allocator(const B_Allocator&) = delete;
    B_Allocator(B_Allocator&&);

    B_Object* alloc(std::string data);
    B_Object* alloc(Value* first, Value* last);
    B_Object* alloc(B_HashPair* first, B_HashPair* last);

    std::vector<B_Object*> memory;
};

std::string get_string(Value obj);
std::vector<Value> get_array(Value obj);
std::unordered_map<Value, B_HashPair, VHash, VEqual> get_hash(Value obj);

std::ostream& operator<<(std::ostream& lhs, Value rhs);
std::ostream& operator<<(std::ostream& lhs, B_HashPair rhs);

#endif

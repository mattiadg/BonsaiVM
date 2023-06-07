#ifndef VM_HPP
#define VM_HPP

#include <array>
#include <exception>
#include <iterator>
#include <memory>
#include <vector>

#include "../include/code.hpp"
#include "../include/object.hpp"


struct ByteCode 
{
    std::vector<unsigned char> instructions;
    std::vector<Value> constants;
};

class B_GC
{
  public:
  B_GC(std::shared_ptr<B_Allocator> alloc);
  void mark_and_sweep(std::array<Value, 256> stack, int64_t sp, std::vector<Value> constants, std::vector<Value> globals, Value last_popped);

  std::shared_ptr<B_Allocator> allocator;
};

struct VM
{
    // Memory areas
    std::array<Value, 256> stack;
    std::vector<Value> constants;
    std::vector<unsigned char> instructions;
    std::vector<Value> globals;
    Value last_popped;

    // Registers
    int64_t ip;
    int64_t sp;

    // Constants
    Value trueValue {true};
    Value falseValue {false};

    // Allocator
    B_GC bgc;

    VM(std::shared_ptr<B_Allocator> alloc = std::make_shared<B_Allocator>());
    VM(const ByteCode&, std::shared_ptr<B_Allocator> alloc = std::make_shared<B_Allocator>());

    void push(Value);
    Value pop();

    void run();

    void executeBinaryOp(Operation op);
    void executeBinaryComparison(Operation op);
    void run_gc();
};

class full_stack_exception: public std::exception
{
  virtual const char* what() const throw()
  {
    return "The stack reached its full capacity";
  }
};

class empty_stack_exception: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Not enough items to remove from the stack";
  }
};

class global_index_too_large_exception: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Tried to write to a too large index for the global variables";
  }
};

class invalid_instruction
{
  std::string message;

  public:
  invalid_instruction(std::string msg) : message{msg} {}; 
  std::string what() {return message;}
};

template <typename InputIt>
requires std::input_iterator<InputIt>
void mark_container(const InputIt it_b, const InputIt it_e, std::vector<B_Object*>& mark_stack);

#endif
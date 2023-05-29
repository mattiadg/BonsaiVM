#ifndef VM_HPP
#define VM_HPP

#include <array>
#include <exception>
#include <memory>
#include <vector>

#include "../include/code.hpp"
#include "../include/object.hpp"


struct ByteCode 
{
    std::vector<unsigned char> instructions;
    std::vector<Value> constants;
};

struct VM
{
    // memory areas
    std::array<Value, 256> stack;
    std::vector<Value> constants;
    std::vector<unsigned char> instructions;
    std::vector<Value> globals;

    // registers
    int64_t ip;
    int64_t sp;

    // Constants
    Value trueValue {true};
    Value falseValue {false};

    VM();
    VM(ByteCode);

    void push(Value);
    Value pop();

    void run();

    void executeBinaryOp(Operation op);
    void executeBinaryComparison(Operation op);
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
    return "No items to remove from the stack";
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

#endif
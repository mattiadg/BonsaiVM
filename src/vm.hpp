#ifndef VM_HPP
#define VM_HPP

#include <array>
#include <exception>
#include <memory>
#include <variant>

#include "../include/code.hpp"


typedef std::variant<int64_t, _Float64, bool> Value;

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

class invalid_instruction
{
  std::string message;

  public:
  invalid_instruction(std::string msg) : message{msg} {}; 
  std::string what() {return message;}
};

#endif
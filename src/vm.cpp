#include <iostream>

#include "vm.hpp"

VM::VM() 
: stack(std::array<Value, 256>()), constants(std::vector<Value>()), 
instructions(std::vector<unsigned char>()), ip(0), sp(0) 
{
    
}

VM::VM(ByteCode bc)
: stack(std::array<Value, 256>()), constants(bc.constants), instructions(bc.instructions), ip(0), sp(0) 
{

}

void VM::push(Value v)
{
    if(sp >= static_cast<int64_t>(stack.size()) - 1)
    {
        throw full_stack_exception();
    }
    stack[sp++] = v;
}

Value VM::pop()
{
    if(sp < 1)
    {
        throw empty_stack_exception();
    }
    auto v = stack[--sp];
    return v;
}

void VM::run()
{
    auto offset = 0;
    while(ip < static_cast<int64_t>(instructions.size()))
    {
        ip += offset;
        std::cout << "ip: " << ip << std::endl;
        auto op = instructions[ip];
        auto byte_count = 0;
        auto def = opDefinitions[op];

        ++ip;
        for (auto i = 0; i < def.numOperands; ++i)
        {
            byte_count += def.operandsWidth[i];
        }

        switch (op)
        {
            case OpConstantInt:
            {
                auto operand_ref = ReadInt16({instructions[ip], instructions[ip+1]});
                push(constants[operand_ref]);
                break;
            }
            case OpTrue:
            {
                push(trueValue);
                break;
            }
            case OpFalse:
            {
                push(falseValue);
                break;
            }
            case OpAddInt:
            case OpSubInt:
            case OpMulInt:
            case OpDivInt:
            {
                executeBinaryOp(static_cast<Operation>(op));
                break;
            }
            case OpGreaterThan:
            case OpEqual:
            case OpGreaterEqual:
            {
                executeBinaryComparison(static_cast<Operation>(op));
                break;
            }
            case OpPop:
                pop();
                break;
            case OpBang:
            {
                auto value = pop();
                if(value == trueValue)
                {
                    push(falseValue);
                } else 
                {
                    push(trueValue);
                }
                break;
            }
            case OpUnaryMinus:
            {
                auto value = std::get<int64_t>(pop());
                push(Value{-value});
                break;
            }
            case OpJumpFalse:
            {
                auto top = pop();
                if (top == falseValue) 
                {
                    int64_t jmp_offset = static_cast<int64_t>(ReadInt16({instructions[ip], instructions[ip+1]}));
                    ip += jmp_offset - 1 - byte_count;
                }
                break;
            }
            case OpJump:
            {
                int64_t jmp_offset = static_cast<int64_t>(ReadInt16({instructions[ip], instructions[ip+1]}));
                ip += jmp_offset - 1 - byte_count;
            }
            default:
                break;
            }
        ip += byte_count;
    }
}

void VM::executeBinaryOp(Operation op)
{
    auto operand_right = std::get<int64_t>(pop());
    auto operand_left = std::get<int64_t>(pop());
    Value value;
    switch(op)
    {
        case OpAddInt:
            value = operand_left + operand_right;
            break;
        case OpSubInt:
            value = operand_left - operand_right;
            break;
        case OpMulInt:
            value = operand_left * operand_right;
            break;
        case OpDivInt:
            value = operand_left / operand_right;
            break;
        default:
            auto def = opDefinitions[op];
            throw invalid_instruction("Found instruction " + def.opName);
    }
    push(value);
}

void VM::executeBinaryComparison(Operation op)
{
    auto operand_right = std::get<int64_t>(pop());
    auto operand_left = std::get<int64_t>(pop());
    Value value;
    switch(op)
    {
        case OpGreaterThan:
            value = operand_left > operand_right;
            break;
        case OpEqual:
            value = operand_left == operand_right;
            break;
        case OpGreaterEqual:
            value = operand_left >= operand_right;
            break;
        case OpDivInt:
            value = operand_left / operand_right;
            break;
        default:
            auto def = opDefinitions[op];
            throw invalid_instruction("Found instruction " + def.opName);
    }
    push(value);
}

#include <iostream>

#include "vm.hpp"

VM::VM(B_Allocator&& alloc) 
: stack(std::array<Value, 256>()), constants(std::vector<Value>()), 
instructions(std::vector<unsigned char>()), ip(0), sp(0), bgc(std::move(alloc))
{
}

VM::VM(const ByteCode& bc, B_Allocator&& alloc)
: stack(std::array<Value, 256>()), constants(bc.constants), instructions(bc.instructions), ip(0), sp(0), bgc(std::move(alloc))
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
            case OpConstant:
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
            case OpAdd:
            case OpSub:
            case OpMul:
            case OpDiv:
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
                break;
            }
            case OpWriteGlobal:
            {
                auto top = pop();
                int64_t idx = static_cast<int64_t>(ReadInt16({instructions[ip], instructions[ip+1]}));
                auto cmp = idx <=> static_cast<int64_t>(globals.size());
                if(cmp < 0)
                {
                    globals[idx] = top;
                } else if (cmp == 0)
                {
                    globals.push_back(top);
                } else
                {
                    throw global_index_too_large_exception();
                }
                break;
            }
            case OpReadGlobal:
            {
                int64_t idx = static_cast<int64_t>(ReadInt16({instructions[ip], instructions[ip+1]}));
                if(idx < static_cast<int64_t>(globals.size()))
                {
                    push(globals[idx]);
                } else
                {
                    throw global_index_too_large_exception();
                }
                break;
            }
            default:
                break;
            }
        ip += byte_count;
    }
}

void VM::executeBinaryOp(Operation op)
{
    Value operand_right_ = pop();
    Value operand_left_ = pop();
    Value value;
    switch(op)
    {
        case OpAdd:
        {
            if (std::holds_alternative<int64_t>(operand_left_) && std::holds_alternative<int64_t>(operand_right_))
            {
                auto operand_left = std::get<int64_t>(operand_left_);
                auto operand_right = std::get<int64_t>(operand_right_);
                value = operand_left + operand_right;
            } else if (std::holds_alternative<B_Object*>(operand_left_) && std::holds_alternative<B_Object*>(operand_right_))
            {
                auto operand_left = std::get<B_Object*>(operand_left_);
                auto operand_right = std::get<B_Object*>(operand_right_);
                value = Value(bgc.allocator.alloc(dynamic_cast<B_String*>(operand_left)->value + dynamic_cast<B_String*>(operand_right)->value));
            }
            break;
        }
        case OpSub:
        {
            auto operand_left = std::get<int64_t>(operand_left_);
            auto operand_right = std::get<int64_t>(operand_right_);
            value = operand_left - operand_right;
            break;
        }
        case OpMul:
        {
            auto operand_left = std::get<int64_t>(operand_left_);
            auto operand_right = std::get<int64_t>(operand_right_);
            value = operand_left * operand_right;
            break;
        }
        case OpDiv:
        {
            auto operand_left = std::get<int64_t>(operand_left_);
            auto operand_right = std::get<int64_t>(operand_right_);
            value = operand_left / operand_right;
            break;
        }
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
        case OpDiv:
            value = operand_left / operand_right;
            break;
        default:
            auto def = opDefinitions[op];
            throw invalid_instruction("Found instruction " + def.opName);
    }
    push(value);
}

void VM::run_gc()
{
    bgc.mark_and_sweep(stack, sp, constants, globals);
}

B_GC::B_GC(B_Allocator&& alloc)
: allocator(alloc)
{
}

void B_GC::mark_and_sweep(std::array<Value, 256> stack, int64_t sp, std::vector<Value> constants, std::vector<Value> globals)
{
    std::vector<B_Object*> mark_stack = {};

    // seed stack
    for (auto i = 0; i < sp; ++i)
    {
        if (std::holds_alternative<B_Object*>(stack[i]))
        {
            auto* obj = std::get<B_Object*>(stack[i]);
            obj->set_used();
            mark_stack.push_back(obj);
        }
    }
    // seed constants
    for (uint i = 0; i < constants.size(); ++i)
    {
        if (std::holds_alternative<B_Object*>(constants[i]))
        {
            auto* obj = std::get<B_Object*>(constants[i]);
            obj->set_used();
            mark_stack.push_back(obj);
        }
    }
    // seed globals
    for (uint i = 0; i < globals.size(); ++i)
    {
        if (std::holds_alternative<B_Object*>(globals[i]))
        {
            auto* obj = std::get<B_Object*>(globals[i]);
            obj->set_used();
            mark_stack.push_back(obj);
        }
    }
    // should now cycle inside the objects, but we still don't have objects containing other objects

    // sweep the others
    for (std::vector<B_Object*>::iterator obj = allocator.memory.begin(); obj != allocator.memory.end();)
    {
        if (!(*obj)->used())
        {
            delete *obj;
            obj = allocator.memory.erase(obj);
        } else 
        {
            (*obj)->set_not_used();
            ++obj;
        }
    }
}
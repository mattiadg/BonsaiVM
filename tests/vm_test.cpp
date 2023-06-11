#include <iostream>
#include <vector>
#include <gtest/gtest.h>

#include "../src/vm.hpp"
#include "../include/object.hpp"

std::vector<unsigned char> make_instructions(std::vector<std::vector<unsigned char>>);


TEST(VMTest, InitAssertions)
{
    ByteCode bc {make(OpConstant, 0)};
    auto testVM = VM(bc);
    EXPECT_EQ(testVM.sp, 0);
    EXPECT_EQ(testVM.ip, 0);
}

TEST(OpTest, OpConstantIntAssertions)
{
    ByteCode bc {make(OpConstant, 0), std::vector{Value{3}}};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[0]), 3);
}

TEST(OpTest, OpConstantIntCorrectValueAssertions)
{
    auto instrs = make_instructions(std::vector{make(OpConstant, 0), make(OpConstant, 1)});
    auto constants = std::vector({Value{3}, Value{2}});
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    EXPECT_EQ(std::get<int64_t>(testVM.constants[0]), 3);
    EXPECT_EQ(std::get<int64_t>(testVM.constants[1]), 2);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[testVM.sp-1]), 2);
}

TEST(OpTest, OpTrueAssertions)
{
    auto bc = ByteCode{make(OpTrue), std::vector<Value>()};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[testVM.sp-1], testVM.trueValue);
    EXPECT_EQ(testVM.stack[testVM.sp-1], testVM.trueValue);
}

TEST(OpTest, OpFalseAssertions)
{
    auto bc = ByteCode{make(OpFalse), std::vector<Value>()};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[testVM.sp-1], testVM.falseValue);
    EXPECT_EQ(testVM.stack[testVM.sp-1], testVM.falseValue);
}

TEST(OpTest, OpAddIntAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpAdd),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{3}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[testVM.sp]), 5);
    EXPECT_EQ(testVM.sp, 0);
}

TEST(OpTest, OpSubIntAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpSub),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{3}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[testVM.sp]), 1);
    EXPECT_EQ(testVM.sp, 0);
}

TEST(OpTest, OpMulIntAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpMul),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{4}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[testVM.sp]), 8);
    EXPECT_EQ(testVM.sp, 0);
}

TEST(OpTest, OpDivIntAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpDiv),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{7}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[testVM.sp]), 3);
    EXPECT_EQ(testVM.sp, 0);
}

TEST(OpTest, OpGreaterThanTrueAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpGreaterThan),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{7}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[testVM.sp], testVM.trueValue);
}

TEST(OpTest, OpGreaterThanFalseAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpGreaterThan),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{2}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[testVM.sp], testVM.falseValue);
}

TEST(OpTest, OpEqualTrueAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpEqual),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{2}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[testVM.sp], testVM.trueValue);
}

TEST(OpTest, OpEqualFalseAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpEqual),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{7}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[testVM.sp], testVM.falseValue);
}

TEST(OpTest, OpGreaterEqualGreaterAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpGreaterEqual),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{7}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[testVM.sp], testVM.trueValue);
}

TEST(OpTest, OpGreaterEqualEqualAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpGreaterEqual),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{2}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[testVM.sp], testVM.trueValue);
}

TEST(OpTest, OpGreaterEqualFalseAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpGreaterEqual),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{2}, Value{7}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[testVM.sp], testVM.falseValue);
}

TEST(OpTest, OpJumpFalseWhenFalseAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpFalse),
                make(OpJumpFalse, 5),
                make(OpAdd),
                make(OpTrue),
                make(OpSub),
            }
        ));
    auto constants = std::vector{Value{5}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[0]), 3);
}

TEST(OpTest, OpJumpFalseWhenTrueAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpTrue),
                make(OpJumpFalse, 5),
                make(OpAdd),
                make(OpPop),
                make(OpJump, 4),
                make(OpSub),
            }
        ));
    auto constants = std::vector{Value{5}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[testVM.sp]), 7);
}

TEST(OpTest, OpBangTrueAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpTrue),
                make(OpBang),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{5}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[testVM.sp], testVM.falseValue);
}

TEST(OpTest, OpBangFalseAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpFalse),
                make(OpBang),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{5}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[testVM.sp], testVM.trueValue);
}

TEST(OpTest, OpUnaryMinusAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpUnaryMinus),
                make(OpPop),
            }
        ));
    auto constants = std::vector<Value>{5, 2};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[testVM.sp], Value{-5});
}

TEST(OpTest, OpWriteGlobalAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpWriteGlobal, 0),
            }
        ));
    auto constants = std::vector{Value{5}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.globals[0], Value{5});
}

TEST(OpTest, OpWriteGlobalSameIndexAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpWriteGlobal, 0),
                make(OpWriteGlobal, 0),
            }
        ));
    auto constants = std::vector<Value>{5, 1.0};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.globals[0], Value{5});
}

TEST(OpTest, OpReadGlobalAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpWriteGlobal, 0),
                make(OpConstant, 1),
                make(OpReadGlobal, 0),
                make(OpAdd),
                make(OpPop),
            }
        ));
    auto constants = std::vector<Value>{5, 1};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.globals[0], Value{5});
    EXPECT_EQ(testVM.stack[testVM.sp], Value{6});
}

TEST(OpTest, OpConstantStringAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
            }
        )
    );
    B_Allocator allocator {};
    ByteCode bc {instrs, std::vector<Value>{allocator.alloc("test string")}};
    auto testVM = VM(bc);
    EXPECT_EQ(testVM.sp, 0);
    testVM.run();
    auto string_value = get_string(testVM.stack[testVM.sp - 1]);
    EXPECT_EQ(string_value, "test string");
}

TEST(OpTest, OpAddStringAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpAdd),
            }
        ));
    B_Allocator allocator {};
    auto constants = std::vector<Value>{allocator.alloc("string1"), allocator.alloc("string2")};
    ByteCode bc {instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    auto string_value = get_string(testVM.stack[testVM.sp - 1]);
    EXPECT_EQ(string_value, "string1string2");
}

TEST(OpTest, OpArrayAssertions)
{
    B_Allocator allocator {};
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpConstant, 2),
                make(OpArray, 3),
            }
        ));
    auto constants = std::vector<Value>{1, 3.5, allocator.alloc("string1")};
    ByteCode bc {instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    auto array_value = get_array(testVM.stack[testVM.sp - 1]);
    EXPECT_EQ(std::get<int64_t>(array_value[0]), 1);
    EXPECT_EQ(std::get<_Float64>(array_value[1]), 3.5);
    EXPECT_EQ(get_string(array_value[2]), "string1");
}

TEST(OpTest, OpHashAssertions)
{
    std::shared_ptr<B_Allocator> allocator = std::make_shared<B_Allocator>();
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpAdd),
                make(OpConstant, 2),
                make(OpConstant, 3),
                make(OpAdd),
                make(OpConstant, 4),
                make(OpConstant, 5),
                make(OpAdd),
                make(OpConstant, 6),
                make(OpConstant, 7),
                make(OpAdd),
                make(OpHash, 4),
            }
        ));
    auto constants = std::vector<Value>{allocator->alloc("str1"), allocator->alloc("-key"), 0, 1, allocator->alloc("key1"), allocator->alloc("-str"), 2, 3,};
    ByteCode bc {instrs, constants};
    auto testVM = VM(bc, allocator);
    testVM.run();
    auto hash_value = get_hash(testVM.stack[testVM.sp - 1]);

    B_String k1{"str1-key"};
    B_String k2{"key1-str"};
    EXPECT_EQ(get_string(hash_value[&k1].key), "str1-key");
    EXPECT_EQ(std::get<int64_t>(hash_value[&k1].value), 1);
    EXPECT_EQ(get_string(hash_value[&k2].key), "key1-str");
    EXPECT_EQ(std::get<int64_t>(hash_value[&k2].value), 5);
}

TEST(GcTest, MarkAndSweepAssertions)
{
    std::shared_ptr<B_Allocator> allocator = std::make_shared<B_Allocator>();
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 0),
                make(OpConstant, 0),
                make(OpConstant, 0),
                make(OpAdd),
                make(OpAdd),
                make(OpAdd),
            }
        ));
    auto constants = std::vector<Value>{allocator->alloc("string1")};
    ByteCode bc {instrs, constants};
    auto testVM = VM(bc, allocator);
    testVM.run();
    testVM.run_gc();
    EXPECT_EQ(testVM.sp, 1);
    EXPECT_EQ(testVM.bgc.allocator->memory.size(), 2);
    EXPECT_EQ(dynamic_cast<B_String*>(allocator->memory[0]), dynamic_cast<B_String*>(std::get<B_Object*>(testVM.constants[0])));
    EXPECT_EQ(dynamic_cast<B_String*>(allocator->memory[1])->value, "string1string1string1string1");
}

TEST(GcTest, MarkAndSweepRespectsArrayAssertions)
{
    std::shared_ptr<B_Allocator> allocator = std::make_shared<B_Allocator>();
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpConstant, 2),
                make(OpConstant, 3),
                make(OpAdd),
                make(OpArray, 3),
            }
        ));
    auto constants = std::vector<Value>{1, 2.0, allocator->alloc("string1"), allocator->alloc("string2")};
    ByteCode bc {instrs, constants};
    auto testVM = VM(bc, allocator);
    testVM.run();
    EXPECT_EQ(testVM.sp, 1);
    EXPECT_EQ(dynamic_cast<B_String*>(allocator->memory[0])->value, "string1");
    EXPECT_EQ(dynamic_cast<B_String*>(allocator->memory[1])->value, "string2");
    EXPECT_EQ(dynamic_cast<B_String*>(allocator->memory[2])->value, "string1string2");
    EXPECT_EQ(get_string(dynamic_cast<B_Array*>(allocator->memory[3])->values[2]), "string1string2");
    EXPECT_EQ(testVM.bgc.allocator->memory.size(), 4);
}

TEST(GcTest, MarkAndSweepEmptyArrayAssertions)
{
    std::shared_ptr<B_Allocator> allocator = std::make_shared<B_Allocator>();
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpConstant, 2),
                make(OpConstant, 3),
                make(OpAdd),
                make(OpArray, 3),
                make(OpPop),
                make(OpConstant, 0),
                make(OpPop),
            }
        ));
    auto constants = std::vector<Value>{1, 2.0, allocator->alloc("string1"), allocator->alloc("string2")};
    ByteCode bc {instrs, constants};
    auto testVM = VM(bc, allocator);
    testVM.run();
    EXPECT_EQ(dynamic_cast<B_String*>(allocator->memory[0])->value, "string1");
    EXPECT_EQ(dynamic_cast<B_String*>(allocator->memory[1])->value, "string2");
    EXPECT_EQ(testVM.bgc.allocator->memory.size(), 2);
}

TEST(OpTest, MarkAndSweepEmptyHashAssertions)
{
    std::shared_ptr<B_Allocator> allocator = std::make_shared<B_Allocator>();
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstant, 0),
                make(OpConstant, 1),
                make(OpAdd),
                make(OpConstant, 2),
                make(OpConstant, 3),
                make(OpAdd),
                make(OpConstant, 4),
                make(OpConstant, 5),
                make(OpAdd),
                make(OpConstant, 6),
                make(OpConstant, 7),
                make(OpAdd),
                make(OpHash, 4),
                make(OpPop),
            }
        ));
    auto constants = std::vector<Value>{allocator->alloc("str1"), allocator->alloc("-key"), 0, 1, allocator->alloc("key1"), allocator->alloc("-str"), 2, 3,};
    ByteCode bc {instrs, constants};
    auto testVM = VM(bc, allocator);
    testVM.run();
    EXPECT_EQ(testVM.bgc.allocator->memory.size(), 4);
}


std::vector<unsigned char> make_instructions(std::vector<std::vector<unsigned char>> instrs) 
{
    std::vector<unsigned char> instructions;
    for (auto v: instrs)
    {
        instructions.insert(instructions.end(), v.begin(), v.end());
    }
    return instructions;
}
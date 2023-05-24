#include <iostream>
#include <vector>
#include <gtest/gtest.h>

#include "../src/vm.hpp"

std::vector<unsigned char> make_instructions(std::vector<std::vector<unsigned char>>);


TEST(VMTest, InitAssertions)
{
    ByteCode bc {make(OpConstantInt, 0)};
    auto testVM = VM(bc);
    EXPECT_EQ(testVM.sp, 0);
    EXPECT_EQ(testVM.ip, 0);
}

TEST(OpTest, OpConstantIntAssertions)
{
    ByteCode bc {make(OpConstantInt, 0), std::vector{Value{3}}};
    auto testVM = VM(bc);
    EXPECT_EQ(testVM.sp, 0);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[0]), 3);
}

TEST(OpTest, OpConstantIntCorrectValueAssertions)
{
    auto instrs = make_instructions(std::vector{make(OpConstantInt, 0), make(OpConstantInt, 1)});
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
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpAddInt),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{3}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[0]), 5);
    EXPECT_EQ(testVM.sp, 0);
}

TEST(OpTest, OpSubIntAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpSubInt),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{3}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[0]), 1);
    EXPECT_EQ(testVM.sp, 0);
}

TEST(OpTest, OpMulIntAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpMulInt),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{4}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[0]), 8);
    EXPECT_EQ(testVM.sp, 0);
}

TEST(OpTest, OpDivIntAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpDivInt),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{7}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[0]), 3);
    EXPECT_EQ(testVM.sp, 0);
}

TEST(OpTest, OpGreaterThanTrueAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpGreaterThan),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{7}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[0], testVM.trueValue);
}

TEST(OpTest, OpGreaterThanFalseAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpGreaterThan),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{2}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[0], testVM.falseValue);
}

TEST(OpTest, OpEqualTrueAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpEqual),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{2}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[0], testVM.trueValue);
}

TEST(OpTest, OpEqualFalseAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpEqual),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{7}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[0], testVM.falseValue);
}

TEST(OpTest, OpGreaterEqualGreaterAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpGreaterEqual),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{7}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[0], testVM.trueValue);
}

TEST(OpTest, OpGreaterEqualEqualAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpGreaterEqual),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{2}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[0], testVM.trueValue);
}

TEST(OpTest, OpGreaterEqualFalseAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpGreaterEqual),
                make(OpPop),
            }
        ));
    auto constants = std::vector{Value{2}, Value{7}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[0], testVM.falseValue);
}

TEST(OpTest, OpJumpFalseWhenFalseAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpFalse),
                make(OpJumpFalse, 5),
                make(OpAddInt),
                make(OpTrue),
                make(OpSubInt),
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
                make(OpConstantInt, 0),
                make(OpConstantInt, 1),
                make(OpTrue),
                make(OpJumpFalse, 5),
                make(OpAddInt),
                make(OpPop),
                make(OpJump, 4),
                make(OpSubInt),
            }
        ));
    auto constants = std::vector{Value{5}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(std::get<int64_t>(testVM.stack[0]), 7);
}

TEST(OpTest, OpBangTrueAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpTrue),
                make(OpBang),
            }
        ));
    auto constants = std::vector{Value{5}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[0], testVM.falseValue);
}

TEST(OpTest, OpBangFalseAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpFalse),
                make(OpBang),
            }
        ));
    auto constants = std::vector{Value{5}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[0], testVM.trueValue);
}

TEST(OpTest, OpUnaryMinusAssertions)
{
    auto instrs = make_instructions(
        std::vector(
            {
                make(OpConstantInt, 0),
                make(OpUnaryMinus),
            }
        ));
    auto constants = std::vector{Value{5}, Value{2}};
    auto bc = ByteCode{instrs, constants};
    auto testVM = VM(bc);
    testVM.run();
    EXPECT_EQ(testVM.stack[0], Value{-5});
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
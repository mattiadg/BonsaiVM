#ifndef CODE_HPP
#define CODE_HPP

#include <array>
#include <memory>
#include <string>
#include <vector>

typedef enum : unsigned char{
    OpConstantInt,
    OpTrue,
    OpFalse,
    OpPop,
    OpAddInt,
    OpSubInt,
    OpMulInt,
    OpDivInt,
    OpEqual,
    OpGreaterThan,
    OpGreaterEqual,
    OpUnaryMinus,
    OpBang,
    OpJumpFalse,
    OpJump,
} Operation;

struct Instruction
{
    unsigned char op;
    std::unique_ptr<unsigned char*> operands;
    ~Instruction() = default;
};

typedef struct
{
    std::string opName;
    int numOperands;
    std::array<char, 5> operandsWidth;
} Definition;

static std::array<Definition, 256> opDefinitions {
    Definition{"OpConstantInt", 1, {2}},
    Definition{"OpTrue", 0, {}},
    Definition{"OpFalse", 0, {}},
    Definition{"OpPop", 0, {}},
    Definition{"OpAddInt", 0, {}},
    Definition{"OpSubInt", 0, {}},
    Definition{"OpMulInt", 0, {}},
    Definition{"OpDivInt", 0, {}},
    Definition{"OpEqual", 0, {}},
    Definition{"OpGreaterThan", 0, {}},
    Definition{"OpGreaterEqual", 0, {}},
    Definition{"OpUnaryMinus", 0, {}},
    Definition{"OpBang", 0, {}},
    Definition{"OpJumpFalse", 1, {2}},
    Definition{"OpJump", 1, {2}},
};

std::vector<unsigned char> make(Operation op);

std::vector<unsigned char> make(Operation op, int16_t arg);

int16_t ReadInt16(unsigned char*);
constexpr bool is_system_little_endian();
std::array<unsigned char, 2> WriteInt16(int16_t value);

#endif
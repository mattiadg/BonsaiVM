#include <algorithm>
#include <bit>
#include <iostream>

#include "../include/code.hpp"

std::vector<unsigned char> make(Operation op)
{
    return std::vector<unsigned char>{static_cast<unsigned char>(op)};
}

std::vector<unsigned char> make(Operation op, int16_t arg)
{
    auto bytes = std::vector<unsigned char>{static_cast<unsigned char>(op)};
    std::cout << "arg: " << arg << "\n";

    auto def = opDefinitions[op];
    auto offset = 1;
    auto bytes_it = bytes.begin();
    switch(def.operandsWidth[0])
    {
        case 2:
        auto op_bytes = WriteInt16(arg);
        std::cout << "while writing, bytes: " << (int) op_bytes[0] << ", " << (int) op_bytes[1] << "\n";
        bytes.insert(bytes_it + offset, op_bytes.begin(), op_bytes.end());
        offset = 2;
    }

    return bytes;
}

std::array<unsigned char, 2> WriteInt16(int16_t value){
    std::array<unsigned char, 2> bytes{0, 0};
    if (is_system_little_endian())
    {
        bytes[0] = (value >> 8);
        bytes[1] = 0xff & value; 
    } else 
    {
        bytes = std::bit_cast<std::array<unsigned char, 2>>(value);
    }
    return bytes;
}

int16_t ReadInt16(std::array<unsigned char, 2> bytes) 
{
    int16_t value;
    if (is_system_little_endian())
    {
        std::array<unsigned char, 2> _bytes {bytes[1], bytes[0]};
        value = std::bit_cast<int16_t>(_bytes);
    } else {
        value = std::bit_cast<int16_t>(bytes);
    }
    return value;
}

constexpr inline bool is_system_little_endian()
{
    const int value { 0x01 };
    const void * address = static_cast<const void *>(&value);
    const unsigned char * least_significant_address = static_cast<const unsigned char *>(address);
    return (*least_significant_address == 0x01);
}
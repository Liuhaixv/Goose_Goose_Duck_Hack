#pragma once
#include<../memory.hpp>
#include "../../Data/offsets.hpp"

extern Memory memory;
extern Utils utils;
class string
{
public:
    string(int64_t address) {
        this->address = address;
    }

    std::string get_std_string() {
        int64_t firstChar_Addr = this->address + 0x14;

        // 字符个数
        int length = memory.read_mem<int>(this->address + Offsets::System_String::i_length, 0);

        if (length > 40) {
            return "";
        }

        char16_t buffer[40];

        for (int i = 0; i < length; i++)
        {
            char16_t c = memory.read_mem<char16_t>(firstChar_Addr + sizeof(char16_t) * i, NULL);
            // byte byte_= *(p_str + i);
            buffer[i] = c;
        }
        buffer[length] = 0;

        return utils.u8From16(buffer);
    }

private:
    int64_t address = NULL;
};


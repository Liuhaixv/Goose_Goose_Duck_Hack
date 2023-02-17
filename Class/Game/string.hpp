#pragma once

#include<string>

class string
{
public:
    string(int64_t address) {
        this->address = address;
    }

    /// <summary>
    /// 获取utf-8编码的std::string
    /// </summary>
    /// <returns></returns>
    std::string get_std_string();

private:
    int64_t address = 0;
};


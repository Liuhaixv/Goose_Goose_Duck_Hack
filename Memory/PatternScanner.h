#pragma once
#include<Windows.h>
#include <vector>
#include<memory>

class Memory;
//TODO:待完成，特征码扫描
class PatternScanner
{
public:
    PatternScanner(IN Memory* memory, IN const char* moduleName) {
        this->memory = memory;
        this->moduleName = moduleName;
    }

    //搜索Pattern并返回在进程内存中对应的地址
    int64_t FindPattern(IN std::vector<byte> pattern);
private:
    Memory* memory = nullptr;
    //模块名称
    const char* moduleName = nullptr;

    //模块的所有字节
    std::unique_ptr<byte[]> data;
    //模块的总字节数
    int64_t* size = nullptr;
};


#pragma once
#include"virtual/RemoteServerHttpApi.h"

struct UploadUserInfoApi :RemoteServerHttpApi
{
public:
    std::string getPath() {
        return this->path;
    }
private:
#if defined(DEVELOP)
    const std::string path = "/ggd/v2";
#else
    const std::string path = "/ggd/v2";
#endif
};
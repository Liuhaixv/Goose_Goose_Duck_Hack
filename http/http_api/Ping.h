#pragma once
#include"virtual/RemoteServerHttpApi.h"

struct PingApi : RemoteServerHttpApi
{
public:
    std::string getPath() {
        return this->path;
    }

private:
#if defined(DEVELOP)
    const std::string path = "/ping";
#else
    const std::string path = "/ggd/getVersions";
#endif
};
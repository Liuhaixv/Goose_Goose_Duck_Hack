#pragma once
#include"virtual/RemoteServerHttpApi.h"

struct PingApi : RemoteServerHttpApi
{
public:
    std::string getPath() {
        return this->path;
    }

private:
    const std::string path = "/ping";
};
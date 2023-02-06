#pragma once
#include"virtual/RemoteServerHttpApi.h"

struct GetVersionsApi:RemoteServerHttpApi
{
public:
     std::string getPath() {
        return this->path;
    }
private:
#if defined(DEVELOP)
     const std::string path= "/getVersions";
#else
    const std::string path = "/ggd/getVersions";
#endif
};
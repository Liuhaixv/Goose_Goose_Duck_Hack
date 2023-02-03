#pragma once
#include"virtual/RemoteServerHttpApi.h"

struct GetVersionsApi:RemoteServerHttpApi
{
public:
     std::string getPath() {
        return this->path;
    }
private:
     const std::string path= "/getVersions";
};
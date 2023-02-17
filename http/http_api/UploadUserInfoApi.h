#pragma once
#include"virtual/RemoteServerHttpApi.h"
#include "../../Struct/UserInfo.h"

struct UploadUserInfoApi :RemoteServerHttpApi
{
public:
    std::string getPath() {
        return this->path;
    }

    //httplib::Params getParams(UserInfo userInfo);
private:
#if defined(DEVELOP)
    const std::string path = "/ggd/v2";
#else
    const std::string path = "/ggd/v2";
#endif
};
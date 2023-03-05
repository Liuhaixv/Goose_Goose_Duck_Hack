#include "UploadUserInfoApi.h"

#include "../../utils.hpp"

extern Utils utils;
Params UploadUserInfoApi::getParams(UserInfo userInfo)
{
    Params params;
    params.emplace("uid", userInfo.userId);
    params.emplace("name", userInfo.nickname);
    params.emplace("fname", userInfo.GaggleID);
    params.emplace("sign", userInfo.getSign());

    return params;
}


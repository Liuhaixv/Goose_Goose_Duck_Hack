#include"UserInfo.h"
#include "../md5/md5.h"

std::string UserInfo::getSign()
{
    std::string sign = "";
    //计算md5
    if (this->GaggleID != "") {
        sign = MD5(this->GaggleID.append(this->userId)).toStr();
    }

    return sign;
}
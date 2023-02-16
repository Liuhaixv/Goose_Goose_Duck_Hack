#include "UserInfoJson.h"

#include"../../md5/md5.h"

using namespace JsonStruct;

void UserInfoJson::to_json(json& j, const UserInfoJson& p) {
    std::string gaggleID = p.GaggleID;

    //计算md5
    std::string md5 = "";
    if (gaggleID != "") {
        md5 = MD5(gaggleID.append(p.userId)).toStr();
    }

    j = json{
        {"uid", p.userId},
        {"name", p.nickname},
        {"fname", p.GaggleID},
        {"sign", md5}
    };
}
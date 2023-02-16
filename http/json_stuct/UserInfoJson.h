#pragma once
#include"virtual/BaseJsonStruct.h"
#include <string>
#include"../../json.hpp"


using json = nlohmann::json;

namespace JsonStruct {
    struct UserInfoJson :BaseJsonStruct
    {
    public:
        //pig#9123
        std::string GaggleID = "";
        //野猪佩奇
        std::string nickname = "";
        //w4rhMT8ZJseijNoZqJC8szCucXr2
        std::string userId = "";

        //由于nickname可以随意变换，所以取固定的GaggleID和userId做签名校验
        //sign = MD5( str(GaggleID) + str(userId) )
        //例：sign = MD5("pig#9123w4rhMT8ZJseijNoZqJC8szCucXr2")
        std::string sign = "";

    private:

        void to_json(json& j, const UserInfoJson& p);

        //void from_json(const json& j, UserInfoJson& p);
    };
}
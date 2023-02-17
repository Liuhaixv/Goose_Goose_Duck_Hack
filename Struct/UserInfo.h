#pragma once
#include <string>

struct UserInfo {
    //pig#9123
    std::string GaggleID = "";
    //野猪佩奇
    std::string nickname = "";
    //w4rhMT8ZJseijNoZqJC8szCucXr2
    std::string userId = "";

    std::string getSign();
};
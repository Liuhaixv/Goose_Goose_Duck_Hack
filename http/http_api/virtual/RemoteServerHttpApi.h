#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include<string>
#include<map>

using Params = std::multimap<std::string, std::string>;

#if defined(DEVELOP)
struct RemoteServerHttpApi {
public:
    std::string domain = "localhost:8080";

    virtual std::string getPath() = 0;

    /// <summary>
    /// 获取参数
    /// </summary>
    /// <returns></returns>
    static Params getParams(...) {
        return Params();
    }

private:
    const std::string path = "/";
};
#else
//发行版
struct RemoteServerHttpApi {
public:
    std::string domain = "https://api.xinshion.net";

    virtual std::string getPath() = 0;

private:
    const std::string path = "/";
};
#endif
#pragma once
#include<string>
struct RemoteServerHttpApi {
public:
    std::string domain = "localhost";
    int port = 8080;

    virtual std::string getPath() = 0;

private:
    const std::string path = "/";
};
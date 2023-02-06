#pragma once
#include"virtual/BaseJsonStruct.h"
#include <string>
#include"../../json.hpp"


using json = nlohmann::json;

namespace JsonStruct {
    struct LatestVersionsJson :BaseJsonStruct
    {
    public:
        std::string latestHackVersion = "";
        std::string url = "";

        bool hasUpdatedLatestVersions() {
            return this->b_versionsHasBeenUpdated;
        }

        void update(const LatestVersionsJson& latestVersions) {
            this->url = latestVersions.url;
            this->latestHackVersion = latestVersions.latestHackVersion;
            this->b_versionsHasBeenUpdated = true;
        }

    private:
        //版本信息是否已经获取过
        bool b_versionsHasBeenUpdated = false;
    };

    void to_json(json& j, const LatestVersionsJson& p) {
        j = json{
            {"version", p.latestHackVersion},
            {"url", p.url}
        };
    }

    void from_json(const json& j, LatestVersionsJson& p) {
        j.at("version").get_to(p.latestHackVersion);
        j.at("url").get_to(p.url);
    }
}
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
        std::string latestGameVersion = "";

        bool hasUpdatedLatestVersions() {
            return this->b_versionsHasBeenUpdated;
        }

        void update(const LatestVersionsJson& latestVersions) {
            this->latestGameVersion = latestVersions.latestGameVersion;
            this->latestHackVersion = latestVersions.latestHackVersion;
            this->b_versionsHasBeenUpdated = true;
        }

    private:
        //版本信息是否已经获取过
        bool b_versionsHasBeenUpdated = false;
    };

    void to_json(json& j, const LatestVersionsJson& p) {
        j = json{
            {"latestHackVersion", p.latestHackVersion},
            {"latestGameVersion", p.latestGameVersion}
        };
    }

    void from_json(const json& j, LatestVersionsJson& p) {
        j.at("latestHackVersion").get_to(p.latestHackVersion);
        j.at("latestGameVersion").get_to(p.latestGameVersion);
    }
}
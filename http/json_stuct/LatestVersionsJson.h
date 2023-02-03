#pragma once
#include"virtual/BaseJsonStruct.h"
#include <string>

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
}
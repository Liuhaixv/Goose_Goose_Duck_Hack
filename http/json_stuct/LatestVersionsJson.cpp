#pragma once
#include"../../json.hpp"
#include"LatestVersionsJson.h"

using json = nlohmann::json;

namespace JsonStruct {

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
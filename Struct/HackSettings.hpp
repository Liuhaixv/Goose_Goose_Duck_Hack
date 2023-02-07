#pragma once

#include"./GuiSettings.hpp"
#include"./GameStateSettings.hpp"
#include"./GameOriginalData.hpp"

#include"RemoteMasterServerSettings.hpp"
#include"../http/json_stuct/LatestVersionsJson.h"

struct HackSettings {
    //退出程序标志
    bool b_quitProgram = false;

    bool tempEnableNoclip = false;

    bool b_removeSkillCoolDown = false;

    bool b_debug_disableWriteMemory = false;
    //bool b_debug_disableReadMemory = false;

    //从服务器获取的最新版本信息
    JsonStruct::LatestVersionsJson latestVersions;

    //服务器状态及设置
    RemoteMasterServerSettings remoteServerSettings;

    GameOriginalData gameOriginalData;
    GameStateSettings gameStateSettings;
    GuiSettings guiSettings;
};

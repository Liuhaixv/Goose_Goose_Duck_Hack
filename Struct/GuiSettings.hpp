#pragma once
#include <string>
struct GuiSettings {
    //辅助程序版本
    const std::string hackVersion = "0.7.7";
    //TODO：通过signature获取
    //https://www.unknowncheats.me/forum/programming-for-beginners/171994-understanding-pattern-scanning-concept.html#post1421991
    const std::string gameVersion = "2.17.01";

    //全局开关
    bool b_draw = true;

    //打开地图
    bool b_enableMinimap = true;
    int i_selectedMinimap = -1;

    //打开设置菜单
    bool b_enableMenu = true;

    //开启玩家绘制
    bool b_enableESP = false;
    bool b_alwaysEnableNoclip = false;

    //速度修改
    bool b_enableSpeedHack = false;
    float f_movementSpeed = -1;

    //縮放修改
    bool b_enableZoomHack = false;
    float f_zoomSize = 4;

    //战争迷雾
    bool b_disableFogOfWar = false;

    //自动完成任务 & 自动准备
    bool b_enable_autoTasks_and_autoReady = false;
    bool b_autoCompleteTasks = false;
    bool b_autoReady = false;
    bool b_oneTapCompletingTask = false;
    float f_delayedEnableTime = 0.0f;//延时自动做任务
    float f_minInterval = 1.0f;//TODO


    //彩虹颜色
    bool b_enableRainbowColors = false;

    //反挂机踢出房间
    bool b_antiIdleKick = true;

    //debugmode
    bool b_debug = false;

    //bypass ban
    bool b_bypassNormalBan = false;

    //bypass version check
    bool b_bypassVersionCheck = false;
};

﻿#pragma once
struct GuiSettings {
    //辅助程序版本
    const char* version = "v0.7.1.3.2";

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

    //自动完成任务
    bool b_autoCompleteTasks_and_autoReady = false;
    bool b_autoCompleteTasks = false;
    bool b_autoReady = false;
    float f_delayedEnableTime = 0.0f;//延时自动做任务

    //反挂机踢出房间
    bool b_antiIdleKick = true;

    //debugmode
    bool b_debug = false;
};

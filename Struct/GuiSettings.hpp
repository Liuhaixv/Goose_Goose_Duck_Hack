#pragma once
struct GuiSettings {
    //游戏版本
    const char* version = "v0.6.4.3";

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

    //战争迷雾
    bool b_disableFogOfWar = false;

    //debugmode
    bool b_debug = false;
};
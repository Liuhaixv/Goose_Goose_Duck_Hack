#pragma once
struct GuiSettings {
    //全局开关
    bool b_draw = true;
    //打开设置菜单
    bool b_enableMenu = true;

    //开启玩家绘制
    bool b_enableESP = true;
    bool b_alwaysEnableNoclip = false;

    //速度修改
    bool b_enableSpeedHack = false;
    float f_baseMovementSpeed = -1;
};
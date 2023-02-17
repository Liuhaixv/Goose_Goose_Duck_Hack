#pragma once

#include<Windows.h>

#include"PlayerController.h"
#include "../../Struct/UserInfo.h"

class LocalPlayer {
public:
    int64_t address = NULL;

    PlayerController playerController;

    /// <summary>
    /// 获取当前移动速度
    /// </summary>
    /// <returns></returns>
    float getMovementSpeed();

    /// <summary>
    /// 获取初始速度
    /// </summary>
    /// <returns></returns>
    float getBaseMovementSpeed();

    void reset() {
        this->address = 0;

        resetMemberFields();
    }

    void resetMemberFields() {
        this->playerController.reset();
    }

    /// <summary>
    /// Reset and update after replacing address
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    bool update(int64_t address) {
        if (address == NULL) {
            return false;
        }

        if (!validateAddress(address)) {
            return false;
        }

        if (address != this->address) {
            reset();
            this->address = address;
        }

        return update();
    }

    UserInfo getUserInfo();

private:
    bool update();

    //检查该地址是LocalPlayer实例
    bool validateAddress(IN int64_t address);
};
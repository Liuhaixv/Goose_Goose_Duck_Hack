#pragma once

#include<../memory.h>
#include "../../Data/offsets.hpp"

class PlayerCustomizationPanelHandler {
public:
    int64_t address = NULL;

    int i_SilverBalance = -1;
    int i_GoldBalance = -1;
    int i_Xp = -1;

    PlayerCustomizationPanelHandler() {
    }

    void reset() {
        this->address = 0;

        resetMemberFields();
    }

    void resetMemberFields();

    /// <summary>
    /// Reset and update after replacing address
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    bool update(IN int64_t address) {
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

private:
    bool update();

    //检查该地址是PlayerCustomizationPanelHandler实例
    bool validateAddress(IN int64_t address);
};

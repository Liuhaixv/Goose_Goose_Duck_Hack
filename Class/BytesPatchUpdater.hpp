#pragma once

#include"../Data/offsets.hpp"
#include "../memory.hpp"
#include "../Struct/HackSettings.hpp"

extern HackSettings hackSettings;

enum ActivationState {
    IDLE_DO_NOTHING,
    SHOULD_ACTIVATE_NOW,
    SHOULD_DEACTIVATE_NOW
};

class BytesPatchUpdater {
public:
    BytesPatchUpdater(Memory* memory) {
        this->memory = memory;
    }

    void bytesPatchUpdater() {
        while (true) {
            Sleep(100);
            if (!this->memory) {
                continue;
            }
            //移除技能冷却
            {
                ActivationState state = shouldActivateOnce(hackSettings.b_removeSkillCoolDown, &b_noSkillCooldown);

                if (state == SHOULD_ACTIVATE_NOW) {
                    //移除冷却
                    int64_t address = memory->gameAssemblyBaseAddress + GameAssembly::BytesPatch::CooldownTime::address;
                    if (address == NULL) {
                        continue;
                    }

                    const std::string& patchBytes = GameAssembly::BytesPatch::CooldownTime::removeCooldownTime;
                    for (int i = 0; i < patchBytes.size(); i++) {
                        memory->write_mem<byte>(address + i, (byte)patchBytes[i]);
                    }
                }
                else if (state == SHOULD_DEACTIVATE_NOW) {
                    //恢复冷却
                    int64_t address = memory->gameAssemblyBaseAddress + GameAssembly::BytesPatch::CooldownTime::address;
                    if (address == NULL) {
                        continue;
                    }

                    const std::string& recoverBytes = GameAssembly::BytesPatch::CooldownTime::raw;
                    for (int i = 0; i < recoverBytes.size(); i++) {
                        memory->write_mem<byte>(address + i, (byte)recoverBytes[i]);
                    }
                }
            }
        }
    }
private:
    Memory* memory = nullptr;

    bool b_noSkillCooldown = hackSettings.b_removeSkillCoolDown;



    ActivationState shouldActivateOnce(bool settingEnabled, bool* b_activatedAlready) {
        //当前启用
        if (settingEnabled)
        {
            //之前没有激活启用，说明现在是刚刚启用，此时返回1
            if (!(*b_activatedAlready)) {
                //之前没激活过，现在已经是激活的状态了
                *b_activatedAlready = true;

                return SHOULD_ACTIVATE_NOW;
            }
        }
        else {
            //之前是激活启用状态，说明现在是刚刚禁用，此时返回True
            if (*b_activatedAlready) {
                *b_activatedAlready = false;

                return SHOULD_DEACTIVATE_NOW;
            }
        }

        //已经激活过或者一直没有激活过
        return IDLE_DO_NOTHING;
    }
};

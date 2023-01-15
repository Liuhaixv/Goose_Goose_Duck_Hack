#pragma once

#include"../Data/offsets.hpp"
#include "../memory.hpp"
#include "../Struct/HackSettings.hpp"
#include"../utils.hpp"

extern HackSettings hackSettings;
extern Utils utils;

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
                ActivationState state = utils.shouldActivateOnce(hackSettings.b_removeSkillCoolDown, &b_noSkillCooldown);

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

    
};

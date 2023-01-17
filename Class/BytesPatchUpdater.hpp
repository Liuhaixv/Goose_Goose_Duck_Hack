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

                    const byte* patchBytes = GameAssembly::BytesPatch::CooldownTime::removeCooldownTime;
                    memory->write_bytes(address , patchBytes, GameAssembly::BytesPatch::CooldownTime::bytesNum);
                }
                else if (state == SHOULD_DEACTIVATE_NOW) {
                    //恢复冷却
                    int64_t address = memory->gameAssemblyBaseAddress + GameAssembly::BytesPatch::CooldownTime::address;
                    if (address == NULL) {
                        continue;
                    }

                    const byte* patchBytes = GameAssembly::BytesPatch::CooldownTime::raw;
                    memory->write_bytes(address, patchBytes, GameAssembly::BytesPatch::CooldownTime::bytesNum);
                }
            }
        }
    }
private:
    Memory* memory = nullptr;

    bool b_noSkillCooldown = hackSettings.b_removeSkillCoolDown;

    
};

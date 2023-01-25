#pragma once

#include"../Data/offsets.hpp"
#include "../memory.hpp"
#include "../Struct/HackSettings.hpp"
#include"../utils.hpp"

extern HackSettings hackSettings;
extern Utils utils;
extern Memory memory;

/// <summary>
/// 负责patch汇编区指令字节。
/// 由于目前没有使用到hook框架，该类暂时也负责维护hook功能
/// </summary>
class BytesPatchUpdater {
public:

    void bytesPatchUpdater() {
        while (true) {
            Sleep(100);

            //移除技能冷却
            {
                ActivationState state = utils.shouldActivateOnce(hackSettings.b_removeSkillCoolDown, &b_noSkillCooldown);

                if (state == SHOULD_ACTIVATE_NOW) {
                    //移除冷却
                    int64_t address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::CooldownTime::address;
                    if (address == NULL) {
                        continue;
                    }

                    const byte* patchBytes = GameAssembly::BytesPatch::CooldownTime::removeCooldownTime;
                    memory.write_bytes(address , patchBytes, GameAssembly::BytesPatch::CooldownTime::bytesNum);
                }
                else if (state == SHOULD_DEACTIVATE_NOW) {
                    //恢复冷却
                    int64_t address = memory.gameAssemblyBaseAddress + GameAssembly::BytesPatch::CooldownTime::address;
                    if (address == NULL) {
                        continue;
                    }

                    const byte* patchBytes = GameAssembly::BytesPatch::CooldownTime::raw;
                    memory.write_bytes(address, patchBytes, GameAssembly::BytesPatch::CooldownTime::bytesNum);
                }
            }

            //自动完成任务
            {
                ActivationState state = utils.shouldActivateOnce(hackSettings.guiSettings.b_autoCompleteTasks, &b_autoCompleteTasks);

                if (state == SHOULD_ACTIVATE_NOW) {
                    //开始注入代码

                    //标记当前是否已经申请内存
                    static bool b_allocatedMemory = false;
                    //申请的内存基址
                    static int64_t allocatedMemoryAddress = NULL;

                    //尚未申请内存
                    if (!b_allocatedMemory) {
                        bool result = memory.allocExecutableMemory(1024, &allocatedMemoryAddress);

                        //申请内存成功
                        if (result == true) {
                            //TODO:将注入代码写入申请的内存中
                        }
                        else {
                            //申请内存失败
                        }
                    }
                }
                else if (state == SHOULD_DEACTIVATE_NOW) {
                    //取消注入代码
                    //TODO: unhook
                }
            }
        }
    }
private:

    bool b_noSkillCooldown = false;
    bool b_autoCompleteTasks = false;
};

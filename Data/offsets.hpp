#pragma once

#include<vector>

namespace Offsets {
    namespace Rigidbody2D {
        namespace UnknownClass0 {
            namespace UnknownFields {
                constexpr int64_t v2_position_readonly = 0x24;
                constexpr int64_t v2_position = 0x2C;//Vector2
            }
            constexpr int64_t ptr_UnknownFields = 0x78;
        }
        constexpr int64_t ptr_UnknownClass0 = 0x10;
    }

    namespace PlayerController {
        namespace Class {
            namespace StaticField {
                constexpr int64_t playersList = 0x10;// Dictionary<string, PlayerController> 
                constexpr int64_t playersListWithAgoraIDs = 0x20;// Dictionary<int, PlayerController>
            }
            constexpr int64_t ptr_staticFields = 0xB8;//TODO
        }
        constexpr int64_t ptr_Class = 0x0;

        constexpr int64_t ptr_Rigidbody2D = 0x58;//Rigidbody2D

        constexpr int64_t ptr_bodyCollider = 0xA8;// CapsuleCollider2D
        constexpr int64_t b_isSilenced = 0xC1; // Boolean
        constexpr int64_t b_isInfected = 0xC3; // Boolean
        constexpr int64_t killedBy = 0xC8; // point64_ter, [instance+killedBy] +  0x14, length: [instance+killedBy] + 0x10
        constexpr int64_t fl_playerRoleId = 0xE8; // point64_ter, [instance+playerRoleId] + 0x10, int64_t
        constexpr int64_t b_isPlayerRoleSet = 0xF0; // Boolean
        constexpr int64_t b_inVent = 0xF1; // Boolean
        constexpr int64_t b_facingRight = 0x130; // Boolean, can make "Moon walk".
        constexpr int64_t b_hasBomb = 0x134; // Boolean
        constexpr int64_t b_isGhost = 0x188; // Boolean
        constexpr int64_t i_timeOfDeath = 0x18C;//int
        constexpr int64_t b_isLocal = 0x1C0; // Boolean
        constexpr int64_t fl_nickname = 0x1D0; // point64_ter, [instance+nickname] +  0x14, length: [instance+nickname] + 0x10
        constexpr int64_t v3_position = 0x2C8; // Value name is randomized. x, y. Float, Float
        constexpr int64_t f_idleTime = 0x2E4; // float
        constexpr int64_t b_hasKilledThisRound = 0x2EC;//Boolean
        constexpr int64_t fl_invisibilityDistance = 0x32C; // int64_t, need this?
        constexpr int64_t b_fogOfWarEnabled = 0x379;
        constexpr int64_t b_isSpectator = 0x37A; // Boolean
        constexpr int64_t b_isRemoteSpectating = 0x37B; // Boolean

        /*
        [Notable offsets]
        2B0, Player location, Float Float,   x, y
        2BC, using left bool. camera accel?
        Using UnityEngine-Vector3.Slerp for camera movement?
        */


    }

    namespace LocalPlayer {
        namespace Class {
            namespace StaticField {

                constexpr int64_t ptr_localPlayer = 0x0;

                constexpr int64_t f_movementSpeed = 0x10;
                constexpr int64_t f_baseMovementSpeed = 0xC;//Read only
            }
            constexpr int64_t ptr_staticFields = 0xB8;
        }
        constexpr int64_t ptr_Class = 0x0;
        constexpr int64_t staticField = 0x10;
        constexpr int64_t ptr_playerController = 0x18;//PlayerController of localplayer
        constexpr int64_t ptr_fogOfWarHandler = 0x20;//FogOfWarHandler
    }

    namespace FogOfWarHandler {
        constexpr int64_t i_layerMask = 0x18;//Bit
        constexpr int64_t f_baseViewDistance = 0x2c;
        constexpr int64_t f_viewDistanceMultiplier = 0x38;
        constexpr int64_t b_targetPlayerSet = 0x50;
    }

    namespace CapsuleCollider2D {
        namespace UnknownClass0 {
            constexpr int64_t b_enableCollider = 0x39;
        }
        constexpr int64_t ptr_unknownClass0 = 0x30;
    }

    namespace UICooldownButton {
        constexpr int64_t f_cooldownTime = 0x70;
    }
}

namespace GameAssembly {
    //RVA offsets
    namespace Method {
        namespace UICooldownButton {
            constexpr int64_t Update = 0xF2D770;//void __stdcall UICooldownButton__Update(UICooldownButton_o *this, const MethodInfo *method)
        }
    }

    namespace Class {
        constexpr int64_t ptr_PlayerControllerClass = 0x3CA6AC0;//Handlers_GameHandlers_PlayerHandlers_PlayerController_c *
        constexpr int64_t ptr_LocalPlayerClass = 0x3C79808;//Handlers_GameHandlers_PlayerHandlers_LocalPlayer_c *
    }

    namespace BytesPatch {
        namespace CooldownTime {
            constexpr int64_t address = GameAssembly::Method::UICooldownButton::Update + 0xB7;
            constexpr byte raw[] = "\x0F\x82";
            constexpr byte removeCooldownTime[] = "\xEB\x0E";//jmp
            constexpr int bytesNum = 2;
        }
    }

    /// <summary>
    /// 通过下标返回PlayerController的偏移数组<para/>
    /// Returns int64_t array of offsets of PlayerController by its index
    /// </summary>
    /// <param name="index">Index of PlayerController, range=[0,15]</param>
    /// <returns>int64_t array</returns>
    static std::vector<int64_t> playerControllerByIndex(int64_t index) {
        int64_t specialOffset = 0x30;
        specialOffset += index * 0x18;

        std::vector<int64_t> offsets = {
            GameAssembly::Class::ptr_PlayerControllerClass,
            Offsets::PlayerController::Class::ptr_staticFields,
            Offsets::PlayerController::Class::StaticField::playersListWithAgoraIDs,
            0x18, specialOffset, 0x0
        };

        return offsets;
    }

    static std::vector<int64_t> localPlayer() {

        std::vector<int64_t> offsets = {
       GameAssembly::Class::ptr_LocalPlayerClass,
       Offsets::LocalPlayer::Class::ptr_staticFields,
       Offsets::LocalPlayer::Class::StaticField::ptr_localPlayer,
       0x0
        };


        return offsets;
    }
}
#pragma once

#include<vector>

namespace Offsets {

    //private sealed class RoomManager.LIPKINCBJEJ : IEnumerator<object>, IEnumerator, IDisposable // TypeDefIndex: 1322
    namespace RoomManager_LIPKINCBJEJ {
        /*
            // Fields
            private int <>1__state; // 0x10
            private object <>2__current; // 0x18
            public string nickname; // 0x20
            public string region; // 0x28
            public int map; // 0x30
            public bool isVisible; // 0x34
            public bool voiceChat; // 0x35
            public string initGamemode; // 0x38
            public int language; // 0x40
            public int levelGate; // 0x44
            public bool streamerMode; // 0x48
            public HangingOutInfo hangingOutInfo; // 0x50
            public RoomManager <>4__this; // 0x58
            private RoomManager.JFIFOFKNKOJ <>8__1; // 0x60
            public string maxPlayers; // 0x68
            private Task<RequestResponse<BanStatusResponse>> <getBanTimeTask>5__2; // 0x70
        */
    }

    //private sealed class RoomManager.NOGLKBCBCCN : IEnumerator<object>, IEnumerator, IDisposable // TypeDefIndex: 1327
    namespace RoomManager_NOGLKBCBCCN {
        /*
            // Fields
            public short returnCode; // 0x10
            public string message; // 0x18
            public RoomManager.CAFCPAIEEKA CS$ < >8__locals1; // 0x20

            // Methods

            // RVA: 0x28DE40 Offset: 0x28D240 VA: 0x18028DE40
            public void.ctor() { }

            // RVA: 0xA1E700 Offset: 0xA1DB00 VA: 0x180A1E700
            internal void <JoinRoomCoroutine>b__5() { }
        */
    }

    //C#的string类
    namespace System_String {
        constexpr int64_t i_length = 0x10;//字符串的字符个数
        constexpr int64_t firstChar = 0x14;//字符串的第一个起始字节
    }

    namespace GameTask {
        constexpr int64_t taskId = 0x10;//string
        constexpr int64_t taskDisplayName = 0x18;//string
    }

    namespace TasksHandler {
        // Fields
        constexpr int64_t tasks = 0x18;//GameTask[] tasks; // 0x18
        constexpr int64_t tasksDictionary = 0x20;//readonly Dictionary<string, GameTask> tasksDictionary; // 0x20
        constexpr int64_t taskUIPrefab = 0x28;//GameObject taskUIPrefab; // 0x28
        constexpr int64_t taskList = 0x30; //Transform taskList; // 0x30
        constexpr int64_t assignedTasks = 0x38;//Dictionary<string, GameTask> assignedTasks; // 0x38
        constexpr int64_t audioSource = 0x40;//AudioSource EAPGEFFHCBE; // 0x40
        constexpr int64_t timesReplenished = 0x48;// timesReplenished; // 0x48
    }

    namespace LobbySceneHandler {
        namespace Class {
            namespace StaticField {
                constexpr int64_t Instance = 0x0;//LobbySceneHandler
                constexpr int64_t  b_InGameScene = 0x8;//表示当前是否在游戏中或游戏房间中
                constexpr int64_t  b_ExploreMode = 0x9;
                constexpr int64_t  i_CurrentMap = 0xC;//当前的地图
                constexpr int64_t  b_IsPublicGame = 0x10;
                constexpr int64_t  b_IsMicEnabled = 0x11;
            }
            constexpr int64_t ptr_staticFields = 0xB8;
        }

        constexpr int64_t ptr_tasksHandler = 0x40;//TasksHandler*
        constexpr int64_t b_gameStarted = 0x220;//游戏是否已经开始(该值在退出游戏后也很可能为1)
        constexpr int64_t f_inactiveTimeThreshold = 0x408;//最大公开房间挂机时长
        constexpr int64_t f_privateInactiveTimeThreshold = 0x40C;//最大私有房间挂机时长
        constexpr int64_t f_timeInRoom = 0x458;
    }

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
            constexpr int64_t ptr_staticFields = 0xB8;
        }
        constexpr int64_t ptr_Class = 0x0;

        constexpr int64_t ptr_Rigidbody2D = 0x58;//Rigidbody2D

        constexpr int64_t ptr_playerCollider = 0xA0;// CapsuleCollider2D
        constexpr int64_t ptr_wallCheckCollider = 0xA8;// CapsuleCollider2D
        constexpr int64_t ptr_bodyCollider = 0xB0;// CapsuleCollider2D
        constexpr int64_t b_isSilenced = 0xD1; // Boolean
        constexpr int64_t b_isHelium = 0xD2; // Boolean
        constexpr int64_t b_isInfected = 0xD3; // Boolean
        constexpr int64_t ptr_killedBy = 0xD8; // System_String
        constexpr int64_t ptr_playerRole = 0xF8; // point64_ter, [instance+playerRoleId] + 0x10, int64_t
        constexpr int64_t b_isPlayerRoleSet = 0x100; // Boolean
        constexpr int64_t b_inVent = 0x101; // Boolean
        constexpr int64_t b_facingRight = 0x140; // Boolean, can make "Moon walk".
        constexpr int64_t b_hasBomb = 0x144; // Boolean
        constexpr int64_t b_isGhost = 0x198; // Boolean
        constexpr int64_t i_timeOfDeath = 0x19C;//int
        constexpr int64_t b_isLocal = 0x1D0; // Boolean
        constexpr int64_t ptr_nickname = 0x1E0; // System_String

        constexpr int64_t v3_position = 0x2D8; // Value name is randomized. x, y. Float, Float
        constexpr int64_t f_idleTime = 0x2F4; // float 当前挂机时长（秒）(未移动过)，移动后会自动重置为0
        constexpr int64_t b_hasKilledThisRound = 0x2FC;//Boolean
        constexpr int64_t i_readyState = 0x328;//ReadyState LobbySceneHandler.DHPEEBDONPM

        constexpr int64_t fl_invisibilityDistance = 0x33C; // int64_t, need this?
        constexpr int64_t b_fogOfWarEnabled = 0x389;
        constexpr int64_t b_isSpectator = 0x38A; // Boolean
        constexpr int64_t b_isRemoteSpectating = 0x38B; // Boolean
    }

    namespace CinemachineStateDrivenCamera {
        constexpr int64_t ptr_m_AnimatedTarget = 0xB0;//Animator
    }

    namespace CinemachineVirtualCamera {
        constexpr int64_t f_m_zoomSize = 0xB4;//Float
    }

    namespace LocalPlayer {
        namespace Class {
            namespace StaticField {

                constexpr int64_t ptr_localPlayer = 0x0;

                constexpr int64_t f_baseMovementSpeed = 0xC;//只读
                constexpr int64_t struct_movementSpeed = 0x10;//已加密ObscuredFloat
            }
            constexpr int64_t ptr_staticFields = 0xB8;
        }
        constexpr int64_t ptr_Class = 0x0;
        constexpr int64_t staticField = 0x10;
        constexpr int64_t ptr_playerController = 0x18;//PlayerController of localplayer
        constexpr int64_t ptr_fogOfWarHandler = 0x20;//FogOfWarHandler

        constexpr int64_t b_disableMovement = 0x60;

        constexpr int64_t ptr_cinemachineStateDrivenCamera = 0x68;// CinemachineStateDrivenCamera
        constexpr int64_t ptr_cinemachineVirtualCamera = 0x70;// CinemachineVirtualCamera
    }

    namespace FogOfWarHandler {
        constexpr int64_t struct_obscured_layerMask = 0x18;//已加密ObscuredInt
        constexpr int64_t i_layerMask = 0x2C;//Bit
        constexpr int64_t i_playerLayerMask = 0x30;
        constexpr int64_t struct_baseViewDistance = 0x40;//已加密ObscuredFloat
        constexpr int64_t struct_viewDistanceMultiplier = 0x68;//已加密ObscuredFloat
        constexpr int64_t b_targetPlayerSet = 0x88;
    }

    namespace CapsuleCollider2D {
        namespace UnknownClass0 {
            constexpr int64_t b_enableCollider = 0x38;
        }
        constexpr int64_t ptr_unknownClass0 = 0x10;
        constexpr int64_t ptr_unknownClass1 = 0x30;
    }

    namespace UICooldownButton {
        constexpr int64_t struct_cooldownTime = 0x84;//已加密ObscuredFloat
    }
}

namespace GameAssembly {
    //RVA offsets
    namespace Method {

        namespace RoomManager_LIPKINCBJEJ {
            // RVA: 0xA1EAA0 Offset: 0xA1DEA0 VA: 0x180A1EAA0 Slot: 6
            //private bool MoveNext() { }
            constexpr int64_t MoveNext = 0xA1EAA0;
        }

        namespace RoomManager_NOGLKBCBCCN {
            // RVA: 0xA1F5E0 Offset: 0xA1E9E0 VA: 0x180A1F5E0 Slot: 6
            //private bool MoveNext() { }
            constexpr int64_t MoveNext = 0xA1F5E0;
        }

        namespace PlayerPropertiesManager {
            constexpr int64_t ChangeReadyState = 0x8F2460;//public void ChangeReadyState(int KBGNPKGDFGK) { }
        }

        namespace TasksHandler {
            constexpr int64_t CompleteTask = 0xED2630;//public void CompleteTask(string EDADCOILIAL, bool BEHGOBBIKEO, bool IOFAGBIBBIM = False, bool LHCBIJPIOBC = False, bool CAAPDAFENNA = True) { }
        }

        namespace UICooldownButton {
            constexpr int64_t Update = 0x104E6A0;//void __stdcall UICooldownButton__Update(UICooldownButton_o *this, const MethodInfo *method)
        }

        namespace LocalPlayer {
            constexpr int64_t Update = 0xFCC7E0;
        }

        namespace Application {
            constexpr int64_t Quit = 0x3257DE0;//退出游戏
        }

        namespace AntiCheat {
            namespace Utils {
                namespace ThreadSafeRandom {
                    constexpr int64_t Next = 0x2AAB20;//public static int Next(int minInclusive, int maxExclusive) { }
                }
            }
        }
    }

    namespace Class {
        constexpr int64_t ptr_LobbySceneHandlerClass = 0x3CDAAE0;//Handlers_LobbyHandlers_LobbySceneHandler_c *
        constexpr int64_t ptr_PlayerControllerClass = 0x3D084C8;//Handlers_GameHandlers_PlayerHandlers_PlayerController_c *
        constexpr int64_t ptr_LocalPlayerClass = 0x3CDB720;//Handlers_GameHandlers_PlayerHandlers_LocalPlayer_c *
    }

    namespace BytesPatch {
        namespace CooldownTime {
            constexpr int64_t address = GameAssembly::Method::UICooldownButton::Update + 0xB7;
            constexpr byte raw[] = "\x0F\x82";
            constexpr byte removeCooldownTime[] = "\xEB\x0E";//jmp
            constexpr int bytesNum = 2;
        }

        /// <summary>
        /// 禁用反作弊随机数种子，禁用后加密秘钥将为0，加密后的内容等于明文
        /// </summary>
        namespace RandomSeed {
            constexpr int64_t address = GameAssembly::Method::AntiCheat::Utils::ThreadSafeRandom::Next;

            const std::vector<byte> raw{ 0x48,0x89,0x5C,0x24 };
            /// <summary>
            /// 恒返回0
            /// xor rax,rax
            /// ret
            /// </summary>
            const std::vector<byte> disableRandomSeed{ 0x48,0x31,0xC,0xC3 };
        }

        namespace QuitGame {
            constexpr int64_t address = GameAssembly::Method::Application::Quit;

            const std::vector<byte> raw = { 0x48 };
            /// <summary>
            /// 直接返回
            /// ret
            /// </summary>
            const std::vector<byte>  disableQuitGame = { 0xC3 };
        }

        namespace BypassBan {
            //房主过检测
            namespace Step1 {
                constexpr int64_t address = GameAssembly::Method::RoomManager_LIPKINCBJEJ::MoveNext + 0x301;

                const std::vector<byte> raw = { 0x80, 0x78, 0x10, 0x00,
                                                0x0f, 0x85, 0xAA, 0x01, 0x00, 0x00 };
                const std::vector<byte>  bypassBan = { 0xc7, 0x40, 0x10, 0x00, 0x00, 0x00, 0x00,
                                                        0x90, 0x90, 0x90 };
            }

            //加入游戏过检测
            namespace Step2 {
                constexpr int64_t address = GameAssembly::Method::RoomManager_NOGLKBCBCCN::MoveNext + 0x377;

                const std::vector<byte> raw = {0x48,0x89,0x70,0x20 };
                const std::vector<byte>  bypassBan = {0xC6,0x40,0x20,0x00 };
            }
        }

        /*
        namespace AutoCompleteTasks {
            //GameAssembly.dll+FA149D
            constexpr int64_t address = GameAssembly::Method::LocalPlayer::Update + 0xFDD;
            constexpr byte raw[] = "\x45\x0F\x28\x7B\x90\x49\x8B\xE3\x41\x5F\x41\x5E\x5F\x5E\x5B";
            constexpr byte removeCooldownTime[] = "\xEB\x0E";//jmp
            constexpr int bytesNum = 0;
        }
        */
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

    static std::vector<int64_t> lobbySceneHandler() {

        std::vector<int64_t> offsets = {
       GameAssembly::Class::ptr_LobbySceneHandlerClass,
       Offsets::LobbySceneHandler::Class::ptr_staticFields,
       Offsets::LobbySceneHandler::Class::StaticField::Instance,
       0x0
        };

        return offsets;
    }

    static std::vector<int64_t> lobbySceneHandler_staticFiled() {

        std::vector<int64_t> offsets = {
       GameAssembly::Class::ptr_LobbySceneHandlerClass,
       Offsets::LobbySceneHandler::Class::ptr_staticFields,
       0x0
        };

        return offsets;
    }
}
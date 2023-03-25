#pragma once

#include<vector>

namespace Offsets {

    //2.16.02
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

    //2.16.02
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

    //2.16.02
    namespace GameTask {
        constexpr int64_t taskId = 0x10;//string
        constexpr int64_t taskDisplayName = 0x18;//string
    }

    //2.16.02
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

    //2.17.02
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

    //2.16.02
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

    //2.18.00 未详细检查
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

        constexpr int64_t ptr_userId = 0x1A8; // System_String

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
        constexpr int64_t  b_isInPelican = 0x3A4;//Boolean
    }

    //2.18.00
    namespace CinemachineStateDrivenCamera {
        constexpr int64_t ptr_m_AnimatedTarget = 0xB0;//Animator
    }

    //2.16.02
    //LocalPlayer组件游戏对象名VCAM Scriptable
    namespace CinemachineVirtualCamera {
        constexpr int64_t struct_m_Lens = 0xB0;//LensSettings

        //以下几个数据实际上属于LensSettings结构体的一部分
        
        //This is the camera view in degrees. Display will be in vertical degress, unless the associated camera has its FOV axis setting set to Horizontal, in which case display will be in horizontal degress. Internally, it is always vertical degrees.  For cinematic people, a 50mm lens on a super-35mm sensor would equal a 19.6 degree FOV
        constexpr int64_t f_FieldOfView = 0xB0;
        //When using an orthographic camera, this defines the half-height, in world coordinates, of the camera view.
        constexpr int64_t f_OrthographicSize = 0xB4;
        //This defines the near region in the renderable range of the camera frustum. Raising this value will stop the game from drawing things near the camera, which can sometimes come in handy.  Larger values will also increase your shadow resolution.
        constexpr int64_t f_NearClipPlane = 0xB8;
        //This defines the far region of the renderable range of the camera frustum. Typically you want to set this value as low as possible without cutting off desired distant objects
        constexpr int64_t f_FarClipPlane = 0xBC;
        //Camera Z roll, or tilt, in degrees.
        constexpr int64_t f_Dutch = 0xC0;
        constexpr int64_t i_ModeOverride = 0xC4;//enum LensSettings.OverrideModes
    }

    //2.18.00
    namespace FriendManager {
        namespace Class {
            namespace StaticField {
                constexpr int64_t ptr_friendManager = 0x0;
            }
            constexpr int64_t ptr_staticFields = 0xB8;
        }
        constexpr int64_t ptr_GaggleID = 0x1B0;//string
    }

    //2.18.00
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
        constexpr int64_t ptr_playerController = 0x18;//PlayerController of localplayer
        constexpr int64_t ptr_fogOfWarHandler = 0x20;//FogOfWarHandler

        constexpr int64_t b_disableMovement = 0x78;

        constexpr int64_t ptr_cinemachineStateDrivenCamera = 0x68;// CinemachineStateDrivenCamera
        constexpr int64_t ptr_cinemachineVirtualCamera = 0x70;// CinemachineVirtualCamera
    }

    //2.18.00
    namespace FogOfWarHandler {
        constexpr int64_t struct_obscured_layerMask = 0x18;//已加密ObscuredInt
        constexpr int64_t i_layerMask = 0x2C;//LayerMask Bit
        constexpr int64_t i_playerLayerMask = 0x30;
        constexpr int64_t struct_baseViewDistance = 0x40;//已加密ObscuredFloat
        constexpr int64_t struct_viewDistanceMultiplier = 0x68;//已加密ObscuredFloat
        constexpr int64_t b_targetPlayerSet = 0x88;
    }

    //2.16.02
    namespace CapsuleCollider2D {
        namespace UnknownClass0 {
            constexpr int64_t b_enableCollider = 0x38;
        }
        constexpr int64_t ptr_unknownClass0 = 0x10;
        constexpr int64_t ptr_unknownClass1 = 0x30;
    }

    //2.18.00.2
    namespace UICooldownButton {
        constexpr int64_t struct_cooldownTime = 0x84;//已加密ObscuredFloat
    }
}

namespace GameAssembly {
    //RVA offsets
    namespace Method {
        //2.16.02
        namespace RoomManager_LIPKINCBJEJ {
            // RVA: 0xA1EAA0 Offset: 0xA1DEA0 VA: 0x180A1EAA0 Slot: 6
            //private bool MoveNext() { }
            constexpr int64_t MoveNext = 0xA1EAA0;
        }
        //2.16.02
        namespace RoomManager_NOGLKBCBCCN {
            // RVA: 0xA1F5E0 Offset: 0xA1E9E0 VA: 0x180A1F5E0 Slot: 6
            //private bool MoveNext() { }
            constexpr int64_t MoveNext = 0xA1F5E0;
        }

        //2.18.02
        namespace PlayerPropertiesManager {
            constexpr int64_t ChangeReadyState = 0xDE9670;//public void ChangeReadyState(int KBGNPKGDFGK) { }
        }
        //2.18.02
        namespace TasksHandler {
            constexpr int64_t CompleteTask = 0x1C49CA0;//public void CompleteTask(string EDADCOILIAL, bool BEHGOBBIKEO, bool IOFAGBIBBIM = False, bool LHCBIJPIOBC = False, bool CAAPDAFENNA = True) { }
        }
        //2.18.02
        namespace UICooldownButton {
            constexpr int64_t Update = 0x1124840;//void __stdcall UICooldownButton__Update(UICooldownButton_o *this, const MethodInfo *method)
        }
        //2.18.02
        namespace LocalPlayer {
            constexpr int64_t Update = 0x1D30C60;
        }
        //2.16.02
        namespace Application {
            constexpr int64_t Quit = 0x3257DE0;//退出游戏
        }
        //2.18.00.02
        namespace AntiCheat {
            namespace Utils {
                namespace ThreadSafeRandom {
                    constexpr int64_t Next = 0x885B60;//public static int Next(int minInclusive, int maxExclusive) { }
                }
            }
        }
        //2.18.00.02
        namespace UnityEngine {
            namespace Random {
                constexpr int64_t RandomRangeInt = 0x3F4F1E0;//int32_t UnityEngine_Random__RandomRangeInt(int32_t minInclusive, int32_t maxExclusive, const MethodInfo *method)
            }
        }

        //2.17.02
        namespace PlayerCustomizationPanelHandler {
            constexpr int64_t ChangeColor = 0x1DE8810;//void __stdcall Handlers_LobbyHandlers_PlayerCustomizationPanelHandler__ChangeColor(Handlers_LobbyHandlers_PlayerCustomizationPanelHandler_o *this, int32_t GHCFJPBDGED, const MethodInfo *method)
        }

        //2.18.02
        namespace TaskPanelHandler {
            constexpr int64_t OpenPanel = 0x1C2B2C0;//void __stdcall Handlers_GameHandlers_TaskHandlers_TaskPanelHandler__OpenPanel(Handlers_GameHandlers_TaskHandlers_TaskPanelHandler_o *this, const MethodInfo *method)
        }
    }

    //2.18.02
    namespace Class {
        constexpr int64_t ptr_FriendManagerClass = 0x4CCB470;//Gaggle_Friends_FriendManager_c *

        constexpr int64_t ptr_LobbySceneHandlerClass = 0x4D248A8;//Handlers_LobbyHandlers_LobbySceneHandler_c *
        //Handlers.GameHandlers.PlayerHandlers.PlayerController
        constexpr int64_t ptr_PlayerControllerClass = 0x4D59F40;//Handlers_GameHandlers_PlayerHandlers_PlayerController_c *
        //Handlers.GameHandlers.PlayerHandlers.LocalPlayer
        constexpr int64_t ptr_LocalPlayerClass = 0x4D25558;//Handlers_GameHandlers_PlayerHandlers_LocalPlayer_c *
        constexpr int64_t ptr_PlayerCustomizationPanelHandlerClass = 0x4D5A028;//Handlers_LobbyHandlers_PlayerCustomizationPanelHandler_c *
    }

    namespace BytesPatch {

        //2.18.02
        //E8 ?? ?? ?? ?? F2 0F 10 8F C4 00 00 00 48 8D 4D E7
        namespace CooldownTime {
            constexpr int64_t address = GameAssembly::Method::UICooldownButton::Update + 0x22F;
            const std::vector<byte> raw{ 0xE8,0xAC ,0xA3 ,0xE4 ,0x02 };//call Time.get_deltaTime
            const std::vector<byte> removeCooldownTime{ 0x41,0x0F,0x28,0xC1,0x90 };//movaps xmm0, xmm9
        }

        /// <summary>
        /// 2.16.02
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

        //2.16.02
        namespace QuitGame {
            constexpr int64_t address = GameAssembly::Method::Application::Quit;

            const std::vector<byte> raw = { 0x48 };
            /// <summary>
            /// 直接返回
            /// ret
            /// </summary>
            const std::vector<byte>  disableQuitGame = { 0xC3 };
        }

        //2.16.02
        namespace BypassBan {
            //房主过检测
            namespace Step1 {
                constexpr int64_t address = GameAssembly::Method::RoomManager_LIPKINCBJEJ::MoveNext + 0x301;

                //if ( body->fields.isBanned )
                const std::vector<byte> raw = { 0x80, 0x78, 0x10, 0x00,
                                                0x0f, 0x85, 0xAA, 0x01, 0x00, 0x00 };
                const std::vector<byte>  bypassBan = { 0xc7, 0x40, 0x10, 0x00, 0x00, 0x00, 0x00,
                                                        0x90, 0x90, 0x90 };
            }

            //加入游戏过检测
            namespace Step2 {
                constexpr int64_t address = GameAssembly::Method::RoomManager_NOGLKBCBCCN::MoveNext + 0x377;

                const std::vector<byte> raw = { 0x48,0x89,0x70,0x20 };
                const std::vector<byte>  bypassBan = { 0xC6,0x40,0x20,0x00 };
            }
        }

        //跳过任务小游戏，直接完成任务
        //原理为替换api，让OpenPanel跳转到CompleteTask，因为都是无参函数，所以直接jmp即可
        //2.18.02
        namespace SkipPlayingGameToCompleteTask {
            constexpr int64_t address = GameAssembly::Method::TaskPanelHandler::OpenPanel;

            const std::vector<byte> raw = { 0x48, 0x89, 0x5C, 0x24, 0x08 };// mov     [rsp+arg_0], rbx
            const std::vector<byte>  oneTapCompleteTask = { 0xE9,0xDB,0xE7,0xFF,0xFF };//Handlers.GameHandlers.TaskHandlers.TaskPanelHandler.OpenPanel - E9 6BEFFFFF - jmp Handlers.GameHandlers.TaskHandlers.TaskPanelHandler.CompleteTask
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

namespace UnityPlayer {
    namespace BytesPatch {

        //2.16.02
        namespace BypassVersionCheck {
            //利用dll空闲空间创建CodeCave
            namespace BuildCodeCave {
                constexpr int64_t address = 0x33F911;

                const std::vector<byte>  bytes = { 0x48,0x83,0xC4,0x50,
                                                        0x48,0x83,0xC3,0x10,
                                                        0xC6,0x03,0x07,
                                                        0x48,0x83,0xC3,0x08,
                                                        0xC7,0x03,0x31,0x00,0x37,0x00,
                                                        0x48,0x83,0xC3,0x06,
                                                        0xC7,0x03,0x30,0x00,0x30,0x00,
                                                        0x5B,
                                                        0xC3 };
            }

            //加入游戏过检测
            namespace JmpToCodeCave {
                constexpr int64_t address = 0xB1639;

                const std::vector<byte> raw = { 0x48,0x83,0xC4,0x50,0x5B };
                const std::vector<byte>  bypass = { 0xE9, 0xD3,0xE2,0x28,0x00 };
            }
        }
    }
}
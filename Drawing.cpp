#define STB_IMAGE_IMPLEMENTATION

#include "Drawing.h"
#include "client.hpp"

#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR)))) 

LPCSTR Drawing::lpWindowName = "ImGui Standalone";
ImVec2 Drawing::vWindowSize = { 500, 500 };
ImGuiWindowFlags Drawing::WindowFlags = /*ImGuiWindowFlags_NoSavedSettings |*/ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
//bool Drawing::bDraw = true;

extern Utils utils;
extern HackSettings hackSettings;
extern Client* g_client;

//#define str(eng,cn) (const char*)u8##cn
//#define str(eng,cn) (const char*)u8##cnshij
#define str(eng,cn) utils.b_chineseOS?(const char*)u8##cn:eng

void drawMinimap();
void drawMenu();
void drawESP();

void Drawing::Active()
{
    hackSettings.guiSettings.b_draw = true;
}

bool Drawing::isActive()
{
    return (hackSettings.guiSettings.b_draw == true);
}

void Drawing::Draw() {
    static bool* b_previousEnableMenu = nullptr;
    if (isActive())
    {
        drawMinimap();

        //绘制菜单
        if (hackSettings.guiSettings.b_enableMenu) {
            drawMenu();
        }

        //ESP
        if (hackSettings.guiSettings.b_enableESP) {
            drawESP();
        }
        else {

        }
    }
}


// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}



bool drawLocalPlayerOnMap(GameMap& map, const ImVec2& mapLeftBottomPointOnScreen) {
    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    static float circleRadius = 10;

    LocalPlayer* localPlayer = &g_client->localPlayer;

    if (localPlayer->address == NULL) {
        return false;
    }

    PlayerController* playerController = &localPlayer->playerController;

    if (playerController->address == NULL || playerController->b_isLocal == false) {
        return false;
    }

    Vector3* position = &playerController->v3_position;

    Vector2 relativePosition = map.positionIngameToScreenPoint({ position->x, position->y });
    Vector2 positionOnScreen{ mapLeftBottomPointOnScreen.x + relativePosition.x * map.scaleToDisplay, mapLeftBottomPointOnScreen.y + relativePosition.y * map.scaleToDisplay };

    drawList->AddCircleFilled({ positionOnScreen.x,positionOnScreen.y }, circleRadius, ImColor(1.0f, 1.0f, 1.0f));
    drawList->AddText({ positionOnScreen.x, positionOnScreen.y + circleRadius }, ImColor(1.0f, 1.0f, 1.0f), str("You", "你"));
}

/// <summary>
/// 在地图上绘制玩家
/// </summary>
bool drawOtherPlayersOnMap(GameMap& map, const ImVec2& mapLeftBottomPointOnScreen) {
    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    static float circleRadius = 10;

    PlayerController* playerControllers = g_client->playerControllers;

    PlayerController* ptr = playerControllers;

    for (int i = 0; i < g_client->n_players; (i++, ptr++)) {
        if (ptr->address == NULL) {
            continue;
        }

        //单独处理本地玩家的绘制
        if (ptr->b_isLocal) {
            continue;
        }

        Vector3* position = &ptr->v3_position;

        Vector2 relativePosition = map.positionIngameToScreenPoint({ position->x, position->y });
        Vector2 positionOnScreen{ mapLeftBottomPointOnScreen.x + relativePosition.x * map.scaleToDisplay, mapLeftBottomPointOnScreen.y + relativePosition.y * map.scaleToDisplay };

        drawList->AddCircleFilled({ positionOnScreen.x,positionOnScreen.y }, circleRadius, ImColor(1.0f, 0.0f, 0.0f));
        drawList->AddText({ positionOnScreen.x, positionOnScreen.y + circleRadius }, ImColor(1.0f, 1.0f, 1.0f), ptr->nickname.c_str());
    }
    return true;
}

void drawMinimap() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Minimap");

    GameMap* gameMap = nullptr;


    const char* mapNames[] = {
        str("Ancient Sands", "古代沙地"),
        str("The Basement","地下室"),
        str("Jungle Temple","丛林神殿"),
        str("GooseChapel","鹅教堂"),
        str("Mallard Manor","马拉德庄园"),
        str("Nexus Colony","连结殖民地"),
        str("Black Swan","黑天鹅"),
        str("SS MotherGoose","老妈鹅星球飞船")
    };

    static bool toggles[] = { true, false, false, false, false };


    static int selected_map = -1;

    if (ImGui::Button(str("Select map", "选择地图")))
        ImGui::OpenPopup("select_map");
    ImGui::SameLine();
    ImGui::TextUnformatted(selected_map == -1 ? str("<None>", "无") : mapNames[selected_map]);

    ImGui::SameLine();
    if (ImGui::Button(str("Debug map offsets", "调试地图偏移"))) {
        ImGui::OpenPopup("debug_map_offsets");
    }

    //选择地图图片
    if (ImGui::BeginPopup("select_map"))
    {
        ImGui::Text("Aquarium");
        ImGui::Separator();
        for (int i = 0; i < IM_ARRAYSIZE(mapNames); i++)
            if (ImGui::Selectable(mapNames[i])) {
                //修改当前地图图片
                selected_map = i;
            }
        ImGui::EndPopup();
    }

    if (selected_map < 0) {
        //尚未选择地图
        //Have not selected map
        ImGui::Text(str("You have not selected map", "你还没有选择地图"));
    }
    else {
        //读取地图
        gameMap = &UI::miniMaps.at(selected_map);

        //弹出调试界面
        if (ImGui::BeginPopup("debug_map_offsets"))
        {
            static Vector2 tpPosition = { 0.0f,0.0f };
            if (ImGui::Button(str("TP to: ", "传送到: "))) {
                g_client->teleportTo(tpPosition);
            }
            ImGui::InputFloat("debug_map_tp_X", &tpPosition.x);
            ImGui::InputFloat("debug_map_tp_Y", &tpPosition.y);


            ImGui::Text("Offsets:");
            ImGui::SliderFloat("debug_map_offsets_X", &gameMap->offset.x, -50, 50);
            ImGui::SliderFloat("debug_map_offsets_Y", &gameMap->offset.y, -50, 50);
            ImGui::Text("Scale To Game Position:");
            ImGui::SliderFloat("Scale", &gameMap->scaleToGamePosition, 0.02, 0.05, "%.4f");

            ImGui::EndPopup();
        }

        //ImGui::Text("pointer = %p", gameMap);
        //ImGui::Text("size = %d x %d", gameMap->width, gameMap->height);

        ImVec2 leftTopOfImage = ImGui::GetCursorScreenPos();

        //ImGui::GetForegroundDrawList()->AddCircleFilled({ leftTopOfImage.x,leftTopOfImage.y }, 5, ImColor(1.0f, 0.0f, 0.0f));
        //ImVec2 roomForImage = { view.x, view.y -  };


        //检查长宽是否为0
        if (gameMap->width > 0 && gameMap->height > 0) {
            static bool minimapShowedBefore = false;

            ImVec2 pos;

            //处理显示地图的尺寸问题
            {
                //显示默认大小
                if (!minimapShowedBefore) {
                    minimapShowedBefore = true;
                }
                //根据地图尺寸调整窗口大小
                else {
                    //当前窗口剩余可容纳图片的范围
                    ImVec2 roomSpaceForImage = ImGui::GetContentRegionAvail();
                    //ImGui::Text("roomSpaceForImage. %.1f, %.1f", roomSpaceForImage.x ,roomSpaceForImage.y);

                    if (roomSpaceForImage.x > 10 && roomSpaceForImage.y > 10) {
                        //width / height
                        float roomSpaceRatio = roomSpaceForImage.x / roomSpaceForImage.y;
                        float mapImageRatio = 1.0f * gameMap->width / gameMap->height;

                        //宽大于图片等比缩放所需的长度，
                        //此时按照剩余空间的高和图片的高来缩放
                        if (roomSpaceRatio >= mapImageRatio) {
                            gameMap->scaleToDisplay = roomSpaceForImage.y / gameMap->height;
                        }
                        else {
                            //高大于图片等比缩放所需的长度，
                            //此时按照剩余空间的宽和图片的宽来缩放
                            gameMap->scaleToDisplay = roomSpaceForImage.x / gameMap->width;
                        }
                    }
                }
            }

            //显示游戏地图
            bool gameMapClicked = ImGui::ImageButton((void*)gameMap->texture, ImVec2(gameMap->width * gameMap->scaleToDisplay, gameMap->height * gameMap->scaleToDisplay));

            //记录本次鼠标悬停在游戏地图上这段时间是否有过TP
            static bool hasTPedWhenHoveringOnGameMap = false;
            //记录上一次TP的坐标
            static Vector2 lastTPedPosition;

            //处理鼠标移动到图片上的逻辑
            pos = ImGui::GetCursorScreenPos();
            if (ImGui::IsItemHovered())
            {
                Vector2 positionInGame = gameMap->screenPointToPositionIngame({ io.MousePos.x - pos.x, io.MousePos.y - pos.y });

                

                //处理点击传送的逻辑
                ImGui::BeginTooltip();


                //地图刚被点击
                if (gameMapClicked) {
                    //TODO 传送玩家
                    g_client->teleportTo(positionInGame);
                    hasTPedWhenHoveringOnGameMap = true;
                    lastTPedPosition = positionInGame;
                }

                if (!hasTPedWhenHoveringOnGameMap) {
                    ImGui::Text(str("Click to TP\n(%.1f, %.1f)", "点击传送\n(%.1f, %.1f)"), positionInGame.x, positionInGame.y);
                }
                else {
                    //尚未点击
                    ImGui::Text(str("You have been teleported to\n(%.1f, %.1f)", "你已被传送至\n(%.1f, %.1f)"), lastTPedPosition.x, lastTPedPosition.y);
                }

                ImGui::EndTooltip();
            }
            else {
                //游戏地图没有鼠标焦点
                hasTPedWhenHoveringOnGameMap = false;
            }

            //ImGui::GetForegroundDrawList()->AddCircleFilled({ pos.x,pos.y }, 20, ImColor(1.0f, 0.0f, 0.0f));
            //在地图上绘制玩家位置
            drawOtherPlayersOnMap(*gameMap, pos);
            drawLocalPlayerOnMap(*gameMap, pos);
        }
        else {
            //不显示游戏地图
        }
    }

    ImGui::End();
}

void drawMenu() {
    ImGui::ShowDemoWindow();

    bool b_open = true;
    bool* ptr_bOpen = &b_open;

    ImGui::Begin(str("Main", "主菜单"));

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("Main menu", tab_bar_flags))
    {
        PlayerController* playerController = &g_client->localPlayer.playerController;
        //菜单3
        if (ImGui::BeginTabItem(str("LocalPlayer Info", "本地玩家信息")))
        {
            ImGui::Text(playerController->nickname.c_str());

            float minSpeed = hackSettings.gameOriginalData.f_baseMovementSpeed;
            if (minSpeed <= 0) {
                minSpeed = 5.0f;
            }

            ImGui::SliderFloat(
                str("Movement speed", "移速"),
                &hackSettings.guiSettings.f_baseMovementSpeed,
                minSpeed,
                minSpeed * 2
            );

            ImGui::Text("{%.2f, %.2f}", playerController->v3_position.x, playerController->v3_position.y);

            ImGui::EndTabItem();
        }

        //菜单1
        if (ImGui::BeginTabItem(str("Players Info", "角色信息")))
        {
            if (ImGui::BeginTable("table1", 5,
                ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
            ))
            {
                ImGui::TableSetupColumn(str("Nickname", "昵称"));
                ImGui::TableSetupColumn(str("Role", "角色"));
                ImGui::TableSetupColumn(str("Killed this round", "本轮杀过人"));
                ImGui::TableSetupColumn(str("Death Time", "死亡时间"));
                ImGui::TableSetupColumn(str("Pos", "坐标"));
                //ImGui::TableSetupColumn("Three");
                ImGui::TableHeadersRow();

                PlayerController* player = g_client->playerControllers;
                for (int row = 0; row < g_client->n_players; (row++, player++))
                {
                    //跳过无效玩家和本地玩家
                    if (player->address == NULL || player->b_isLocal) {
                        continue;
                    }
                    ImGui::TableNextRow();

                    ImGui::TableNextColumn(); ImGui::Text(player->nickname.c_str());
                    ImGui::TableNextColumn(); ImGui::Text(player->roleName.c_str());
                    if (player->b_hasKilledThisRound) {
                        ImGui::TableNextColumn(); ImGui::Text(str("Yes", "是"));
                    }
                    else {
                        ImGui::TableNextColumn(); ImGui::Text(str("", ""));
                    }
                    if (player->i_timeOfDeath != 0) {
                        ImGui::TableNextColumn(); ImGui::Text("%d", player->i_timeOfDeath);
                    }
                    else {
                        ImGui::TableNextColumn(); ImGui::Text("");
                    }
                    ImGui::TableNextColumn(); ImGui::Text("(%.1f, %.1f)", player->v3_position.x, player->v3_position.y);

                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        //菜单2
        if (ImGui::BeginTabItem(str("Misc", "功能类")))
        {
            ImGui::Checkbox(str("Remove fog of war", "隐藏战争迷雾"), &hackSettings.disableFogOfWar);
            HelpMarker(
                str("Remove shadows and let you see other players behind walls", "可以透过墙看到和听到其他玩家，隐藏视野阴影")
            );

            ImGui::Checkbox(str("Noclip", "穿墙"), &hackSettings.guiSettings.b_alwaysEnableNoclip);
            HelpMarker(
                str("Walk through anything\nYou can press Left ALT to temporarily enable noclip", "穿墙模式\n长按左ALT键来临时穿墙")
            );

            ImGui::EndTabItem();
        }

        //菜单2
        if (ImGui::BeginTabItem(str("ESP", "透视")))
        {
            ImGui::Text(str("Button below is just for testing if overlay works", "下面的按钮目前只是为了测试绘制能否正常工作"));
            ImGui::Checkbox(str("Enable ESP", "全局开关"), &hackSettings.guiSettings.b_enableESP);
            HelpMarker(
                str("Create Issue to report bug if you can't see two green lines and yellow rect line", "如果你看不到屏幕上有横竖两条绿线以及环绕整个显示器的黄色矩形的话,请到Issue提交bug")
            );

            ImGui::EndTabItem();
        }

        //菜单2
        if (ImGui::BeginTabItem(str("README", "说明")))
        {
            ImGui::Text(str("This an open-source project from Liuhaixv", "这是一个来自Liuhaixv的开源项目"));
            ImGui::SameLine();
            if (ImGui::Button(str("Link to project", "查看项目"))) {
                ShellExecute(0, 0, "https://github.com/Liuhaixv/Goose_Goose_Duck_Hack", 0, 0, SW_SHOW);
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}

void drawESP() {
    //TODO
    ///*
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImDrawList* drawList = ImGui::GetBackgroundDrawList(viewport);

    /*
    drawList->AddCircleFilled(
        { 500,500 },
        30,
        ImColor{ 1.0f, 1.0f, 0.0f }
    );
    */

    drawList->AddRect({ 0, 0 }, { ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y }, ImColor(1.0f, 1.0f, 0.0f), 50.0f, 0, 6.0f);

    ImColor lineColor{ 0.0f,1.0f,0.0f };
    float lineThichness = 4;
    drawList->AddLine({ 0, ImGui::GetIO().DisplaySize.y / 2 }, { ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y / 2 },
        lineColor, lineThichness);
    drawList->AddLine({ ImGui::GetIO().DisplaySize.x / 2, 0 }, { ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y },
        lineColor, lineThichness);
    //*/

    /*
    ImGui::GetForegroundDrawList()->AddCircleFilled(
        { 500,500 },
        30,
        ImColor{ 1.0f, 1.0f, 0.0f }
    );

    ImGui::GetForegroundDrawList()->AddRect({ 0, 0 }, { ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y }, ImColor(1.0f, 1.0f, 0.0f), 50.0f, 0, 3.0f);
    */
}

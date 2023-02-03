#define STB_IMAGE_IMPLEMENTATION

#include "Drawing.h"
#include "../Memory/memory.h"
#include "../Client.h"
#include "../Struct/UserSettings.hpp"
#include <sstream>
#include "../Class/Game/string.hpp"
#include<algorithm>

#include"../Class/HttpDataUpdater.h"
//#include "Struct/UserSettings.hpp"

#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR)))) 

LPCSTR Drawing::lpWindowName = "ImGui Standalone";
ImVec2 Drawing::vWindowSize = { 500, 500 };
ImGuiWindowFlags Drawing::WindowFlags = /*ImGuiWindowFlags_NoSavedSettings |*/ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
//bool Drawing::bDraw = true;

extern Utils utils;
extern HackSettings hackSettings;
extern Client g_client;
extern Memory memory;

extern UserSettings userSettings;
extern HttpDataUpdater httpDataUpdater;

//#define str(eng,cn) (const char*)u8##cn
//#define str(eng,cn) (const char*)u8##cnshij
#define str(eng,cn) utils.b_chineseOS?(const char*)u8##cn:(const char*)u8##eng
//拼接组件和常量名
//#define labelName(componentName,constStr) std::u8string(componentName).append(constStr).c_str()

//#define labelName(componentName,constStr) getLabelName

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
        if (hackSettings.guiSettings.b_debug) {
            ImGui::ShowDemoWindow();
        }


        if (hackSettings.guiSettings.b_enableMinimap) {
            drawMinimap();
        }

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

/// <summary>
/// 旋转圆圈动画
/// </summary>
/// <param name="label"></param>
/// <param name="radius"></param>
/// <param name="thickness"></param>
/// <param name="color"></param>
/// <returns></returns>
bool Spinner(const char* label, float radius, int thickness, const ImU32& color) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    // Render
    window->DrawList->PathClear();

    int num_segments = 30;
    int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

    const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

    const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

    for (int i = 0; i < num_segments; i++) {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
            centre.y + ImSin(a + g.Time * 8) * radius));
    }

    window->DrawList->PathStroke(color, false, thickness);
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

    LocalPlayer* localPlayer = &g_client.localPlayer;

    if (localPlayer->address == NULL) {
        return false;
    }

    PlayerController* playerController = &localPlayer->playerController;

    if (playerController->address == NULL || playerController->b_isLocal == false) {
        return false;
    }

    Vector3* position = &playerController->v3_position;

    Vector2 relativePosition = map.positionInGame_to_relativePositionLeftBottom({ position->x, position->y });
    Vector2 positionOnScreen{ mapLeftBottomPointOnScreen.x + relativePosition.x, mapLeftBottomPointOnScreen.y - relativePosition.y };

    //绘制filled圆
    drawList->AddCircleFilled(
        { positionOnScreen.x,positionOnScreen.y },
        userSettings.getFloat(UserSettingsNames::minimap_radius_circleFilled_local, DefaultSettings::Size::f_radius_circleFilled_local),
        userSettings.getColor(UserSettingsNames::minimap_color_circleFilled_local, DefaultSettings::Color::color_circleFilled_local)
    );
    //绘制昵称
    drawList->AddText(
        { positionOnScreen.x + DefaultSettings::Offset::nickname_to_circleFilled.x,
        positionOnScreen.y + DefaultSettings::Offset::nickname_to_circleFilled.y },
        userSettings.getColor(UserSettingsNames::minimap_color_name_local,
            DefaultSettings::Color::color_nickname_local),
        str("You", "你")
    );

    return true;
}

bool drawPlayersNearbyDeadPlayer(GameMap& map, PlayerController* deadPlayer, const ImVec2& mapLeftBottomPointOnScreen) {
    ImDrawList* drawList = ImGui::GetForegroundDrawList();

    //绘制死亡玩家位置
    {
        if (deadPlayer->address == NULL) {
            return false;
        }

        if (deadPlayer->playersNearbyOnDeath.size() == 0) {
            //TODO: ??没有死亡玩家附近的人的数据，目前已知原因有：中途关闭了辅助重新打开，或中途加入游戏
        }

        PlayerController* deadPlayerRecord = &deadPlayer->playersNearbyOnDeath[0];

        //死亡玩家昵称（忽略）
        const char* nickname = "";

        Vector3* position = &deadPlayerRecord->v3_position;

        Vector2 relativePosition = map.positionInGame_to_relativePositionLeftBottom({ position->x, position->y }, true);
        Vector2 positionOnScreen{ mapLeftBottomPointOnScreen.x + relativePosition.x , mapLeftBottomPointOnScreen.y - relativePosition.y };

        //绘制坐标filled圆
        drawList->AddCircleFilled(
            { positionOnScreen.x,positionOnScreen.y },
            userSettings.getFloat(UserSettingsNames::minimap_radius_circleFilled_dead, DefaultSettings::Size::f_radius_circleFilled_dead),
            userSettings.getColor(UserSettingsNames::minimap_color_circleFilled_dead, DefaultSettings::Color::color_circleFilled_dead)
        );

        //绘制玩家姓名
        drawList->AddText(
            { positionOnScreen.x + DefaultSettings::Offset::nickname_to_circleFilled.x,
            positionOnScreen.y + DefaultSettings::Offset::nickname_to_circleFilled.y },
            ImColor{ 1.0f, 1.0f, 1.0f },
            nickname
        );
    }

    PlayerController* ptr_playerController;
    for (int i = 1; i < deadPlayer->playersNearbyOnDeath.size(); i++) {

        ptr_playerController = &deadPlayer->playersNearbyOnDeath[i];

        if (!ptr_playerController) {
            continue;
        }

        //嫌疑人昵称
        const char* nickname = nullptr;

        //单独处理本地玩家的绘制
        if (ptr_playerController->b_isLocal) {
            nickname = ptr_playerController->nickname.c_str();
        }
        else {
            nickname = ptr_playerController->nickname.c_str();
        }

        Vector3* position = &ptr_playerController->v3_position;

        Vector2 relativePosition = map.positionInGame_to_relativePositionLeftBottom({ position->x, position->y }, true);
        Vector2 positionOnScreen{ mapLeftBottomPointOnScreen.x + relativePosition.x , mapLeftBottomPointOnScreen.y - relativePosition.y };

        //绘制filled圆
        drawList->AddCircleFilled({ positionOnScreen.x,positionOnScreen.y },
            userSettings.getFloat(UserSettingsNames::minimap_radius_circleFilled_alive, DefaultSettings::Size::f_radius_circleFilled_alive),
            userSettings.getColor(UserSettingsNames::minimap_color_circleFilled_alive, DefaultSettings::Color::color_circleFilled_alive));
        //绘制昵称
        drawList->AddText(
            { positionOnScreen.x + DefaultSettings::Offset::nickname_to_circleFilled.x,
            positionOnScreen.y + DefaultSettings::Offset::nickname_to_circleFilled.y },
            userSettings.getColor(UserSettingsNames::minimap_color_name_dead, DefaultSettings::Color::color_circleFilled_suspect),
            nickname);
    }
    return true;
}

/// <summary>
/// 在地图上绘制玩家
/// </summary>
bool drawOtherPlayersOnMap(GameMap& map, const ImVec2& mapLeftBottomPointOnScreen) {
    ImDrawList* drawList = ImGui::GetForegroundDrawList();

    auto playerControllers = g_client.playerControllers;
    //PlayerController* playerControllers = g_client.playerControllers;

    for (auto ptr_playerController : playerControllers) {
        if (ptr_playerController->address == NULL) {
            continue;
        }

        //单独处理本地玩家的绘制
        if (ptr_playerController->b_isLocal) {
            continue;
        }

        //跳过死亡玩家
        if (ptr_playerController->i_timeOfDeath > 0) {
            continue;
        }

        Vector3* position = &ptr_playerController->v3_position;

        Vector2 relativePosition = map.positionInGame_to_relativePositionLeftBottom({ position->x, position->y });
        Vector2 positionOnScreen{ mapLeftBottomPointOnScreen.x + relativePosition.x , mapLeftBottomPointOnScreen.y - relativePosition.y };

        //绘制filled圆
        drawList->AddCircleFilled(
            { positionOnScreen.x,positionOnScreen.y },
            userSettings.getFloat(UserSettingsNames::minimap_radius_circleFilled_alive, DefaultSettings::Size::f_radius_circleFilled_alive),
            userSettings.getColor(UserSettingsNames::minimap_color_circleFilled_alive, DefaultSettings::Color::color_circleFilled_alive)
        );
        //绘制昵称
        drawList->AddText(
            { positionOnScreen.x + DefaultSettings::Offset::nickname_to_circleFilled.x,
            positionOnScreen.y + DefaultSettings::Offset::nickname_to_circleFilled.y },
            userSettings.getColor(UserSettingsNames::minimap_color_name_alive, DefaultSettings::Color::color_nickname_alive),
            ptr_playerController->nickname.c_str()
        );
    }
    return true;
}

/// <summary>
/// 绘制玩家死亡快照
/// </summary>
/// <returns></returns>
bool drawPlayerDeathSnapshot(PlayerController* deadPlayer, const char* str_id) {
    //当前已选择的地图
    int i_selectedMinimap = hackSettings.guiSettings.i_selectedMinimap;

    if (ImGui::BeginPopup(str_id, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (i_selectedMinimap < 0) {
            ImGui::Text(str("You have to select a map from minimap first to enable this feature.", "你必须先在minimap中选择地图才能查看玩家位置"));
        }
        else {
            GameMap* gameMap = &UI::miniMaps.at(i_selectedMinimap);

            ImGui::Image((void*)gameMap->texture, ImVec2(gameMap->width, gameMap->height));

            //图片左下角
            auto mousePositionLeftBottomOfGamemap = ImGui::GetCursorScreenPos();
            drawPlayersNearbyDeadPlayer(*gameMap, deadPlayer, mousePositionLeftBottomOfGamemap);
        }
        ImGui::EndPopup();
    }
    return true;
}

void drawMinimap() {
    ImGuiIO& io = ImGui::GetIO();

    //设置小地图初始化大小
    ImGui::SetNextWindowSize({ 500.0f, 400.0f }, ImGuiCond_Once);
    ImGui::Begin("Minimap", NULL, ImGuiWindowFlags_NoScrollbar);

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

    //设置颜色
    if (ImGui::Button(str("Settings", "设置"))) {
        ImGui::OpenPopup("minimap_settings_colors");
    }

    //弹出minimap设置
    if (ImGui::BeginPopup("minimap_settings_colors")) {

        if (ImGui::BeginTable("minimap_color_settings_table", 4,
            ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
        ))
        {
            //设置表头
            ImGui::TableSetupColumn(str("", ""));
            ImGui::TableSetupColumn(str("Nickname Colors", "昵称颜色"));
            ImGui::TableSetupColumn(str("Position circle filled color", "坐标点填充颜色"));
            ImGui::TableSetupColumn(str("Position circle size", "坐标点大小"));
            ImGui::TableHeadersRow();

            //取消显示拖拽和名称
            ImGuiColorEditFlags colorEditFlags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs;

            //死亡
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text(str("Dead", "死亡"));


                //昵称颜色
                ImGui::TableNextColumn();
                ImGui::ColorEdit3(str("Dead player's nickname##ColorEdit", "死亡玩家昵称##ColorEdit"),
                    &userSettings.getColor(UserSettingsNames::minimap_color_name_dead, ImColor(IM_COL32_WHITE)).Value.x,
                    colorEditFlags);

                //坐标点填充颜色
                ImGui::TableNextColumn();
                ImGui::ColorEdit3(str("Dead player's position##ColorEdit", "死亡玩家位置##ColorEdit"),
                    &userSettings.getColor(UserSettingsNames::minimap_color_circleFilled_dead, ImColor(IM_COL32_WHITE)).Value.x,
                    colorEditFlags);

                //坐标点大小
                ImGui::TableNextColumn();
                ImGui::SliderFloat(str("##Dead player's position circle size", "##死亡玩家坐标点大小"),
                    &userSettings.getFloat(UserSettingsNames::minimap_radius_circleFilled_dead,
                        DefaultSettings::Size::f_radius_circleFilled_dead),
                    DefaultSettings::SliderFloat::size_of_circleFilled.x,
                    DefaultSettings::SliderFloat::size_of_circleFilled.y);
            }

            //存活
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text(str("Alive", "存活"));

                //昵称颜色
                ImGui::TableNextColumn();
                //ImGui::ColorEdit3(labelName2,
                ImGui::ColorEdit3(str("Alive player's nickname##ColorEdit", "存活玩家昵称##ColorEdit"),
                    &userSettings.getColor(UserSettingsNames::minimap_color_name_alive, DefaultSettings::Color::color_nickname_alive).Value.x,
                    colorEditFlags);

                //坐标点填充颜色
                ImGui::TableNextColumn();
                ImGui::ColorEdit3(str("Alive player's position##ColorEdit", "存活玩家位置##ColorEdit"),
                    &userSettings.getColor(UserSettingsNames::minimap_color_circleFilled_alive, DefaultSettings::Color::color_circleFilled_alive).Value.x,
                    colorEditFlags);

                //坐标点大小
                ImGui::TableNextColumn();
                ImGui::SliderFloat(str("##Alive player's position circle size", "##存活玩家坐标点大小"),
                    &userSettings.getFloat(UserSettingsNames::minimap_radius_circleFilled_alive,
                        DefaultSettings::Size::f_radius_circleFilled_alive),
                    DefaultSettings::SliderFloat::size_of_circleFilled.x,
                    DefaultSettings::SliderFloat::size_of_circleFilled.y);
            }

            //你
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text(str("LocalPlayer", "本地玩家"));

                //昵称颜色
                ImGui::TableNextColumn();
                ImGui::ColorEdit3(str("Your nickname##ColorEdit", "你的昵称##ColorEdit"),
                    &userSettings.getColor(UserSettingsNames::minimap_color_name_local, DefaultSettings::Color::color_nickname_local).Value.x,
                    colorEditFlags);

                //坐标点填充颜色
                ImGui::TableNextColumn();
                ImGui::ColorEdit3(str("Your position##ColorEdit", "你的位置##ColorEdit"),
                    &userSettings.getColor(UserSettingsNames::minimap_color_circleFilled_local, DefaultSettings::Color::color_circleFilled_local).Value.x,
                    colorEditFlags);

                //坐标点大小
                ImGui::TableNextColumn();
                ImGui::SliderFloat(str("##Local player's position circle size", "##本地玩家坐标点大小"),
                    &userSettings.getFloat(UserSettingsNames::minimap_radius_circleFilled_local,
                        DefaultSettings::Size::f_radius_circleFilled_local),
                    DefaultSettings::SliderFloat::size_of_circleFilled.x,
                    DefaultSettings::SliderFloat::size_of_circleFilled.y);
            }

            ImGui::EndTable();
        }
        /*
        //玩家颜色
        ImGui::Text(str("Nickname Colors", "昵称颜色"));
        ImGui::ColorEdit3(str("Dead", "死亡"),
            &userSettings.getColor(UserSettingsName::minimap_color_name_dead, ImColor(IM_COL32_WHITE)).Value.x,
            ImGuiColorEditFlags_NoInputs);

        ImGui::ColorEdit3(str("Alive", "存活"),
            &userSettings.getColor(UserSettingsName::minimap_color_name_alive, ImColor(IM_COL32_WHITE)).Value.x,
            ImGuiColorEditFlags_NoInputs);

        ImGui::ColorEdit3(str("You", "你"),
            &userSettings.getColor(UserSettingsName::minimap_color_name_local, ImColor(IM_COL32_WHITE)).Value.x,
            ImGuiColorEditFlags_NoInputs);

        //实心圆绘制颜色
        ImGui::Text(str("Position circle filled color", "坐标点填充颜色"));
        ImGui::ColorEdit3(str("Dead", "死亡"),
            &userSettings.getColor(UserSettingsName::minimap_color_circle_dead, ImColor(IM_COL32_WHITE)).Value.x,
            ImGuiColorEditFlags_NoInputs);

        ImGui::ColorEdit3(str("Alive", "存活"),
            &userSettings.getColor(UserSettingsName::minimap_color_circle_alive, ImColor(255, 0, 0)).Value.x,//默认红色
            ImGuiColorEditFlags_NoInputs);

        ImGui::ColorEdit3(str("You", "你"),
            &userSettings.getColor(UserSettingsName::minimap_color_circle_local, ImColor(IM_COL32_WHITE)).Value.x,
            ImGuiColorEditFlags_NoInputs);

        //实心圆大小
        ImGui::Text(str("Position circle size", "坐标点大小"));
        //ImGui::DragFloat
        */

        ImGui::EndPopup();
    }ImGui::SameLine();

    if (ImGui::Button(str("Select map", "选择地图")))
        ImGui::OpenPopup("select_map");
    ImGui::SameLine();
    ImGui::TextUnformatted(hackSettings.guiSettings.i_selectedMinimap == -1 ? str("<None>", "无") : mapNames[hackSettings.guiSettings.i_selectedMinimap]);

    if (hackSettings.guiSettings.b_debug) {
        ImGui::SameLine();
        if (ImGui::Button(str("Debug map offsets", "调试地图偏移"))) {
            ImGui::OpenPopup("debug_map_offsets");
        }
    }

    //选择地图图片
    if (ImGui::BeginPopup("select_map"))
    {
        ImGui::Text(str("Select map", "选择地图"));
        ImGui::Separator();
        for (int i = 0; i < IM_ARRAYSIZE(mapNames); i++)
            if (ImGui::Selectable(mapNames[i])) {
                //修改当前地图图片
                hackSettings.guiSettings.i_selectedMinimap = i;
            }
        ImGui::EndPopup();
    }

    if (hackSettings.guiSettings.i_selectedMinimap < 0) {
        //尚未选择地图
        //Have not selected map
        ImGui::Text(str("You have not selected map", "你还没有选择地图"));
    }
    else {
        //读取地图
        gameMap = &UI::miniMaps.at(hackSettings.guiSettings.i_selectedMinimap);

        //弹出调试界面
        if (ImGui::BeginPopup("debug_map_offsets"))
        {
            static Vector2 tpPosition = { 0.0f,0.0f };
            if (ImGui::Button(str("TP to: ", "传送到: "))) {
                g_client.teleportTo(tpPosition);
            }
            ImGui::InputFloat("debug_map_tp_X", &tpPosition.x);
            ImGui::InputFloat("debug_map_tp_Y", &tpPosition.y);


            ImGui::Text("Offsets:");
            ImGui::SliderFloat("debug_map_offsets_X", &gameMap->offset.x, -50, 50);
            ImGui::SliderFloat("debug_map_offsets_Y", &gameMap->offset.y, -50, 50);
            ImGui::Text("Scale To Game Position:");

            static float min_scale = 0.01;
            static float max_scale = 1;

            ImGui::SliderFloat("Scale", &gameMap->scaleToGamePosition, min_scale, max_scale, "%.4f");
            ImGui::InputFloat("min_scale", &min_scale);
            ImGui::InputFloat("max_scale", &max_scale);

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

            ImVec2 mousePositionLeftBottomOfGamemap;

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

            //图片最左下角的坐标
            mousePositionLeftBottomOfGamemap = ImGui::GetCursorScreenPos();
            //处理鼠标移动到图片上的逻辑
            if (ImGui::IsItemHovered())
            {
                Vector2 positionInGame = gameMap->relativePositionLeftBottom_to_PositionInGame({
                    io.MousePos.x - mousePositionLeftBottomOfGamemap.x,
                    //因为屏幕坐标Y轴是和游戏内Y轴相反的
                    mousePositionLeftBottomOfGamemap.y - io.MousePos.y
                    });

                //处理点击传送的逻辑
                ImGui::BeginTooltip();


                //地图刚被点击
                if (gameMapClicked) {
                    g_client.teleportTo(positionInGame);
                    hasTPedWhenHoveringOnGameMap = true;
                    lastTPedPosition = positionInGame;
                }

                //Debug
                if (hackSettings.guiSettings.b_debug) {
                    if (!hasTPedWhenHoveringOnGameMap) {
                        ImGui::Text(str("Click to TP\n(%.1f, %.1f)", "点击传送\n(%.1f, %.1f)\n相对图片左下角(%.1f, %.1f)"),
                            positionInGame.x, positionInGame.y,
                            io.MousePos.x - mousePositionLeftBottomOfGamemap.x, mousePositionLeftBottomOfGamemap.y - io.MousePos.y
                        );
                    }
                    else {
                        //尚未点击
                        ImGui::Text(str("You have been teleported to\n(%.1f, %.1f)", "你已被传送至\n(%.1f, %.1f)"), lastTPedPosition.x, lastTPedPosition.y);
                    }
                }
                //Release
                else {
                    if (!hasTPedWhenHoveringOnGameMap) {
                        ImGui::Text(str("Click to TP\n(%.1f, %.1f)", "点击传送\n(%.1f, %.1f)"),
                            positionInGame.x, positionInGame.y
                        );
                    }
                    else {
                        //尚未点击
                        ImGui::Text(str("You have been teleported to\n(%.1f, %.1f)", "你已被传送至\n(%.1f, %.1f)"), lastTPedPosition.x, lastTPedPosition.y);
                    }
                }

                ImGui::EndTooltip();
            }
            else {
                //游戏地图没有鼠标焦点
                hasTPedWhenHoveringOnGameMap = false;
            }

            //ImGui::GetForegroundDrawList()->AddCircleFilled({ pos.x,pos.y }, 20, ImColor(1.0f, 0.0f, 0.0f));
            //在地图上绘制玩家位置
            drawOtherPlayersOnMap(*gameMap, mousePositionLeftBottomOfGamemap);
            drawLocalPlayerOnMap(*gameMap, mousePositionLeftBottomOfGamemap);
        }
        else {
            //不显示游戏地图
        }
    }

    ImGui::End();
}

void drawMenu() {
    bool b_open = true;
    bool* ptr_bOpen = &b_open;

    ImGui::SetNextWindowSize({ 500.0f, 400.0f }, ImGuiCond_Once);

    ImGui::Begin(str("Open source project from github.com/Liuhaixv", "免费项目:github.com/Liuhaixv"), NULL, ImGuiWindowFlags_MenuBar /* | ImGuiWindowFlags_AlwaysAutoResize*/);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginTable("minimap_color_settings_table", 2,
            ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_NoBordersInBody
        ))
        {
            {
                //游戏运行状态
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text(str("Game status: ", "游戏状态: "));
                ImGui::SameLine();
                if (hackSettings.gameStateSettings.b_gameProcessRunning) {
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), str("Game Running", "运行中"));
                    //ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Yellow");
                }
                else {
                    ImGui::TextDisabled(str("Not Running", "未运行"));
                }

                //版本
                {
                    ImGui::TableNextColumn();

                    ImGui::Text(str("Version:", "版本："));
                    ImGui::SameLine();

                    //点击版本按钮
                    if (ImGui::Button(hackSettings.guiSettings.hackVersion.c_str())) {
                        //TODO: 检查更新
                        ImGui::OpenPopup("check version");
                        //手动更新一次
                        httpDataUpdater.addHttpTask(std::bind(HttpTask::checkLatestVersions, 1));
                    }
                    //TODO: 检查更新
                    if (ImGui::BeginPopup("check version")) {
                        if (ImGui::BeginTable("check_version_table", 2,
                            ImGuiTableFlags_SizingFixedFit /* | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg*/
                        )) {
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            //辅助版本
                            //TODO: 判断版本是否有更新，标红提示
                            ImGui::Text(str("Current hack version:", "当前辅助版本:"));
                            ImGui::TableNextColumn();
                            ImGui::TextDisabled(hackSettings.guiSettings.hackVersion.c_str());

                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            //适配游戏版本
                            ImGui::Text(str("Works on game version:", "适配的游戏版本:"));
                            ImGui::TableNextColumn();
                            ImGui::TextDisabled(hackSettings.guiSettings.gameVersion.c_str());

                            ImGui::EndTable();
                        }
                        ImGui::NewLine();

                        if (ImGui::BeginTable("check_server_versions_table", 2,
                            ImGuiTableFlags_SizingFixedFit /* | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg*/
                        )) {
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            //最新辅助版本
                            ImGui::Text(str("Latest hack version:", "最新辅助版本:"));
                            ImGui::TableNextColumn();
                            if (!hackSettings.latestVersions.hasUpdatedLatestVersions()) {
                                //显示加载动画
                                Spinner("latest_hack_version_spinner", 10.0f, 2.0f,ImColor(0,255,0));
                            }
                            else {
                                ImGui::TextDisabled("%s%s",
                                    "v",
                                    hackSettings.latestVersions.latestHackVersion);
                            }                            

                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            //最新游戏版本
                            ImGui::Text(str("Latest game version:", "最新游戏版本:"));
                            ImGui::TableNextColumn();
                            if (!hackSettings.latestVersions.hasUpdatedLatestVersions()) {
                                //显示加载动画
                                Spinner("latest_game_version_spinner", 10.0f, 2.0f, ImColor(0, 255, 0));
                            }
                            else {
                                ImGui::TextDisabled("%s%s",
                                    "v",
                                    hackSettings.latestVersions.latestGameVersion);
                            }

                            ImGui::EndTable();
                        }
                        //服务器返回的版本信息


                        ImGui::EndPopup();
                    }

                    ImGui::SameLine();

                    //未连接远程服务器
                    if (hackSettings.remoteServerSettings.serverState == RemoteMasterServerState::DOWN) {
                        //红色
                        ImGui::TextColored(ImColor(255, 0, 0), str("Connection Failed", "连接失败"));
                    }
                    else if (hackSettings.remoteServerSettings.serverState == RemoteMasterServerState::NORMAL) {
                        ImGui::TextColored(ImColor(0, 255, 0), str("Connected", "已连接"));
                    }
                    else if (hackSettings.remoteServerSettings.serverState == RemoteMasterServerState::UNKNOWN) {
                        ImGui::TextDisabled(str("Unknown", "未知"));
                    }
                    else {
                        ImGui::TextDisabled(str("Unknown", "未知"));
                    }
                    ImGui::SameLine();
                    HelpMarker(str("Remote master server connection state\nClick version to check update", "远程主服务器连接状态"));

                    //TODO:警告，当前版本不适配游戏
                    //判断游戏版本是否匹配
                    //TODO:
                }
            }
            ImGui::EndTable();
        }

        /*
        ImGui::Text(str("Game status: ", "游戏状态: "));
        ImGui::SameLine();
        if (hackSettings.gameStateSettings.b_gameProcessRunning) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), str("Game Running", "运行中"));
            //ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Yellow");
        }
        else {
            ImGui::TextDisabled(str("Not Running", "未运行"));
        }

        //版本
        ImGui::Text(str("      Version:", "      版本："));
        ImGui::SameLine();
        ImGui::Text(hackSettings.guiSettings.version);
        ImGui::SameLine();
        */

        ImGui::EndMenuBar();
    }

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("Main menu", tab_bar_flags))
    {
        PlayerController* playerController = &g_client.localPlayer.playerController;
        //菜单1
        if (ImGui::BeginTabItem(str("LocalPlayer", "本地玩家")))
        {
            ImGui::Text(playerController->nickname.c_str());

            float minSpeed = hackSettings.gameOriginalData.f_baseMovementSpeed;
            if (minSpeed <= 0) {
                minSpeed = 5.0f;
            }

            //ImGui::BeginDisabled();
            ImGui::Checkbox(str("Remove skill cooldown", "移除技能冷却时间"), &hackSettings.b_removeSkillCoolDown);
            HelpMarker(str("Notice: server - sided CD can not be removed", "注意：服务器端CD无法修改，如鸭子刀人CD"));
            //::EndDisabled();

            ImGui::Checkbox(str("Enable##speedHack", "启用##speedHack"), &hackSettings.guiSettings.b_enableSpeedHack);
            ImGui::SameLine();
            ImGui::SliderFloat(
                str("Movement speed", "移速"),
                &hackSettings.guiSettings.f_movementSpeed,
                minSpeed,
                minSpeed * 2
            );

            ImGui::Checkbox(str("Enable##zoomHack", "启用##zoomHack"), &hackSettings.guiSettings.b_enableZoomHack);
            ImGui::SameLine();
            ImGui::SliderFloat(
                str("Zoom Size", "相机缩放"),
                &hackSettings.guiSettings.f_zoomSize,
                0.5,
                40
            );

            //玩家移速
            //ImGui::Text("{%.2f, %.2f}", playerController->v3_position.x, playerController->v3_position.y);

            ImGui::EndTabItem();
        }

        //菜单2
        if (ImGui::BeginTabItem(str("Players Info", "角色信息")))
        {
            if (ImGui::BeginTable("players_info_table", 5,
                ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
                /*TODO: 由于Bug暂时禁用排序 | ImGuiTableFlags_Sortable*/
            ))
            {
                ImGui::TableSetupColumn(str("Nickname", "昵称"), ImGuiTableColumnFlags_NoSort);
                ImGui::TableSetupColumn(str("Role", "角色"), ImGuiTableColumnFlags_NoSort);
                ImGui::TableSetupColumn(str("Killed this round", "本轮杀过人"), ImGuiTableColumnFlags_NoSort);
                ImGui::TableSetupColumn(str("Death Time", "死亡时间"));
                ImGui::TableSetupColumn(str("Snapshot onDeath", "死亡快照"), ImGuiTableColumnFlags_NoSort);

                //ImGui::TableSetupColumn(str("Pos", "坐标"));

                ImGui::TableHeadersRow();

                //要显示附近玩家信息的死亡玩家下标
                static int deadplayerIndex = -1;
                bool hasOpenedPopup = false;
                const char* playerDeathSnapshotPopup = "playerDeathSnapshot_Popup";

                //TODO:待完善排序
                //TODO: 以下代码排序会导致错误的重置玩家数据，从而引发死亡玩家附近的玩家数据丢失
                /*
                ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs();

                if (sorts_specs->SpecsDirty)
                {
                    ImGuiSortDirection sortDirection;
                    for (int n = 0; n < sorts_specs->SpecsCount; n++)
                    {
                        const ImGuiTableColumnSortSpecs* sort_spec = &sorts_specs->Specs[n];
                        if (sort_spec->SortDirection != ImGuiSortDirection_None) {
                            sortDirection = sort_spec->SortDirection;
                            break;
                        }
                    }

                    if (sortDirection == ImGuiSortDirection_Ascending) {
                        sort(g_client.playerControllers.begin(), g_client.playerControllers.end(), [](const PlayerController* lhs, const PlayerController* rhs) {
                            return lhs->i_timeOfDeath < rhs->i_timeOfDeath;
                            }
                        );
                    }
                    else {
                        sort(g_client.playerControllers.begin(), g_client.playerControllers.end(), [](const PlayerController* lhs, const PlayerController* rhs) {
                            return lhs->i_timeOfDeath >= rhs->i_timeOfDeath;
                            }
                        );
                    }

                    sorts_specs->SpecsDirty = false;
                }
                */

                //PlayerController* player = g_client.playerControllers;
                auto playerControllers = g_client.playerControllers;
                for (int i = 0; i < g_client.playerControllers.size(); i++)
                {
                    PlayerController* ptr_playerController = g_client.playerControllers[i];

                    //跳过无效玩家和本地玩家
                    if (ptr_playerController->address == NULL || /*ptr_playerController->b_isLocal ||*/ ptr_playerController->nickname == "") {
                        continue;
                    }
                    ImGui::TableNextRow();

                    //昵称
                    ImGui::TableNextColumn(); ImGui::Text(ptr_playerController->nickname.c_str());

                    //角色
                    ImGui::TableNextColumn(); ImGui::Text(ptr_playerController->roleName.c_str());

                    //本轮杀过人
                    if (ptr_playerController->b_hasKilledThisRound) {
                        ImGui::TableNextColumn(); ImGui::Text(str("Yes", "是"));
                    }
                    else {
                        ImGui::TableNextColumn(); ImGui::Text(str("", ""));
                    }

                    //死亡时间
                    if (ptr_playerController->i_timeOfDeath != 0) {
                        ImGui::TableNextColumn(); ImGui::Text("%d", ptr_playerController->i_timeOfDeath);
                    }
                    else {
                        ImGui::TableNextColumn(); ImGui::Text("");
                    }

                    //死亡快照
                    //添加一个按钮，首先判断玩家是否死亡，死亡的话就显示按钮。按钮点击即可显示地图，显示当时玩家的位置

                    ImGui::TableNextColumn();

                    //只有死亡玩家有按钮
                    if (ptr_playerController->i_timeOfDeath > 0) {
                        if (ImGui::Button((std::string(str("Show", "显示")) + "##" + std::to_string(i)).c_str())) {
                            ImGui::OpenPopup(playerDeathSnapshotPopup);
                            deadplayerIndex = i;
                        }

                        if (deadplayerIndex >= 0) {
                            if (!hasOpenedPopup) {
                                hasOpenedPopup = true;
                                drawPlayerDeathSnapshot(g_client.playerControllers[deadplayerIndex], playerDeathSnapshotPopup);
                            }
                        }
                    }
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        //菜单3
        if (ImGui::BeginTabItem(str("Misc", "功能类")))
        {
            //TODO:失效
            //ImGui::BeginDisabled();
            ImGui::Checkbox(str("Remove fog of war", "隐藏战争迷雾"), &hackSettings.guiSettings.b_disableFogOfWar);
            HelpMarker(
                str("Remove shadows and let you see other players behind walls", "可以透过墙看到和听到其他玩家，隐藏视野阴影")
            );
            //ImGui::EndDisabled();

            ImGui::Checkbox(str("Noclip", "穿墙"), &hackSettings.guiSettings.b_alwaysEnableNoclip);
            HelpMarker(
                str("Walk through anything\nYou can press Left ALT to temporarily enable noclip", "穿墙模式\n长按左ALT键来临时穿墙")
            );
            ImGui::SameLine();
            //ImGui::TextDisabled(str("Not available now due to game update", "由于更新暂不可用"));
            ImGui::EndTabItem();
        }

        //菜单
        if (ImGui::BeginTabItem(str("Tasks", "任务")))
        {

            ImGui::Text(str("Tasks num:", "任务数量：")); ImGui::SameLine();

            //显示任务的数量
            ImGui::Text("%d", g_client.lobbySceneHandler.tasksHandler.tasksNum);

            ImGui::Text(str("Game start time:", "游戏开始时间：")); ImGui::SameLine();

            if (g_client.gameHasStarted()) {
                ImGui::Text("%d", g_client.timeSinceGameStarted());
            }
            else {
                //游戏未开始
                ImGui::TextDisabled(str("N/A", "未开始"));
            }

            //显示是否在房间中
            ImGui::Text(str("In room: ", "在房间中: "));
            ImGui::SameLine();
            if (g_client.inGameScene()) {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), str("Yes", "是"));
                //ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Yellow");
            }
            else {
                ImGui::TextDisabled(str("No", "否"));
            }

            //显示准备状态
            ImGui::Text(str("Ready status: ", "准备状态: "));
            ImGui::SameLine();
            if (g_client.localPlayerReadied()) {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), str("Readied", "已准备"));
                //ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Yellow");
            }
            else {
                ImGui::TextDisabled(str("Not ready", "未准备"));
            }

            ImGui::NewLine();

            ImGui::Text(str("Delayed time for completing tasks", "开局延时自动完成任务")); HelpMarker(str("Tasks will not be completed until game has begined ? seconds ago", "自动完成任务将在游戏开局?秒后才会生效"));
            ImGui::SetNextItemWidth(6.0f * ImGui::GetFontSize());
            ImGui::SameLine(); ImGui::InputFloat("##CompleteTasks_f_delayedEnableTime", &hackSettings.guiSettings.f_delayedEnableTime, 10.0f, 15.0f, "%.0f");
            ImGui::SameLine(); ImGui::Text(str("sec", "秒"));

            ImGui::Checkbox(str("Auto Complete Tasks + Auto Ready", "自动完成任务+自动准备"), &hackSettings.guiSettings.b_autoCompleteTasks_and_autoReady);
            ImGui::Checkbox(str("Auto Complete Tasks", "仅自动完成任务"), &hackSettings.guiSettings.b_autoCompleteTasks);
            ImGui::Checkbox(str("Auto Ready", "仅自动准备"), &hackSettings.guiSettings.b_autoReady);

            /*无单独完成任务功能，暂时注释掉
            //显示所有任务
            if (ImGui::BeginTable("tasks_table", 3,
                ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
            ))
            {
                //设置表头
                ImGui::TableSetupColumn(str("Index", "索引"));
                ImGui::TableSetupColumn(str("Task Id", "任务Id"));
                ImGui::TableSetupColumn(str("One tap complete task", "一键完成任务"));
                ImGui::TableHeadersRow();

                //取消显示拖拽和名称
                ImGuiColorEditFlags colorEditFlags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs;

                int index = 0;
                for (const auto& task : g_client.lobbySceneHandler.tasksHandler.assignedTasks) {
                    ImGui::TableNextRow();

                    //索引
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", index++);

                    //任务Id
                    ImGui::TableNextColumn();
                    ImGui::Text(string(task.taskId).get_std_string().c_str());

                    //一键完成任务
                    ImGui::TableNextColumn();
                    if (ImGui::Button(str("Complete", "完成"))) {
                        // 完成任务
                    }
                }

                ImGui::EndTable();
            }
            */

            ImGui::EndTabItem();
        }

        //菜单4
        if (ImGui::BeginTabItem(str("README", "说明")))
        {
            //显示版本信息
            ImGui::Text(str("Version: ", "版本: ")); ImGui::SameLine(); ImGui::Text(hackSettings.guiSettings.hackVersion.c_str());
            ImGui::Text(str("This an free software from Liuhaixv", "这是一个来自Liuhaixv的免费项目"));
            ImGui::SameLine();
            if (ImGui::Button(str("Link to project", "查看项目"))) {
                ShellExecute(0, 0, "https://github.com/Liuhaixv/Goose_Goose_Duck_Hack", 0, 0, SW_SHOW);
            }

            ImGui::EndTabItem();
        }

        //菜单5
        if (hackSettings.guiSettings.b_debug && ImGui::BeginTabItem(str("ESP", "透视")))
        {
            ImGui::Text(str("Button below is just for testing if overlay works", "下面的按钮目前只是为了测试绘制能否正常工作"));
            ImGui::Checkbox(str("Enable ESP", "全局开关"), &hackSettings.guiSettings.b_enableESP);
            HelpMarker(
                str("Create Issue to report bug if you can't see two green lines and yellow rect line", "如果你看不到屏幕上有横竖两条绿线以及环绕整个显示器的黄色矩形的话,请到Issue提交bug")
            );

            ImGui::EndTabItem();
        }

        //菜单6
        if (ImGui::BeginTabItem(str("Secret zone", "秘密菜单")))
        {
            //选择进程
            {
                //游戏进程ID
                ImGui::Text(str("PID:", "PID: "));
                ImGui::SameLine();

                ImGui::Text("%d", memory.pID); ImGui::SameLine();

                static int selected_process = -1;
                const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };

                //枚举所有游戏进程
                if (ImGui::Button(str("Select Process", "选择进程"))) {
                    memory.searchGameProcess();
                    ImGui::OpenPopup("select_process_popup");
                }
                if (ImGui::BeginPopup("select_process_popup"))
                {
                    ImGui::Separator();
                    if (memory.pIDs.size() == 0) {
                        //没有可用进程
                        ImGui::Text(str("None", "无"));
                    }
                    else {
                        for (int i = 0; i < memory.pIDs.size(); i++) {
                            if (ImGui::Selectable(std::to_string(memory.pIDs[i]).c_str(), memory.pIDs[i])) {
                                //附加到进程
                                memory.attachToGameProcess(memory.pIDs[i]);
                            }
                        }
                    }

                    ImGui::EndPopup();
                }
            }

            //反封禁
            {
                ImGui::Checkbox(str("Bypass Ban", "反封禁"), &hackSettings.guiSettings.b_bypassNormalBan);
                HelpMarker(
                    str("Allow you to play game with banned account", "获得可以使用被封禁的账号进行游戏的能力")
                );
            }

            //反挂机
            {
                ImGui::Checkbox(str("No idle kick", "反挂机"), &hackSettings.guiSettings.b_antiIdleKick);
                HelpMarker(
                    str("You will not be kicked automatically if idled for too much time in room", "你将不会因为挂机而被系统自动踢出房间")
                );

            }

            ImGui::Checkbox(str("Enable debug", "开启调试"), &hackSettings.guiSettings.b_debug);

            //秘密菜单
            if (hackSettings.guiSettings.b_debug) {
                ImGui::Checkbox(str("Disable write memory", "禁用写入内存"), &hackSettings.b_debug_disableWriteMemory);

                ImColor& test_color = userSettings.getColor(UserSettingsNames::test_color_0, ImColor(1.0f, 0.0f, 0.0f));
                //测试颜色
                ImGui::TextColored(test_color, str("Test color", "测试颜色"));
                ImGui::ColorEdit3("Coloredit3_test", &test_color.Value.x, ImGuiColorEditFlags_NoInputs);

                //测试VirtualAllocEx
                {

                    static int64_t allocatedAddress = 0x0;
                    static std::stringstream s;
                    static std::string hexStringAddress;

                    s.clear();
                    if (ImGui::Button(str("Allocate memory", "申请内存空间"))) {
                        allocatedAddress = (int64_t)VirtualAllocEx(memory.processHandle,
                            NULL,
                            0x1000,
                            MEM_COMMIT | MEM_RESERVE,
                            PAGE_EXECUTE_READWRITE);
                        s << std::hex << allocatedAddress;
                        hexStringAddress.clear();
                        s >> hexStringAddress;
                    }
                    auto a = NULL;
                    ImGui::Text("Allocated memory's base address: "); ImGui::SameLine();
                    ImGui::Text(hexStringAddress.c_str());

                }
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

/// <summary>
/// 全屏绘制
/// </summary>
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

#define STB_IMAGE_IMPLEMENTATION

#include "Drawing.h"

#include "../Memory/memory.h"
#include "../Client.h"
#include "../Struct/UserSettings.hpp"
#include <sstream>
#include "../Class/Game/string.hpp"
#include<algorithm>

#include "../Class/HttpDataUpdater.h"
#include "../Class/DebugConsole.h"
#include "../Class/MelonLoaderHelper.h"
//#include "Struct/UserSettings.hpp"

#include "IconsFontAwesome6Pro.h"
#include "../Password_Offline.h"
#include "../obfuscator.hpp"

#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR)))) 

LPCSTR Drawing::lpWindowName = "ImGui Standalone";
ImVec2 Drawing::vWindowSize = { 900,500 };
ImGuiWindowFlags Drawing::WindowFlags = /*ImGuiWindowFlags_NoSavedSettings |*/ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
//bool Drawing::bDraw = true;

extern Utils utils;
extern HackSettings hackSettings;
extern Client g_client;
extern Memory memory;

extern UserSettings userSettings;
extern HttpDataUpdater httpDataUpdater;

extern DebugConsole debugConsole;

#define str(eng,cn) utils.b_useChineseLanguage?(const char*)u8##cn:(const char*)u8##eng

#define EnOrCn(a, b) utils.b_useChineseLanguage?b:a

#define icon_str(icon,str) (std::string(icon)+" "+std::string(str)).c_str()

void drawMinimap();
void drawMenu();
void drawESP();
void drawVerification();

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
        do {
            if (hackSettings.guiSettings.b_hasVerified == false) {
                static bool hasInitedFromSettings = false;//是否从配置文件判断过验证

                if (hasInitedFromSettings == false) {
                    //从配置文件读取判断是否正确
                    int captcha = userSettings.getInt(u8"captcha", 0);

                    if (to_string(captcha) == HashVerificator::get_password(HashVerificator::get_hwid_password())) {
                        hackSettings.guiSettings.b_hasVerified = true;
                        continue;
                    }

                    hasInitedFromSettings = true;
                }

                //验证
                drawVerification();
            }

            if (hackSettings.guiSettings.b_hasVerified == false) {
                continue;
            }

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
        } while (false);
    }
}

//TODO:
//显示调试Log窗口
// Demonstrate creating a simple log window with basic filtering.
static void ShowExampleAppLog(bool* p_open)
{

    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
    // Most of the contents of the window will be added by the log.Draw() call.
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Debug log console", p_open);
    if (ImGui::SmallButton("[Debug] Add 5 entries"))
    {
        for (int n = 0; n < 5; n++)
        {
            debugConsole.log("test");
        }
    }
    ImGui::End();

    // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
    debugConsole.Draw("Debug log console", p_open);
}

static void ShowDemoWindowPopups()
{
    if (!ImGui::CollapsingHeader("Popups & Modal windows"))
        return;

    // The properties of popups windows are:
    // - They block normal mouse hovering detection outside them. (*)
    // - Unless modal, they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
    // - Their visibility state (~bool) is held internally by Dear ImGui instead of being held by the programmer as
    //   we are used to with regular Begin() calls. User can manipulate the visibility state by calling OpenPopup().
    // (*) One can use IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) to bypass it and detect hovering even
    //     when normally blocked by a popup.
    // Those three properties are connected. The library needs to hold their visibility state BECAUSE it can close
    // popups at any time.

    // Typical use for regular windows:
    //   bool my_tool_is_active = false; if (ImGui::Button("Open")) my_tool_is_active = true; [...] if (my_tool_is_active) Begin("My Tool", &my_tool_is_active) { [...] } End();
    // Typical use for popups:
    //   if (ImGui::Button("Open")) ImGui::OpenPopup("MyPopup"); if (ImGui::BeginPopup("MyPopup") { [...] EndPopup(); }

    // With popups we have to go through a library call (here OpenPopup) to manipulate the visibility state.
    // This may be a bit confusing at first but it should quickly make sense. Follow on the examples below.

    if (ImGui::TreeNode("Popups"))
    {
        ImGui::TextWrapped(
            "When a popup is active, it inhibits interacting with windows that are behind the popup. "
            "Clicking outside the popup closes it.");

        static int selected_fish = -1;
        const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
        static bool toggles[] = { true, false, false, false, false };

        // Simple selection popup (if you want to show the current selection inside the Button itself,
        // you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
        if (ImGui::Button("Select.."))
            ImGui::OpenPopup("my_select_popup");
        ImGui::SameLine();
        ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
        if (ImGui::BeginPopup("my_select_popup"))
        {
            ImGui::Text("Aquarium");
            ImGui::Separator();
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                if (ImGui::Selectable(names[i]))
                    selected_fish = i;
            ImGui::EndPopup();
        }

        // Showing a menu with toggles
        if (ImGui::Button("Toggle.."))
            ImGui::OpenPopup("my_toggle_popup");
        if (ImGui::BeginPopup("my_toggle_popup"))
        {
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                ImGui::MenuItem(names[i], "", &toggles[i]);
            if (ImGui::BeginMenu("Sub-menu"))
            {
                ImGui::MenuItem("Click me");
                ImGui::EndMenu();
            }

            ImGui::Separator();
            ImGui::Text("Tooltip here");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("I am a tooltip over a popup");

            if (ImGui::Button("Stacked Popup"))
                ImGui::OpenPopup("another popup");
            if (ImGui::BeginPopup("another popup"))
            {
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    ImGui::MenuItem(names[i], "", &toggles[i]);
                if (ImGui::BeginMenu("Sub-menu"))
                {
                    ImGui::MenuItem("Click me");
                    if (ImGui::Button("Stacked Popup"))
                        ImGui::OpenPopup("another popup");
                    if (ImGui::BeginPopup("another popup"))
                    {
                        ImGui::Text("I am the last one here.");
                        ImGui::EndPopup();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndPopup();
            }
            ImGui::EndPopup();
        }

        // Call the more complete ShowExampleMenuFile which we use in various places of this demo
        if (ImGui::Button("With a menu.."))
            ImGui::OpenPopup("my_file_popup");
        if (ImGui::BeginPopup("my_file_popup", ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"))
                {
                    ImGui::MenuItem("Dummy");
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::Text("Hello from popup!");
            ImGui::Button("This is a dummy button..");
            ImGui::EndPopup();
        }

        ImGui::TreePop();
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

static void HintUpdateModIfFailed(bool failed) {
    if (failed) {
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 0), str("Failed", "失败"));
        HelpMarker(str("Failed to execute command\nCheck if your mod is up to date", "执行命令失败！\n检查你的mod是否是最新版本"));
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
        if (ImGui::BeginTable("minimap_color_settings_table", 3,
            ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoBordersInBody
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


                    static time_t lastTimeClickCheckVersions = -1;

                    //点击版本按钮
                    if (ImGui::Button(hackSettings.guiSettings.hackVersion.c_str())) {

                        time(&lastTimeClickCheckVersions);

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

                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            //最新辅助版本
                            ImGui::Text(str("Latest hack version:", "最新辅助版本:"));
                            ImGui::TableNextColumn();
                            if (!hackSettings.latestVersions.hasUpdatedLatestVersions()) {
                                //显示加载动画
                                Spinner("latest_hack_version_spinner", 10.0f, 2.0f, ImColor(0, 255, 0));
                            }
                            else {
                                //显示数据
                                if (time(NULL) - lastTimeClickCheckVersions < 2) {
                                    //如果动画没有播放到指定时长仍显示加载动画
                                    //显示加载动画
                                    Spinner("latest_hack_version_spinner", 10.0f, 2.0f, ImColor(0, 255, 0));
                                }
                                else {
                                    ImGui::TextDisabled("%s%s",
                                        "",
                                        hackSettings.latestVersions.latestHackVersion);
                                }
                            }

                            //提示是否是最新版本,下载最新版本
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();

                            //有服务器版本信息
                            if (hackSettings.latestVersions.hasUpdatedLatestVersions()) {
                                //
                                if (utils.isLatestHackVersion(hackSettings.guiSettings.hackVersion, hackSettings.latestVersions.latestHackVersion)) {
                                    ImGui::TextColored(ImColor(0, 255, 0), str("Up to date Version", "已是最新版本!"));
                                }
                                else {
                                    ImGui::TextColored(ImColor(255, 255, 0), str("Update available", "新版本已可用"));
                                    ImGui::TableNextColumn();
                                    if (ImGui::Button(str("Download##latest_hack", "下载##最新版本"))) {
                                        ShellExecute(0, 0, hackSettings.latestVersions.url.c_str(), 0, 0, SW_SHOW);
                                    }
                                }
                            }

                            ImGui::EndTable();
                        }

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

                //切换语言
                {
                    ImGui::TableNextColumn();

                    //ImGui::PushFont(ImGui::GetIO().Fonts->Fonts.back());
                    if (ImGui::Button((std::string(ICON_FA_GLOBE) + "##language").c_str())) {
                        utils.changeLanguage();
                    }
                    //ImGui::PopFont();
                    ImGui::SameLine();
                    ImGui::Text("Change Language");

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
            //彩虹换色
            /*
            {
                ImGui::Checkbox(str("Rainbow", "彩虹"), &hackSettings.guiSettings.b_enableRainbowColors);
                HelpMarker(str("You are the most shining one","变成最闪耀的那一个"));
            }*/

            float minSpeed = hackSettings.gameOriginalData.f_baseMovementSpeed;
            if (minSpeed <= 0) {
                minSpeed = 5.0f;
            }

            //ImGui::BeginDisabled();
            ImGui::Checkbox(icon_str(ICON_FA_CLOCK_ROTATE_LEFT, str("Remove skill cooldown", "移除技能冷却时间")), &hackSettings.b_removeSkillCoolDown);
            HelpMarker(str("Notice: server - sided CD can not be removed", "注意：服务器端CD无法修改，如鸭子刀人CD"));
            //::EndDisabled();

            ImGui::Checkbox(icon_str(ICON_FA_PERSON_RUNNING, str("##Enable speedHack", "##启用 speedHack")), &hackSettings.guiSettings.b_enableSpeedHack);
            ImGui::SameLine();
            ImGui::BeginDisabled(!hackSettings.guiSettings.b_enableSpeedHack);
            ImGui::SliderFloat(
                str("Movement speed", "移速"),
                &hackSettings.guiSettings.f_movementSpeed,
                minSpeed,
                minSpeed * 2
            );
            ImGui::EndDisabled();

            ImGui::Checkbox(icon_str(ICON_FA_MAGNIFYING_GLASS_PLUS, str("##Enable zoomHack", "##启用 zoomHack")), &hackSettings.guiSettings.b_enableZoomHack);
            ImGui::SameLine();
            ImGui::BeginDisabled(!hackSettings.guiSettings.b_enableZoomHack);
            ImGui::SliderFloat(
                str("Zoom Size", "相机缩放"),
                &hackSettings.guiSettings.f_zoomSize,
                0.5,
                40
            );
            ImGui::EndDisabled();

            //玩家移速
            //ImGui::Text("{%.2f, %.2f}", playerController->v3_position.x, playerController->v3_position.y);

            ImGui::EndTabItem();
        }

        //角色信息菜单
        if (ImGui::BeginTabItem(str("Players Info", "角色信息")))
        {
            if (ImGui::BeginTable("players_info_table", 5,
                ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
                /*TODO: 由于Bug暂时禁用排序 | ImGuiTableFlags_Sortable*/
            ))
            {
                ImGui::TableSetupColumn(str("Nickname", "昵称"), ImGuiTableColumnFlags_NoSort);
                ImGui::TableSetupColumn(str("Role", "身份"), ImGuiTableColumnFlags_NoSort);
                ImGui::TableSetupColumn(str("Possible role", "可能的身份"), ImGuiTableColumnFlags_NoSort);//TODO:
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

                    //角色身份
                    ImGui::TableNextColumn(); ImGui::Text(ptr_playerController->roleName.c_str());

                    //TODO: 可能的身份
                    ImGui::TableNextColumn();
                    {
                        ImGui::Text(ICON_FA_SQUARE_QUESTION);
                        HelpMarker(str("Not available now", "功能尚未开发完毕"));
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

        //功能类菜单
        if (ImGui::BeginTabItem(str("Misc", "功能类")))
        {
            //TODO:失效
            //ImGui::BeginDisabled();
            ImGui::Checkbox(icon_str(ICON_FA_SMOKE, str("Remove fog of war", "隐藏战争迷雾")), &hackSettings.guiSettings.b_disableFogOfWar);
            HelpMarker(
                str("Remove shadows and let you see other players behind walls", "可以透过墙看到和听到其他玩家，隐藏视野阴影")
            );
            //ImGui::EndDisabled();

            ImGui::Checkbox(icon_str((const char*)u8"\ue3db", str("Noclip", "穿墙")), &hackSettings.guiSettings.b_alwaysEnableNoclip);
            HelpMarker(
                str("Walk through anything\nYou can press Left ALT to temporarily enable noclip", "穿墙模式\n长按左ALT键来临时穿墙")
            );
            ImGui::SameLine();
            //ImGui::TextDisabled(str("Not available now due to game update", "由于更新暂不可用"));
            ImGui::EndTabItem();
        }

        //任务菜单
        if (ImGui::BeginTabItem(str("Tasks", "任务")))
        {
            if (ImGui::CollapsingHeader(str("Patched", "已废弃功能"))) {

                ImGui::TextColored(ImColor(255, 0, 0), str("Not recommend to use due to server-side checking\nServer may kick you instantly", "由于服务器添加了坐标等验证，自动完成任务会导致你被踢出游戏"));
                ImGui::NewLine();

                ImGui::Checkbox(str("Enable##auto_tasks", "启用##auto_tasks"), &hackSettings.guiSettings.b_enable_autoTasks_and_autoReady);

                ImGui::BeginDisabled(!hackSettings.guiSettings.b_enable_autoTasks_and_autoReady);

                if (ImGui::BeginTable("float_settings_for_autoTasks", 2,
                    ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_NoBordersInBody
                ))
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    ImGui::Text(str("Delayed time for completing tasks", "开局延迟自动完成任务")); HelpMarker(str("Tasks will not be completed until game has begined ? seconds ago", "自动完成任务将在游戏开局?秒后才会生效"));
                    ImGui::SetNextItemWidth(6.0f * ImGui::GetFontSize());

                    ImGui::TableNextColumn(); ImGui::InputFloat("##CompleteTasks_f_delayedEnableTime", &hackSettings.guiSettings.f_delayedEnableTime, 10.0f, 15.0f, "%.0f");
                    ImGui::SameLine(); ImGui::Text(str("sec", "秒"));

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    ImGui::Text(str("Interval between tasks", "最小间隔时间")); HelpMarker(str("Min interval between every completed task", "限制自动完成任务的间隔不小于?秒"));
                    ImGui::SetNextItemWidth(6.0f * ImGui::GetFontSize());

                    ImGui::TableNextColumn(); ImGui::InputFloat("##CompleteTasks_f_minInterval", &hackSettings.guiSettings.f_minInterval, 10.0f, 15.0f, "%.0f");
                    ImGui::SameLine(); ImGui::Text(str("sec", "秒"));

                    ImGui::EndTable();
                }

                //ImGui::NewLine();

                ImGui::Checkbox(str("Auto Complete Tasks", "自动完成任务"), &hackSettings.guiSettings.b_autoCompleteTasks);
                ImGui::Checkbox(str("Auto Ready", "自动准备"), &hackSettings.guiSettings.b_autoReady);
                ImGui::EndDisabled();

                ImGui::NewLine();
            }


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

            ImGui::Checkbox(str("One-tap completing task", "秒任务"), &hackSettings.guiSettings.b_oneTapCompletingTask);


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

            ImGui::Text(str("Donate via Alipay", "支付宝捐赠"));
            ImGui::SameLine();
            if (ImGui::Button(str("Donate##alipay", "捐赠##alipay"))) {
                static const char*  aliUrl = cryptor::create("https://ibb.co/Xb89VJF").decrypt();
                ShellExecute(0, 0, aliUrl, 0, 0, SW_SHOW);
            }

            ImGui::Text(str("Donate via Wechat", "微信捐赠"));
            ImGui::SameLine();
            if (ImGui::Button(str("Donate##wechat", "捐赠##wechat"))) {
                static  const char* wechatUrl = cryptor::create("https://ibb.co/9H9DbSk").decrypt();
                ShellExecute(0, 0, wechatUrl, 0, 0, SW_SHOW);
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
        if (ImGui::BeginTabItem(str("ML", "ML")))
        {
            //TODO：

            if (ImGui::Button(str("How to install mod##install mod", "安装mod教程##install mod"))) {
                ShellExecute(0, 0, "https://github.com/Liuhaixv/GGDH_ML", 0, 0, SW_SHOW);
            }
            ImGui::SameLine();
            ImGui::Text(str("Features below only work when you have installed GGD_Hack mod", "下面的所有功能只有安装了GGD_Hack的mod后才能使用"));

            ImGui::NewLine();

            //检查TCP连接状态
            {
                static int testTCP_connection = -1;
                if (ImGui::Button(icon_str(ICON_FA_NETWORK_WIRED, str("Test TCP connection with ML", "测试与ML框架的TCP连接")))) {
                    testTCP_connection = MelonLoaderHelper::testConnection();
                }
                ImGui::SameLine();
                switch (testTCP_connection) {
                case -1:
                    ImGui::TextDisabled(str("Unknown", "未知"));
                    break;
                case 0:
                    ImGui::TextColored(ImColor(255, 0, 0), str("Failed", "失败"));
                    HelpMarker(str("Check if you have installed MelonLoader\nCheck if you have installed mod\nCheck if game is running",
                        "检查是否正确安装MelonLoader框架\n检查是否正确安装了mod\n检查游戏是否正在运行"));
                    break;
                case 1:
                    ImGui::TextColored(ImColor(0, 255, 0), str("Success", "成功"));
                    break;
                }
            }

            ImGui::NewLine();

            //功能开始

            //地图Minimap传送
            {
                ImGui::Text(icon_str(ICON_FA_MAP_LOCATION_DOT, str("Minimap Teleport", "Minimap传送")));
                HelpMarker(str("Right click minimap to TP", "右键点击游戏内地图传送"));
            }

            //刺客可以狙击肉汁和大白鹅
            {
                ImGui::Text(icon_str(ICON_FA_CROSSHAIRS, str("Enhanced Assassin", "刺客增强版")));
                HelpMarker(str("Allow you to shoot Gravy and Goose(which has no skill)", "允许你狙击肉汁和大白鹅"));
            }

            //游戏内发送聊天消息
            {
                static bool sendChatFailed = false;
                const int chatMessageLen = 256;
                static char chatMessage[chatMessageLen] = "";
                ImGui::InputTextWithHint("##Chat input", icon_str(ICON_FA_MESSAGES, str("Enter chat message", "要发送的聊天消息")), chatMessage, chatMessageLen);
                ImGui::SameLine();

                if (ImGui::Button(str("Send##Send message ingame", "发送"))) {
                    sendChatFailed = !MelonLoaderHelper::sendChat(chatMessage);
                }
                HintUpdateModIfFailed(sendChatFailed);
            }


            //远程杀人
            {
                static bool remoteKillFailed = false;
                // 显示一个按钮
                if (ImGui::Button(icon_str(ICON_FA_KNIFE_KITCHEN, str("Remote Kill/Eat", "远程杀人/吃人")))) {
                    ImGui::OpenPopup("Select player to kill");  // 打开弹出窗口
                }
                HelpMarker(str("Pay attention to the remaining CD of kill\nOtherwise you will fail to kill", "注意杀戮剩余CD时间\n否则击杀无效"));

                // 显示弹出窗口选择要杀的人
                if (ImGui::BeginPopup("Select player to kill", ImGuiWindowFlags_AlwaysAutoResize)) {

                    ImGui::BeginTable("People Table", 2);

                    // 设置表格列宽
                    ImGui::TableSetupColumn(str("Nickname", "昵称"), ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Age", ImGuiTableColumnFlags_WidthStretch);

                    bool hasTargetData = false;
                    auto playerControllers = g_client.playerControllers;
                    for (int i = 0; i < g_client.playerControllers.size(); i++)
                    {
                        PlayerController* ptr_playerController = g_client.playerControllers[i];

                        //跳过无效玩家、本地玩家、死亡玩家
                        if (ptr_playerController->address == NULL ||
                            ptr_playerController->b_isLocal ||
                            ptr_playerController->nickname == "" ||
                            ptr_playerController->i_timeOfDeath != 0) {
                            continue;
                        }

                        hasTargetData = true;

                        ImGui::TableNextRow();

                        //昵称
                        ImGui::TableNextColumn();
                        ImGui::Text(ptr_playerController->nickname.c_str());

                        //击杀按钮
                        ImGui::TableNextColumn();

                        std::string killButtonName = icon_str(ICON_FA_CROSSHAIRS, str("Kill", "击杀"));

                        if (ImGui::Button((killButtonName + "##" + std::to_string(i)).c_str())) {
                            remoteKillFailed = !MelonLoaderHelper::remoteKill(ptr_playerController->userId);
                        }
                    }

                    // 结束ImGui表格布局
                    ImGui::EndTable();

                    if (!hasTargetData) {
                        ImGui::Text(icon_str((const char*)u8"\ue5c5", str("No targets available!", "当前无可用目标!")));
                    }
                    ImGui::EndPopup();
                }
                HintUpdateModIfFailed(remoteKillFailed);
            }

            //启动飞船
            {
                static bool moveShuttleFailed = false;
                if (ImGui::Button(icon_str(ICON_FA_SHUTTLE_SPACE, str("Remote control shuttle", "远程控制飞船")))) {
                    moveShuttleFailed = !MelonLoaderHelper::moveShuttle();
                }
                ImGui::SameLine();
                HelpMarker(str("Move shuttle in map of nexus colony\nYou can fart to move shuttle too", "点击按钮远程控制连结殖民地地图中的飞船\n你也可以通过放屁来移动飞船"));
                HintUpdateModIfFailed(moveShuttleFailed);
            }            

            //追踪所有玩家箭头
            {
                static bool showAllPlayers = false;
                if (ImGui::Button(icon_str(ICON_FA_LOCATION_ARROW, str("Track players", "追踪所有玩家")))) {
                    showAllPlayers = !MelonLoaderHelper::showAllPlayersArrow();
                }
                ImGui::SameLine();
                HelpMarker(str("Track all players with arrows", "箭头追踪所有玩家"));
                HintUpdateModIfFailed(showAllPlayers);
            }

            //自杀
            {
                static bool suicideFailed = false;
                if (ImGui::Button(icon_str(ICON_FA_SKULL_CROSSBONES, str("Suicide", "非自然死亡")))) {
                    suicideFailed = !MelonLoaderHelper::suicide();
                }
                ImGui::SameLine();
                HelpMarker(str("Suicide", "自杀"));
                HintUpdateModIfFailed(suicideFailed);
            }

            //静音其他玩家
            {
                static bool remoteSilenceFailed = false;
                // 显示一个按钮
                if (ImGui::Button(icon_str(ICON_FA_MICROPHONE_LINES_SLASH, str("Remote silence", "远程静音")))) {
                    ImGui::OpenPopup("Select player to silence");  // 打开弹出窗口
                }
                HelpMarker(str("Silence player per round\nSilencer only", "每轮可远程静音一名玩家\n仅限静语者鸭子"));

                // 显示弹出窗口选择要静音的人
                if (ImGui::BeginPopup("Select player to silence", ImGuiWindowFlags_AlwaysAutoResize)) {

                    ImGui::BeginTable("People Table##silence", 2);

                    // 设置表格列宽
                    ImGui::TableSetupColumn(str("Nickname", "昵称"), ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn(str("Button", "按钮"), ImGuiTableColumnFlags_WidthStretch);

                    bool hasTargetData = false;
                    auto playerControllers = g_client.playerControllers;
                    for (int i = 0; i < g_client.playerControllers.size(); i++)
                    {
                        PlayerController* ptr_playerController = g_client.playerControllers[i];

                        //跳过无效玩家、本地玩家、死亡玩家
                        if (ptr_playerController->address == NULL ||
                            ptr_playerController->b_isLocal ||
                            ptr_playerController->nickname == "" ||
                            ptr_playerController->i_timeOfDeath != 0) {
                            continue;
                        }

                        hasTargetData = true;

                        ImGui::TableNextRow();

                        //昵称
                        ImGui::TableNextColumn();
                        ImGui::Text(ptr_playerController->nickname.c_str());

                        //静音按钮
                        ImGui::TableNextColumn();

                        std::string silenceButtonName = icon_str(ICON_FA_MICROPHONE_LINES_SLASH, str("Silence", "静语"));

                        if (ImGui::Button((silenceButtonName + "##" + std::to_string(i)).c_str())) {
                            remoteSilenceFailed = !MelonLoaderHelper::silencePlayer(ptr_playerController->userId);
                        }
                    }

                    // 结束ImGui表格布局
                    ImGui::EndTable();

                    if (!hasTargetData) {
                        ImGui::Text(icon_str((const char*)u8"\ue5c5", str("No targets available!", "当前无可用目标!")));
                    }

                    ImGui::EndPopup();
                }

                HintUpdateModIfFailed(remoteSilenceFailed);
            }


            //远程吃尸体
            {
                //                
                static bool remoteEatFailed = false;
                if (ImGui::Button(icon_str(ICON_FA_UTENSILS, str("Launch Time", "午餐时刻")))) {
                    remoteEatFailed = !MelonLoaderHelper::remoteEat();
                }
                HelpMarker(str("Eat one random body", "随机吃掉一个尸体"));
                HintUpdateModIfFailed(remoteEatFailed);
            }

            //一键捡尸体
            {
                static bool throwAllBodiesFailed = false;
                if (ImGui::Button(icon_str(ICON_FA_PERSON_CARRY_BOX, str("Pick up all bodies", "一键捡尸")))) {
                    throwAllBodiesFailed = !MelonLoaderHelper::pickUpAllBodies();
                }
                ImGui::SameLine();
                HelpMarker(str("Pick up all bodies\n(You must be the Undertaker)", "捡起所有鸡腿\n(你必须是丧葬者)"));
                HintUpdateModIfFailed(throwAllBodiesFailed);
            }

            //RingBell
           //拉铃
            {
                static bool ringBellFailed = false;
                if (ImGui::Button(icon_str(ICON_FA_BELL_EXCLAMATION, str("Ring bell", "拉铃")))) {
                    ringBellFailed = !MelonLoaderHelper::ringBell();
                }
                ImGui::SameLine();
                HelpMarker(str("Ring bell from any distance", "远程拉铃"));
                HintUpdateModIfFailed(ringBellFailed);
            }

            ImGui::NewLine();

            //解锁使用装扮
            {
                static bool unlockAllItemsFailed = false;
                if (ImGui::Button((const char*)u8"\uf6e2 \uf553 \uf6e8 \uf5e4")) {
                    unlockAllItemsFailed = !MelonLoaderHelper::unlockAllItems();
                }
                ImGui::SameLine();
                HelpMarker(str("Grants you access to all unlockable items", "获取使用所有可解锁项的权限"));
                HintUpdateModIfFailed(unlockAllItemsFailed);
            }

            ImGui::EndTabItem();
        }
        //菜单7
        if (ImGui::BeginTabItem(str("Secret zone", "秘密菜单")))
        {
            //选择进程 TODO: 由于增加了验证，切换进程会导致已保存的验证码丢失，待解决
            /*
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
                                //防止重新验证
                                hackSettings.guiSettings.b_hasVerified = true;
                            }
                        }
                    }

                    ImGui::EndPopup();
                }
            }*/

            ImGui::BeginDisabled();

            //篡改版本号为2.17.00
            {
                ImGui::Checkbox("##enable_bypass_version", &hackSettings.guiSettings.b_bypassVersionCheck);
                ImGui::SameLine();
                ImGui::Text(str("Anti update", "反更新"));
                HelpMarker(
                    str("Must be checked to bypass update checking\nMust run cheat before game", "必须勾选以跳过版本更新\n必须在游戏启动前启动辅助")
                );
            }

            //反封禁
            {
                ImGui::Checkbox(str("Bypass Ban", "反封禁"), &hackSettings.guiSettings.b_bypassNormalBan);
                HelpMarker(
                    str("Allow you to play game with banned account", "获得可以使用被封禁的账号进行游戏的能力")
                );
            }
            ImGui::EndDisabled();

            //反挂机
            {
                ImGui::Checkbox(icon_str(ICON_FA_SNOOZE, str("No idle kick", "反挂机")), &hackSettings.guiSettings.b_antiIdleKick);
                HelpMarker(
                    str("You will not be kicked automatically if idled for too much time in room", "你将不会因为挂机而被系统自动踢出房间")
                );

            }

            ImGui::Checkbox(icon_str(ICON_FA_BUG, str("Enable debug", "开启调试")), &hackSettings.guiSettings.b_debug);


            //秘密菜单
            if (hackSettings.guiSettings.b_debug) {

                static bool b_open_debug_log = false;
                if (ImGui::Button(str("Debug log##secret zone", "Log窗口##secret zone"))) {
                    b_open_debug_log = true;
                }

                if (b_open_debug_log) {
                    ShowExampleAppLog(&b_open_debug_log);
                }

                ImGui::Checkbox(str("Disable write memory", "禁用写入内存"), &hackSettings.b_debug_disableWriteMemory);

                ImColor& test_color = userSettings.getColor(UserSettingsNames::test_color_0, ImColor(1.0f, 0.0f, 0.0f));
                //测试颜色
                ImGui::TextColored(test_color, str("Test color", "测试颜色"));
                ImGui::ColorEdit3("Coloredit3_test", &test_color.Value.x, ImGuiColorEditFlags_NoInputs);


                static std::stringstream ss;
                //测试VirtualAllocEx
                {
                    static int64_t allocatedAddress = 0x0;
                    static std::string hexStringAddress;

                    ss.clear();
                    if (ImGui::Button(str("Allocate memory", "申请内存空间"))) {
                        allocatedAddress = (int64_t)VirtualAllocEx(memory.processHandle,
                            NULL,
                            0x1000,
                            MEM_COMMIT | MEM_RESERVE,
                            PAGE_EXECUTE_READWRITE);
                        ss << std::hex << allocatedAddress;
                        hexStringAddress.clear();
                        ss >> hexStringAddress;
                    }
                    auto a = NULL;
                    ImGui::Text("Allocated memory's base address: "); ImGui::SameLine();
                    ImGui::Text(hexStringAddress.c_str());

                }

                //测试createRemoteThread
                {
                    static char createRemoteThreadStartAddress[2 + 8 * 2];//0x????????????????
                    static int64_t remoteThreadStartAddress = 0;
                    ImGui::InputText("createRemoteThread", createRemoteThreadStartAddress, 2 + 8 * 2);
                    if (ImGui::Button(str("Start thread", "启动线程"))) {
                        //获取地址
                        ss.clear();
                        ss << std::hex << createRemoteThreadStartAddress;
                        ss >> remoteThreadStartAddress;
                        memory.createRemoteThread(remoteThreadStartAddress);
                    }
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

void drawVerification()
{
    static std::string login_token = HashVerificator::get_hwid_password();

    ImGui::Begin(str("Welcome", "欢迎页"), NULL, ImGuiWindowFlags_AlwaysAutoResize |/* ImGuiWindowFlags_NoResize  |*/ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    if (ImGui::BeginTable("login_table", 1, ImGuiTableFlags_SizingStretchSame)) {

        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::TextColored(ImColor(255, 255, 0), str("This is a free cheat software", "这是一个免费辅助项目"));

        HelpMarker(str("If you bought this from anywhere, you are scammed", "如果你是从别处购买的，那么你被骗了"));
        //ImGui::Text(str("Input CAPTCHA to access", "输入验证码以继续"));

        if (ImGui::BeginTable("login_token", 2, ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextDisabled(str("Your Login Token:", "你的登录令牌:"));
            ImGui::TableNextColumn();
            ImGui::Text(login_token.c_str());

            ImGui::EndTable();
        }

        //6位数字
        static char captcha[6 + 1];
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::InputTextWithHint("##CAPTCHA", str("CAPTCHA", "验证码"), captcha, IM_ARRAYSIZE(captcha), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::BeginTable("get_captcha", 2, ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextDisabled(str("Where can I get my CAPTCHA", "如何获取验证码"));
            HelpMarker(str("Subscribe to wechat or join Discord\nSend your login token to bot",
                "关注微信号或Discord服务器\n发送登录令牌给机器人"));
            ImGui::TableNextColumn();

            if (ImGui::Button(ICON_FA_COMMENTS)) {
                //微信
                ImGui::OpenPopup("wechat_qr_popup");
            }
            ImGui::SameLine();

            if (ImGui::Button(ICON_FA_DISCORD)) {
                //discord
                ImGui::OpenPopup("discord_qr_popup");
            }

            if (ImGui::BeginPopup("wechat_qr_popup"))
            {
                if (ImGui::ImageButton(UI::wx_QR.texture, ImVec2(512, 512))) {
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("discord_qr_popup"))
            {
                if (ImGui::ImageButton(UI::dc_QR.texture, ImVec2(512, 512))) {
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            ImGui::EndTable();
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);

   
        static time_t lastTimeClickLogin = -1;
        static bool showLoginFailedIcon = false;
        if (ImGui::Button("Login", ImVec2(260.f, 50.f)))
        {
            lastTimeClickLogin = time(NULL);
        }

        ImGui::SameLine();

        if (lastTimeClickLogin != -1) {
            if (time(NULL) - lastTimeClickLogin <= 2 && lastTimeClickLogin) {
                showLoginFailedIcon = false;
                Spinner("login_spinner", 20.0f, 2.0f, ImColor(0, 255, 0));
            }
            else {
                //验证
                lastTimeClickLogin = -1;
                if (captcha == HashVerificator::get_password(login_token)) {
                    hackSettings.guiSettings.b_hasVerified = true;

                    //保存验证过正确的密码到配置文件
                    userSettings.getInt(u8"captcha", 0) = stoi(HashVerificator::get_password(login_token));
                }
                else {
                    showLoginFailedIcon = true;
                }
            }
        }

        if (showLoginFailedIcon) {

            ImGui::SetWindowFontScale(1.5f); // 设置新的缩放比例

            ImGui::TextColored(ImColor(255, 0, 0), ICON_FA_CIRCLE_XMARK);

            ImGui::SetWindowFontScale(1.0f); // 恢复原始缩放比例
        }



        ImGui::PopStyleVar();
        ImGui::TableNextColumn();

        ImGui::EndTable();
    }
    ImGui::End();
}

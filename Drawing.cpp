#include "Drawing.h"
#include "client.hpp"

LPCSTR Drawing::lpWindowName = "ImGui Standalone";
ImVec2 Drawing::vWindowSize = { 500, 500 };
ImGuiWindowFlags Drawing::WindowFlags = /*ImGuiWindowFlags_NoSavedSettings |*/ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
//bool Drawing::bDraw = true;

extern Utils utils;
extern HackSettings hackSettings;
extern Client* g_client;

//#define str(eng,cn) (const char*)u8##cn
//#define str(eng,cn) (const char*)u8##cn
#define str(eng,cn) utils.b_chineseOS?(const char*)u8##cn:eng

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
        //绘制菜单
        if (hackSettings.guiSettings.b_enableMenu) {
            drawMenu();
        }

        //ESP
        if (hackSettings.guiSettings.b_enableESP) {
            drawESP();
        }
        else {

            /*
            ImGui::ShowDemoWindow();

            ImGui::GetBackgroundDrawList()->AddCircleFilled(
                { 500,500 },
                30,
                ImColor{ 1.0f, 1.0f, 0.0f }
            );

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(1024, 768));
            ImGui::SetNextWindowBgAlpha(0.0f);
            ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

            auto pDrawList = ImGui::GetWindowDrawList();

            pDrawList->AddRect(ImVec2(10, 10), ImVec2(100, 100), ImColor(255, 0, 0));
            pDrawList->AddText(ImVec2(10, 10), ImColor(255, 0, 0), "test");

            ImGui::End();
            */
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

void drawMenu() {
    //ImGui::ShowDemoWindow();

    bool b_open = true;
    bool* ptr_bOpen = &b_open;

    //ImGui::Begin("A", ptr_bOpen, ImGuiWindowFlags_NoInputs);
    // 
    ImGui::Begin(str("Liuhaixv@github.com  ||  Press Insert to switch", "Liuhaixv@github.com  ||  Insert开关菜单"));

    //游戏状态指示
    //ImGui::

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("Main menu", tab_bar_flags))
    {
        //菜单3
        if (ImGui::BeginTabItem(str("LocalPlayer Info", "本地玩家信息")))
        {         
            ImGui::Text(g_client->localPlayer.playerController.nickname.c_str());

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

            ImGui::EndTabItem();
        }

        //菜单1
        if (ImGui::BeginTabItem(str("Players Info", "角色信息")))
        {
            if (ImGui::BeginTable("table1", 3,
                ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
            ))
            {
                ImGui::TableSetupColumn(str("Nickname","昵称"));
                ImGui::TableSetupColumn(str("Role", "角色"));
                ImGui::TableSetupColumn(str("Killed this round", "本轮杀过人"));
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
                        ImGui::TableNextColumn(); ImGui::Text(str("Yes","是"));
                    }
                    else {
                        ImGui::TableNextColumn(); ImGui::Text(str("",""));
                    }
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

        ImGui::EndTabBar();
    }
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
    drawList->AddLine({0, ImGui::GetIO().DisplaySize.y / 2 }, { ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y / 2 },
        lineColor, lineThichness);
    drawList->AddLine({ ImGui::GetIO().DisplaySize.x /2, 0 } , { ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y },
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

#include "Drawing.h"

LPCSTR Drawing::lpWindowName = "ImGui Standalone";
ImVec2 Drawing::vWindowSize = { 500, 500 };
ImGuiWindowFlags Drawing::WindowFlags = /*ImGuiWindowFlags_NoSavedSettings |*/ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
//bool Drawing::bDraw = true;

extern Utils utils;

void drawMenu();
void drawESP();

void Drawing::Active()
{
    UI::hackSettings->guiSettings.b_draw = true;
}

bool Drawing::isActive()
{
    return (UI::hackSettings->guiSettings.b_draw == true);
}

void Drawing::Draw() {
    static bool* b_previousEnableMenu = nullptr;
    if (isActive())
    {
        //绘制菜单
        if (UI::hackSettings->guiSettings.b_enableMenu) {
            drawMenu();
        }

        //ESP
        if (UI::hackSettings->guiSettings.b_enableESP) {
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
    ImGui::ShowDemoWindow();

    bool b_open = true;
    bool* ptr_bOpen = &b_open;

    //ImGui::Begin("A", ptr_bOpen, ImGuiWindowFlags_NoInputs);
    // 
    ImGui::Begin((const char*)u8"主菜单", ptr_bOpen, ImGuiWindowFlags_None);
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("Main menu", tab_bar_flags))
    {
        //菜单1
        if (ImGui::BeginTabItem((const char*)u8"角色信息"))
        {
            ImGui::Text((const char*)u8"玩家信息在这里显示");
            ImGui::EndTabItem();
        }
        //菜单2
        if (ImGui::BeginTabItem((const char*)u8"设置"))
        {
            ImGui::Checkbox((const char*)u8"隐藏战争迷雾", &UI::hackSettings->disableFogOfWar);
            HelpMarker(
                (const char*)u8"可以透过墙看到和听到其他玩家，隐藏视野阴影"
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

    drawList->AddCircleFilled(
        { 500,500 },
        30,
        ImColor{ 1.0f, 1.0f, 0.0f }
    );

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

#include "Drawing.h"

LPCSTR Drawing::lpWindowName = "ImGui Standalone";
ImVec2 Drawing::vWindowSize = { 500, 500 };
ImGuiWindowFlags Drawing::WindowFlags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
//bool Drawing::bDraw = true;

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
            ImGui::ShowDemoWindow();
        }

        //ESP
        if (UI::hackSettings->guiSettings.b_enableESP) {
            //TODO
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

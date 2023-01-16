#include<imgui.h>

/// <summary>
/// 保存string对应的属性值
/// </summary>
struct UserSettings {
    //自定义颜色
    std::map<std::string, ImColor> customColors = { {"testNum1", ImColor(1.0f, 0.5f, 0.0f)}};
    //自定义数字
    std::map<std::string, int> customNumbers = { {"testNum1", 1}};
    //自定义浮点数
    std::map<std::string, float> customFloats= { {"testFloat1", 1.223f} };
};

//全局变量保存用户配置
UserSettings userSettings;

static void UserSettingsHandler_ClearAll(ImGuiContext* ctx, ImGuiSettingsHandler*)
{
    /*
    ImGuiContext& g = *ctx;
    for (int i = 0; i != g.Windows.Size; i++)
        g.Windows[i]->SettingsOffset = -1;
    g.SettingsWindows.clear();
    */
}

static void* UserSettingsHandler_ReadOpen(ImGuiContext*, ImGuiSettingsHandler*, const char* name)
{
    ImGuiWindowSettings* settings = ImGui::FindOrCreateWindowSettings(name);
    ImGuiID id = settings->ID;
    *settings = ImGuiWindowSettings(); // Clear existing if recycling previous entry
    settings->ID = id;
    settings->WantApply = true;
    return (void*)settings;
}

static void UserSettingsHandler_ReadLine(ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line)
{
    /*
    ImGuiWindowSettings* settings = (ImGuiWindowSettings*)entry;
    int x, y;
    int i;
    ImU32 u1;
    if (sscanf(line, "Pos=%i,%i", &x, &y) == 2) { settings->Pos = ImVec2ih((short)x, (short)y); }
    else if (sscanf(line, "Size=%i,%i", &x, &y) == 2) { settings->Size = ImVec2ih((short)x, (short)y); }
    else if (sscanf(line, "ViewportId=0x%08X", &u1) == 1) { settings->ViewportId = u1; }
    else if (sscanf(line, "ViewportPos=%i,%i", &x, &y) == 2) { settings->ViewportPos = ImVec2ih((short)x, (short)y); }
    else if (sscanf(line, "Collapsed=%d", &i) == 1) { settings->Collapsed = (i != 0); }
    else if (sscanf(line, "DockId=0x%X,%d", &u1, &i) == 2) { settings->DockId = u1; settings->DockOrder = (short)i; }
    else if (sscanf(line, "DockId=0x%X", &u1) == 1) { settings->DockId = u1; settings->DockOrder = -1; }
    else if (sscanf(line, "ClassId=0x%X", &u1) == 1) { settings->ClassId = u1; }
    */
}

// Apply to existing windows (if any)
static void UserSettingsHandler_ApplyAll(ImGuiContext* ctx, ImGuiSettingsHandler*)
{
    /*
    ImGuiContext& g = *ctx;
    for (ImGuiWindowSettings* settings = g.SettingsWindows.begin(); settings != NULL; settings = g.SettingsWindows.next_chunk(settings))
        if (settings->WantApply)
        {
            if (ImGuiWindow* window = ImGui::FindWindowByID(settings->ID))
                ApplyWindowSettings(window, settings);
            settings->WantApply = false;
        }
    */
}

/*
//保存UserSettings
static void UserSettingsHandler_WriteAll(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf)
{
    buf->appendf("[%s][%s]\n", handler->TypeName, "data");

    buf->append("Test\n");
}
*/

//保存UserSettings
static void UserSettingsHandler_WriteAll(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf)
{
    buf->appendf("[%s][%s]\n", handler->TypeName, "customColors");
    for (auto color : userSettings.customColors) {
        const std::string* name = &color.first;
        const ImColor* colorV = &color.second;
        buf->appendf("%s=%f,%f,%f,%f\n", name,
            colorV->Value.x, colorV->Value.y, colorV->Value.z, colorV->Value.w);
    }

    buf->appendf("[%s][%s]\n", handler->TypeName, "customNumbers");
    for (auto num : userSettings.customNumbers) {
        const std::string* name = &num.first;
        const int* v = &num.second;
        buf->appendf("%s=%d\n", name, *v);
    }

    buf->appendf("[%s][%s]\n", handler->TypeName, "customFloats");
    for (auto float_ : userSettings.customFloats) {
        const std::string* name = &float_.first;
        const float* v = &float_.second;
        buf->appendf("%s=%f\n", name, *v);
    }

    buf->append("\n");
}
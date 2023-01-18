#define CustomName(customConstString)  constexpr const char8_t* customConstString = u8#customConstString

#include<imgui.h>
#include<imgui_internal.h>

namespace UserSettingsNames {
    CustomName(test_color_0);

    //定义字符串常量
    CustomName(minimap_color_circleFilled_dead);
    CustomName(minimap_color_circleFilled_alive);
    CustomName(minimap_color_circleFilled_local);

    CustomName(minimap_color_name_dead);
    CustomName(minimap_color_name_alive);
    CustomName(minimap_color_name_local);

    CustomName(minimap_fontSize_dead);
    CustomName(minimap_fontSize_alive);

    CustomName(minimap_radius_circleFilled_dead);
    CustomName(minimap_radius_circleFilled_alive);
    CustomName(minimap_radius_circleFilled_local);
}

namespace DefaultSettings {
    namespace SliderFloat {
        //static float speed = 1.0f;
        //设置调整玩家坐标点的范围
        static Vector2 size_of_circleFilled = { 2.0f, 25.0f };
    }

    namespace Color {
        static ImColor color_circleFilled_dead(1.0f, 1.0f, 1.0f);//死亡玩家是黑色
        static ImColor color_circleFilled_suspect(1.0f, 0.0f, 0.0f);//嫌疑人是红色
        static ImColor color_circleFilled_alive(255, 0, 0);//红色
        static ImColor color_circleFilled_local(255, 0, 0);//红色

        static ImColor color_nickname_alive(IM_COL32_WHITE);//白色
        static ImColor color_nickname_local(IM_COL32_WHITE);//白色
        static ImColor color_nickname_dead(IM_COL32_WHITE);//白色
    }

    namespace Size {
        static float f_radius_circleFilled_dead = 10;
        static float f_radius_circleFilled_alive = 5;
        static float f_radius_circleFilled_local = 5;
    }

    namespace Offset {
        //玩家昵称相对绘制的坐标点的偏移位置
        static Vector2 nickname_to_circleFilled{ 0.0f, 5.0f };
    }
}

/// <summary>
/// 保存string对应的属性值
/// </summary>
struct UserSettings {

    //自定义颜色
    std::map<std::string, ImColor> customColors = {
        //{"testColor", ImColor(1.0f, 0.5f, 0.0f)}
    };
    //自定义数字
    std::map<std::string, int> customInts = {
        //{"testNum1", 1}
    };
    //自定义浮点数
    std::map<std::string, float> customFloats = {
        //{"testFloat1", 1.223f}
    };

    ImColor& getColor(const char8_t* name, const ImColor deafultColorIfNotFound) {
        std::u8string nameStru8 = name;
        std::string nameStr = std::string(nameStru8.begin(), nameStru8.end());

        auto it = customColors.find(nameStr);
        if (it == customColors.end()) {
            customColors[nameStr] = ImColor(deafultColorIfNotFound);
        }
        return customColors[nameStr];
    }

    int& getInt(const char8_t* name, const int deafultIntIfNotFound) {
        std::u8string nameStru8 = name;
        std::string nameStr = std::string(nameStru8.begin(), nameStru8.end());

        auto it = customInts.find(nameStr);
        if (it == customInts.end()) {
            customInts[nameStr] = deafultIntIfNotFound;
        }
        return customInts[nameStr];
    }

    float& getFloat(const char8_t* name, const float deafultFloatIfNotFound) {
        std::u8string nameStru8 = name;
        std::string nameStr = std::string(nameStru8.begin(), nameStru8.end());

        auto it = customFloats.find(nameStr);
        if (it == customFloats.end()) {
            customFloats[nameStr] = deafultFloatIfNotFound;
        }
        return customFloats[nameStr];
    }

    /*
    const char* getLabelName(const char8_t* componentName, const char8_t* constUserSettingsName) {
        std::u8string connectedStrU8 = std::u8string(componentName).append(constUserSettingsName);

        return std::string(connectedStrU8.begin(), connectedStrU8.end()).c_str();
    }
    */

    /*
    template <typename V>
    V& GetWithDef(std::map <std::string, V>& m, std::string& key, const V& defval) {
        auto it = m.find(key);
        if (it == m.end()) {
            m[key] = defval;
            return m[key];
        }
        else {
            return m[key];
        }
    }
    */
};



extern UserSettings userSettings;

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

/// <summary>
/// 初始化UserSettings
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
/// <param name="entry"></param>
/// <param name="line"></param>
static void UserSettingsHandler_ReadLine(ImGuiContext* a, ImGuiSettingsHandler* b, void* entry, const char* line)
{
    if (strncmp(line, "ImColor", 7) == 0) {
        line = ImStrSkipBlank(line + strlen("ImColor"));
        char name[60];
        float x, y, z, w;
        if (sscanf(line, "%[^=]=%f,%f,%f,%f", name, &x, &y, &z, &w) == 5) {
            ImColor color(x, y, z, w);
            //读取成功
            userSettings.customColors[name] = color;
        }
        else {
            //读取失败
        }
    }
    else if (strncmp(line, "int", 3) == 0) {
        line = ImStrSkipBlank(line + strlen("int"));
        char name[60];
        int number;
        if (sscanf(line, "%[^=]=%d", name, &number) == 2) {
            //读取成功
            userSettings.customInts[name] = number;
        }
        else {
            //读取失败
        }
    }
    else if (strncmp(line, "float", 5) == 0) {
        line = ImStrSkipBlank(line + strlen("float"));
        char name[60];
        float floatNumber;
        if (sscanf(line, "%[^=]=%f", name, &floatNumber) == 2) {
            //读取成功
            userSettings.customFloats[name] = floatNumber;
        }
        else {
            //读取失败
        }
    }
    else return;


    char c = 0;
    int x = 0, y = 0;
    int r = 0;
    /*
    // Parsing, e.g.
    // " DockNode   ID=0x00000001 Pos=383,193 Size=201,322 Split=Y,0.506 "
    // "   DockNode ID=0x00000002 Parent=0x00000001 "
    // Important: this code expect currently fields in a fixed order.
    ImGuiDockNodeSettings node;
    line = ImStrSkipBlank(line);
    if (strncmp(line, "DockNode", 8) == 0) { line = ImStrSkipBlank(line + strlen("DockNode")); }
    else if (strncmp(line, "DockSpace", 9) == 0) { line = ImStrSkipBlank(line + strlen("DockSpace")); node.Flags |= ImGuiDockNodeFlags_DockSpace; }
    else return;
    if (sscanf(line, "ID=0x%08X%n", &node.ID, &r) == 1) { line += r; }
    else return;
    if (sscanf(line, " Parent=0x%08X%n", &node.ParentNodeId, &r) == 1) { line += r; if (node.ParentNodeId == 0) return; }
    if (sscanf(line, " Window=0x%08X%n", &node.ParentWindowId, &r) == 1) { line += r; if (node.ParentWindowId == 0) return; }
    if (node.ParentNodeId == 0)
    {
        if (sscanf(line, " Pos=%i,%i%n", &x, &y, &r) == 2) { line += r; node.Pos = ImVec2ih((short)x, (short)y); }
        else return;
        if (sscanf(line, " Size=%i,%i%n", &x, &y, &r) == 2) { line += r; node.Size = ImVec2ih((short)x, (short)y); }
        else return;
    }
    else
    {
        if (sscanf(line, " SizeRef=%i,%i%n", &x, &y, &r) == 2) { line += r; node.SizeRef = ImVec2ih((short)x, (short)y); }
    }
    if (sscanf(line, " Split=%c%n", &c, &r) == 1) { line += r; if (c == 'X') node.SplitAxis = ImGuiAxis_X; else if (c == 'Y') node.SplitAxis = ImGuiAxis_Y; }
    if (sscanf(line, " NoResize=%d%n", &x, &r) == 1) { line += r; if (x != 0) node.Flags |= ImGuiDockNodeFlags_NoResize; }
    if (sscanf(line, " CentralNode=%d%n", &x, &r) == 1) { line += r; if (x != 0) node.Flags |= ImGuiDockNodeFlags_CentralNode; }
    if (sscanf(line, " NoTabBar=%d%n", &x, &r) == 1) { line += r; if (x != 0) node.Flags |= ImGuiDockNodeFlags_NoTabBar; }
    if (sscanf(line, " HiddenTabBar=%d%n", &x, &r) == 1) { line += r; if (x != 0) node.Flags |= ImGuiDockNodeFlags_HiddenTabBar; }
    if (sscanf(line, " NoWindowMenuButton=%d%n", &x, &r) == 1) { line += r; if (x != 0) node.Flags |= ImGuiDockNodeFlags_NoWindowMenuButton; }
    if (sscanf(line, " NoCloseButton=%d%n", &x, &r) == 1) { line += r; if (x != 0) node.Flags |= ImGuiDockNodeFlags_NoCloseButton; }
    if (sscanf(line, " Selected=0x%08X%n", &node.SelectedTabId, &r) == 1) { line += r; }
    if (node.ParentNodeId != 0)
        if (ImGuiDockNodeSettings* parent_settings = DockSettingsFindNodeSettings(ctx, node.ParentNodeId))
            node.Depth = parent_settings->Depth + 1;
    ctx->DockContext.NodesSettings.push_back(node);
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
//TODO: 文件错误保存为GBK
static void UserSettingsHandler_WriteAll(ImGuiContext*, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf)
{
    buf->appendf("[%s][%s]\n", handler->TypeName, "customColors");
    for (auto color : userSettings.customColors) {
        const std::string* name = &color.first;
        const ImColor* colorV = &color.second;
        buf->appendf("ImColor %s=%f,%f,%f,%f\n", name->c_str(),
            colorV->Value.x, colorV->Value.y, colorV->Value.z, colorV->Value.w);
    }

    buf->appendf("[%s][%s]\n", handler->TypeName, "customNumbers");
    for (auto num : userSettings.customInts) {
        const std::string* name = &num.first;
        const int* v = &num.second;
        buf->appendf("int %s=%d\n", name->c_str(), *v);
    }

    buf->appendf("[%s][%s]\n", handler->TypeName, "customFloats");
    for (auto float_ : userSettings.customFloats) {
        const std::string* name = &float_.first;
        const float* v = &float_.second;
        buf->appendf("float %s=%f\n", name->c_str(), *v);
    }

    buf->append("\n");
}
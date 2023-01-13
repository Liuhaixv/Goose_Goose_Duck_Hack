#include "UI.h"

ID3D11Device* UI::pd3dDevice = nullptr;
ID3D11DeviceContext* UI::pd3dDeviceContext = nullptr;
IDXGISwapChain* UI::pSwapChain = nullptr;
ID3D11RenderTargetView* UI::pMainRenderTargetView = nullptr;

std::map<int, GameMap> UI::miniMaps;

HWND UI::hwnd = NULL;

extern Utils utils;

void getScaledResolution(int& resolutionX, int& resolutionY);

// Simple helper function to load an image into a DX11 texture with common settings
bool UI::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{

    ID3D11Device* pd3dDevice = UI::pd3dDevice;
    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();

    *out_width = image_width;
    *out_height = image_height;
    stbi_image_free(image_data);

    return true;
}

void UI::loadMapsTexture() {

    GameMap map0(u8"Ancient Sands", u8"古代沙地");
    GameMap map1(u8"The Basement", u8"地下室");
    GameMap map2(u8"Jungle Temple", u8"丛林神殿");
    GameMap map3(u8"GooseChapel", u8"鹅教堂");
    GameMap map4(u8"Mallard Manor", u8"马拉德庄园");
    GameMap map5(u8"NEXUS Colony", u8"连结殖民地");
    GameMap map6(u8"BlackSwan", u8"黑天鹅");
    GameMap map7(u8"SS MotherGoose", u8"老妈鹅星球飞船");

    UI::LoadTextureFromFile("./img/maps/0.png", &map0.texture, &map0.width, &map0.height);
    UI::LoadTextureFromFile("./img/maps/1.png", &map1.texture, &map1.width, &map1.height);
    UI::LoadTextureFromFile("./img/maps/2.png", &map2.texture, &map2.width, &map2.height);
    UI::LoadTextureFromFile("./img/maps/3.png", &map3.texture, &map3.width, &map3.height);
    UI::LoadTextureFromFile("./img/maps/4.png", &map4.texture, &map4.width, &map4.height);
    UI::LoadTextureFromFile("./img/maps/5.png", &map5.texture, &map5.width, &map5.height);
    UI::LoadTextureFromFile("./img/maps/6.png", &map6.texture, &map6.width, &map6.height);
    UI::LoadTextureFromFile("./img/maps/7.png", &map7.texture, &map7.width, &map7.height);

    //处理缩放
    map1.scaleToGamePosition = 78.48 / map1.width;
    //(5.84, 8.35) -(10.8, 5.26)
    map1.offset = { 5.84-10.8, 8.35-5.26};

    UI::miniMaps.insert(std::pair<int, GameMap>(ANCIENT_SANDS, map0));
    UI::miniMaps.insert(std::pair<int, GameMap>(THE_BASEMENT, map1));
    UI::miniMaps.insert(std::pair<int, GameMap>(JUNGLE_TEMPLE, map2));
    UI::miniMaps.insert(std::pair<int, GameMap>(GOOSECHAPEL, map3));
    UI::miniMaps.insert(std::pair<int, GameMap>(MALLARD_MANOR, map4));
    UI::miniMaps.insert(std::pair<int, GameMap>(NEXUS_COLONY, map5));
    UI::miniMaps.insert(std::pair<int, GameMap>(BLACKSWAN, map6));
    UI::miniMaps.insert(std::pair<int, GameMap>(SS_MOTHERGOOSE, map7));
}

bool UI::CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;

    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2;

    sd.OutputWindow = hWnd;
    sd.Windowed = TRUE;

    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.SampleDesc.Count = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    //sd.SampleDesc.Quality = 0;
    //sd.BufferDesc.Width = 0;
    //sd.BufferDesc.Height = 0;

    const UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };

    if (D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevelArray,
        2,
        D3D11_SDK_VERSION,
        &sd,
        &pSwapChain,
        &pd3dDevice,
        &featureLevel,
        &pd3dDeviceContext)
        != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void UI::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer != nullptr)
    {
        pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pMainRenderTargetView);
        pBackBuffer->Release();
    }
}

void UI::CleanupRenderTarget()
{
    if (pMainRenderTargetView)
    {
        pMainRenderTargetView->Release();
        pMainRenderTargetView = nullptr;
    }
}

void UI::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (pSwapChain)
    {
        pSwapChain->Release();
        pSwapChain = nullptr;
    }

    if (pd3dDeviceContext)
    {
        pd3dDeviceContext->Release();
        pd3dDeviceContext = nullptr;
}

    if (pd3dDevice)
    {
        pd3dDevice->Release();
        pd3dDevice = nullptr;
    }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

LRESULT WINAPI UI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;

    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;

    default:
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void UI::Render(HINSTANCE instance, INT cmd_show)
{
    ImGui_ImplWin32_EnableDpiAwareness();
    const WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0L, 0L,
        instance,
        nullptr, nullptr, nullptr, nullptr,
        "External Overlay Class",
        nullptr };

    ::RegisterClassEx(&wc);
    //const HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("ImGui Standalone"), WS_OVERLAPPEDWINDOW, 100, 100, 50, 50, NULL, NULL, wc.hInstance, NULL);

    //Get resolution
    int resolutionX = 500, resolutionY = 500;
    getScaledResolution(resolutionX, resolutionY);


    const HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
        wc.lpszClassName,
        "External Overlay",
        WS_POPUP,
        0, 0, resolutionX, resolutionY, nullptr, nullptr,
        wc.hInstance,
        nullptr
    );

    UI::hwnd = hwnd;

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

    {
        RECT client_area{};
        GetClientRect(hwnd, &client_area);

        RECT window_area{};
        GetClientRect(hwnd, &window_area);

        POINT diff{};
        ClientToScreen(hwnd, &diff);

        const MARGINS margins{
            window_area.left + (diff.x - window_area.left),
            window_area.top + (diff.y - window_area.top),
            client_area.right,
            client_area.bottom

        };

        DwmExtendFrameIntoClientArea(hwnd, &margins);
    }

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    ::ShowWindow(hwnd, cmd_show);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        //style.WindowRounding = 4.0f;
        //style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    const HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info = {};
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    const int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;

    ImFontConfig cfg;

    const float fScale = 2.0f;
    cfg.SizePixels = 13;

    if (monitor_height > 1080)
    {
        cfg.SizePixels = 13 * fScale;
    }
    //ImGui::GetIO().Fonts->AddFontDefault();

    //中文字体
    //chinese font
    if (utils.b_chineseOS) {
        ImGui::GetIO().Fonts->AddFontFromFileTTF(
            "c:/Windows/Fonts/simhei.ttf",
            cfg.SizePixels,
            NULL,
            ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());
    }
    else {
        ImGui::GetIO().Fonts->AddFontDefault(&cfg);
    }


    //保存GUI窗口信息
    ImGui::GetIO().IniFilename = "imgui.ini";

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(pd3dDevice, pd3dDeviceContext);

    const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //读取地图图片
    loadMapsTexture();

    bool bDone = false;
    while (!bDone)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                bDone = true;
        }
        if (bDone)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            Drawing::Draw();
        }

        ImGui::Render();

        const float clear_color_with_alpha[4] = { 0.0f,0.0f,0.0f,0.0f };
        //const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        pd3dDeviceContext->OMSetRenderTargets(1, &pMainRenderTargetView, nullptr);
        pd3dDeviceContext->ClearRenderTargetView(pMainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        //检测是否需要创建新window来容纳GUI窗口
        //TODO 将菜单窗口和绘制窗口分离，菜单窗口设置为可点击，绘制窗口设置为不可点击（鼠标点击穿透窗口到游戏）
        //imgui.cpp:6748 通过判断窗口是否在主窗口的矩形范围内来决定是否创建新的Viewport
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        pSwapChain->Present(1, 0);

        if (!Drawing::isActive())
            break;
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

//获取缩放后的分辨率
void getScaledResolution(int& x, int& y) {
    HDC hdc = GetDC(NULL);
    x = GetDeviceCaps(hdc, HORZRES);
    y = GetDeviceCaps(hdc, VERTRES);
}
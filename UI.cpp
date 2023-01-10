#include "UI.h"

ID3D11Device* UI::pd3dDevice = nullptr;
ID3D11DeviceContext* UI::pd3dDeviceContext = nullptr;
IDXGISwapChain* UI::pSwapChain = nullptr;
ID3D11RenderTargetView* UI::pMainRenderTargetView = nullptr;

HackSettings* UI::hackSettings = nullptr;
HWND UI::hwnd = NULL;

extern Utils utils;

void getScaledResolution(int& resolutionX, int& resolutionY);

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

void UI::Render(HackSettings* hackSettings, HINSTANCE instance, INT cmd_show)
{
    UI::hackSettings = hackSettings;

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
    int resolutionX = 500, resolutionY=500;
    getScaledResolution(resolutionX, resolutionY);


    const HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT,
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

    if (monitor_height > 1080)
    {
        const float fScale = 2.0f;
        ImFontConfig cfg;
        cfg.SizePixels = 13 * fScale;

        //ImGui::GetIO().Fonts->AddFontDefault();

        //中文字体
        //chinese font
        if (utils.b_chineseOS) {
            //TODO
            ImGui::GetIO().Fonts->AddFontFromFileTTF(
                "c:/Windows/Fonts/simhei.ttf",
                cfg.SizePixels,
                NULL,
                ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());
        }
        else {
            ImGui::GetIO().Fonts->AddFontDefault(&cfg);
        }
    }

    //保存GUI窗口信息
    ImGui::GetIO().IniFilename = "imgui.ini";

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(pd3dDevice, pd3dDeviceContext);

    const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
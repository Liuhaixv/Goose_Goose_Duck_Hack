#pragma once

#include"Struct/HackSettings.hpp"

#include <Windows.h>
#include <d3d11.h>
#include <tchar.h>

#include <imgui.h>
#include "Drawing.h"
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include<dwmapi.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class UI
{
public:
    static HackSettings* hackSettings;
    static HWND hwnd;

    static void Render(HackSettings* hackSettings, HINSTANCE instance, INT cmd_show);

    //可点击,鼠标操作不能穿透窗口
    //SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT);

    //不可点击,鼠标操作穿透窗口
    //SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED);
    static void makeWindowClickable(HWND hwnd, bool clickable = true) {
        if (clickable) {
            SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT);
        }
        else {
            SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED);
        }
    }
private:

	static ID3D11Device* pd3dDevice;
	static ID3D11DeviceContext* pd3dDeviceContext;
	static IDXGISwapChain* pSwapChain;
	static ID3D11RenderTargetView* pMainRenderTargetView;

	static bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void CreateRenderTarget();
	static void CleanupRenderTarget();
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
#pragma once

#include <Windows.h>

#include"Struct/HackSettings.hpp"
#include "ImGui/imgui.h"
#include"UI.h"

class Drawing
{
private:
	static LPCSTR lpWindowName;
	static ImVec2 vWindowSize;
	static ImGuiWindowFlags WindowFlags;
	//static bool bDraw;

public:
	static void Active();
	static bool isActive();
	static void Draw();
};

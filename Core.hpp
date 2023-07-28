#pragma once

#include "includes.hpp"

#include "d3d_Hook.hpp"
#include "Settings.hpp"
#include "Visuals.hpp"
#include "Player.hpp"
#include "Aimbot.hpp"
#include "Weapon.hpp"
#include "NoClip.hpp"
#include "vehicule.hpp"

#include "Executor.hpp"

#include "Fonts.hpp"



#pragma comment(lib, "ntdll.lib")

#define IMGUI_DISABLE_DEMO_WINDOWS
#define IMGUI_DISABLE_METRICS_WINDOW
#define IM_ASSERT(_EXPR) ((void)(_EXPR))

//namespace fonts
ImFont* medium;
ImFont* bold;
ImFont* tab_icons;
ImFont* logo;
ImFont* tab_title;
ImFont* tab_title_icon;
ImFont* subtab_title;
ImFont* combo_arrow;

namespace Core
{
	DWORD Init(LPVOID lpvParam);
	//bool Init();
	void Render();
	void Loader();
	void GetOffset();

	static bool Open = false;
	static bool FirstTime = true;
	//static int Tab = 0;

	const char* project_version = "0.1";
	const char* updated_date = "01-04-2023";

	void Drawing(void);
}
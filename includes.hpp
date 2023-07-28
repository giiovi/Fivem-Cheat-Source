#pragma once

#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX

#include <iostream>
#include <d3d11.h>

#include <tchar.h>
#include <vector>
#include <string>

#include <filesystem>
#include <fstream>

#include <mutex>
#include <map>
#include <unordered_map>
#include <math.h>

// Boost
#include <boost/function.hpp>
#include <boost/filesystem.hpp>

//#include <imgui.h>
#include <imgui.cpp>
#include <imgui_internal.h>

#include <imgui_impl_dx9.h>

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <imgui_animated.h>
#include <imguinotify.hpp>

//#include "EncryptString.hpp"
//#include "EncryptFuncs.hpp"
#include "Memory.hpp"
#include "Drawing.hpp"

#include "Extensions/byte_array.h"
#include "Extensions/nav_elements.h"
#include "Extensions/etc_elements.h"

#include "Classes.hpp"

#define M_PI 3.14159265358979323846

#pragma warning(disable: 4005)
#pragma warning(disable:4006)
#pragma warning(disable:4221)

//#define DEBUG 0


typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

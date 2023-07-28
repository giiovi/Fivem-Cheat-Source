#include "Core.hpp"

#include "Classes.hpp"
#include <native.hpp>
#include "func.hpp"
#include <process.h>
#pragma warning(disable : 6031)

BOOLEAN APIENTRY DllMain(HINSTANCE hk_dll, DWORD hk_reason, LPVOID hk_lpReserved)
{

	UNREFERENCED_PARAMETER(hk_lpReserved);

	if (hk_reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hk_dll);
	
		/*
		if (DEBUG == 1)
		{
			//typedef BOOL(__stdcall* AllocConsole_t)();
			//AllocConsole_t AllocConsole_o = (AllocConsole_t)(GetProcAddress)((GetModuleHandle)((E("kernel32.dll"))), E("AllocConsole"));
			//AllocConsole_o();
			AllocConsole();

			FILE* x;
			freopen_s(&x, "CONOUT$", "w", stdout);
			//freopen_s((FILE**)stdout, E("CONOUT$"), E("w"), stdout);
			//freopen_s((FILE**)stdout, E("CONOUT$"), E("w"), stdout);
		}*/

		RAIDOS::World = RAIDMEMORY::PatternScan(skCrypt("48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81"), NULL, 7); // CPedFactory // UPDATED
		RAIDOS::ReplayInterface = RAIDMEMORY::PatternScan(skCrypt("48 8D 0D ? ? ? ? 89 44 24 30 E8 ? ? ? ? 48 83 C4 28 C3 48 8B 05"), NULL, 7); // CReplayInterface // ADDRESS_REPLAY_INTERFACE //GTA DUMP // UPDATED
		RAIDOS::PlayerInterface = RAIDMEMORY::PatternScan(skCrypt("48 8B 0D ? ? ? ? 8A D3 48 8B 01 FF 50 ? 4C 8B 07 48 8B CF"), NULL, 7); // ADDRESS_PLAYER_LIST // Network Player Manager // UPDATED
		RAIDOS::Camera = RAIDMEMORY::PatternScan(skCrypt("48 8B 05 ? ? ? ? 48 8B 98 ? ? ? ? EB"), NULL, 7); //CameraViewAngles

		RAIDOS::pointer_to_handle = reinterpret_cast<RAIDOS::pizza_to_spaghetti_t>(RAIDMEMORY::PatternScan(skCrypt("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 8B 15 ? ? ? ? 48 8B F9 48 83 C1 10 33 DB"), NULL, NULL));

		CreateThread(NULL, 0, Core::Init, NULL, 0, NULL);

	}

	return TRUE;
}
#pragma once

#define ProbeForWriteGenericType(Ptr, Type) *(volatile Type *)(Ptr) = *(volatile Type *)(Ptr); 
#define ProbeForWriteLargeInteger(Ptr) ProbeForWriteGenericType(&((PLARGE_INTEGER)Ptr)->QuadPart, LONGLONG)
#define SPOOF_RETURN_ADDRESS FALSE

namespace { namespace winapi {

	inline WNDPROC(*oNtUserSetWindowLongPtr)(HWND, unsigned int, LONG_PTR, BOOL);
	inline HWND(*oNtUserGetForegroundWindow)();
	inline std::uint64_t(*oNtUserCallOneParam)(HWND, int);
	inline std::uint64_t(*oNtUserTransformPoint)(tagPOINT*, std::uint64_t, unsigned int, int);
	inline BOOL(*oClientToScreen)(HWND, LPPOINT);
	inline BOOL(*oScreenToClient)(HWND, LPPOINT);
	inline BOOL(*oNtUserTrackMouseEvent)(LPTRACKMOUSEEVENT lpEventTrack);
	inline HCURSOR(*oNtUserSetCursor)(HCURSOR hCursor);
	inline BOOL(*oNtUserSetCursorPos)(int, int);
	inline BOOL(*oNtUserCallTwoParam)(LPPOINT, std::uint64_t, std::uint64_t);
	inline BOOL(__stdcall* oGetClientRect)(HWND, LPRECT);
	inline HWND(__stdcall* oNtUserGetThreadState)(std::uint64_t);
	inline HWND(__stdcall* oNtUserSetCapture)(HWND);
	inline BOOL(__stdcall* oNtUserCallNoParam)(std::uint64_t);
	inline HANDLE(*oLoadImageW)(HINSTANCE, LPCWSTR, UINT, int, int, UINT);
	inline UINT(*NtUserSendInput)(UINT, LPINPUT, INT);

	inline bool initialize()
	{
		*(std::uint64_t*)&oNtUserSetWindowLongPtr = memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserSetWindowLongPtr"));
		*(std::uint64_t*)&oNtUserGetForegroundWindow = memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserGetForegroundWindow"));
		*(std::uint64_t*)&oNtUserCallOneParam = memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserCallOneParam"));
		*(std::uint64_t*)&oNtUserTransformPoint = memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserTransformPoint"));	
		*(std::uint64_t*)&oClientToScreen = memory::get_export(xorstr_("user32.dll"), xorstr_("ClientToScreen"));
		*(std::uint64_t*)&oScreenToClient = memory::get_export(xorstr_("user32.dll"), xorstr_("ScreenToClient"));
		*(std::uint64_t*)&oNtUserTrackMouseEvent = memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserTrackMouseEvent"));
		*(std::uint64_t*)&oNtUserSetCursor = memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserSetCursor"));
		*(std::uint64_t*)&oNtUserSetCursorPos = memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserSetCursorPos"));
		*(std::uint64_t*)&oNtUserCallTwoParam = memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserCallTwoParam"));
		*(std::uint64_t*)&oGetClientRect = memory::get_export(xorstr_("user32.dll"), xorstr_("GetClientRect"));
		*(std::uint64_t*)&oNtUserGetThreadState = memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserGetThreadState"));
		*(std::uint64_t*)&oNtUserSetCapture = memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserSetCapture"));
		*(std::uint64_t*)&oNtUserCallNoParam = memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserCallNoParam"));
		*(std::uint64_t*)&oLoadImageW = memory::get_export(xorstr_("user32.dll"), xorstr_("LoadImageW"));
		*(std::uint64_t*)&NtUserSendInput = memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserSendInput"));

		return true;
	}

	inline WNDPROC fnSetWindowLongPtrA(HWND hWnd, unsigned int nIndex, LONG_PTR dwNewLong)
	{
#if SPOOF_RETURN_ADDRESS
		return fake_retaddr(oNtUserSetWindowLongPtr, hWnd, nIndex, dwNewLong, TRUE);
#else
		return oNtUserSetWindowLongPtr(hWnd, nIndex, dwNewLong, TRUE);
#endif
	}

	inline HWND NtUserGetForegroundWindow()
	{
#if SPOOF_RETURN_ADDRESS
		return fake_retaddr(oNtUserGetForegroundWindow);
#else
		return GetForegroundWindow();
#endif
	}

	inline std::uint64_t fnValidateHwnd(HWND a1)
	{
#if SPOOF_RETURN_ADDRESS
		return fake_retaddr(oNtUserCallOneParam,a1, 56);
#else
		return oNtUserCallOneParam(a1, 56);
#endif
	}

	inline __int64 fnIsChildEx(__int64 a1, __int64 a2)
	{
		__int64 RAX; 

		do
		{
			if (!a2 || (*(unsigned char*)(a2 + 0x1F) & 0xC0) != 0x40) return 0;
			RAX = *(unsigned __int64*)(a2 + 0x30);
			
			if (RAX) { a2 += RAX - *(unsigned __int64*)(a2 + 8); }
			else { a2 = 0; }

		} while (a1 != a2);
		return 1;
	}

	inline BOOL fnIsChild(HWND hWndParent, HWND hWnd)
	{
		return IsChild(hWndParent, hWnd);

		std::uint64_t RAX; 
		std::uint64_t RBX = fnValidateHwnd(hWnd);

		return (RBX && (RAX = fnValidateHwnd(hWndParent)) != 0) ? fnIsChildEx(RAX, RBX) : 0;
	}

	inline std::uint64_t fnNtUserTransformPoint(tagPOINT* a1, std::uint64_t a2, unsigned int a3, int a4)
	{
		return oNtUserTransformPoint(a1, a2, a3, a4);
	}

	inline BOOL fnClientToScreen(HWND hWnd, LPPOINT lpPoint)
	{
		return oClientToScreen(hWnd, lpPoint);
	}

	inline BOOL fnScreenToClient(HWND hWnd, LPPOINT lpPoint)
	{
		return oScreenToClient(hWnd, lpPoint);
	}

	inline BOOL fnTrackMouseEvent(LPTRACKMOUSEEVENT lpEventTrack)
	{
		return oNtUserTrackMouseEvent(lpEventTrack);
	}

	inline HCURSOR fnSetCursor(HCURSOR hCursor)
	{
#if SPOOF_RETURN_ADDRESS
		return fake_retaddr(oNtUserSetCursor, hCursor);
#else
		return SetCursor(hCursor);
#endif
	}

	inline BOOL fnSetCursorPos(int X, int Y)
	{
#if SPOOF_RETURN_ADDRESS
		return fake_retaddr(oNtUserSetCursorPos, X, Y);
#else
		return oNtUserSetCursorPos(X, Y);
#endif		
	}
	
	inline BOOL fnGetCursorPos(LPPOINT lpPoint)
	{
#if SPOOF_RETURN_ADDRESS
		return fake_retaddr(oNtUserCallTwoParam, lpPoint, static_cast<std::uint64_t>(0x1), static_cast<std::uint64_t>(0x7F));
#else
		return oNtUserCallTwoParam(lpPoint, static_cast<std::uint64_t>(0x1), static_cast<std::uint64_t>(0x7F));
#endif
	}

	inline BOOL fnGetClientRect(HWND a1, LPRECT a2)
	{
#if SPOOF_RETURN_ADDRESS
		return fake_retaddr(oGetClientRect, a1, a2);
#else
		return GetClientRect(a1, a2);
#endif
	}

	inline HWND fnGetCapture()
	{
		static std::uint64_t flag = 0;
		if (!flag)
		{
			memory::pattern flag_pattern = memory::pattern(xorstr_("user32.dll"), xorstr_("48 8B 05 ? ? ? ? 48 03 D1"));
			if (flag_pattern.valid())
			{
				flag_pattern.rva(3);
				flag_pattern.size(7);
				flag_pattern.set(flag, true);
			}
		}

		if (*(DWORD*)(flag + 0x1B50))
		{
#if SPOOF_RETURN_ADDRESS
			return (HWND)fake_retaddr(oNtUserGetThreadState, static_cast<std::uint64_t>(2));
#else
			return (HWND)oNtUserGetThreadState(static_cast<std::uint64_t>(2));
#endif
		}
		else
			return 0;
	}

	inline HWND fnSetCapture(HWND hWnd)
	{
#if SPOOF_RETURN_ADDRESS
		return fake_retaddr(oNtUserSetCapture, hWnd);
#else
		return oNtUserSetCapture(hWnd);
#endif
	}

	inline BOOL fnReleaseCapture()
	{
#if SPOOF_RETURN_ADDRESS
		return fake_retaddr(oNtUserCallNoParam, static_cast<std::uint64_t>(22));
#else
		return oNtUserCallNoParam(static_cast<std::uint64_t>(22));
#endif
	}

	inline HANDLE fnLoadImageA(HINSTANCE hinst, LPCSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad)
	{
#if SPOOF_RETURN_ADDRESS
		return fake_retaddr(oLoadImageW, hinst, (LPCWSTR)lpszName, uType, cxDesired, cyDesired, fuLoad);
#else
		return oLoadImageW(hinst, (LPCWSTR)lpszName, uType, cxDesired, cyDesired, fuLoad);
#endif
	}

	inline HCURSOR fnLoadCursorA(HINSTANCE hInstance, LPCSTR lpCursorName)
	{
		return (HCURSOR)LoadImageA(hInstance, lpCursorName, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE);
	}

	inline LONG fnNtQueryPerformanceCounter(PLARGE_INTEGER PerformanceCounter, PLARGE_INTEGER PerformanceFrequency)
	{
		LARGE_INTEGER PerfFrequency;

		ProbeForWriteLargeInteger(PerformanceCounter);
		if (PerformanceFrequency)
			ProbeForWriteLargeInteger(PerformanceFrequency);

		PerfFrequency.QuadPart = 0;

		if (PerformanceFrequency)
			*PerformanceFrequency = PerfFrequency;

		return 0;
	}

	inline BOOL fnQueryPerformanceFrequency(PLARGE_INTEGER lpFrequency)
	{
		LARGE_INTEGER Count;
		return (fnNtQueryPerformanceCounter(&Count, lpFrequency) == 0);
	}

	inline BOOL fnQueryPerformanceCounter(PLARGE_INTEGER lpPerformanceCount)
	{
		LARGE_INTEGER Frequency;
		return (fnNtQueryPerformanceCounter(lpPerformanceCount, &Frequency) == 0);
	}

	inline void fnmouse_event(DWORD dwFlags, DWORD dx, DWORD dy, DWORD dwData, ULONG_PTR dwExtraInfo)
	{
		if (!NtUserSendInput)
			NtUserSendInput = (decltype(NtUserSendInput))(memory::get_export(xorstr_("win32u.dll"), xorstr_("NtUserSendInput")));

		INPUT Input;

		Input.type = INPUT_MOUSE;
		Input.mi.dx = dx;
		Input.mi.dy = dy;
		Input.mi.mouseData = dwData;
		Input.mi.dwFlags = dwFlags;
		Input.mi.time = 0;
		Input.mi.dwExtraInfo = dwExtraInfo;

		NtUserSendInput(1, &Input, sizeof(INPUT));
	}
} }

namespace { namespace d3d {
	
	inline std::uint64_t(*create_render_target_view_worker)(ID3D11Device*, ID3D11Resource*, const D3D11_RENDER_TARGET_VIEW_DESC*, const IID&, ...);

	inline bool initialize()
	{
		*(std::uint64_t*)&create_render_target_view_worker = memory::pattern(xorstr_("d3d11.dll"), xorstr_("48 8B C4 4C 89 48 ? 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 C7 40 ? ? ? ? ? 48 89 58 ? 48 89 70 ? 4D 8B F0")).get();
		return true;
	}

	inline HWND get_hwnd(IDXGISwapChain* _this)
	{
		return reinterpret_cast<HWND>(*((std::uint64_t*)_this + 0x1F));
	}

	// IDXGISwapChain::GetDevice
	inline HRESULT get_device(IDXGISwapChain* _this, const IID& a2, void** a3)
	{
		return (***((__int64(__fastcall****)(std::uint64_t, const IID&, void**))_this + 0x13))(*((std::uint64_t*)_this + 0x13), a2, a3);
	}

	// IDXGISwapChain::GetBuffer
	inline HRESULT get_buffer(IDXGISwapChain* _this, int Buffer, const IID& riid, void** ppSurface)
	{
		unsigned int v6 = Buffer;
		int v9 = *((DWORD*)_this + 0x32);
		__int64(__fastcall*** v10)(std::uint64_t, const IID&, void**); // rcx

		if (v6 >= *((DWORD*)_this + 0x1EA))
		{
			v10 = (__int64(__fastcall***)(std::uint64_t, const IID&, void**)) * ((std::uint64_t*)_this + 0xDA);
		}
		else
		{
			v10 = (__int64(__fastcall***)(std::uint64_t, const IID&, void**)) * ((std::uint64_t*)_this + v6 + 0xC9);
		}

		if ((v9 & 0x80) != 0)
		{
			__int64 v20 = (__int64)(v10 + 9);
			if (!v10)
				v20 = 0x50;
			v10 = *(__int64(__fastcall****)(std::uint64_t, const IID&, void**))v20;
		}

		return (v10 != 0 ? (**v10)((__int64)v10, riid, ppSurface) : S_FALSE);
	}

	// ID3D11Device::CreateRenderTargetView
	inline HRESULT create_render_target_view(ID3D11Device* _this, ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
	{
		return create_render_target_view_worker((ID3D11Device*)((std::uint64_t)_this - 0x10), pResource, pDesc, __uuidof(ID3D11RenderTargetView), ppRTView, 0, 1);
	}

	// IUnknown::Release
	inline ULONG release(void* a1)
	{
		return (*(ULONG(__fastcall**)(std::uint64_t))(**(std::uint64_t**)((std::uint64_t)a1 + 0x50) + 0x10))(*(std::uint64_t*)((std::uint64_t)a1 + 0x50));
	}

	inline void get_immediate_context(ID3D11Device* device, ID3D11DeviceContext** output)
	{
		// ID3D11Device::GetImmediateContext = VTable index 40	
		//std::uint64_t* vtable = *(std::uint64_t**)device;

		std::uint64_t a1 = *(std::uint64_t*)(*((std::uint64_t*)device + 0x7C) + 0x50);
		if (!a1) return;

		constexpr long long key = 0x100000000;
		constexpr int shift = 0x20;

		*(signed __int64*)(a1 + 0x8) += key;
		if (static_cast<DWORD>((unsigned __int64)(*(signed __int64*)(a1 + 0x8) + key) >> shift) == 1)
			(*(void(__fastcall**)(std::uint64_t))(**(std::uint64_t**)(a1 + 0x18) + 0x30i64))(*(std::uint64_t*)(a1 + 0x18));

		if (output)
			*output = *(ID3D11DeviceContext**)(*((std::uint64_t*)device + 0x7C) + 0x60);
	}
} }
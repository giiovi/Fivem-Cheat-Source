#pragma once

#define ProbeForWriteGenericType(Ptr, Type) *(volatile Type *)(Ptr) = *(volatile Type *)(Ptr); 
#define ProbeForWriteLargeInteger(Ptr) ProbeForWriteGenericType(&((PLARGE_INTEGER)Ptr)->QuadPart, LONGLONG)
#define SPOOF_RETURN_ADDRESS FALSE

namespace { namespace winapi {

	inline WNDPROC fnSetWindowLongPtrA(HWND hWnd, unsigned int nIndex, LONG_PTR dwNewLong)
	{
		return (WNDPROC)SetWindowLongPtrA(hWnd, nIndex, dwNewLong);
	}

	inline HWND NtUserGetForegroundWindow()
	{
		return GetForegroundWindow();
	}

	inline BOOL fnClientToScreen(HWND hWnd, LPPOINT lpPoint)
	{
		return ClientToScreen(hWnd, lpPoint);
	}

	inline BOOL fnScreenToClient(HWND hWnd, LPPOINT lpPoint)
	{
		return ScreenToClient(hWnd, lpPoint);
	}

	inline BOOL fnTrackMouseEvent(LPTRACKMOUSEEVENT lpEventTrack)
	{
		return TrackMouseEvent(lpEventTrack);
	}

	inline HCURSOR fnSetCursor(HCURSOR hCursor)
	{
		return SetCursor(hCursor);
	}

	inline BOOL fnSetCursorPos(int X, int Y)
	{
		return SetCursorPos(X, Y);
	}
	
	inline BOOL fnGetCursorPos(LPPOINT lpPoint)
	{
		return GetCursorPos(lpPoint);
	}

	inline BOOL fnGetClientRect(HWND a1, LPRECT a2)
	{
		return GetClientRect(a1, a2);
	}

	inline HWND fnGetCapture()
	{
		return GetCapture();
	}

	inline HWND fnSetCapture(HWND hWnd)
	{
		return SetCapture(hWnd);
	}

	inline BOOL fnReleaseCapture()
	{
		return ReleaseCapture();
	}

	inline HANDLE fnLoadImageA(HINSTANCE hinst, LPCSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad)
	{
		return LoadImageW(hinst, (LPCWSTR)lpszName, uType, cxDesired, cyDesired, fuLoad);
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
} }

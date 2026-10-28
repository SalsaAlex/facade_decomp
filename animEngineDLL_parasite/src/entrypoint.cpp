#include "facadecommon.h"

#include "keyboarddevice.h"
#include "sounddevice.h"

//main.cpp

extern LRESULT CALLBACK mainloop(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
extern void InitGlobals();

int __declspec(dllexport) AnimEngineWinMain(HINSTANCE p1, HINSTANCE p2, char* p3, int p4)
{
	static bool g_bUnusedFlag;
	char stageplaypath[100];

	if (GetAsyncKeyState(VK_F12) < 0 && GetAsyncKeyState(VK_SHIFT) < 0)
	{
		g_bUnusedFlag = 0;
	}
	if (GetAsyncKeyState(VK_CONTROL) & 0x08000)
	{
		char buildname[256];
		sprintf(buildname, "Beta build %s", "8 (7.9.05)");
		_set_g_bPrintBetaInfo(true);
		AddEventInfoToLog(
			eEventLogType_unknown00,
			eSpriteID_NOTASPRITE,
			buildname,
			false,
			eSpriteID_NOTASPRITE,
			GetTickCount());
	}
	_set_g_bFacadeHasBeenRunMultipleTimes(false);

	strcpy(stageplaypath, "..\\..\\..\\stageplays\\");
	strcat(stageplaypath, "*.txt");
	HANDLE hFile;
	WIN32_FIND_DATA finddata;
	if ((hFile = FindFirstFile(stageplaypath, &finddata)) != INVALID_HANDLE_VALUE)
	{
		if (FindNextFile(hFile, &finddata))
		{
			_set_g_bFacadeHasBeenRunMultipleTimes(true);
		}
		FindClose(hFile);
	}
	if (CoInitialize(NULL) < 0)
	{
		ShouldntBeHere("CoInitialize Failed!");
		exit(1);
	}

	WNDCLASS classinfo;
	classinfo.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	classinfo.lpfnWndProc = mainloop;
	classinfo.cbClsExtra = NULL;
	classinfo.cbWndExtra = NULL;
	classinfo.hInstance = p1;
	classinfo.hIcon = NULL;
	classinfo.hCursor = LoadCursorA(0, IDC_ARROW);
	classinfo.hbrBackground = NULL;
	classinfo.lpszMenuName = NULL;
	classinfo.lpszClassName = "Façade";
	if (!RegisterClass(&classinfo))
		return NULL;

	const int winflags = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
	_set_g_hMainAppWindow(CreateWindowEx(NULL, "Façade", "Façade", winflags, NULL, NULL, 1024, 768, NULL, NULL, p1, NULL));
	if (!g_hMainAppWindow)
		return NULL;

	InitGlobals();
	if (S_OK != Keyboard_Init())
		ShouldntBeHere("Keyboard did not initialize");
	if(!DSound_Init())
		ShouldntBeHere("DSound did not initialize");

	ShowWindow(g_hMainAppWindow, SW_SHOW);
	UpdateWindow(g_hMainAppWindow);
	BOOL result = SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	animengine_assert(result == TRUE);
	GetPriorityClass(GetCurrentProcess());

	MSG winmsg;
	while (GetMessage(&winmsg, NULL, NULL, NULL))
	{
		TranslateMessage(&winmsg);
		DispatchMessage(&winmsg);
	}
	CoUninitialize();
	ReleaseDC(g_hMainAppWindow, g_hDeviceContext);
	return winmsg.wParam;
}
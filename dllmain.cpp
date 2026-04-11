#include <Windows.h>
#include <stdio.h>
#include <gl/GL.h>
#include "dinput.h"

HWND g_hwnd;
HDC g_hdc;
HINSTANCE g_hInstance;
static HGLRC g_hGLContext;
static HGDIOBJ hPalette;
IDirectInput8* g_pDirectInputInterface = nullptr;
IDirectInputDevice8* g_pDirectInputDevice = nullptr;
static char inputstates[32];
GLuint g_GLListBase = 0;
GLYPHMETRICSFLOAT glyphdata;

bool DumpPrintToFile(const char* outputstring, const char* filename)
{
	HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == (HANDLE)-1)
		return 0;
	int len =  lstrlenA(outputstring);
	DWORD numbytes = 0;
	BOOL result = WriteFile(hFile, outputstring, len, &numbytes, 0);
	if (result)
		FlushFileBuffers(hFile);
	CloseHandle(hFile);
	return result;
}
int logprint(const char* string)
{
	int i = 0;
	while (!DumpPrintToFile(string, "\\\\.\\mailslot\\WinLog"))
	{
		if (i < 0)
		{
			OutputDebugString(string);
			return;
		}
		Sleep(10);
	}
}

int printfunc2(const char* format, ...)
{
	static char OutputString[2868];
	va_list va;
	va_start(va, format);
	vsnprintf(OutputString, 2047, format, va);
	return logprint(OutputString);
}

int printfunc(const char* format, ...)
{
	static char buffer[2868];
	va_list va;
	va_start(va, format);
	vsnprintf(buffer, 2047, format, va);
	printfunc2("%s\n", buffer);
}


void GL_Init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	//if (unknown_var2) // ?
	//	glEnable(GL_LINE_SMOOTH);
}

void GL_InitBitmapFont()
{
	int tickcount = GetTickCount();
	printfunc("Building bitmap font...");

	g_GLListBase = glGenLists(256);
	SelectObject(g_hdc, CreateFont(-12, 0, 0, 0, 700, 0, 0, 0, 0, 4, 0, 4, 0, "Comic Sans MS"));
	wglUseFontOutlines(g_hdc, 0, 0xFF, g_GLListBase, 0.0, 0.0, 1, &glyphdata);
	double curtickcount = (GetTickCount() - tickcount) / 1000;

	printfunc("Done building bitmap font (%.3f seconds)", curtickcount);
}
void GL_ShutdownBitmapFont()
{
	glDeleteLists(g_GLListBase, 256);
}

int GetKeyboardState()
{
	LPDIDEVICEOBJECTDATA devicedata[160];
	static BYTE KeyState;
	int numelements = 32;
	if (!g_pDirectInputDevice)
		return 0;

	HRESULT result = g_pDirectInputDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), *devicedata, (LPDWORD)&numelements, 0);
	if (result != DI_OK)
	{
		while (result == DIERR_INPUTLOST)
			result = g_pDirectInputDevice->Acquire();

		return 0;
	}

	int len = strlen(inputstates);
	for (int i = 0; i < 32; ++i)
	{
		if (len < 32 && (devicedata[5 * i + 1] & 0x80) != 0)
		{
			UINT uCode = devicedata[5 * i];
			GetKeyboardState(&KeyState);
			UINT uVirtKey = MapVirtualKeyExA(uCode, 1u, dwhkl);
			ToAsciiEx(uVirtKey, uCode, &KeyState, Char, 0, dwhkl);
			v5 = Char[0];
			if (SLOBYTE(Char[0]) == 8)
				v5 = 60;
			if (uCode == 156)
				v5 = 13;
			if ((unsigned __int8)sub_1000237E(v5)
				|| (unsigned __int8)sub_10001208(v5)
				|| (unsigned __int8)sub_100014DD(v5)
				|| (unsigned __int8)sub_10001262(v5)
				|| v5 == 60)
			{
				*(&byte_109782D0 + v8++) = v5;
				v4 = 1;
			}
		}
	}
	if (v4 || (GetAsyncKeyState(VK_BACK) & 0x8000) == 0 || len >= 32)
	{
		dword_109794B4 = 0;
	}
	else if (++dword_109794B4 > 15 && !(dword_1083F62C % 2))
	{
		*(&byte_109782D0 + len++) = 60;
	}
	byte_10978400 = byte_109782D0;
	for (j = 0; j < 31; ++j)
		*(&byte_109782D0 + j) = byte_109782D1[j];
	byte_109782EF = 0;
	return 0;

}

LRESULT CALLBACK WindowProcLoop(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		case WM_CREATE:
		{
			g_hInstance = ((CREATESTRUCT*)(lParam))->hInstance;
			g_hdc = GetDC(hWnd);
			HandlePixelFormat(g_hdc);
			g_GLContext = wglCreateContext(g_hdc);
			wglMakeCurrent(g_hdc, g_GLContext);
			hPalette = GeneratePalette(g_hdc);
			GL_Init();
			break;
		}
		case WM_DESTROY:
			break;
		case WM_ACTIVATE:
		{
			if (g_pDirectInputDevice && wParam)
				g_pDirectInputDevice->Acquire();
			break;
		}
		case WM_PAINT:
		{
			//if (byte_10979460 || byte_1082D58C)
			//{
			//	BeginPaint(hWnd, &Paint);
			//	EndPaint(hWnd, &Paint);
			//	SendMessageA(g_hwnd, 0x10u, 0, 0);
			//}
			//else
			//{
				
			//}
			break;
		}
	}
}

extern "C" WPARAM AnimEngineWinMain(HINSTANCE hInstance)
{
	char filesearch[256];
	WIN32_FIND_DATA FileData;
	WNDCLASS WndClass;
	HANDLE hFoundFile, hProcess;
	LPMSG Msg;
	if ((GetAsyncKeyState(VK_F12) < 0) && (GetAsyncKeyState(VK_SHIFT) < 0))
	{
		//unused_var1 = 0;
		//unused_localvar1 = 0;
	}
	bool bJunjaMode = GetAsyncKeyState(VK_JUNJA) & 0x8000;
	g_debugmode = bJunjaMode;
	if (bJunjaMode)
	{
		char buffer[128];
		sprintf(buffer, "Beta build %s", "8 (7.9.05)");
		//unknown_function(0, -1, buffer, -1, GetTickCount());
	}

	g_bMoreThanOneStagePlay = 0;
	strcpy(filesearch, "..\\..\\..\\stageplays\\");
	strcat(filesearch, "*.txt");
	hFoundFile = FindFirstFileA(filesearch, &FileData);
	if (hFoundFile != (HANDLE)-1)
	{
		if (FindNextFileA(hFoundFile, &FileData))
			g_bMoreThanOneStagePlay = 1;
		FindClose(hFoundFile);
	}
	if (CoInitialize(0) < 0)
	{
		//assertfunc((char)"CoInitialize Failed!");
		exit(1);
	}

	WndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = sub_100017BC;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = 0;
	WndClass.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
	WndClass.hbrBackground = 0;
	WndClass.lpszMenuName = 0;
	WndClass.lpszClassName = "Facade";
	if (!RegisterClass(&WndClass))
		return 0;
	g_hwnd = CreateWindowExA(0, "Facade", "Facade", 0x6CC0000u, 0, 0, 1024, 768, 0, 0, hInstance, 0);
	if (!g_hwnd)
		return 0;

	InitGlobals();
	if (InitDirect8Input())
		assertfunc("Keyboard did not initialize");
	if(!InitDSound())
		assertfunc("DSound did not initialize");

	ShowWindow(g_hwnd, 5);
	UpdateWindow(g_hwnd);
	hProcess = GetCurrentProcess();
	Assert(SetPriorityClass(hProcess, 0x80u));
	GetPriorityClass(hProcess);
	while (GetMessage(Msg, 0, 0, 0))
	{
		TranslateMessage(Msg);
		DispatchMessage(Msg);
	}
	CoUninitialize();
	ReleaseDC(g_hwnd, g_hdc);
	return Msg->wParam;
}

BOOL APIENTRY DllMain(
	HMODULE hinstDLL,
	DWORD  fdwReason,
	LPVOID lpReserved
)
{
	static HANDLE hObject;
	switch (fdwReason)
	{
	case  DLL_PROCESS_ATTACH:
		printfunc("Process attaching to DLL\n");
		hMutex = CreateMutex(0, 0, "BufferMutex");
		Assert(hMutex);
		hObject = CreateFileMapping(INVALID_HANDLE_VALUE, 0, FILE_MAP_READ, 0, 617728, "dllmemfilemap");
		if (!hObject)
			return 0;

		int filenoexists = GetLastError() != ERROR_ALREADY_EXISTS;
		LPCVOID baseaddress = MapViewOfFile(hObject, FILE_MAP_WRITE, 0, 0, 0);
		if (!baseaddress)
			return;


	}
}
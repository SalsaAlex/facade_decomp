#include "facadecommon.h"
#include "keyboarddevice.h"
#include "classes/AblRequestBuffer.h"


int DllMain(HINSTANCE instDLL, int reason)
{
	static HANDLE s_hDLLMemoryFilemap;
	static void* s_DLLMappedMemoryPointer;

	if (reason == DLL_PROCESS_ATTACH)
	{
		dprintf("Process attaching to DLL\n");
		g_ablRequestBufferMutexHandle = CreateMutex(NULL, FALSE, "BufferMutex");
		animengine_assert(g_ablRequestBufferMutexHandle);

		uint32_t allocate_size = sizeof(AblRequest) * REQUESTBUFFERSIZE + sizeof(unknownablstructure1) + sizeof(unknownablstructure2) * 1000;

		s_hDLLMemoryFilemap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, allocate_size, "dllmemfilemap");
		if (!s_hDLLMemoryFilemap)
			return 0;
		s_DLLMappedMemoryPointer = MapViewOfFile(s_hDLLMemoryFilemap, FILE_MAP_WRITE, NULL, NULL, NULL);
		if (!s_DLLMappedMemoryPointer)
			return 0;

		g_aplrequestbuffer = new AblRequestBuffer();
		g_aplrequestbuffer->InitBuffer(s_DLLMappedMemoryPointer, GetLastError() != ERROR_ALREADY_EXISTS);
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		dprintf("Animengine detaching from DLL\n");
		UnmapViewOfFile(s_DLLMappedMemoryPointer);
		CloseHandle(s_hDLLMemoryFilemap);

	}
}


float g_curFps = 30.0;
int g_millisecondsToSleep = 30;
int g_renderEveryNthFrame = 1;
int g_cpuClockSpeed = 2000;



bool g_bLockedInMSToSleep;
bool g_bDoingFPSAdjustment;


//UNDONE
LRESULT mainloop(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static HPALETTE s_hPalette = NULL;
	static DWORD s_timeAtStartOfCounting = 0, s_frameTimeAccum = 0, s_framesSinceLastStatistic = 0;


	if (msg > WM_QUERYNEWPALETTE)
	{
		switch (msg)
		{
			case WM_PALETTECHANGED:
				if (s_hPalette && wparam != (UINT_PTR)hwnd)
				{
					SelectPalette(g_hDeviceContext, s_hPalette, FALSE);
					RealizePalette(g_hDeviceContext);
					UpdateColors(g_hDeviceContext);
				}
				return 0;
			case WM_EDITGRACESCRIPT: EditCharacterScript(g_GraceSprite); return 0;
			case WM_EDITGRACEFACE: EditCharacterFace(g_GraceSprite); return 0;
			case WM_EDITTRIPSCRIPT: EditCharacterScript(g_TripSprite); return 0;
			case WM_EDITTRIPFACE: EditCharacterFace(g_TripSprite); return 0;
			default: 
				return DefWindowProcA(hwnd, msg, wparam, lparam);
		}
	}

	if (msg == WM_QUERYNEWPALETTE)
	{
		HRESULT returncode = S_OK;
		if (s_hPalette)
		{
			SelectPalette(g_hDeviceContext, s_hPalette, 0);
			returncode = RealizePalette(g_hDeviceContext);
			InvalidateRect(hwnd, 0, 0);
		}
		return returncode;
	}

	if (msg > 6)
	{
		switch (msg)
		{
		case WM_PAINT:
			if (g_bAssertionThrown || g_bQuittingApp)
			{
				PAINTSTRUCT paint;
				BeginPaint(hwnd, &paint);
				EndPaint(hwnd, &paint);
				SendMessage(g_hMainAppWindow, WM_CLOSE, NULL, NULL);
			}
			else
			{
				if (ReadBufferedKeyboardData() < 0)
					dprintf("\n### Error reading keyboard state.\n");

				DWORD tickcount_atstart = GetTickCount();
				static bool startcounttime_safeguard = false;
				if (!startcounttime_safeguard)
				{
					startcounttime_safeguard = true;
					s_timeAtStartOfCounting = GetTickCount();
				}
				++g_numFramesRun;
				g_bShouldRenderThisFrame = g_SkipFramesCounter++ == 0;
				if (g_SkipFramesCounter >= g_renderEveryNthFrame)
					g_SkipFramesCounter = 0;

				if (g_numFramesRun == 2)
					FinishRoomInit();
				LaunchAblIfNeeded();

				bool loaded = g_bLockedInMSToSleep || g_bDoingFPSAdjustment || g_numFramesRun == 1;
				if (loaded)
				{
					UpdateCursor();
					UpdateEnvironment();
					UpdateCinematography();
					UpdateAndRenderWorld();
					UpdateMP3Fades();
				}
				else
				{
					ChangeSize();
					glClearColor(0, 0, 0, 0);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					LoadingMessage();
					UpdateFade();
					glFlush();
				}
				if (g_bShouldRenderThisFrame)
				{
					SwapBuffers(g_hDeviceContext);
				}
				GetRotationKeyPresses();
				if (loaded)
					UpdatePlayerNavigation();

				if (!g_bQuittingApp)
				{
					//UNDONE
				}
				s_frameTimeAccum += GetTickCount() - tickcount_atstart;
				s_framesSinceLastStatistic++;

				if (s_framesSinceLastStatistic == 90)
				{
					g_curFps = 90.f / ((float)(GetTickCount() - s_timeAtStartOfCounting) / 1000.f);
					int rounded_fps = FloatToInt(g_curFps);
					s_frameTimeAccum = s_framesSinceLastStatistic = 0;
					s_timeAtStartOfCounting = GetTickCount();

					bool unknown = true;
					if (!g_bLockedInMSToSleep)
					{
						if (g_bDoingFPSAdjustment)
						{

						}
						else if (g_bAblRequestingFPSAdjustment)
						{
							g_bLockedInMSToSleep = 1;
							if (g_cpuClockSpeed >= 1800)
							{
								g_renderEveryNthFrame = 1;
								if (g_cpuClockSpeed <= 2900)
									g_millisecondsToSleep = 20;
								else
									g_millisecondsToSleep = 30;
							}
							else
							{
								g_renderEveryNthFrame = 2;
								g_millisecondsToSleep = 25;
							}
							g_bAblRequestingFPSAdjustment = 0;
						}
					}
				}
			}
			break;

		}
	}

}
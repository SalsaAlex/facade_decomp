#include "facadecommon.h"
#include "keyboarddevice.h"
#include "sounddevice.h"
#include "glfuncs.h"
#include "classes/AblRequestBuffer.h"

int g_startTime;

int g_mousePosition[2];
bool g_mouseLButtonClicked;

HINSTANCE g_hAppInstance;
HGLRC g_hGLContext;

Sprite* g_PlayerSprite;
Sprite* g_CursorSprite;


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

static void selectglpixelformat(HDC devicecontext)
{
	static PIXELFORMATDESCRIPTOR descript{
		sizeof(PIXELFORMATDESCRIPTOR), 
		1, 
		PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
		0,
		24, //color bits
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		32, /*depth bits*/};
	int format = ChoosePixelFormat(devicecontext, &descript);
	SetPixelFormat(devicecontext, format, &descript);
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
	case WM_QUERYNEWPALETTE:
		HRESULT returncode = S_OK;
		if (s_hPalette)
		{
			SelectPalette(g_hDeviceContext, s_hPalette, 0);
			returncode = RealizePalette(g_hDeviceContext);
			InvalidateRect(hwnd, 0, 0);
		}
		return returncode;
		break;
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
			bool haventrenderedyet = true;
			if (g_bShouldRenderThisFrame)
			{
				SwapBuffers(g_hDeviceContext);
				haventrenderedyet = false;
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

			bool unknown = false;
			if (s_framesSinceLastStatistic == 90)
			{
				g_curFps = 90.f / ((float)(GetTickCount() - s_timeAtStartOfCounting) / 1000.f);
				int rounded_fps = FloatToInt(g_curFps);
				s_frameTimeAccum = s_framesSinceLastStatistic = 0;
				s_timeAtStartOfCounting = GetTickCount();

				unknown = true;
				if (!g_bLockedInMSToSleep)
				{
					if (g_bDoingFPSAdjustment)
					{
						//UNDONE
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
			ProcessAblRequests(unknown);
			if (g_millisecondsToSleep > 0)
				Sleep(g_millisecondsToSleep);
			if (haventrenderedyet)
				InvalidateRect(hwnd, 0, 0);
		}
		break;

	case WM_MOUSEFIRST:
		g_mousePosition[0] = (uint16_t)lparam;
		g_mousePosition[1] = ((uint16_t)(uint8_t*)lparam + 2);
		break;
	case WM_LBUTTONDOWN:
		g_mouseLButtonClicked = true;
		break;
	case WM_ACTIVATE:
		if (wparam && g_pKeyboard)
			g_pKeyboard->Acquire();
		break;
	case WM_CREATE:
		g_hAppInstance = *(HINSTANCE*)lparam + 4;
		g_hDeviceContext = GetDC(hwnd);
		g_hGLContext = wglCreateContext(g_hDeviceContext);
		wglMakeCurrent(g_hDeviceContext, g_hGLContext);
		s_hPalette = createglpalette(g_hDeviceContext);
		SetupRC();
		break;
	case WM_DESTROY:
		dprintf("\n### AnimEngine cleaning up\n");
		StopAllMP3s();
		if (g_bAblDisconnect)
			ErrorMessage(
				"We're sorry, Facade is having trouble running, possibly because other applications are running, or some ot"
				"her factor limiting the CPU available.\n"
				"\n"
				"Please try running Facade again, after closing other applications or restarting.\n"
				"\n"
				"Also visit www.interactivestory.net for bug fixes and patches."
			);

		OutputEventLog(true);
		dprintf("Deallocating globals");
		for (int i = 0; i < g_spriteMgr.GetNumSprites(); ++i)
		{
			Sprite* spr = g_spriteMgr.GetSpritePtr(i);
			if (!spr)
				continue;
			spr->~Sprite(); //?
		}

		CloseHandle(g_ablRequestBufferMutexHandle);

		if (!DSound_Shutdown())
			ShouldntBeHere("DSound did not shutdown properly");

		wglMakeCurrent(g_hDeviceContext, 0);
		wglDeleteContext(g_hGLContext);
		if (s_hPalette)
			DeleteObject(s_hPalette);

		XCursor::theirCursor->~XCursor(); //?

		Keyboard_Shutdown();
		CoUninitialize();
		ReleaseDC(g_hMainAppWindow, g_hDeviceContext);

		exit(0);
		break;
	case WM_SIZE:
		ChangeSize();
		break;
	default:
		DefWindowProcA(hwnd, msg, wparam, lparam);
		break;
	}
}


void LoadTextureFilmstrip(ETextureFilmstrip filmstrip)
{
	bool isalpha = false;
	int width1, height1, width2, height2;
	width1 = height1 = width2 = height2 = 256;
	char texturenames[20][256];
	for (int i = 0; i < 20; i++)
		texturenames[i][0] = '\0';

	switch (filmstrip)
	{
	case eTextureFilmstrip_Plant: 			strcpy(texturenames[0], "plant.bmp"); 					isalpha = 1; break;
	case eTextureFilmstrip_Ashtray: 		strcpy(texturenames[0], "ashtray.bmp"); 				isalpha = 1; break;
	case eTextureFilmstrip_MartiniEmpty: 	strcpy(texturenames[0], "martini_empty_hold.bmp"); 		isalpha = 1; break;
	case eTextureFilmstrip_RedWineBottle: 	strcpy(texturenames[0], "redwinebottle.bmp"); 			isalpha = 1; break;
	case eTextureFilmstrip_WhiteWineBottle: strcpy(texturenames[0], "whitewinebottle.bmp"); 		isalpha = 1; break;
	case eTextureFilmstrip_MartiniClassic:
		strcpy(texturenames[0], "martini_empty_hold.bmp");
		strcpy(texturenames[1], "martini_classic_hold1.bmp");
		strcpy(texturenames[2], "martini_classic_hold33.bmp");
		strcpy(texturenames[3], "martini_classic_hold66.bmp");
		strcpy(texturenames[4], "martini_classic_hold100.bmp");
		strcpy(texturenames[5], "martini_empty_pdrink.bmp");
		strcpy(texturenames[6], "martini_classic_pdrink1.bmp");
		strcpy(texturenames[7], "martini_classic_pdrink33.bmp");
		strcpy(texturenames[8], "martini_classic_pdrink66.bmp");
		strcpy(texturenames[9], "martini_classic_pdrink100.bmp");
		strcpy(texturenames[10], "martini_empty_pdrink.bmp");
		strcpy(texturenames[11], "martini_classic_pdrink1.bmp");
		strcpy(texturenames[12], "martini_classic_pdrink33.bmp");
		strcpy(texturenames[13], "martini_classic_pdrink66.bmp");
		strcpy(texturenames[14], "martini_classic_pdrink100.bmp");
		isalpha = 1;
		break;
	case eTextureFilmstrip_MartiniCosmo:
		strcpy(texturenames[0], "martini_empty_hold.bmp");
		strcpy(texturenames[1], "martini_cosmo_hold1.bmp");
		strcpy(texturenames[2], "martini_cosmo_hold33.bmp");
		strcpy(texturenames[3], "martini_cosmo_hold66.bmp");
		strcpy(texturenames[4], "martini_cosmo_hold100.bmp");
		strcpy(texturenames[5], "martini_empty_pdrink.bmp");
		strcpy(texturenames[6], "martini_cosmo_pdrink1.bmp");
		strcpy(texturenames[7], "martini_cosmo_pdrink33.bmp");
		strcpy(texturenames[8], "martini_cosmo_pdrink66.bmp");
		strcpy(texturenames[9], "martini_cosmo_pdrink100.bmp");
		strcpy(texturenames[10], "martini_empty_pdrink.bmp");
		strcpy(texturenames[11], "martini_cosmo_pdrink1.bmp");
		strcpy(texturenames[12], "martini_cosmo_pdrink33.bmp");
		strcpy(texturenames[13], "martini_cosmo_pdrink66.bmp");
		strcpy(texturenames[14], "martini_cosmo_pdrink100.bmp");
		isalpha = 1;
		break;
	case eTextureFilmstrip_WineWhite:
		strcpy(texturenames[0], "wine_empty_hold.bmp");
		strcpy(texturenames[1], "wine_white_hold1.bmp");
		strcpy(texturenames[2], "wine_white_hold33.bmp");
		strcpy(texturenames[3], "wine_white_hold66.bmp");
		strcpy(texturenames[4], "wine_white_hold100.bmp");
		strcpy(texturenames[5], "wine_empty_pdrink.bmp");
		strcpy(texturenames[6], "wine_white_pdrink1.bmp");
		strcpy(texturenames[7], "wine_white_pdrink33.bmp");
		strcpy(texturenames[8], "wine_white_pdrink66.bmp");
		strcpy(texturenames[9], "wine_white_pdrink100.bmp");
		strcpy(texturenames[10], "wine_empty_pdrink.bmp");
		strcpy(texturenames[11], "wine_white_pdrink1.bmp");
		strcpy(texturenames[12], "wine_white_pdrink33.bmp");
		strcpy(texturenames[13], "wine_white_pdrink66.bmp");
		strcpy(texturenames[14], "wine_white_pdrink100.bmp");
		isalpha = 1;
		break;
	case eTextureFilmstrip_WineRed:
		strcpy(texturenames[0], "wine_empty_hold.bmp");
		strcpy(texturenames[1], "wine_red_hold1.bmp");
		strcpy(texturenames[2], "wine_red_hold33.bmp");
		strcpy(texturenames[3], "wine_red_hold66.bmp");
		strcpy(texturenames[4], "wine_red_hold100.bmp");
		strcpy(texturenames[5], "wine_empty_pdrink.bmp");
		strcpy(texturenames[6], "wine_red_pdrink1.bmp");
		strcpy(texturenames[7], "wine_red_pdrink33.bmp");
		strcpy(texturenames[8], "wine_red_pdrink66.bmp");
		strcpy(texturenames[9], "wine_red_pdrink100.bmp");
		strcpy(texturenames[10], "wine_empty_pdrink.bmp");
		strcpy(texturenames[11], "wine_red_pdrink1.bmp");
		strcpy(texturenames[12], "wine_red_pdrink33.bmp");
		strcpy(texturenames[13], "wine_red_pdrink66.bmp");
		strcpy(texturenames[14], "wine_red_pdrink100.bmp");
		isalpha = 1;
		break;
	case eTextureFilmstrip_Beerlight:
		strcpy(texturenames[0], "pint_empty_hold.bmp");
		strcpy(texturenames[1], "beer_light_hold1.bmp");
		strcpy(texturenames[2], "beer_light_hold33.bmp");
		strcpy(texturenames[3], "beer_light_hold66.bmp");
		strcpy(texturenames[4], "beer_light_hold100.bmp");
		strcpy(texturenames[5], "pint_empty_pdrink.bmp");
		strcpy(texturenames[6], "beer_light_pdrink1.bmp");
		strcpy(texturenames[7], "beer_light_pdrink33.bmp");
		strcpy(texturenames[8], "beer_light_pdrink66.bmp");
		strcpy(texturenames[9], "beer_light_pdrink100.bmp");
		strcpy(texturenames[10], "pint_empty_pdrink.bmp");
		strcpy(texturenames[11], "beer_light_pdrink1.bmp");
		strcpy(texturenames[12], "beer_light_pdrink33.bmp");
		strcpy(texturenames[13], "beer_light_pdrink66.bmp");
		strcpy(texturenames[14], "beer_light_pdrink100.bmp");
		isalpha = 1;
		break;
	case eTextureFilmstrip_BeerDark:
		strcpy(texturenames[0], "pint_empty_hold.bmp");
		strcpy(texturenames[1], "beer_dark_hold1.bmp");
		strcpy(texturenames[2], "beer_dark_hold33.bmp");
		strcpy(texturenames[3], "beer_dark_hold66.bmp");
		strcpy(texturenames[4], "beer_dark_hold100.bmp");
		strcpy(texturenames[5], "pint_empty_pdrink.bmp");
		strcpy(texturenames[6], "beer_dark_pdrink1.bmp");
		strcpy(texturenames[7], "beer_dark_pdrink33.bmp");
		strcpy(texturenames[8], "beer_dark_pdrink66.bmp");
		strcpy(texturenames[9], "beer_dark_pdrink100.bmp");
		strcpy(texturenames[10], "pint_empty_pdrink.bmp");
		strcpy(texturenames[11], "beer_dark_pdrink1.bmp");
		strcpy(texturenames[12], "beer_dark_pdrink33.bmp");
		strcpy(texturenames[13], "beer_dark_pdrink66.bmp");
		strcpy(texturenames[14], "beer_dark_pdrink100.bmp");
		isalpha = 1;
		break;
	case eTextureFilmstrip_LiquorScotch:
		strcpy(texturenames[0], "liquor_empty_hold.bmp");
		strcpy(texturenames[1], "liquor_scotch_hold1.bmp");
		strcpy(texturenames[2], "liquor_scotch_hold33.bmp");
		strcpy(texturenames[3], "liquor_scotch_hold66.bmp");
		strcpy(texturenames[4], "liquor_scotch_hold100.bmp");
		strcpy(texturenames[5], "liquor_empty_pdrink.bmp");
		strcpy(texturenames[6], "liquor_scotch_pdrink1.bmp");
		strcpy(texturenames[7], "liquor_scotch_pdrink33.bmp");
		strcpy(texturenames[8], "liquor_scotch_pdrink66.bmp");
		strcpy(texturenames[9], "liquor_scotch_pdrink100.bmp");
		strcpy(texturenames[10], "liquor_empty_pdrink.bmp");
		strcpy(texturenames[11], "liquor_scotch_pdrink1.bmp");
		strcpy(texturenames[12], "liquor_scotch_pdrink33.bmp");
		strcpy(texturenames[13], "liquor_scotch_pdrink66.bmp");
		strcpy(texturenames[14], "liquor_scotch_pdrink100.bmp");
		isalpha = 1;
		break;
	case eTextureFilmstrip_LiquorClear:
		strcpy(texturenames[0], "liquor_empty_hold.bmp");
		strcpy(texturenames[1], "liquor_clear_hold1.bmp");
		strcpy(texturenames[2], "liquor_clear_hold33.bmp");
		strcpy(texturenames[3], "liquor_clear_hold66.bmp");
		strcpy(texturenames[4], "liquor_clear_hold100.bmp");
		strcpy(texturenames[5], "liquor_empty_pdrink.bmp");
		strcpy(texturenames[6], "liquor_clear_pdrink1.bmp");
		strcpy(texturenames[7], "liquor_clear_pdrink33.bmp");
		strcpy(texturenames[8], "liquor_clear_pdrink66.bmp");
		strcpy(texturenames[9], "liquor_clear_pdrink100.bmp");
		strcpy(texturenames[10], "liquor_empty_pdrink.bmp");
		strcpy(texturenames[11], "liquor_clear_pdrink1.bmp");
		strcpy(texturenames[12], "liquor_clear_pdrink33.bmp");
		strcpy(texturenames[13], "liquor_clear_pdrink66.bmp");
		strcpy(texturenames[14], "liquor_clear_pdrink100.bmp");
		isalpha = 1;
		break;
	case eTextureFilmstrip_Water:
		strcpy(texturenames[0], "pint_empty_hold.bmp");
		strcpy(texturenames[1], "water_hold1.bmp");
		strcpy(texturenames[2], "water_hold33.bmp");
		strcpy(texturenames[3], "water_hold66.bmp");
		strcpy(texturenames[4], "water_hold100.bmp");
		strcpy(texturenames[5], "pint_empty_pdrink.bmp");
		strcpy(texturenames[6], "water_pdrink1.bmp");
		strcpy(texturenames[7], "water_pdrink33.bmp");
		strcpy(texturenames[8], "water_pdrink66.bmp");
		strcpy(texturenames[9], "water_pdrink100.bmp");
		strcpy(texturenames[10], "pint_empty_pdrink.bmp");
		strcpy(texturenames[11], "water_pdrink1.bmp");
		strcpy(texturenames[12], "water_pdrink33.bmp");
		strcpy(texturenames[13], "water_pdrink66.bmp");
		strcpy(texturenames[14], "water_pdrink100.bmp");
		isalpha = 1;
		break;
	case eTextureFilmstrip_Cola:
		strcpy(texturenames[0], "pint_empty_hold.bmp");
		strcpy(texturenames[1], "cola_hold1.bmp");
		strcpy(texturenames[2], "cola_hold33.bmp");
		strcpy(texturenames[3], "cola_hold66.bmp");
		strcpy(texturenames[4], "cola_hold100.bmp");
		strcpy(texturenames[5], "pint_empty_pdrink.bmp");
		strcpy(texturenames[6], "cola_pdrink1.bmp");
		strcpy(texturenames[7], "cola_pdrink33.bmp");
		strcpy(texturenames[8], "cola_pdrink66.bmp");
		strcpy(texturenames[9], "cola_pdrink100.bmp");
		strcpy(texturenames[10], "pint_empty_pdrink.bmp");
		strcpy(texturenames[11], "cola_pdrink1.bmp");
		strcpy(texturenames[12], "cola_pdrink33.bmp");
		strcpy(texturenames[13], "cola_pdrink66.bmp");
		strcpy(texturenames[14], "cola_pdrink100.bmp");
		isalpha = 1;
		break;
	case eTextureFilmstrip_Juice:
		strcpy(texturenames[0], "pint_empty_hold.bmp");
		strcpy(texturenames[1], "juice_hold1.bmp");
		strcpy(texturenames[2], "juice_hold33.bmp");
		strcpy(texturenames[3], "juice_hold66.bmp");
		strcpy(texturenames[4], "juice_hold100.bmp");
		strcpy(texturenames[5], "pint_empty_pdrink.bmp");
		strcpy(texturenames[6], "juice_pdrink1.bmp");
		strcpy(texturenames[7], "juice_pdrink33.bmp");
		strcpy(texturenames[8], "juice_pdrink66.bmp");
		strcpy(texturenames[9], "juice_pdrink100.bmp");
		strcpy(texturenames[10], "pint_empty_pdrink.bmp");
		strcpy(texturenames[11], "juice_pdrink1.bmp");
		strcpy(texturenames[12], "juice_pdrink33.bmp");
		strcpy(texturenames[13], "juice_pdrink66.bmp");
		strcpy(texturenames[14], "juice_pdrink100.bmp");
		isalpha = 1;
		break;
	case eTextureFilmstrip_Painting:			strcpy(texturenames[0], "painting1.bmp");			break;
	case eTextureFilmstrip_LittlePainting:		strcpy(texturenames[0], "littlepainting.bmp");		break;
	case eTextureFilmstrip_WeddingPicture:		strcpy(texturenames[0], "weddingpic.bmp");			break;
	case eTextureFilmstrip_Tuscany_Romance:		strcpy(texturenames[0], "tuscany.bmp");				break;
	case eTextureFilmstrip_GownSketch1:			strcpy(texturenames[0], "gownsketch1.bmp");			break;
	case eTextureFilmstrip_PostIts:				strcpy(texturenames[0], "post_its.bmp");			break;
	case eTextureFilmstrip_GownSketch4:			strcpy(texturenames[0], "gownsketch4.bmp");			break;
	case eTextureFilmstrip_Phone:
		strcpy(texturenames[0], "phoneontable.bmp");
		strcpy(texturenames[1], "phoneheld.bmp");
		isalpha = 1;
		height1 = width1 = height2 = width2 = 256;
		break;
	case eTextureFilmstrip_AnsweringMachine:	strcpy(texturenames[0], "answeringmachine.bmp");	isalpha = 1;	height1 = width1 = height2 = width2 = 256;	break;
	case eTextureFilmstrip_EightBall:
		strcpy(texturenames[0], "eightball.bmp");
		strcpy(texturenames[1], "eightball2.bmp");
		isalpha = 1;
		height1 = width1 = height2 = width2 = 256;
		break;
	case eTextureFilmstrip_Brassbull:			strcpy(texturenames[0], "brassbull.bmp");			isalpha = 1;	break;
	case eTextureFilmstrip_Trinket1:			strcpy(texturenames[0], "trinket1.bmp");			isalpha = 1;	break;
	case eTextureFilmstrip_Trinket2:			strcpy(texturenames[0], "trinket2.bmp");			isalpha = 1;	break;
	case eTextureFilmstrip_Trinket3:			strcpy(texturenames[0], "trinket3.bmp");			isalpha = 1;	break;
	case eTextureFilmstrip_Trinket4:			strcpy(texturenames[0], "trinket4.bmp");			isalpha = 1;	break;
	case eTextureFilmstrip_Trinket5:			strcpy(texturenames[0], "trinket5.bmp");			isalpha = 1;	break;
	case eTextureFilmstrip_Trinket6:			strcpy(texturenames[0], "trinket6.bmp");			isalpha = 1;	break;
	case eTextureFilmstrip_Trinket7:			strcpy(texturenames[0], "trinket7.bmp");			isalpha = 1;	break;
	case eTextureFilmstrip_Trinket8:			strcpy(texturenames[0], "trinket8.bmp");			isalpha = 1;	break;
	case eTextureFilmstrip_Rug:					strcpy(texturenames[0], "rug.bmp");					height2 = height1 = 512;			break;
	case eTextureFilmstrip_FrontDoor:			strcpy(texturenames[0], "frontdoor.bmp");			break;
	case eTextureFilmstrip_BedroomDoor:			strcpy(texturenames[0], "bedroomdoor.bmp");			break;
	case eTextureFilmstrip_Elevator:			strcpy(texturenames[0], "elevator.bmp");			break;
	case eTextureFilmstrip_ElevatorButton:		strcpy(texturenames[0], "elevatorButton.bmp");		height1 = width1 = height2 = width2 = 128;	break;
	case eTextureFilmstrip_YellowFont1:			strcpy(texturenames[0], "yellowfont.bmp");			height1 = width1 = height2 = width2 = 512;	isalpha = 1;	break;
	case eTextureFilmstrip_YellowFont2:			strcpy(texturenames[0], "yellowfont2.bmp");			height1 = width1 = height2 = width2 = 512;	isalpha = 1;	break;
	case eTextureFilmstrip_EightBallAnims:
		strcpy(texturenames[0], "eightballMessages.bmp");
		strcpy(texturenames[1], "eightballFadein.bmp");
		height1 = width1 = 512;
		height2 = width2 = 83;
		break;
	case eTextureFilmstrip_BlackStripes:		strcpy(texturenames[0], "blackstripes.bmp");		break;
	case eTextureFilmstrip_WhiteCabinetDoor1:	strcpy(texturenames[0], "whitecabinetdoor.bmp");	break;
	case eTextureFilmstrip_WhiteCabinetDoor2:	strcpy(texturenames[0], "whitecabinetdoor2.bmp");	break;
	case eTextureFilmstrip_NYC_Sky:				strcpy(texturenames[0], "nyc.bmp");					height1 = width1 = height2 = width2 = 512;	break;
	case eTextureFilmstrip_Instruct_Keyboard:	strcpy(texturenames[0], "instr_keyboard.bmp");		break;
	case eTextureFilmstrip_Instruct_ArrowKeys:	strcpy(texturenames[0], "instr_arrowKeys.bmp");		break;
	case eTextureFilmstrip_Instruct_Mouse:		strcpy(texturenames[0], "instr_mouse.bmp");			break;
	case eTextureFilmstrip_LabelLogo:			strcpy(texturenames[0], "label_logo.bmp");			height1 = width1 = height2 = width2 = 128;	break;
	case eTextureFilmstrip_RedCurtain:			strcpy(texturenames[0], "redCurtain.bmp");			height1 = width1 = height2 = width2 = 512;	break;
	case eTextureFilmstrip_TipJar:				strcpy(texturenames[0], "tipjar.bmp");				break;
	default:									ShouldntBeHere("unknown texture filmstrip");		return;
	}

	textureentry_t* pEntry = &g_TextureFilmstrips[filmstrip];
	for (int i = 0; i < 20; i++)
	{
		animengine_assert(texturenames[0][0] != '\0');
		if (texturenames[i][0] == '\0')
			continue;

		char filepath[256];
		strcpy(filepath, "textures\\");
		strcat(filepath, texturenames[i]);
		strcpy(pEntry->entries[i].name, filepath);
		pEntry->numentries++;
		bool alreadyhavetexture = false;
		for (int j = 0; j < eTextureFilmstrip_Max; j++)
		{
			if (!g_TextureFilmstrips[j].valid)
				continue;
			for (int k = 0; k < 20; k++)
			{
				gltexture_t* pGLTex = &g_TextureFilmstrips[j].entries[k];
				if (pGLTex->texid >= 0 && !strcmp(filepath, pGLTex->name))
				{
					alreadyhavetexture = true;
					pEntry->entries[i].texid = pGLTex->texid;
					break;
				}
			}
			if (alreadyhavetexture)
				break;
		}
		if (!alreadyhavetexture)
			pEntry->entries[i].texid = LoadTexture(filepath, isalpha, GL_LINEAR, GL_LINEAR, GL_LINEAR);
	}
	pEntry->isalpha = isalpha;
	pEntry->width1 = width1;
	pEntry->height1 = height1;
	pEntry->width2 = width2;
	pEntry->height2 = height2;
	pEntry->valid = true;

}

void CreateRoom()
{

}
void InitEnvironment()
{

}
void InitCinematography()
{

}
void InitPlayerNavigation()
{

}
Sprite* CreateCursorSprite()
{
	Sprite* spr = new Sprite();
	spr->InitSprite(eSpriteID_cursor, "cursor", false, true, -1);
	return spr;
}
void InitGraceSprite()
{

}
void InitTripSprite()
{

}
//void InitTransRotScale(CharacterSprite* spr = nullptr, int a2 = 0)
//{
//
//}
void InitCollisionZonesAndPathPlanGrid()
{

}
void InitPathPlanning()
{

}
void InitCircleDrawing()
{

}
void InitQueuedUpRequestedPerformance()
{

}

void InitGlobals()
{
	dprintf("\n---------------------------\nInitting globals");
	srand(time(0));

	g_startTime = GetTickCount();
	g_numFramesRun = 0;
	g_bShouldRenderThisFrame = 1;
	g_SkipFramesCounter = 0;
	g_bAssertionThrown = 0;
	g_launchErrorString[0] = '\0';
	g_bAllQuadLineOutline = 0;
	g_bAllCircleOutline = 0;

	for (int i = eTextureFilmstrip_YellowFont1; i <= eTextureFilmstrip_TipJar; ++i)
		LoadTextureFilmstrip((ETextureFilmstrip)i);

	InitEnvironment();
	InitCinematography();
	InitPlayerNavigation();

	g_PlayerSprite = new Sprite();
	g_PlayerSprite->InitSprite(eSpriteID_player, "player", false, false, -1);
	g_PlayerSprite->SetUpdateMe(false);

	g_CursorSprite = CreateCursorSprite();
	g_CursorSprite->SetUpdateMe(false);

	CreateRoom();

	InitGraceSprite();
	InitTripSprite();
	InitTransRotScale(0, 0);
	InitCollisionZonesAndPathPlanGrid();
	InitPathPlanning();
	InitCircleDrawing();
	InitQueuedUpRequestedPerformance();

	g_soundSource.InitSoundSource("sounds\\global\\globalsnd.txt");
	g_soundSourceID = g_soundMgr.AddSoundSource(g_soundSource);

	BuildBitmapFont();

	XCursor::theirCursor = new XCursor();
	XCursor::theirCursor->InitCursor();
	XCursor::theirCursor->JumpCutToCursor(eCursorType_Idle, true);
	LoadIconA(g_hAppInstance, (LPCSTR)IDI_FACADEWINDOWICON);

	for (int i = 0; i < 32; i++)
		g_curTextInput[i] = 0;
	g_curTextCharInput = 0;
}
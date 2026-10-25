#include "facadecommon.h"
#include "glfuncs.h"
#include "keyboarddevice.h"
#include "sounddevice.h"
#include "zone.h"
#include "initialization.h"
#include "facade_mdldata.h"

#include "classes/AblRequestBuffer.h"
#include "classes/Controller.h"
#include "classes/SoundMgr.h"
#include "classes/SoundSource.h"
#include "classes/Sprite/CharacterSprite.h"
#include "classes/XCursor.h"

#include "classes/Sprite/AnsweringMachineSprite.h"
#include "classes/Sprite/DoorSprite.h"
#include "classes/Sprite/DrinkSprite.h"
#include "classes/Sprite/EightballMessageSprite.h"
#include "classes/Sprite/PhoneSprite.h"

#include "ABLRequestsPerform_impl.h"

#define DISABLE_ABLTIMEOUT

enum ECursorContext
{
	eCursorContext_idle = 0,
	eCursorContext_holding,
	eCursorContext_sip,
	eCursorContext_knock,
	eCursorContext_comfort,
	eCursorContext_hug,
	eCursorContext_kiss,
	eCursorContext_throw
};

ECursorContext g_cursorContext;

#define MAX_CURSORPOSHISTORY 30

int g_startTime;

int g_mousePosition[2];
bool g_mouseLButtonClicked;

int g_quitAppCountdown = -1;

HINSTANCE g_hAppInstance;
HDC g_hDeviceContext;
HGLRC g_hGLContext;
HANDLE g_ablRequestBufferMutexHandle;

Sprite* g_PlayerSprite;
Sprite* g_CursorSprite;
Sprite* g_cursorContextSprite;

XYZPointf g_cursorPositionHistory[MAX_CURSORPOSHISTORY];

bool g_bDisplayDialogText;

float g_curFps = 30.0;
int g_millisecondsToSleep = 30;
int g_renderEveryNthFrame = 1;
int g_cpuClockSpeed = 2000;

CharacterSprite* g_GraceSprite;
CharacterSprite* g_TripSprite;

XYZPointf g_prevPlayerPosition;
XYZPointf g_prevPlayerRotation;

XYZPointf g_autoMovePlayerPt;
float g_autoMovePlayerRot; //yaw

textureentry_t g_TextureFilmstrips[eTextureFilmstrip_Max];

AblRequestBuffer* g_ablrequestbuffer;
int g_blastFlagSignalToAbl;

EBaseScreenMessage g_baseScreenMessage;

char g_launchErrorString[52];

#define TRICIRCLE_ITERATIONS 21

float g_triangleFanSineCache[TRICIRCLE_ITERATIONS];
float g_triangleFanCosineCache[TRICIRCLE_ITERATIONS + 10];

float g_playerTransAmt;
float g_playerRotAmt;
int g_AutoMovePlayerStatus;

int g_frameWhenLastCinematographyMove;
int g_framePlayerLastMoved;
int g_numFramesStillAtFoyerWallEdge;

int g_ticksIntroStarted = -1;
int g_introEventCtr = -1;

int g_framesWhenLastSentPlayerTypedCue = 0;


bool g_bLockedInMSToSleep;

bool g_bAllowCursorKeysInEditor = false;

bool g_bF1Pressed = false;
bool g_bF2Pressed = false;
bool g_bF3Pressed = false;
bool g_bF4Pressed = false;
bool g_bF6Pressed = false;
bool g_bF7Pressed = false;
bool g_bF8Pressed = false;
bool g_bF9Pressed = false;
bool g_bF12Pressed = false;
bool g_bEscapePressed = false;
bool g_bSpacePressed = false;

static bool g_bAblDisconnect = false;

SpriteMgr g_spriteMgr;

SoundMgr g_soundMgr;
SoundSource g_soundSource;

Controller g_PlayerXController;
Controller g_PlayerYController;
Controller g_PlayerZController;
Controller g_PlayerRotController;

Controller g_autoTurnController;
float g_autoTurnAmount;



int g_introChoices[8];
float g_introTimings[8 + 1];
float g_introBaseTimings[8][6] = {
	{5.0, 0.0, 0.0, 0.0, 0.0, 0.0},
	{1.8, 1.7, 2.1, 2.1, 2.4, 0.0},
	{4.4, 6.8, 6.8, 5.8, 5.6, 0.0},
	{4.8, 6.8, 5.8, 5.3, 0.0, 0.0},
	{6.8, 8.8, 7.7, 6.9, 0.0, 0.0},
	{2.0, 2.4, 4.7, 2.1, 5.1, 3.9},
	{2.0, 2.0, 2.0, 2.0, 2.0, 2.0},
	{2.0, 0.0, 0.0, 0.0, 0.0, 0.0}
};
float g_initialIntroDelay = 2.0;
float g_startTitleFadeIn;



int g_numPanels = 0;
panelmdlframes_t g_panelModelFrames[MAX_PANELS];
panelmdlinfos_t g_panelModelInfo[MAX_PANELS];
panelmdlquadpolys_t g_panelModelQuadPolys[MAX_PANELS];


BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID lpReserved)
{
	static HANDLE s_hDLLMemoryFilemap;
	static void* s_DLLMappedMemoryPointer;

	if (reason == DLL_PROCESS_ATTACH)
	{
		dprintf("Process attaching to DLL\n");
		g_ablRequestBufferMutexHandle = CreateMutex(NULL, FALSE, "BufferMutex");
		animengine_assert(g_ablRequestBufferMutexHandle);

		uint32_t allocate_size = sizeof(AblRequest) * REQUESTBUFFERSIZE 
			+ sizeof(unknownablstructure1) 
			+ sizeof(unknownablstructure2) * 1000;

		s_hDLLMemoryFilemap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, allocate_size, "dllmemfilemap");
		if (!s_hDLLMemoryFilemap)
			return 0;
		s_DLLMappedMemoryPointer = MapViewOfFile(s_hDLLMemoryFilemap, FILE_MAP_WRITE, NULL, NULL, NULL);
		if (!s_DLLMappedMemoryPointer)
			return 0;

		g_ablrequestbuffer = new AblRequestBuffer();
		g_ablrequestbuffer->InitBuffer(s_DLLMappedMemoryPointer, GetLastError() != ERROR_ALREADY_EXISTS);
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		dprintf("Animengine detaching from DLL\n");
		UnmapViewOfFile(s_DLLMappedMemoryPointer);
		CloseHandle(s_hDLLMemoryFilemap);
	}

	return 1;
}


static bool IsEditorActive() {
	return g_scriptDialogHWnd || g_faceDialogHWnd;
}

static void ProcessAblRequests(bool clear) {

	static int timeAtStart = 0;
	static int whenLastPrint = 0;
	static int numIndivAblRequestsProcessedThisPeriod = 0;
	static int performAllRequestsTicksAccum = 0;
	static int numPerformAllRequestsSinceLastStatistic = 0;
	static bool lastPrintGuard = false;

	if (g_bQuittingApp)
		return;
#ifndef DISABLE_ABLTIMEOUT
	if (g_lastFrameGotAblRequest > 0 && !g_bAtEndOfDrama && g_numFramesRun > g_lastFrameGotAblRequest + 600)
	{
		g_bQuittingApp = true;
		g_bAblDisconnect = true;
		MessageBox(NULL, "USER WARNING\n", "façade is closing due to a lack of response from abl.\n", MB_OK);
		SendMessage(g_hMainAppWindow, WM_CLOSE, 0, 0);
		return;
	}
#endif

	timeAtStart = GetTickCount();
	if (!lastPrintGuard)
	{
		lastPrintGuard = true;
		whenLastPrint = timeAtStart;
	}

	if (WaitForSingleObject(g_ablRequestBufferMutexHandle, 10000))
	{
		dprintf("Anim engine timed out waiting for access to abl request buffer; owner %s", g_ablrequestbuffer->unknown001->name);
	}
	else
	{
		strcpy(g_ablrequestbuffer->unknown001->name, "ProcessAblRequests");
		numIndivAblRequestsProcessedThisPeriod += g_ablrequestbuffer->PerformAllRequests();
		g_ablrequestbuffer->unknown001->name[0] = '\0';
		ReleaseMutex(g_ablRequestBufferMutexHandle);
		performAllRequestsTicksAccum += GetTickCount() - timeAtStart;
		++numPerformAllRequestsSinceLastStatistic;
		if (clear)
		{
			performAllRequestsTicksAccum = 0;
			numPerformAllRequestsSinceLastStatistic = 0;
			numIndivAblRequestsProcessedThisPeriod = 0;
			whenLastPrint = GetTickCount();
		}
	}

}

static void StartPlayerMoveControllers(XYZPointf point, float f1, float f2, bool option)
{
	XYZPointf playerpos = g_PlayerSprite->GetPosition();
	XYZPointf playerrot = g_PlayerSprite->GetRotation();
	float v10 = f1 - playerrot.m_y;
	if (v10 >= -180)
	{
		if (v10 > 180)
			f1 -= 360;
	}
	else
	{
		f1 += 360;
	}
	if (option)
	{
#define SETCONTROLLERLIMITS(controller, val1, val2) \
	if (val1 != val2) \
	{ \
		if (val1 > val2) \
		{ \
			controller.SetMinVal(val2); \
			controller.SetMaxVal(10000); \
		} \
		else \
		{ \
			controller.SetMaxVal(val2); \
			controller.SetMinVal(-10000); \
		} \
	}

		SETCONTROLLERLIMITS(g_PlayerXController, playerpos.m_x, point.m_x);
		SETCONTROLLERLIMITS(g_PlayerYController, playerpos.m_y, point.m_y);
		SETCONTROLLERLIMITS(g_PlayerZController, playerpos.m_z, point.m_z);

#undef SETCONTROLLERLIMITS

	}
	g_PlayerXController.SetVal(playerpos.m_x, false);
	g_PlayerXController.SetTargetByRate(point.m_x, f2, eControllerType_unknown03, playerpos.m_x - g_prevPlayerPosition.m_x);
	g_PlayerYController.SetVal(playerpos.m_y, false);
	g_PlayerYController.SetTargetByRate(point.m_y, 1, eControllerType_unknown03, playerpos.m_y - g_prevPlayerPosition.m_y);
	g_PlayerZController.SetVal(playerpos.m_z, false);
	g_PlayerZController.SetTargetByRate(point.m_z, f2, eControllerType_unknown03, playerpos.m_z - g_prevPlayerPosition.m_z);

	g_PlayerRotController.SetVal(playerrot.m_y, false);
	g_PlayerRotController.SetTargetByRate(f1, f2, eControllerType_unknown03, playerrot.m_y - g_prevPlayerRotation.m_y);
}

static void DoHugComfortKissPlayerMotion(Sprite* spr, float f1) {
	XYZPointf playerpos = g_PlayerSprite->GetPosition();
	XYZPointf actorpos = spr->GetPosition();

	float angle = GetXZAngleBetweenPoints(playerpos, actorpos);

	XYZPointf pointatangle = PointAtAngleAndDist(actorpos, angle + 180, f1);

	StartPlayerMoveControllers(XYZPointf(pointatangle.m_x, playerpos.m_y, playerpos.m_z), angle, 1, 0);
	g_AutoMovePlayerStatus = 4;
}

void UpdateFrontDoorCollisionZone(bool dooropen)
{
	for (int i = 0; i < 3; ++i)
	{
		Sprite* spr = nullptr;
		bool isdoor = i == 0;
		switch (i)
		{
			case 0: spr = g_spriteMgr.GetSpritePtrByName("front door"); break;
			case 1: spr = g_spriteMgr.GetSpritePtrByName("FD_leftpanel"); break;
			case 2: spr = g_spriteMgr.GetSpritePtrByName("FD_rightpanel"); break;
		}
		
		for (int j = 0; j < g_numZones; ++j)
		{
			Sprite* zonespr = g_zones[j].unknown001;
			if (zonespr != spr)
				continue;
			if (isdoor)
			{
				XZRectf rect = ComputeSingleCollisionRectForSprite(spr);
				if (dooropen)
				{
					rect.m_xmax = rect.m_xmin;
					rect.m_zmax = rect.m_zmin;
				}
				ModifyZoneRect(j, 0, rect);
			}
			spr->a1_unknown00011c = !dooropen;
		}
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

static void DivvyUpDiagonalWallsIntoSubzone(Sprite* spr)
{
	for (int i = 0; i < g_numZones; ++i)
	{
		Sprite* zonespr = g_zones[i].unknown001;
		if (zonespr != spr) continue;

		XZRectf rect = ComputeSingleCollisionRectForSprite(zonespr);

		const int iterations = 5;

		float riterations = 1.0 / iterations;

		float xminfactor = 0.0;
		float xmaxfactor = riterations;
		float zminfactor = 1.0 - riterations;
		float zmaxfactor = 1.0;

		for (int j = 0; j < iterations; ++i)
		{
			XZRectf zonerect;
			zonerect.m_xmin = (rect.m_xmax - rect.m_xmin) * xminfactor + rect.m_xmin;
			zonerect.m_xmax = (rect.m_xmax - rect.m_xmin) * xmaxfactor + rect.m_xmin;
			zonerect.m_zmin = (rect.m_zmax - rect.m_zmin) * zminfactor + rect.m_zmin;
			zonerect.m_zmax = (rect.m_zmax - rect.m_zmin) * zmaxfactor + rect.m_zmin;

			if (i == 0)
				AddRectToZone(i, zonerect);
			else
				ModifyZoneRect(i, 0, zonerect);

			xminfactor += riterations;
			xmaxfactor += riterations;

			zminfactor -= riterations;
			zmaxfactor -= riterations;
		}
	}
}

static void LaunchAblIfNeeded()
{
	static bool launchedabl_gate = false;
	if (!launchedabl_gate)
	{
		char parameters[500];
		dprintf("\n### Launching ABL...\n");
		sprintf(parameters,
			"-Xms70m "
			"-Xmx150m "
			"-XX:NewSize=25m "
			"-enableassertions "
			"-Xfuture "
			"-Xincgc "
			"-showversion "
			"-classpath "
			"../../classes;"
			"../../classes/jess.jar "
			"facade.Main");
		if (g_bPrintBetaInfo)
			strcat(parameters, " generateEventLog");
		HINSTANCE program = ShellExecute(NULL, "open", "..\\..\\j2re1.4.2_06\\bin\\java.exe", parameters, NULL, NULL);
		if((int)program <= 32)
			sprintf(g_launchErrorString, "ERROR launching ABL: %d", program);

		launchedabl_gate = true;
	}
}

static bool IsPlayerMoving() {
	if (g_prevPlayerPosition == g_PlayerSprite->GetPosition())
		return true;
	if (g_prevPlayerRotation == g_PlayerSprite->GetRotation())
		return true;

	return false;
}
static int GetSecondsSinceFrame(int frame) {
	return (int)((g_numFramesRun - frame) / 30.f);
}

static void UpdateTitleSequence()
{
	static bool g_bTitleMusicStarted = false;
	if (g_introEventCtr == -1)
		return;

	if (g_curTextCharInput && g_introEventCtr < 9)
	{
		//StopAllGlobalSounds();
		g_introEventCtr = 9;
		g_ticksIntroStarted = (int)((float)GetTickCount() - (g_startTitleFadeIn - 3.f) * 1000.f);
		if (!g_bTitleMusicStarted)
		{
			PerformMusic(eMusicAction_start, eMusicName_title);
			g_bTitleMusicStarted = true;
		}
	}
	if (g_introEventCtr == 0)
	{
		g_introEventCtr = 1;
		g_bPlayerInputActive = false;

		StartFade(-1.0);
		InitTransRotScale(nullptr, false);

		g_PlayerSprite->SetPosition(XYZPointf(-63, 97, -39));

		XYZPointf playerrot = g_PlayerSprite->GetRotation();
		playerrot.m_y = 24;
		g_PlayerSprite->SetRotation(playerrot);

		g_ticksIntroStarted = GetTickCount();
	}
	else
	{
		float timeelapsed = GetSecsSinceTicks(g_ticksIntroStarted);
		int tickspassed = GetTickCount() - g_ticksIntroStarted;

#define BEGIN_INTROEVENT(eventnum, timemark) \
		else if (g_introEventCtr == eventnum && timeelapsed >= timemark) \
		{ \
			dprintf("Doing intro event %d at secs %.3f, ticks %d\n", g_introEventCtr++, timeelapsed, tickspassed);

#define END_INTROEVENT() \
		}

		if (g_introEventCtr == 1 && timeelapsed >= g_initialIntroDelay)
		{
			dprintf("Doing intro event %d at secs %.3f, ticks %d\n", g_introEventCtr++, timeelapsed, tickspassed);
			PlayGlobalSound(eGlobalSound_tr_intro_start);
		}
		BEGIN_INTROEVENT(2, g_introTimings[1])
			PlayGlobalSound((EGlobalSounds)(g_introChoices[1] + eGlobalSound_tr_intro_a1));
		END_INTROEVENT()
		BEGIN_INTROEVENT(3, g_introTimings[2])
			PlayGlobalSound((EGlobalSounds)(g_introChoices[2] + eGlobalSound_tr_intro_b1));
		END_INTROEVENT()
		BEGIN_INTROEVENT(4, g_introTimings[3])
			PlayGlobalSound((EGlobalSounds)(g_introChoices[3] + eGlobalSound_tr_intro_c1));
			PerformMusic(eMusicAction_start, eMusicName_title);
			g_bTitleMusicStarted = true;
		END_INTROEVENT()
		BEGIN_INTROEVENT(5, g_introTimings[4])
			PlayGlobalSound((EGlobalSounds)(g_introChoices[4] + eGlobalSound_tr_intro_e1));
		END_INTROEVENT()
		BEGIN_INTROEVENT(6, g_introTimings[5])
			PlayGlobalSound((EGlobalSounds)(g_introChoices[5] + 73));
		END_INTROEVENT()
		BEGIN_INTROEVENT(7, g_introTimings[6])
			if (rand100(50))
				PlayGlobalSound((EGlobalSounds)(g_introChoices[6] + 79));
		END_INTROEVENT()
		BEGIN_INTROEVENT(8, g_introTimings[7])
			PlayGlobalSound((EGlobalSounds)(g_introChoices[7] + 85));
		END_INTROEVENT()
		BEGIN_INTROEVENT(9, (g_startTitleFadeIn + 1.0))
			StartFade(0.008);
		END_INTROEVENT()
		BEGIN_INTROEVENT(10, (g_startTitleFadeIn + 10.0))
			StartFade(-0.008);
		END_INTROEVENT()
		BEGIN_INTROEVENT(11, (g_startTitleFadeIn + 18.0))
			g_introEventCtr = -1;
			g_blastFlagSignalToAbl = 5;
			g_bTextCharWasPressed = 1;
		END_INTROEVENT()

		if (g_introEventCtr > 9)
		{
			XYZPointf playerpos = g_PlayerSprite->GetPosition();
			XYZPointf playerrot = g_PlayerSprite->GetRotation();

			float yawrad = ConvertWorldDegreesToRadians(playerrot.m_y);

			playerpos.m_x += cos(yawrad) * -0.2;
			playerpos.m_z += sin(yawrad) * -0.2;

			g_PlayerSprite->SetPosition(playerpos);

			DrawArbitraryScreenText("Façade",	 1, -60, 200, 20, -10, eColor_white, eColor_black);
			DrawArbitraryScreenText("by",		23, -72,  50,  8, -10, eColor_white, eColor_black);
			DrawArbitraryScreenText("Michael",  -9, -80,  50, 10, -10, eColor_white, eColor_black);
			DrawArbitraryScreenText("Mateas",   -8, -88,  50, 10, -10, eColor_white, eColor_black);
			DrawArbitraryScreenText("and",		21, -83,  50,  8, -10, eColor_white, eColor_black);
			DrawArbitraryScreenText("Andrew",	35, -80,  50, 10, -10, eColor_white, eColor_black);
			DrawArbitraryScreenText("Stern",	37, -88,  50, 10, -10, eColor_white, eColor_black);
		}

	}
}

static bool AreWeDoingObjectOfferingOrTaking(CharacterSprite* holder, Sprite* obj){
	if (obj == g_GraceSprite || g_TripSprite->m_GestureFxnMgr1.unknown[2] != 164)
		return false;

	Sprite* spriteheldlist[5];
	int spriteheldinfolist[5];
	int listcount = holder->GetSpriteListAttachedToModelPoint(50, spriteheldlist, spriteheldinfolist);
	
	if (!obj && listcount == 0)
		return true;

	if (listcount > 0 && spriteheldlist[0] == obj)
		return true;

	return false;
}
static bool CheckCursorClickableOnSprite(Sprite* spr) {
	XYZPointf* bbox = spr->Get2DBoundingBox();
	XYZPointf cursorpos = g_CursorSprite->GetPosition();
	float v1 = 100.0;
	if (spr->m_eID == eSpriteID_frontDoor)
		v1 = 150.0;
	if (spr->m_eID == eSpriteID_bigAbstractPainting
		|| spr->m_eID == eSpriteID_littleTreePainting
		|| spr->m_eID == eSpriteID_italyPicture
		|| spr->m_eID == eSpriteID_elevatorButton)
	{
		v1 = 200;
	}
	if (!spr->GetGrabable())
		v1 /= 2.0;
	return cursorpos.m_x >= bbox->m_x
		&& cursorpos.m_x <= bbox[1].m_x
		&& cursorpos.m_y >= bbox->m_y
		&& cursorpos.m_y <= bbox[2].m_y
		&& bbox[2].m_z <= -5.0
		&& cursorpos.m_z - v1 <= bbox[2].m_z;
}
static bool IsHugComfortKissAllowed() {

	DoorSprite* SpritePtrByID = (DoorSprite*)g_spriteMgr.GetSpritePtrByID(eSpriteID_frontDoor);
	bool check1 = !IsPlayerOutsideApt() || SpritePtrByID->IsDoorOpen();
	Sprite** heldsprlist = g_CursorSprite->GetHeldSpritesList();
	bool check2 = heldsprlist[0] != nullptr
		&& !heldsprlist[0]->GetAmIStillGrabDropAnimating()
		&& heldsprlist[0]->m_eObjectType == eObjectType_Drink
		|| g_CursorSprite->GetPosition().m_y >= -60.0;
	bool check3 = g_AutoMovePlayerStatus == 2 || g_bTripCarry;
	return check1 && !check2 && !check3;
}
static bool IsCursorOverAShoulder(Sprite* actor) {
	animengine_assert(actor->GetIsCharacterSprite());

	//undone

	return false;
}
static bool IsCursorOverUpperBody(Sprite* actor) {
	animengine_assert(actor->GetIsCharacterSprite());

	//undone

	return false;
}
static bool IsCursorOverFace(Sprite* actor, bool option) {
	animengine_assert(actor->GetIsCharacterSprite());

	//undone

	return false;
}
static XZPointf ComputeSurfaceDropXZPoint(XZRectf rect, float f1, float f2, float f3)
{
	XZPointf xzpoint;

	if (rect.m_xmin > (double)rect.m_xmax)
	{
		VAL_SWAP(float, rect.m_xmax, rect.m_xmin);
	}
	if (rect.m_zmin > (double)rect.m_zmax)
	{
		VAL_SWAP(float, rect.m_zmax, rect.m_zmin);
	}
	float v1 = rect.m_xmax - rect.m_xmin;
	animengine_assert(v1 >= 0.0);
	float v2 = rect.m_zmax - rect.m_zmin;
	animengine_assert(v2 >= 0.0);
	float factor = Normalize0to360(f3);
	if (factor < 315 && factor > 45)
	{
		if (factor < 45 || factor > 135)
		{
			if (factor < 135 || factor > 225)
			{
				xzpoint.m_x = (1 - f2) * v1 + rect.m_xmin;
				xzpoint.m_z = (1 - f1) * v2 + rect.m_zmin;
			}
			else
			{
				xzpoint.m_x = (1 - f1) * v1 + rect.m_xmin;
				xzpoint.m_z = v2 * f2 + rect.m_zmin;
			}
		}
		else
		{
			xzpoint.m_x = v1 * f2 + rect.m_xmin;
			xzpoint.m_z = v2 * f1 + rect.m_zmin;
		}
	}
	else
	{
		xzpoint.m_x = v1 * f1 + rect.m_xmin;
		xzpoint.m_z = (1.0 - f2) * v2 + rect.m_zmin;
	}
	return xzpoint;
}
static void GetSurfaceClickedOnForSprite(Sprite* spr, Sprite*& sprout, int& surfindex, XYZPointf& pointout)
{
	XYZPointf cursorpos = g_CursorSprite->GetPosition();
	spritesurface_t* surflist = nullptr;

	float offset = 10;
	float x = 0;
	float y = 0;

	sprout = nullptr;
	surfindex = -1;

	for (int i = 0; i < g_spriteMgr.GetNumSprites() && surfindex == -1; ++i)
	{
		Sprite* thisspr = g_spriteMgr.GetSpritePtr(i);
		if (g_CursorSprite->GetAmIHoldingSprite(thisspr))
			continue;

		surflist = thisspr->GetSurfaceListPtr();
		for (int j = 0; j < thisspr->GetNumSurfaces(); ++j)
		{
			if (cursorpos.m_x < surflist[j].points2[0].m_x
				|| cursorpos.m_x > surflist[j].points2[1].m_x
				|| cursorpos.m_y < surflist[j].points2[0].m_y
				|| offset + surflist[j].points2[2].m_y < cursorpos.m_y
				|| cursorpos.m_z - 100.0 > surflist[j].points2[0].m_z)
			{
				continue;
			}
			if (surfindex == -1 
				|| surflist[j].points2[0].m_z > surflist[surfindex].points2[0].m_z)
			{
				sprout = thisspr;
				surfindex = j;
				x = (cursorpos.m_x - surflist[j].points2[0].m_x)
					/ (surflist[j].points2[1].m_x - surflist[j].points2[0].m_x);
				animengine_assert(x >= 0.0 && x <= 1.0);

				y = (cursorpos.m_y - surflist[j].points2[0].m_y)
					/ (surflist[j].points2[2].m_y - surflist[j].points2[0].m_y + offset);
				animengine_assert(y >= 0.0 && y <= 1.0);
			}
			if (!spr)
				return;
		}
	}
	if (surfindex == -1)
		return;

	XYZPointf bboxpoints[4];
	XYZPointf bboxpointsout[4];

	for (int i = 0; i < 4; ++i)
	{
		bboxpoints[i].m_x = surflist[surfindex].points1[i].m_x;
		bboxpoints[i].m_y = surflist[surfindex].points1[i].m_z;
		bboxpoints[i].m_z = 0.0;
	}
	GetXYBoundingBoxOf4Points(bboxpoints, bboxpointsout);
	animengine_assert(x >= 0 && x <= 1);
	animengine_assert(y >= 0 && y <= 1);

	XZRectf rect;
	rect.m_xmin = bboxpointsout[0].m_x;
	rect.m_zmin = bboxpointsout[0].m_y;

	rect.m_xmax = bboxpointsout[2].m_x;
	rect.m_zmax = bboxpointsout[2].m_y;


	XZPointf xzpoint = ComputeSurfaceDropXZPoint(rect, x, y, g_PlayerSprite->GetRotation().m_y);
	pointout.m_x = xzpoint.m_x;
	pointout.m_z = xzpoint.m_z;
	pointout.m_y = spr->GetVolume().m_y / 2.0 + surflist[surfindex].points1[0].m_y;
}

static void UpdateCursor()
{
	static int g_lastObjCursorNotice = -99999;

	//undone

	bool spriteinteractedwith = false;
	bool canknock = false;

	XYZPointf playerpos = g_PlayerSprite->GetPosition();

	float player_grace_dist = GetXZDistanceBetweenPoints(playerpos, g_GraceSprite->GetPosition());
	float player_trip_dist = GetXZDistanceBetweenPoints(playerpos, g_TripSprite->GetPosition());

	ECursorContext nextcursorcontext = g_cursorContext;

	XCursor::theirCursor->Update();
	g_playerGesture = ePlayerGesture_NOTAGESTURE;
	for (int i = 0; i < g_spriteMgr.GetNumSprites(); ++i)
	{
		Sprite* spr = g_spriteMgr.GetSpritePtr(i);
		spr->SetClickedThisFrame(false);
		spr->b1_unknown20584 = -1;
	}

	if (g_CursorSprite->GetNumHeldSprites() > 0
		&& g_numFramesRun > g_lastObjCursorNotice + 210)
	{
		//undone
	}

	float cursorxpos = ((g_mousePosition[0] - g_windowWidth / 2) / 5.f);
	float cursorypos = ((g_windowHeight / 2 - g_mousePosition[1]) / 5.f) - 28.0;
	XYZPointf cursorpos = XYZPointf(cursorxpos, cursorypos, -30);

	g_CursorSprite->SetPosition(cursorpos);

	for (int i = 29; i > 0; --i)
		g_cursorPositionHistory[i] = g_cursorPositionHistory[i - 1];

	g_cursorPositionHistory[0] = cursorpos;

	if (!g_bPlayerInputActive)
	{
		XCursor::theirCursor->JumpCutToCursor(eCursorType_Idle, true);
		return;
	}

	bool offeringortaking = false;
	bool graceholdingobj = false;
	bool tripholdingobj = false;

	if (XCursor::theirCursor->IsCursorStackEmpty() && !g_AutoMovePlayerStatus)
	{
		if (g_cursorContext == eCursorContext_knock
			|| g_cursorContext == eCursorContext_comfort
			|| g_cursorContext == eCursorContext_hug
			|| g_cursorContext == eCursorContext_kiss
			|| g_cursorContext == eCursorContext_throw)
		{
			nextcursorcontext = g_CursorSprite->GetNumHeldSprites() != 0 ? eCursorContext_holding : eCursorContext_idle;
		}

		for (int i = 0; i < g_spriteMgr.GetNumSprites(); ++i)
		{
			Sprite* spr = g_spriteMgr.GetSpritePtr(i);
			if (!spr->GetGrabable()
				|| !spr->GetDrawMe()
				|| !CheckCursorClickableOnSprite(spr)
				|| !spr->GetSpriteHoldingMe())
				continue;

			Sprite* holder = spr->GetSpriteHoldingMe();
			offeringortaking = holder->GetIsCharacterSprite()
				&& AreWeDoingObjectOfferingOrTaking((CharacterSprite*)holder, spr);

			if (holder == g_GraceSprite)	graceholdingobj = true;
			if (holder == g_TripSprite)		tripholdingobj = true;
		}
	}

	ESpriteID gesturespriteid = eSpriteID_NOTASPRITE;
	Sprite* victiminteraction = nullptr;

	if (!offeringortaking)
	{
		bool cursor_readyforaction = false;
		for (int i = 0; i < 2 && !cursor_readyforaction; i++)
		{
			CharacterSprite* actor;
			if (i != 0)
			{
				if (player_grace_dist >= player_trip_dist)
					actor = g_GraceSprite;
				else
					actor = g_TripSprite;
			}
			else
			{
				if (player_grace_dist >= player_trip_dist)
					actor = g_TripSprite;
				else
					actor = g_GraceSprite;
			}
			if (actor == g_GraceSprite)
			{
				if (graceholdingobj)
					continue;
				gesturespriteid = eSpriteID_FIRST_OBJECT;
			}
			else if (actor == g_TripSprite)
			{
				if (tripholdingobj)
					continue;
				gesturespriteid = eSpriteID_trip;
			}

			float selecteddist = actor == g_GraceSprite ? player_grace_dist : player_trip_dist;

			if (!IsPointVisibleToPlayer(actor->GetPosition(), false)
				|| selecteddist >= 75
				|| actor->AreWeInAnEditor(false)
				|| !IsHugComfortKissAllowed())
				continue;

			if (IsCursorOverAShoulder(actor))
			{
				nextcursorcontext = eCursorContext_comfort;
				victiminteraction = actor;
				cursor_readyforaction = true;
			}
			else if (IsCursorOverUpperBody(actor))
			{
				nextcursorcontext = eCursorContext_hug;
				victiminteraction = actor;
				cursor_readyforaction = true;
			}
			else if (IsCursorOverFace(actor, false))
			{
				nextcursorcontext = eCursorContext_kiss;
				victiminteraction = actor;
				cursor_readyforaction = true;
			}
		}

		if (g_cursorContext != nextcursorcontext)
		{
			switch (nextcursorcontext)
			{
			case eCursorContext_comfort:
				XCursor::theirCursor->JumpCutToCursor(eCursorType_Comfort, 1);
				g_cursorContext = nextcursorcontext;
				break;
			case eCursorContext_hug:
				XCursor::theirCursor->JumpCutToCursor(eCursorType_Hug, 1);
				g_cursorContext = nextcursorcontext;
				break;
			case eCursorContext_kiss:
				XCursor::theirCursor->JumpCutToCursor(eCursorType_Kiss, 1);
				g_cursorContext = nextcursorcontext;
				break;
			case eCursorContext_throw:
				XCursor::theirCursor->JumpCutToCursor(eCursorType_Throw, 1);
				g_cursorContext = nextcursorcontext;
				break;
			default:
				break;
			}
		}
	}

	if (!g_mouseLButtonClicked
		|| !victiminteraction
		|| offeringortaking
		|| !XCursor::theirCursor->IsCursorStackEmpty() || g_AutoMovePlayerStatus)
	{
		LABEL_99:
		int numheldsprites = g_CursorSprite->GetNumHeldSprites();
		if (numheldsprites <= 0)
		{
			for (int i = 0; i < g_spriteMgr.GetNumSprites(); i++)
			{
				Sprite* spr = g_spriteMgr.GetSpritePtr(i);
				if (spr->GetGrabable()
					&& spr->GetDrawMe()
					&& CheckCursorClickableOnSprite(spr))
				{
					Sprite* spriteholdingme = spr->GetSpriteHoldingMe();
					bool spritenotbeingheld = spriteholdingme == 0;
					bool spriteholdingmeisofferingortakingobject = 0;
					if (spriteholdingme)
					{
						spriteholdingmeisofferingortakingobject = spriteholdingme->GetIsCharacterSprite()
							&& AreWeDoingObjectOfferingOrTaking((CharacterSprite*)spriteholdingme, spr);
					}
					if (spritenotbeingheld || spriteholdingmeisofferingortakingobject)
					{
						if (!g_mouseLButtonClicked)
						{
							nextcursorcontext = eCursorContext_knock;
						}
						else
						{
							if (spritenotbeingheld)
							{
								g_CursorSprite->SetSpriteToHold(spr, 0, -DATA_NULLVAL);
								spr->m_bGettingGrabbed = true;
								spr->m_GrabDropAnimFactor = 0.0;
								spr->m_targetPos = spr->GetPosition();
								spr->RemoveFromAnySurfaceIMayBeOn();
							}
							else
							{
								animengine_assert(spriteholdingmeisofferingortakingobject);
								spriteholdingme->DropSprite(spr);
								g_CursorSprite->SetSpriteToHold(spr, false, -DATA_NULLVAL);
								spr->SetPosition(g_CursorSprite->GetPosition());
								spr->m_bGettingGrabbed = false;
							}
							spriteinteractedwith = true;
							XCursor::theirCursor->JumpCutToCursor(eCursorType_Idle, true);
							XCursor::theirCursor->PushTransitionToNeutral(eCursorType_Holding);
							g_cursorContext = nextcursorcontext = eCursorContext_holding;
							if (!spriteholdingmeisofferingortakingobject)
							{
								spr->b1_unknown20584 = 0;
								spr->m_pInteractor = g_PlayerSprite;
							}
							AddEventInfoToLog(eEventLogType_EnterInteraction,
								eSpriteID_player,
								nullptr, false,
								g_spriteMgr.GetSpriteIDBySpritePtr(spr), 0);
						}
					}
				}
				else if (spr->GetClickable()
					&& CheckCursorClickableOnSprite(spr))
				{
					if (g_mouseLButtonClicked)
					{
						spr->SetClickedThisFrame(true);
						spriteinteractedwith = true;
						dprintf("Clicked on %s\n", spr->m_name);
					}
					else
					{
						nextcursorcontext = eCursorContext_knock;
						if (spr->m_eObjectType == eObjectType_Door)
						{
							if (((DoorSprite*)spr)->IsKnockOccurring())
							{
								nextcursorcontext = eCursorContext_idle;
							}
							else if (((DoorSprite*)spr)->IsKnockPossible())
							{
								canknock = true;
							}
						}
					}
				}
			}
		}
		else if (g_mouseLButtonClicked)
		{
			Sprite** heldsprlist = g_CursorSprite->GetHeldSpritesList();

			Sprite* spr;
			int surfindex;
			XYZPointf point;
			GetSurfaceClickedOnForSprite(nullptr, spr, surfindex, point);

			if (g_cursorContext == 2 && heldsprlist[0] && heldsprlist[0]->m_eObjectType == eObjectType_Drink)
			{
				((DrinkSprite*)heldsprlist[0])->PlayerSipDrink();
				nextcursorcontext = eCursorContext_holding;
			}
			else if (spr)
			{
				for (int i = 0; i < numheldsprites && !spriteinteractedwith; ++i)
				{
					Sprite* thisspr = heldsprlist[i];
					if (!thisspr->GetAmIStillGrabDropAnimating())
					{
						GetSurfaceClickedOnForSprite(thisspr, spr, surfindex, point);
						if (spr)
						{
							thisspr->m_bGettingGrabbed = true;
							thisspr->m_bGettingDropped = true;
							thisspr->m_GrabDropAnimFactor = MATH_PI;
							thisspr->m_targetPos = point;
							thisspr->b1_unknown20321 = spr;
							thisspr->b1_unknown20322 = surfindex;
							spriteinteractedwith = 1;
							if (g_cursorContext == 2)
							{
								nextcursorcontext = eCursorContext_idle;
							}
							else
							{
								XCursor::theirCursor->PushTransitionToNeutral(eCursorType_Idle);
								g_cursorContext = nextcursorcontext = eCursorContext_idle;
							}
							AddEventInfoToLog(eEventLogType_StopInteraction, eSpriteID_player,
								nullptr, false,
								g_spriteMgr.GetSpriteIDBySpritePtr(thisspr), 0);
						}
					}
				}
			}
		}
		else
		{
			float m_x = fabs(cursorpos.m_x);

			if (m_x >= 20.0 || cursorpos.m_y >= -60.0)
			{
				if (g_cursorContext == 2)
					nextcursorcontext = eCursorContext_holding;
			}
			else
			{
				Sprite** sprlist = g_CursorSprite->GetHeldSpritesList();
				if (sprlist[0] != nullptr)
				{
					if (!sprlist[0]->GetAmIStillGrabDropAnimating()
						&& sprlist[0]->m_eObjectType == eObjectType_Drink)
					{
						if (sprlist[0]->c1_unknown00001 > 0 && !((DrinkSprite*)sprlist[0])->IsSipping())
						{
							//v60 = 1; //unused
							nextcursorcontext = eCursorContext_sip;
						}
					}
				}
			}
		}
		if (XCursor::theirCursor->IsCursorStackEmpty()
			&& g_cursorContext != nextcursorcontext
			&& !g_AutoMovePlayerStatus)
		{
			g_cursorContext = nextcursorcontext;
			switch (nextcursorcontext)
			{
			case eCursorContext_idle:
				XCursor::theirCursor->JumpCutToCursor(eCursorType_Idle, true);
				break;
			case eCursorContext_holding:
				XCursor::theirCursor->JumpCutToCursor(eCursorType_Holding, true);
				break;
			case eCursorContext_sip:
				XCursor::theirCursor->JumpCutToCursor(eCursorType_Sip, true);
				break;
			case eCursorContext_knock:
				if (canknock)
					XCursor::theirCursor->JumpCutToCursor(eCursorType_Knock, true);
				else
					XCursor::theirCursor->JumpCutToCursor(eCursorType_unknown1_animframe1, true);
				break;
			default:
				ShouldntBeHere("unknown g_cursorContext");
				break;
			}
		}
		return;
	}
	switch (g_cursorContext)
	{
	case eCursorContext_comfort:
		g_playerGesture = ePlayerGesture_comfort;
		g_playerGestureSpriteID = gesturespriteid;
		g_cursorContextSprite = victiminteraction;
		DoHugComfortKissPlayerMotion(victiminteraction, 30);
		AddEventInfoToLog(eEventLogType_Comfort, eSpriteID_player, nullptr, false, gesturespriteid, 0);
		break;
	case eCursorContext_hug:
		g_playerGesture = ePlayerGesture_hug;
		g_playerGestureSpriteID = gesturespriteid;
		g_cursorContextSprite = victiminteraction;
		DoHugComfortKissPlayerMotion(victiminteraction, 20);
		AddEventInfoToLog(eEventLogType_Hug, eSpriteID_player, nullptr, false, gesturespriteid, 0);
		break;
	case eCursorContext_kiss:
		g_playerGesture = ePlayerGesture_kiss;
		g_playerGestureSpriteID = gesturespriteid;
		g_cursorContextSprite = victiminteraction;
		DoHugComfortKissPlayerMotion(victiminteraction, 15);
		AddEventInfoToLog(eEventLogType_Kiss, eSpriteID_player, nullptr, false, gesturespriteid, 0);
		break;
	case eCursorContext_throw:
		g_playerGesture = ePlayerGesture_throwDrink;
		g_playerGestureSpriteID = gesturespriteid;
		g_cursorContextSprite = victiminteraction;
		AddEventInfoToLog(eEventLogType_ThrowDrink, eSpriteID_player, nullptr, false, gesturespriteid, 0);
		break;
	default:
		goto LABEL_99; //what a nightmare
	}
}
static void UpdateEnvironment()
{
	for (int i = 0; i < g_numZones; ++i)
	{
		Sprite* spr = g_zones[i].unknown001;
		if (!spr) continue;

		if (spr->GetCollidableType() == eCollidableType_SimpleBox)
			ModifyZoneRect(i, 0, ComputeSingleCollisionRectForSprite(spr));
	}

	if (g_numFramesRun == 2)
	{
		UpdateFrontDoorCollisionZone(false);
		DivvyUpDiagonalWallsIntoSubzone(g_spriteMgr.GetSpritePtrByName("FY_leftwall1"));
		DivvyUpDiagonalWallsIntoSubzone(g_spriteMgr.GetSpritePtrByName("FY_rightwall1"));
	}

	//if (g_printMapCtr >= 0 && !(g_numFramesRun % 3))
	//	PrintPathPlanGrid();
}
static void UpdateCinematography()
{
	g_autoTurnController.UpdateController();

	if (IsPlayerMoving())
		g_framePlayerLastMoved = g_numFramesRun;

	if (!g_bCinematographyOn || IsEditorActive())
		return;

	if (g_autoTurnController.GetIsMoving())
	{
		//undone
	}
	else if (g_bRequestAutoFraming && !IsPlayerMoving()
		|| GetSecondsSinceFrame(g_frameWhenLastCinematographyMove) >= 5
		&& GetSecondsSinceFrame(g_framePlayerLastMoved) >= 2
		&& !(g_numFramesRun % 30))
	{
		//undone
	}
	g_bRequestAutoFraming = false;
}

static void UpdateAndRenderAllSprites(bool transparent_only)
{
	static bool bDrawKitchenCabinets = false;

	bool playerinkitchen = g_PlayerSprite->GetPosition().m_x > -140.0;

	if (bDrawKitchenCabinets != playerinkitchen || g_numFramesRun < 5)
	{
		bDrawKitchenCabinets = playerinkitchen;

		g_spriteMgr.GetSpritePtrByName("KI_longwallcabfront1")->SetDrawMe(bDrawKitchenCabinets);
		g_spriteMgr.GetSpritePtrByName("KI_longwallcabfront2")->SetDrawMe(bDrawKitchenCabinets);
		g_spriteMgr.GetSpritePtrByName("KI_longwallcabfront3")->SetDrawMe(bDrawKitchenCabinets);
		g_spriteMgr.GetSpritePtrByName("KI_longwallcabfront4")->SetDrawMe(bDrawKitchenCabinets);
		g_spriteMgr.GetSpritePtrByName("KI_longwallcabtop1")->SetDrawMe(bDrawKitchenCabinets);
		g_spriteMgr.GetSpritePtrByName("KI_longwallcabtop2")->SetDrawMe(bDrawKitchenCabinets);
		g_spriteMgr.GetSpritePtrByName("KI_longwallcabtop3")->SetDrawMe(bDrawKitchenCabinets);
		g_spriteMgr.GetSpritePtrByName("KI_longwallcabtop4")->SetDrawMe(bDrawKitchenCabinets);
	}

	//LOBYTE(g_GraceSprite->data.inherit.inherit.b1_unknown20292) = false;
	//LOBYTE(g_TripSprite->data.inherit.inherit.b1_unknown20292) = false;

	for (int i = 0; i < g_spriteMgr.GetNumSprites(); i++)
	{
		Sprite* sprite = g_spriteMgr.GetSpritePtr(i);

		// salsatobias : temporary :(
		if (sprite->GetIsCharacterSprite())
			continue;

		Sprite* holder = sprite->GetSpriteHoldingMe();

		sprite->boolean1 = false;

		bool shouldupdate = sprite->GetUpdateMe() 
			&& !holder 
			&& transparent_only == sprite->m_bTransparent;

		if (sprite->a1_unknown00005)
			shouldupdate = false;

		bool shoulddraw = shouldupdate && sprite->GetDrawMe();

		if (sprite->a1_unknown00012 != -1)
		{
			shoulddraw = IsPointWithinZone(g_PlayerSprite->GetPosition(), GetZoneIDForRoomZone(sprite->a1_unknown00012));
		}
		if (sprite->GetIsHeadSprite()
			/*&& !BYTE1(SpritePtr[1].vftable->GetModelPointPosition)*/)
		{
			shouldupdate = 0;
			shoulddraw = 0;
		}


		if (shouldupdate)	sprite->UpdateSprite();
		if (shoulddraw)		DrawSprite(sprite, false);
	}
	for (int i = 0; i < g_spriteMgr.GetNumSprites(); i++)
	{
		Sprite* sprite = g_spriteMgr.GetSpritePtr(i);
		Sprite* holder = sprite->GetSpriteHoldingMe();

		bool shouldupdate = sprite->GetUpdateMe()
			&& holder
			&& transparent_only == sprite->m_bTransparent;

		if (sprite->a1_unknown00005)
			shouldupdate = true;

		bool shoulddraw = shouldupdate && sprite->GetDrawMe();

		if (shouldupdate)	sprite->UpdateSprite();
		if (shoulddraw)		DrawSprite(sprite, holder != g_CursorSprite && !sprite->a1_unknown00005);
	}
}

static void UpdateAndRenderWorld()
{
	ChangeSize();
	if (g_bBlackBackground)
		glClearColor(0.0, 0.0, 0.0, 0.0);
	else
		glClearColor(0.4, 0.4, 0.5, 0.0);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	g_opacity = 1.0;
	if (!g_bPlayerChoosingName && !g_bAtEndOfDrama)
		UpdateAndRenderAllSprites(false);
	DrawPlayerScreenText();
	DrawNPCsScreenText();
	DrawScreenMessage();
	UpdateTitleSequence();
	if (!g_bPlayerChoosingName && !g_bAtEndOfDrama)
	{
		DrawExtraEnvironmentStuff();
		UpdateAndRenderAllSprites(true);
	}
	UpdateFade();
	glFlush();
	g_mouseLButtonClicked = false;
	//if (*(int*)dword_10180DB4 >= 0 && !(g_numFramesRun % 4))
	//	sub_1000227F();
}
static void UpdateMP3Fades()
{

}
static void LoadingMessage()
{
	static int fadeCurtainInCtr = 0;
	static bool bFadeCurtainOut = false;
	static int ctr = 0;

	char uppertext[52];
	char undertext[52];

	if (g_ticksIntroStarted >= 0)
		return;

	DrawRedCurtain();
	if (!fadeCurtainInCtr)
	{
		CutToBlack();
		StartFade(0.015);
	}

	if (fadeCurtainInCtr < 60)
	{
		fadeCurtainInCtr++;
		return;
	}

	if (g_bAtEndOfLoad && !bFadeCurtainOut)
	{
		bFadeCurtainOut = true;
		StartFade(-0.015);
		return;
	}

	strcpy(undertext, " ");
	strcpy(uppertext, " ");

	if (g_numFramesRun <= 90)
	{
		strcpy(undertext, "Loading Façade . . .");
	}
	else
	{
		if (ctr <= 30)
			strcpy(undertext, "Loading Façade");
		else if (ctr <= 45)
			strcpy(undertext, "Loading Façade .");
		else if (ctr <= 60)
			strcpy(undertext, "Loading Façade . .");
		else if (ctr <= 105)
			strcpy(undertext, "Loading Façade . . .");

		ctr++;

		if (ctr == 105)
			ctr = 0;
	}
	strcpy(uppertext, "(May take up to 60 seconds)");
	if (g_launchErrorString[0] != '\0')
	{
		strcpy(undertext, "Load Error");
		strcpy(uppertext, g_launchErrorString);
	}


	DrawArbitraryScreenText(undertext, -10, 0, 200, 10, 0, eColor_white, eColor_black);
	DrawArbitraryScreenText(uppertext, 0, -10, 200, 5, 0, eColor_white, eColor_black);

	float verticaltextcursor = -50;

	DrawArbitraryScreenText("www.interactivestory.net", 2, verticaltextcursor, 350, 5, 0, eColor_white, eColor_black);
	verticaltextcursor -= 6;
	DrawArbitraryScreenText("(c)opyright 2005-2006 Procedural Arts", -11, verticaltextcursor, 350, 5, 0, eColor_white, eColor_black);
	verticaltextcursor -= 6;
	DrawArbitraryScreenText("an [auto mata] release ", 4.5, verticaltextcursor, 350, 5, 0, eColor_white, eColor_black);
	verticaltextcursor -= 6;
	DrawArbitraryScreenText("v1.1", 19.5, verticaltextcursor, 350, 5, 0, eColor_white, eColor_black);
	verticaltextcursor -= 6;
	if (g_bPrintBetaInfo)
		DrawArbitraryScreenText("Debug", 18.5, verticaltextcursor, 350, 5, 0, eColor_white, eColor_black);
}

static void EditCharacterScript(CharacterSprite* spr) {
	
}
static void EditCharacterFace(CharacterSprite* spr) {

}
static void GetRotationKeyPresses()
{
	if (!g_bQuittingApp)
	{
		if (g_quitAppCountdown <= 0 || (--g_quitAppCountdown, g_quitAppCountdown > 0))
		{
			if (GetForegroundWindow() == g_hMainAppWindow
				|| GetForegroundWindow() == g_scriptDialogHWnd
				|| GetForegroundWindow() == g_faceDialogHWnd)
			{
				if ((GetAsyncKeyState(VK_F1) & 0x8000) == 0)		g_bF1Pressed = false;
				if ((GetAsyncKeyState(VK_F2) & 0x8000) == 0)		g_bF2Pressed = false;
				if ((GetAsyncKeyState(VK_F3) & 0x8000) == 0)		g_bF3Pressed = false;
				if ((GetAsyncKeyState(VK_F4) & 0x8000) == 0)		g_bF4Pressed = false;
				if ((GetAsyncKeyState(VK_F6) & 0x8000) == 0)		g_bF6Pressed = false;
				if ((GetAsyncKeyState(VK_F7) & 0x8000) == 0)		g_bF7Pressed = false;
				if ((GetAsyncKeyState(VK_F8) & 0x8000) == 0)		g_bF8Pressed = false;
				if ((GetAsyncKeyState(VK_F9) & 0x8000) == 0)		g_bF9Pressed = false;
				if ((GetAsyncKeyState(VK_F12) & 0x8000) == 0)		g_bF12Pressed = false;
				if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) == 0)	g_bEscapePressed = false;
				if ((GetAsyncKeyState(VK_SPACE) & 0x8000) == 0)		g_bSpacePressed = false;

				if (g_faceDialogHWnd)
				{
					//sub_100015A5();
				}
				else if (!g_scriptDialogHWnd || g_bAllowCursorKeysInEditor)
				{
					//if (g_cursorKeysControllingSprite)
					//	(*(int(__thiscall**)(int))(*(_DWORD*)g_cursorKeysControllingSprite + 16))(g_cursorKeysControllingSprite);
					if ((GetAsyncKeyState(VK_LEFT) < 0 || GetAsyncKeyState(VK_RIGHT) < 0) /*&&!g_cursorKeysControllingSprite*/)
					{
						g_bPlayerRotateLeft = GetAsyncKeyState(VK_LEFT) < 0;
						g_bPlayerRotateRight = GetAsyncKeyState(VK_RIGHT) < 0;
					}
					if (GetAsyncKeyState(VK_UP) < 0 || GetAsyncKeyState(VK_DOWN) < 0)
					{
						if (/*g_cursorKeysControllingSprite ||*/ (GetAsyncKeyState(VK_UP) & 0x8000) == 0)
						{
							if (/*!g_cursorKeysControllingSprite &&*/ GetAsyncKeyState(VK_DOWN) < 0)
								g_bPlayerMoveBackwards = true;
						}
						else
						{
							g_bPlayerMoveForwards = true;
						}
					}
				}
			}
		}
		else
		{
#ifndef DISABLE_ABLTIMEOUT
			dprintf("quitAppCountdown quitting app!");
			g_bQuittingApp = 1;
			SendMessageA(g_hMainAppWindow, WM_CLOSE, 0, 0);
#endif
		}
	}
}

static void ClearNavigationFlags() {
	g_bPlayerRotateLeft = g_bPlayerRotateRight =
		g_bPlayerMoveForwards = g_bPlayerMoveBackwards =
		g_bPlayerMoveUpwards = g_bPlayerMoveDownwards =
		false;
}
static void EndPlayerAutoMove(bool option) {
	bool b1 = false;
	switch (g_AutoMovePlayerStatus)
	{
	case 1:	g_bSeatedOnCouch = !g_bSeatedOnCouch;	break;
	case 2:	g_bTripCarry = !g_bTripCarry;			break;
	case 4:
		XYZPointf cursorpos = g_cursorContextSprite->GetPosition();
		XYZPointf playerpos = g_PlayerSprite->GetPosition();
		if (GetXZDistanceBetweenPoints(playerpos, cursorpos) < 30)
			b1 = true;
		break;
	}
	g_AutoMovePlayerStatus = 0;
	g_playerTransAmt = 0.0;
	g_playerRotAmt = 0.0;
	ClearNavigationFlags();
	if (b1 && !option)
		DoHugComfortKissPlayerMotion(g_cursorContextSprite, 30);
}
static bool HackToFixStuckPlayerAtFoyerWallEdge(XYZPointf point) {
	if (point.m_x <= -134 || point.m_x >= -102 || point.m_z <= 86 || point.m_z >= 138)
	{
		g_numFramesStillAtFoyerWallEdge = 0;
		return false;
	}
	if (++g_numFramesStillAtFoyerWallEdge <= 600)
		return false;
	dprintf("## Repositioning player away from foyer wall\n");
	g_PlayerSprite->SetPosition(XYZPointf(-172, point.m_y, 107));
	return true;
}
static void GetNewPlayerPosition(float yaw, XYZPointf& out) {
	float factor = ConvertWorldDegreesToRadians(yaw);
	out.m_x += cos(factor) * g_playerTransAmt;
	out.m_z += sin(factor) * g_playerTransAmt;
}
static bool PlayerNavCollisionTest(Sprite* player, XYZPointf nextpos) {
	if (CollisionTestWithWorld(g_PlayerSprite, nextpos, 10, true))
		return true;

	XYZPointf playerpos = g_PlayerSprite->GetPosition();
	XYZPointf pointtest = XYZPointf(
		(playerpos.m_x + nextpos.m_x) / 2,
		playerpos.m_y,
		(playerpos.m_z + nextpos.m_z) / 2
	);
	if (!CollisionTestWithWorld(g_PlayerSprite, pointtest, 10, true))
	{
		g_PlayerSprite->SetPosition(nextpos);
		return false;
	}

	return true;
}

static void UpdatePlayerNavigation() {
	if (g_bQuittingApp || !g_bPlayerInputActive)
		return;

	static bool bAlreadyCollided = false;
	static int g_moveTowardsCenterOfRoomCountdown = 0;

	XYZPointf playerpos = g_PlayerSprite->GetPosition();
	XYZPointf playerrot = g_PlayerSprite->GetRotation();

	if (HackToFixStuckPlayerAtFoyerWallEdge(playerpos))
		return;

	if (g_AutoMovePlayerStatus)
	{
		if (g_PlayerXController.GetIsMoving()
			|| g_PlayerYController.GetIsMoving()
			|| g_PlayerZController.GetIsMoving()
			|| g_PlayerRotController.GetIsMoving())
		{
			g_PlayerXController.UpdateController();
			g_PlayerYController.UpdateController();
			g_PlayerZController.UpdateController();
			g_PlayerRotController.UpdateController();

			if (g_PlayerXController.GetIsMoving())		playerpos.m_x = g_PlayerXController.GetVal();
			if (g_PlayerYController.GetIsMoving())		playerpos.m_y = g_PlayerYController.GetVal();
			if (g_PlayerZController.GetIsMoving())		playerpos.m_z = g_PlayerZController.GetVal();
			if (g_PlayerRotController.GetIsMoving())	playerrot.m_y = g_PlayerRotController.GetVal();

			if (!CollisionTestWithWorld(g_PlayerSprite, playerpos, 10, false)
				|| g_AutoMovePlayerStatus == 1)
			{
				g_PlayerSprite->SetPosition(playerpos);
				g_PlayerSprite->SetRotation(playerrot);
			}
			else
			{
				EndPlayerAutoMove(true);
			}
		}
		else
		{
			EndPlayerAutoMove(false);
		}
		return;
	}

	XYZPointf playernextpos = playerpos;

	bool moving = false;
	bool rotating = false;

	float playermovforward_limit = FrameRateAdjust(1.5, 1);
	float playermovbackward_limit = FrameRateAdjust(-0.75, 1);
	float playerrotleft_limit = FrameRateAdjust(1.5, 1);
	float playerrotright_limit = FrameRateAdjust(-1.5, 1);

	const float playerrot_factor = FrameRateAdjust(0.09, 1);
	const float playermove_factor = FrameRateAdjust(0.09, 1);
	if (GetAsyncKeyState(VK_CONTROL) < 0)
	{
		playermovforward_limit = FrameRateAdjust(6.0, 1);
		playermovbackward_limit = FrameRateAdjust(-3.0, 1);
		playerrotleft_limit = FrameRateAdjust(2.4000001, 1);
		playerrotright_limit = FrameRateAdjust(-2.4000001, 1);
	}

	bool bVeryNearCharacter = false;
	bool dramaActorInView = false;
	bool dramaActorInTightView = false;
	for (int i = 0; i < 2 && !bVeryNearCharacter; ++i)
	{
		//undone
	}

	if (dramaActorInView)
	{
		if (g_bVeryNearCharacter)
		{
			//undone
		}
		else if (dramaActorInTightView)
		{
			//undone
		}
		//undone
	}

	if (g_bPlayerRotateLeft || g_bPlayerRotateRight)
	{
		if (g_bPlayerRotateLeft && g_playerRotAmt < (double)playerrotleft_limit)
		{
			if (g_playerRotAmt + playerrot_factor >= playerrotleft_limit)
				g_playerRotAmt = playerrotleft_limit;
			else
				g_playerRotAmt += playerrot_factor;
		}
		if (g_bPlayerRotateRight && g_playerRotAmt > (double)playerrotright_limit)
		{
			if (g_playerRotAmt - playerrot_factor <= playerrotright_limit)
				g_playerRotAmt = playerrotright_limit;
			else
				g_playerRotAmt -= playerrot_factor;
		}
		rotating = true;
	}
	bool rotation_within_limits = g_playerRotAmt > playerrotleft_limit
		|| g_playerRotAmt < playerrotright_limit;
	if (g_playerRotAmt != 0.0 && (!rotating || rotation_within_limits))
	{
		if (g_playerRotAmt > 0.001 || g_playerRotAmt < -0.001)
		{
			if (rotation_within_limits)
				g_playerRotAmt *= 0.935;
			else
				g_playerRotAmt *= 0.855;
		}
		else
		{
			g_playerRotAmt = 0;
		}
		rotating = true;
	}
	if (rotating)
	{
		playerrot.m_y = Normalize0to360(playerrot.m_y - g_playerRotAmt);
		g_PlayerSprite->SetRotation(playerrot);
	}

	if (g_bSeatedOnCouch && g_playerTransAmt != 0.0)
	{
		//undone
	}

	if (g_bTripCarry)
	{
		//undone
	}

	if (g_bPlayerMoveForwards || g_bPlayerMoveBackwards)
	{
		if (g_bPlayerMoveForwards && g_playerTransAmt < playermovforward_limit)
		{
			if (g_playerTransAmt + playermove_factor >= playermovforward_limit)
				g_playerTransAmt = playermovforward_limit;
			else
				g_playerTransAmt += playermove_factor;
		}
		if (g_bPlayerMoveBackwards && g_playerTransAmt > playermovbackward_limit)
		{
			if (g_playerTransAmt - playermove_factor / 2 <= playermovbackward_limit)
				g_playerTransAmt = playermovbackward_limit;
			else
				g_playerTransAmt -= playermove_factor / 2;
		}
		moving = true;
	}

	bool movement_within_limits = g_playerRotAmt > playermovforward_limit 
		|| g_playerTransAmt < playermovbackward_limit;
	if (g_playerTransAmt != 0 && (!moving || movement_within_limits))
	{
		if (g_playerTransAmt > 0.0099999998 || g_playerTransAmt < -0.0099999998)
		{
			if (movement_within_limits)
				g_playerTransAmt *= 0.92;
			else
				g_playerTransAmt *= 0.86;
		}
		else
		{
			g_playerTransAmt = 0;
		}
		moving = true;
	}

	if (moving)
		GetNewPlayerPosition(playerrot.m_y, playernextpos);

	if (g_bPlayerMoveUpwards || g_bPlayerMoveDownwards)
	{
		if (g_bPlayerMoveUpwards)	playernextpos.m_y += playermovforward_limit;
		if (g_bPlayerMoveDownwards)	playernextpos.m_y -= playermovforward_limit;
		moving = true;
	}
	ClearNavigationFlags();

	bool collided = false;

	if (g_moveTowardsCenterOfRoomCountdown > 0)
	{
		--g_moveTowardsCenterOfRoomCountdown;
		playernextpos = playerpos;
		float xzangle = GetXZAngleBetweenPoints(playerpos, XYZPointf(-20, 0, -100));
		GetNewPlayerPosition(xzangle, playernextpos);
		g_PlayerSprite->SetPosition(playernextpos);
		if (NormalizeNeg180to180(xzangle - playerrot.m_y) >= 0.0)
			g_bPlayerRotateRight = true;
		else
			g_bPlayerRotateLeft = true;
		return;
	}

	if (moving)
	{
		if (!bAlreadyCollided)
			bAlreadyCollided = CollisionTestWithWorld(g_PlayerSprite, playerpos, 0, true);

		if (bAlreadyCollided)
		{
			g_PlayerSprite->SetPosition(playernextpos);
			bAlreadyCollided = CollisionTestWithWorld(g_PlayerSprite, playernextpos, 10, true);
		}
		else
		{
			collided = PlayerNavCollisionTest(g_PlayerSprite, playernextpos);
			if (collided)
			{
				Sprite* couch = g_spriteMgr.GetSpritePtrByName("couch");
				if (CollisionTestWithSprite(g_PlayerSprite, playernextpos, 10, couch) &&
						IsPointVisibleToPlayer(couch->GetPosition(), false))
				{
					dprintf("Collision with couch, sitting");
					g_PlayerSprite->SetPosition(playernextpos);
					XYZPointf target(183, 73, 0);
					float rottarget = 0;
					if (playernextpos.m_z >= -400)
					{
						target.m_z = playernextpos.m_z;

						if (playernextpos.m_z >= -350)	target.m_z = -350;
						if (target.m_z <= -460)	target.m_z = -460;

						if (NormalizeNeg180to180(playerrot.m_y) >= 90)
							rottarget = 240;
						else
							rottarget = 300;
					}
					else
					{
						target.m_z = -470.0;
						rottarget = 213.0;
					}
					StartPlayerMoveControllers(target, rottarget, 3.0, 1);
					g_AutoMovePlayerStatus = 1;
					AddEventInfoToLog(eEventLogType_EnterInteraction, eSpriteID_player, nullptr, false, eSpriteID_couch, 0);
					return;
				}

				if (!IsPlayerOutsideApt())
				{
					bool inarea =  playerpos.m_x > -213 && playerpos.m_z < 109 && playerpos.m_x < -166 && playerpos.m_z > 13
								|| playerpos.m_x > -161 && playerpos.m_z < 148 && playerpos.m_x < -83 && playerpos.m_z > 74;
					
					if (inarea && g_moveTowardsCenterOfRoomCountdown <= 0)
						g_moveTowardsCenterOfRoomCountdown = 20;

					if (g_moveTowardsCenterOfRoomCountdown <= 0 && !g_bVeryNearCharacter && !inarea)
					{
						playernextpos = playerpos;
						GetNewPlayerPosition(playerrot.m_y - 90, playernextpos);
						bool collidedleft = PlayerNavCollisionTest(g_PlayerSprite, playernextpos);
						playernextpos = playerpos;
						GetNewPlayerPosition(playerrot.m_y + 90.0, playernextpos);
						bool collidedright = PlayerNavCollisionTest(g_PlayerSprite, playernextpos);
						if (!collidedleft || !collidedright)
						{
							float v61 = 0;
							int v60 = collidedleft ? -15 : 15;
							int v6 = collidedleft ? 16 : 0;
							int v59 = v6 + 120;
							while (collided)
							{
								float v25 = v61 < 0 ? -v61 : v61;
								float v24 = v59 < 0 ? -v59 : v59;
								if (v25 >= v24)
									break;
								v61 += v60;
								playernextpos = playerpos;
								GetNewPlayerPosition(v61 + playerrot.m_y, playernextpos);
								collided = PlayerNavCollisionTest(g_PlayerSprite, playernextpos);
							}
						}
						if (collided)
							playernextpos = playerpos;
					}
				}

				XYZPointf testingpoint;
				testingpoint = XYZPointf(playerpos.m_x + 20, playerpos.m_y, playerpos.m_z);
				bool collided1 = CollisionTestWithWorld(g_PlayerSprite, testingpoint, 10, 1);
				testingpoint = XYZPointf(playerpos.m_x - 20, playerpos.m_y, playerpos.m_z);
				bool collided2 = CollisionTestWithWorld(g_PlayerSprite, testingpoint, 10, 1);
				testingpoint = XYZPointf(playerpos.m_x, playerpos.m_y, playerpos.m_z + 20);
				bool collided3 = CollisionTestWithWorld(g_PlayerSprite, testingpoint, 10, 1);
				testingpoint = XYZPointf(playerpos.m_x, playerpos.m_y, playerpos.m_z - 20);
				bool collided4 = CollisionTestWithWorld(g_PlayerSprite, testingpoint, 10, 1);
				float playeryaw = g_PlayerSprite->GetRotation().m_y;

				animengine_assert(playeryaw >= 0 && playeryaw < 360);

				if (collided1 && collided2)
				{
					if ((playeryaw < 270 || playeryaw >= 360) 
						&& (playeryaw < 90 || playeryaw > 180))
						g_bPlayerRotateRight = true;
					else
						g_bPlayerRotateLeft = true;
				}
				else if (collided3 && collided4)
				{
					if ((playeryaw < 270 || playeryaw >= 360) 
						&& (playeryaw < 90 || playeryaw > 180))
						g_bPlayerRotateLeft = true;
					else
						g_bPlayerRotateRight = true;
				}
				else if (collided1 || collided2)
				{
					if (playeryaw < 90 || playeryaw > 270)
					{
						g_bPlayerRotateLeft = collided1;
						g_bPlayerRotateRight = collided2;
					}
					else
					{
						g_bPlayerRotateRight = collided1;
						g_bPlayerRotateLeft = collided2;
					}
				}
				else if (collided3 || collided4)
				{
					if (playeryaw < 0 || playeryaw > 180)
					{
						g_bPlayerRotateRight = collided3;
						g_bPlayerRotateLeft = collided4;
					}
					else
					{
						g_bPlayerRotateLeft = collided3;
						g_bPlayerRotateRight = collided4;
					}
				}
			}
		}
	}

	if(collided)
		g_playerTransAmt = 0;
	
}

static HPALETTE CreateWinGLPalette(HDC devicecontext)
{
	PIXELFORMATDESCRIPTOR description;
	DescribePixelFormat(devicecontext, GetPixelFormat(devicecontext), sizeof(description), &description);
	if ((description.dwFlags & 0x80) == 0)
		return NULL;

	int numpaletteentries = 1 << description.cColorBits;
	LOGPALETTE *paletteinfo = (LOGPALETTE*)malloc(4 * numpaletteentries + 8);
	paletteinfo->palVersion = 768;
	paletteinfo->palNumEntries = numpaletteentries;

	int regionred = (1 << description.cRedBits) - 1;
	int regiongreen = (1 << description.cGreenBits) - 1;
	int regionblue = (1 << description.cBlueBits) - 1;

	for (int i = 0; i < numpaletteentries; ++i)
	{
		paletteinfo->palPalEntry[i].peRed = (regionred & (i>>description.cRedShift)) * 255.0 / regionred;
		paletteinfo->palPalEntry[i].peGreen = (regiongreen & (i >> description.cGreenShift)) * 255.0 / regiongreen;
		paletteinfo->palPalEntry[i].peBlue = (regionblue & (i >> description.cBlueShift)) * 255.0 / regionblue;
	}

	HPALETTE hpal = CreatePalette(paletteinfo);
	SelectPalette(devicecontext, hpal, FALSE);
	RealizePalette(devicecontext);
	free(paletteinfo);
	return hpal;

}

//UNDONE
LRESULT CALLBACK mainloop(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static HPALETTE s_hPalette = NULL;
	static DWORD s_timeAtStartOfCounting = 0, s_frameTimeAccum = 0, s_framesSinceLastStatistic = 0;

	HRESULT returncode;

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
	case WM_EDITGRACESCRIPT:	EditCharacterScript(g_GraceSprite); return 0;
	case WM_EDITGRACEFACE:		EditCharacterFace(g_GraceSprite); return 0;
	case WM_EDITTRIPSCRIPT:		EditCharacterScript(g_TripSprite); return 0;
	case WM_EDITTRIPFACE:		EditCharacterFace(g_TripSprite); return 0;
	case WM_QUERYNEWPALETTE:
		returncode = S_OK;
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
				g_prevPlayerPosition = g_PlayerSprite->GetPosition();
				g_prevPlayerRotation = g_PlayerSprite->GetRotation();

				//undone
			}
			s_frameTimeAccum += GetTickCount() - tickcount_atstart;
			s_framesSinceLastStatistic++;

			bool clear = false;
			if (s_framesSinceLastStatistic == 90)
			{
				g_curFps = 90.f / ((float)(GetTickCount() - s_timeAtStartOfCounting) / 1000.f);
				int rounded_fps = FloatToInt(g_curFps);
				s_frameTimeAccum = s_framesSinceLastStatistic = 0;
				s_timeAtStartOfCounting = GetTickCount();

				clear = true;
				if (!g_bLockedInMSToSleep)
				{
					if (g_bDoingFPSAdjustment)
					{
						if (rounded_fps >= 25 || g_millisecondsToSleep == 10)
						{
							dprintf("\n## Locking in %d ms to sleep, %d fps\n", g_millisecondsToSleep, rounded_fps);
							g_bLockedInMSToSleep = 1;
							g_bDoingFPSAdjustment = 0;
						}
						else if (g_millisecondsToSleep <= 20 && g_renderEveryNthFrame == 1)
						{
							g_renderEveryNthFrame = 2;
							g_millisecondsToSleep = 30;
							dprintf("## Currently %d fps, rendering every other frame, resetting ms to sleep to %d", rounded_fps, 30);
						}
						else
						{
							g_millisecondsToSleep -= 5;
							if (g_millisecondsToSleep < 10)
								g_millisecondsToSleep = 10;
							dprintf("## Currently %d fps, decreasing ms to sleep to %d", rounded_fps, g_millisecondsToSleep);
						}
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
			ProcessAblRequests(clear);
			if (g_millisecondsToSleep > 0)
				Sleep(g_millisecondsToSleep);
			if (haventrenderedyet)
				InvalidateRect(hwnd, 0, 0);
		}
		break;

	case WM_MOUSEMOVE:
		g_mousePosition[0] = GET_X_LPARAM(lparam);
		g_mousePosition[1] = GET_Y_LPARAM(lparam);
		break;
	case WM_LBUTTONDOWN:
		g_mouseLButtonClicked = true;
		break;
	case WM_ACTIVATE:
		if (wparam && g_pKeyboard)
			g_pKeyboard->Acquire();
		break;
	case WM_CREATE:
		g_hAppInstance = ((CREATESTRUCTA*)lparam)->hInstance;
		g_hDeviceContext = GetDC(hwnd);
		selectglpixelformat(g_hDeviceContext);
		g_hGLContext = wglCreateContext(g_hDeviceContext);
		wglMakeCurrent(g_hDeviceContext, g_hGLContext);
		s_hPalette = CreateWinGLPalette(g_hDeviceContext);
		SetupRC();
		break;
	case WM_DESTROY:
		dprintf("\n### AnimEngine cleaning up\n");
		//StopAllMP3s();
		//if (g_bAblDisconnect)
		//	ErrorMessage(
		//		"We're sorry, Facade is having trouble running, possibly because other applications are running, or some ot"
		//		"her factor limiting the CPU available.\n"
		//		"\n"
		//		"Please try running Facade again, after closing other applications or restarting.\n"
		//		"\n"
		//		"Also visit www.interactivestory.net for bug fixes and patches."
		//	);

		OutputEventLog(true);
		dprintf("Deallocating globals");
		for (int i = 0; i < g_spriteMgr.GetNumSprites(); ++i)
		{
			Sprite* spr = g_spriteMgr.GetSpritePtr(i);
			if (!spr)
				continue;
			spr->~Sprite(); //? "delete spr" ?
		}

		CloseHandle(g_ablRequestBufferMutexHandle);

		if (!DSound_Shutdown())
			ShouldntBeHere("DSound did not shutdown properly");

		wglMakeCurrent(g_hDeviceContext, 0);
		wglDeleteContext(g_hGLContext);
		if (s_hPalette)
			DeleteObject(s_hPalette);

		XCursor::theirCursor->~XCursor(); //? "delete XCursor::theirCursor" ?

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

void StopAllMP3s()
{
	//undone
}
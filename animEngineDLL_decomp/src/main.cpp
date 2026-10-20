#include "facadecommon.h"
#include "glfuncs.h"
#include "keyboarddevice.h"
#include "sounddevice.h"

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

#define MAX_CURSORPOSHISTORY 30

int g_startTime;

int g_mousePosition[2];
bool g_mouseLButtonClicked;

int g_quitAppCountdown = -1;

int g_numPanels = 0;

HINSTANCE g_hAppInstance;
HDC g_hDeviceContext;
HGLRC g_hGLContext;
HANDLE g_ablRequestBufferMutexHandle;

Sprite* g_PlayerSprite;
Sprite* g_CursorSprite;
Sprite* g_cursorContextSprite;

XYZPointf g_cursorPositionHistory[MAX_CURSORPOSHISTORY];

int g_roomZoneIDs[1];

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

int g_textDisplayState;

SpriteMgr g_spriteMgr;

SoundMgr g_soundMgr;
SoundSource g_soundSource;
int g_soundSourceID;


Controller g_PlayerXController;
Controller g_PlayerYController;
Controller g_PlayerZController;
Controller g_PlayerRotController;

Controller g_autoTurnController;
float g_autoTurnAmount;



int g_introChoices[8];
float g_introTimings[8 + 1];
float g_introBaseTimings[8 * 6];
float g_initialIntroDelay = 2.0;
float g_startTitleFadeIn;

#define NUMRECTS_PERZONE 5
struct zone_t
{
	XZRectf rects[NUMRECTS_PERZONE];
	int numrects;
	Sprite* unknown001;
	bool unknown002;
};

#define MAXZONES 250
zone_t g_zones[MAXZONES];
int g_numZones;

/////////////////////MODEL FRAME/////////////////////
mdlframe_t g_GraceHeadModel_modelFrame[] = {
	{}//empty ?
};
mdlframe_t g_TripHeadModel_modelFrame[] = {
	{}//empty ?
};
mdlframe_t g_Couch_modelFrame[] = {
		{0.0, 0.0, -90.0},
		{0.0, 12.0, -90.0},
		{0.0, 12.0, 105.0},
		{0.0, 0.0, 105.0},
		{-55.0, 34.0, -90.0},
		{-40.0, 34.0, -90.0},
		{0.0, 12.0, -90.0},
		{0.0, 34.0, 105.0},
		{0.0, 34.0, 90.0},
		{0.0, 12.0, 90.0},
		{-55.0, 34.0, -135.0},
		{-55.0, 0.0, -135.0},
		{45.0, 34.0, 105.0},
		{45.0, 0.0, 105.0},
		{0.0, 24.0, -90.0},
		{0.0, 24.0, -30.0},
		{0.0, 12.0, -30.0},
		{0.0, 24.0, 30.0},
		{0.0, 12.0, 30.0},
		{0.0, 24.0, 90.0},
		{30.0, 24.0, -90.0},
		{30.0, 24.0, -30.0},
		{30.0, 24.0, 30.0},
		{30.0, 24.0, 90.0},
		{-40.0, 34.0, -135.0},
		{-40.0, 34.0, -90.0},
		{45.0, 24.0,90.0},
		{45.0, 34.0, 90.0},
		{40.0, 55.0, -120.0},
		{40.0, 55.0, -30.0},
		{40.0, 55.0, 30.0},
		{40.0, 55.0, 90.0},
		{-40.0, 52.0, -140.0},
		{-45.0, 21.0, -135.0},
		{45.0, 52.0, 90.0},
		{40.0, 21.0, 90.0},
		{30.0, 24.0, -120.0},
		{-40.0, 24.0, -120.0},
		{-40.0, 24.0, -90.0},
		{-40.0, 12.0, -90.0},
		{-55.0, 0.0, -90.0},
		{-55.0, 12.0, -90.0},
		{-40.0, 24.0, -135.0},
		{-40.0, 55.0, -135.0},
		{30.0, 55.0, -135.0},
		{40.0, 55.0, -135.0},
		{45.0, 52.0, 30.0},
		{45.0, 52.0, -30.0},
		{45.0, 52.0, -90.0},
		{-40.0, 52.0, -140.0},
		{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL}
};
mdlframe_t g_SideTable_modelFrame[] = {
		{-16.0, 0.0, -16.0},
		{-16.0, 30.0, -16.0},
		{-16.0, 30.0, -12.0},
		{-16.0, 0.0, -12.0},
		{-12.0, 30.0, -16.0},
		{-12.0, 0.0, -16.0},
		{-12.0, 30.0, -12.0},
		{-12.0, 0.0, -12.0},
		{-16.0, 0.0, 16.0},
		{-16.0, 30.0, 16.0},
		{-16.0, 30.0, 12.0},
		{-16.0, 0.0, 12.0},
		{-12.0, 30.0, 16.0},
		{-12.0, 0.0, 16.0},
		{-12.0, 30.0, 12.0},
		{-12.0, 0.0, 12.0},
		{12.0, 0.0, -16.0},
		{12.0, 30.0, -16.0},
		{12.0, 30.0, -12.0},
		{12.0, 0.0, -12.0},
		{16.0, 30.0, -16.0},
		{16.0, 0.0, -16.0},
		{16.0, 30.0, -12.0},
		{16.0, 0.0, -12.0},
		{12.0, 0.0, 16.0},
		{12.0, 30.0, 16.0},
		{12.0, 30.0, 12.0},
		{12.0, 0.0, 12.0},
		{16.0, 30.0, 16.0},
		{16.0, 0.0, 16.0},
		{16.0, 30.0, 12.0},
		{16.0, 0.0, 12.0},
		{-20.0, 29.0, -20.0},
		{-20.0, 32.0, -20.0},
		{-20.0, 32.0,20.0},
		{-20.0, 29.0, 20.0},
		{20.0, 32.0, -20.0},
		{20.0, 29.0, -20.0},
		{20.0, 32.0, 20.0},
		{20.0, 29.0, 20.0},
		{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL},
		{DATA_NULLVAL, 0.0, 0.0}
};
mdlframe_t g_WorkTable_modelFrame[] = {
		{-16.0, 0.0, -51.0},
		{-16.0, 52.0, -51.0},
		{-16.0, 52.0, -47.0},
		{-16.0, 0.0, -47.0},
		{-12.0, 52.0, -51.0},
		{-12.0, 0.0, -51.0},
		{-12.0, 52.0, -47.0},
		{-12.0, 0.0, -47.0},
		{-16.0, 0.0, 51.0},
		{-16.0, 52.0, 51.0},
		{-16.0, 52.0, 47.0},
		{-16.0, 0.0, 47.0},
		{-12.0, 52.0, 51.0},
		{-12.0, 0.0, 51.0},
		{-12.0, 52.0, 47.0},
		{-12.0, 0.0, 47.0},
		{12.0, 0.0, -51.0},
		{12.0, 52.0, -51.0},
		{12.0, 52.0, -47.0},
		{12.0, 0.0, -47.0},
		{16.0, 52.0, -51.0},
		{16.0, 0.0, -51.0},
		{16.0, 52.0, -47.0},
		{16.0, 0.0, -47.0},
		{12.0, 0.0, 51.0},
		{12.0, 52.0, 51.0},
		{12.0, 52.0, 47.0},
		{12.0, 0.0, 47.0},
		{16.0, 52.0, 51.0},
		{16.0, 0.0, 51.0},
		{16.0, 52.0, 47.0},
		{16.0, 0.0, 47.0},
		{-20.0, 54.0, -55.0},
		{-20.0, 57.0, -55.0},
		{-20.0, 57.0,55.0},
		{-20.0, 54.0, 55.0},
		{20.0, 57.0, -55.0},
		{20.0, 54.0, -55.0},
		{20.0, 57.0, 55.0},
		{20.0, 54.0, 55.0},
		{-20.0, 57.0, 0.0},
		{20.0, 57.0, 0.0},
		{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL},
		{DATA_NULLVAL, 0.0, 0.0}
};
mdlframe_t g_Cabinet_modelFrame[] = {
		{0.0, 0.0, 90.0},
		{-20.0, 0.0, 90.0},
		{-20.0, 0.0, 30.0},
		{0.0, 0.0, 30.0},
		{0.0, 20.0, 30.0},
		{0.0, 20.0, -30.0},
		{0.0, 0.0, -30.0},
		{-20.0, 0.0, -30.0},
		{-20.0, 0.0, -120.0},
		{0.0, 0.0, -120.0},
		{0.0, 25.0, 90.0},
		{-20.0, 25.0, 90.0},
		{-20.0, 25.0, 30.0},
		{0.0, 25.0, 30.0},
		{0.0, 35.0, 90.0},
		{-20.0, 35.0, 90.0},
		{-20.0, 35.0, 30.0},
		{0.0, 35.0, 30.0},
		{0.0, 67.0, 90.0},
		{-20.0, 67.0, 90.0},
		{-20.0, 67.0, 30.0},
		{0.0, 67.0, 30.0},
		{0.0, 100.0, 90.0},
		{-20.0, 100.0, 90.0},
		{-20.0, 100.0, 30.0},
		{0.0, 100.0, 30.0},
		{0.0, 20.0, -30.0},
		{-20.0, 20.0, -30.0},
		{-20.0, 20.0, -120.0},
		{0.0, 20.0, -120.0},
		{0.0, 35.0, -30.0},
		{-20.0, 35.0, -30.0},
		{-20.0, 35.0, -120.0},
		{0.0, 35.0, -120.0},
		{0.0, 67.0, -30.0},
		{-20.0, 67.0, -30.0},
		{-20.0, 67.0, -120.0},
		{0.0, 67.0, -120.0},
		{0.0, 88.0, -30.0},
		{-20.0, 88.0, -30.0},
		{-20.0, 88.0, -120.0},
		{0.0, 88.0, -120.0},
		{0.0, 116.0, -30.0},
		{-20.0, 116.0, -30.0},
		{-20.0, 116.0, -120.0},
		{0.0, 116.0, -120.0},
		{0.0, 134.0, -30.0},
		{-20.0, 134.0, -30.0},
		{-20.0, 134.0, -120.0},
		{0.0, 134.0, -120.0},
		{0.0, 100.0, 30.0},
		{0.0, 100.0, 0.0},
		{0.0, 20.0, 0.0},
		{0.0, 100.0, -30.0},
		{0.0, 134.0, 30.0},
		{0.0, 134.0, -30.0},
		{0.0, 134.0, 90.0},
		{0.0, 134.0, 60.0},
		{0.0, 100.0, 60.0},
		{-20.0, 134.0, 90.0},
		{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL},
		{DATA_NULLVAL, 0.0, 0.0}
};
mdlframe_t g_Bar_modelFrame[] = {
		{-40.0, 0.0, 20.0},
		{-40.0, 67.0, 20.0},
		{-5.0, 67.0, 20.0},
		{-5.0, 0.0, 20.0},
		{20.0, 67.0, 0.0},
		{20.0, 0.0, 0.0},
		{20.0, 67.0, -70.0},
		{0.0, 0.0, -70.0},
		{20.0, 0.0, -70.0},
		{0.0, 67.0, -70.0},
		{0.0, 67.0, -10.0},
		{-5.0, 67.0, 0.0},
		{-40.0, 67.0, 0.0},
		{-40.0, 0.0, 0.0},
		{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL},
		{DATA_NULLVAL, 0.0, 0.0}
};
panelmdlframes_t g_panelModelFrames[MAX_PANELS];
mdlframe_t g_unknown1_modelFrame[] = {
		{40, 0, -40}, 
		{40, 80, -40}, 
		{40, 80, 40}, 
		{40, 0, 40} ,
		{-40, 0, -40}, 
		{-40, 80, -40}, 
		{-40, 80, 40}, 
		{-40, 0, 40},
		{38, 60, -40}, 
		{-38, 60, -40}, 
		{-38, 50, 40}, 
		{38, 50, 40},
		{38, 120, 60}, 
		{-38, 120, 60}, 
		{38, 114, 66}, 
		{-38, 114, 66},
		{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL}, 
		{DATA_NULLVAL, 0, 0},
};
mdlframe_t g_unknown2_modelFrame[] = {
		{0, 0, 5},
		{0, 0, 4},
		{0, 0, 0},
		{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL}
};
mdlframe_t g_unknown3_modelFrame[] = {
		{-15, 0, -15},
		{-15, 60, -15},
		{-15, 60, 15},
		{-15, 0, 15},
		{15, 0, -15},
		{15, 60, -15},
		{15, 60, 15},
		{15, 0, 15},
		{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL},
		{DATA_NULLVAL, 0, 0}
};
mdlframe_t g_unknown4_modelFrame[] = {
		{-10, 0, -10},
		{-10, 40, -10},
		{-10, 40, 10},
		{-10, 0, 10},
		{10, 0, -10},
		{10, 40, -10},
		{10, 40, 10},
		{10, 0, 10},
		{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL},
		{DATA_NULLVAL, 0, 0}
};
/////////////////////////////////////////////////////

/////////////////////MODEL INFO/////////////////////
mdlinfo_t g_GraceHeadModel_modelInfo[] = {
	{} //undone
};
mdlinfo_t g_TripHeadModel_modelInfo[] = {
	{} //undone
};
mdlinfo_t g_Couch_modelInfo[] = {
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL},
};
mdlinfo_t g_SideTable_modelInfo[] = {
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed016, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_greyshade_06, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed016, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed016, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, 0.0},
};
mdlinfo_t g_WorkTable_modelInfo[] = {
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed034, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_black, -1.0, -1.0, 0.0, 4.0, 1.0},
	{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, 0.0},
};
mdlinfo_t g_Cabinet_modelInfo[] = {
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed016, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed016, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, 0.0},
};
mdlinfo_t g_Bar_modelInfo[] = {
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed001, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed001, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed002, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed003, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed003, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed003, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed003, -1.0, -1.0, 0.0, 4.0, 1.0},
	{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, 0.0},
};
panelmdlinfos_t g_panelModelInfo[MAX_PANELS];
mdlinfo_t g_unknown1_modelInfo[] = {
	{6.0, eColor_white, 0.0, 20.0, 0.0, 4.0, 0.0},
	{6.0, eColor_white, 0.0, 20.0, 0.0, 4.0, 0.0},
	{6.0, eColor_white, 0.0, 20.0, 0.0, 4.0, 0.0},
	{6.0, eColor_white, 0.0, 20.0, 0.0, 4.0, 0.0},
	{6.0, eColor_white, 0.0, 20.0, 0.0, 4.0, 0.0},
	{6.0, eColor_white, 0.0, 20.0, 0.0, 4.0, 0.0},
	{6.0, eColor_white, 0.0, 20.0, 0.0, 4.0, 0.0},
	{6.0, eColor_white, 0.0, 20.0, 0.0, 4.0, 0.0},
	{2.0, eColor_unnamed008, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed008, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed009, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed009, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed008, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed008, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed008, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed008, -1.0, -1.0, 0.0, 4.0, 1.0},
	{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL},
};
mdlinfo_t g_unknown2_modelInfo[] = {
	{0.5, eColor_unnamed008, 0.0, 20.0, 0.0, 4.0, 0.0},
	{0.5, eColor_white, -1.0, -1.0, 0.0, 4.0, 1.0},
	{0.5, eColor_unnamed011, 0.0, 20.0, 0.0, 4.0, 0.0},
	{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL},
};
mdlinfo_t g_unknown3_modelInfo[] = {
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, 0.0},
};
mdlinfo_t g_unknown4_modelInfo[] = {
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{2.0, eColor_unnamed000, -1.0, -1.0, 0.0, 4.0, 1.0},
	{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, 0.0},
};
/////////////////////////////////////////////////////

/////////////////////MODEL QUADPOLY/////////////////////
mdlquadpoly_t g_GraceHeadModel_modelQuadPolys[] = {
	{} //undone
};
mdlquadpoly_t g_TripHeadModel_modelQuadPolys[] = {
	{} //undone
};
mdlquadpoly_t g_Couch_modelQuadPolys[] = {
	{
		{ 0,  1,  2,  3},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_none, 	eColor_black, 	eColor_black,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{ 2,  7,  8,  9},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_black, 	eColor_none, 	eColor_black, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{ 3, 13, 12,  7},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_black, 	eColor_black, 	eColor_black, 	eColor_black,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{ 6, 14, 15, 16},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_black, 	eColor_black,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{16, 15, 17, 18},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_black, 	eColor_black,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{18, 17, 19,  9},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_none, 	eColor_black,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{14, 20, 21, 15},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_none, 	eColor_black, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{15, 21, 22, 17},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_black, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{17, 22, 23, 19},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_black, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{ 4, 10, 24, 25},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_black, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{19, 26, 27,  8},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_black, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{ 8, 27, 12,  7},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_none, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{36, 28, 29, 21},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_black, 	eColor_black,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{21, 29, 30, 22},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_black, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{22, 30, 31, 23},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_black, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{37, 43, 32, 33},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_black, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{23, 31, 34, 35},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_black, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{38, 37, 36, 20},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_none, 	eColor_none, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{39, 38, 14,  6},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_none, 	eColor_black,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{40, 41,  6,  0},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_black, 	eColor_none, 	eColor_none, 	eColor_black,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{41,  4,  5, 39},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_black, 	eColor_black, 	eColor_black, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{11, 10,  4, 40},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_black, 	eColor_black, 	eColor_none, 	eColor_black,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{38, 25, 24, 42},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_none, 	eColor_none, 	eColor_black,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{37, 43, 44, 36},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_black, 	eColor_black, 	eColor_none, 	eColor_black,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{36, 44, 45, 28},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_black, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{30, 46, 34, 31},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_none, 	eColor_none, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{29, 47, 46, 30},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_none, 	eColor_none, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{28, 48, 47, 29},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_none, 	eColor_none, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{43, 49, 44, 43},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, 	eColor_black, 	eColor_none, 	eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL}, (EQuadPolyDraw)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, (ETextureFilmstrip)DATA_NULLVAL },
};
mdlquadpoly_t g_SideTable_modelQuadPolys[] = {
	//undone
	{{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL}, (EQuadPolyDraw)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, (ETextureFilmstrip)DATA_NULLVAL },
};
mdlquadpoly_t g_WorkTable_modelQuadPolys[] = {
	//undone
	{{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL}, (EQuadPolyDraw)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, (ETextureFilmstrip)DATA_NULLVAL },
};
mdlquadpoly_t g_Cabinet_modelQuadPolys[] = {
	{
		{3, 4, 5, 6},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_grey, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{0, 1, 2, 3},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_grey, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{14, 15, 16, 17},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_grey, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{18, 19, 32, 21},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_grey, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{22, 23, 24, 25},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_grey, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{6, 7, 8, 9},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_grey, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{30, 31, 32, 33},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_grey, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{34, 35, 36, 37},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_grey, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{38, 39, 40, 41},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_grey, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{42, 43, 44, 45},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_grey, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{46, 47, 48, 31},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_grey, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{50, 51, 52, 4},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_none, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_WhiteCabinetDoor2
	},
	{
		{5, 52, 51, 53},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, eColor_grey, eColor_grey, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_WhiteCabinetDoor2
	},
	{
		{50, 54, 55, 53},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_grey, eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{56, 57, 58, 22},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_none, eColor_grey,
		20, 20, 20, 20,
		eTextureFilmstrip_WhiteCabinetDoor1
	},
	{
		{50, 58, 57, 54},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_none, eColor_none, eColor_grey, eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_WhiteCabinetDoor1
	},
	{
		{23, 59, 56, 22},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_grey, eColor_none, eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{2, 24, 25, 3},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_none, eColor_none, eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{7, 47, 46, 6},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_none, eColor_none, eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{0, 22, 22, 0},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_none, eColor_none, eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{1, 23, 23, 1},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_none, eColor_none, eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{9, 49, 49, 9},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_none, eColor_none, eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{8, 48, 48, 8},
		eQuadPolyDraw_quad,
		eColor_none,
		eColor_grey, eColor_none, eColor_none, eColor_none,
		20, 20, 20, 20,
		eTextureFilmstrip_NOTATEXTURE
	},
	{ {DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL}, (EQuadPolyDraw)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, (ETextureFilmstrip)DATA_NULLVAL },
};
mdlquadpoly_t g_Bar_modelQuadPolys[] = {
	{
		{0, 1, 2, 3},
		eQuadPolyDraw_quad,
		eColor_unnamed036,
		eColor_black, eColor_none, eColor_black, eColor_black,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{3, 2, 4, 5},
		eQuadPolyDraw_quad,
		eColor_unnamed037,
		eColor_none, eColor_none, eColor_black, eColor_black,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{5, 4, 6, 8},
		eQuadPolyDraw_quad,
		eColor_unnamed038,
		eColor_none, eColor_none, eColor_black, eColor_black,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{6, 9, 7, 8},
		eQuadPolyDraw_quad,
		eColor_unnamed037,
		eColor_none, eColor_black, eColor_black, eColor_none,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{4, 10, 9, 6},
		eQuadPolyDraw_quad,
		eColor_unnamed039,
		eColor_none, eColor_black, eColor_black, eColor_black,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{2, 11, 10, 4},
		eQuadPolyDraw_quad,
		eColor_unnamed039,
		eColor_none, eColor_black, eColor_none, eColor_black,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{1, 2, 11, 12},
		eQuadPolyDraw_quad,
		eColor_unnamed039,
		eColor_black, eColor_none, eColor_black, eColor_black,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{1, 12, 13, 0},
		eQuadPolyDraw_quad,
		eColor_unnamed037,
		eColor_none, eColor_none, eColor_none, eColor_none,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL}, (EQuadPolyDraw)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, (ETextureFilmstrip)DATA_NULLVAL },
};
panelmdlquadpolys_t g_panelModelQuadPolys[MAX_PANELS];
mdlquadpoly_t g_unknown1_modelQuadPolys[] = {
	{
		{8, 9, 10, 11},
		eQuadPolyDraw_flatdepth,
		eColor_black,
		eColor_black, eColor_black, eColor_black, eColor_unnamed010,
		12, 12, 12, -1,
		eTextureFilmstrip_LiquorScotch
	},
	{
		{10, 11, 12, 1},
		eQuadPolyDraw_never,
		eColor_black,
		eColor_black, eColor_black, eColor_unnamed010, eColor_unnamed010,
		12, 12, 12, 12,
		eTextureFilmstrip_Water
	},
	{
		{15, 14, 1, 0},
		eQuadPolyDraw_never,
		eColor_black,
		eColor_black, eColor_unnamed010, eColor_unnamed010, eColor_unnamed010,
		12, -1, DATA_NULLVAL, DATA_NULLVAL,
		(ETextureFilmstrip)DATA_NULLVAL
	},
	{{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL}, (EQuadPolyDraw)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, DATA_NULLVAL, 0, 0, 0, (ETextureFilmstrip)0 },
};
mdlquadpoly_t g_unknown3_modelQuadPolys[] = {
	{
		{0, 1, 2, 3},
		eQuadPolyDraw_quad,
		eColor_unnamed041,
		eColor_black, eColor_black, eColor_black, eColor_black,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{0, 1, 5, 4},
		eQuadPolyDraw_quad,
		eColor_unnamed040,
		eColor_none, eColor_black, eColor_black, eColor_black,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{3, 2, 6, 7},
		eQuadPolyDraw_quad,
		eColor_unnamed042,
		eColor_none, eColor_black, eColor_black, eColor_black,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{1, 5, 6, 2},
		eQuadPolyDraw_quad,
		eColor_unnamed043,
		eColor_none, eColor_black, eColor_none, eColor_none,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL}, (EQuadPolyDraw)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, (ETextureFilmstrip)DATA_NULLVAL },
};
mdlquadpoly_t g_unknown4_modelQuadPolys[] = {
	{
		{0, 1, 2, 3},
		eQuadPolyDraw_quad,
		eColor_greyshade_20, 
		eColor_black, eColor_black, eColor_black, eColor_black,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{0, 1, 5, 4},
		eQuadPolyDraw_quad,
		eColor_greyshade_15, 
		eColor_none, eColor_black, eColor_black, eColor_black,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{3, 2, 6, 7},
		eQuadPolyDraw_quad,
		eColor_greyshade_25, 
		eColor_none, eColor_black, eColor_black, eColor_black,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{
		{1, 5, 6, 2},
		eQuadPolyDraw_quad,
		eColor_greyshade_30, 
		eColor_none, eColor_black, eColor_none, eColor_none,
		12, 12, 12, 12,
		eTextureFilmstrip_NOTATEXTURE
	},
	{{DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL}, (EQuadPolyDraw)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, (EColor)DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL, (ETextureFilmstrip)DATA_NULLVAL },
};
/////////////////////////////////////////////////////


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


void InitTransRotScale(CharacterSprite* spr, bool b1);

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
	if (g_lastFrameGotAblRequest > 0 && !g_bAtEndOfDrama && g_numFramesRun > g_lastFrameGotAblRequest + 600)
	{
		g_bQuittingApp = true;
		g_bAblDisconnect = true;
		MessageBox(NULL, "USER WARNING\n", "façade is closing due to a lack of response from abl.\n", MB_OK);
		SendMessage(g_hMainAppWindow, WM_CLOSE, 0, 0);
		return;
	}

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

static int CreateNewZone(Sprite* sprite, bool option) {
	animengine_assert(g_numZones < MAXZONES);
	zone_t* zone = &g_zones[g_numZones];

	zone->numrects = 0;
	zone->unknown001 = sprite;
	zone->unknown002 = option;
	if (sprite)
		sprite->a1_unknown00011c = true;

	return g_numZones++;
}

static void AddRectToZone(int zoneindex, XZRectf rect) {
	animengine_assert(zoneindex < g_numZones);

	zone_t* zone = &g_zones[zoneindex];
	animengine_assert(zone->numrects <= NUMRECTS_PERZONE - 1);

	zone->rects[zone->numrects] = rect;
	++zone->numrects;
}

static bool IsRectWithinZone(XZRectf rect, int zoneindex) {
	animengine_assert(zoneindex < g_numZones);

	zone_t* zone = &g_zones[zoneindex];

	for (int i = 0; i < zone->numrects; i++)
	{
		if (DoXZRectsIntersect(rect, zone->rects[i]))
			return true;
	}
	return false;
}

static void ModifyZoneRect(int zoneindex, int rectindex, XZRectf rect) {
	animengine_assert(zoneindex < g_numZones);

	zone_t* zone = &g_zones[zoneindex];
	animengine_assert(rectindex < zone->numrects);

	zone->rects[rectindex] = rect;
}

static bool IsPointWithinZone(XYZPointf point, int zoneindex) {
	XZRectf rect;
	rect.m_xmin = rect.m_xmax = point.m_x;
	rect.m_zmin = rect.m_zmax = point.m_z;

	return IsRectWithinZone(rect, zoneindex);
}

static int GetZoneIDForRoomZone(int roomzoneid) {
	animengine_assert(roomzoneid == 0);
	return g_roomZoneIDs[roomzoneid];
}


static XZRectf ComputeSingleCollisionRectForSprite(Sprite* spr) {
	XZRectf rect;

	XYZPointf spritepos = spr->GetPosition();
	if (spr->GetCollidableType() == eCollidableType_AABB)
	{
		rect.m_xmin = spritepos.m_x - 10;
		rect.m_zmin = spritepos.m_z - 10;

		rect.m_xmax = spritepos.m_x + 10;
		rect.m_zmax = spritepos.m_z + 10;
	}
	else
	{
		for (int i = 0; i < spr->m_iNumModelPoints; ++i)
		{
			XYZPointf mdlpointworldpos;
			if (g_numFramesRun > 1)
			{
				mdlpointworldpos = spr->b1_unknown00020[i] + spritepos;
			}
			else
			{
				mdlpointworldpos.m_x = spritepos.m_x + spr->m_modelframe[i].m_x;
				mdlpointworldpos.m_z = spritepos.m_z + spr->m_modelframe[i].m_z;
			}
			if (i == 0)
			{
				rect.m_xmin = mdlpointworldpos.m_x;
				rect.m_zmin = mdlpointworldpos.m_z;

				rect.m_xmax = mdlpointworldpos.m_x;
				rect.m_zmax = mdlpointworldpos.m_z;
			}
			else
			{
				if (rect.m_xmin >= mdlpointworldpos.m_x)	rect.m_xmin = mdlpointworldpos.m_x;
				if (rect.m_zmin >= mdlpointworldpos.m_z)	rect.m_zmin = mdlpointworldpos.m_z;

				if (rect.m_xmax <= mdlpointworldpos.m_x)	rect.m_xmax = mdlpointworldpos.m_x;
				if (rect.m_zmax <= mdlpointworldpos.m_z)	rect.m_zmax = mdlpointworldpos.m_z;
			}
		}

		if (spr->GetCollidableType() == eCollidableType_unknown)
		{
			float rectsizex;
			if (rect.m_xmin - rect.m_xmax < 0.0)
				rectsizex = -(rect.m_xmin - rect.m_xmax);
			else
				rectsizex = rect.m_xmin - rect.m_xmax;

			if (rectsizex > 0.001)
			{
				float rectsizez;
				if (rect.m_zmin - rect.m_zmax < 0.0)
					rectsizez = -(rect.m_zmin - rect.m_zmax);
				else
					rectsizez = rect.m_zmin - rect.m_zmax;

				if (rectsizez > 0.001)
				{
					//v17 = 0; //unused variable..
				}
				else
				{
					rect.m_zmin = rect.m_zmin - 10.0;
					rect.m_zmax = rect.m_zmax + 10.0;
				}
			}
			else
			{
				rect.m_xmin = rect.m_xmin - 10.0;
				rect.m_xmax = rect.m_xmax + 10.0;
			}
		}
	}

	return rect;
}

bool PlayGlobalSound(EGlobalSounds sound)
{
	return g_soundMgr.PlaySound(g_soundSourceID, sound, true);
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

static void FinishRoomInit()
{
	Sprite* worktable = g_spriteMgr.GetSpritePtrByName("work table");
	Sprite* ashtray = g_spriteMgr.GetSpritePtrByName("ashtray");

	//called but return value not used ?
	g_spriteMgr.GetSpritePtrByName("phone");
	g_spriteMgr.GetSpritePtrByName("answering machine");

	//worktable->AddSpriteToSurface(0, ashtray);

	Sprite* bar = g_spriteMgr.GetSpritePtrByName("bar");
	//bar->AddSpriteToSurface(2, g_spriteMgr.GetSpritePtrByName("white wine bottle"));
	//bar->AddSpriteToSurface(2, g_spriteMgr.GetSpritePtrByName("red wine bottle"));
	//bar->AddSpriteToSurface(0, g_spriteMgr.GetSpritePtrByName("advice ball"));

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
	if (g_introEventCtr)
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
		BEGIN_INTROEVENT(2, g_initialIntroDelay)
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
			//undone

			DrawArbitraryScreenText("Façade",	 1, -60, 200, 20, -10, eColor_white, eColor_black);
			DrawArbitraryScreenText("by",		23, -72,  50,  8, -10, eColor_white, eColor_black);
			DrawArbitraryScreenText("Michael",  -9, -80,  50, 10, -10, eColor_white, eColor_black);
			DrawArbitraryScreenText("Mateas",   -8, -88,  50, 10, -10, eColor_white, eColor_black);
			DrawArbitraryScreenText("and",		21, -83,  50,  8, -10, eColor_white, eColor_black);
			DrawArbitraryScreenText("Andrew",	35, -80,  50, 10, -10, eColor_white, eColor_black);
			DrawArbitraryScreenText("Stern",	37, -88,  50, 10, -10, eColor_white, eColor_black);
		}

	}
	else
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
}

static void UpdateCursor()
{
	static int g_lastObjCursorNotice = -99999;

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

	g_CursorSprite->SetPosition(XYZPointf(cursorxpos, cursorypos, -30));


	//undone
}
static void UpdateEnvironment()
{
	for (int i = 0; i < g_numZones; ++i)
	{
		Sprite* spr = g_zones[i].unknown001;
		if (!spr) continue;

		if (spr->GetCollidableType() == eCollidableType_AABB)
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
		//temporary :(
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
			dprintf("quitAppCountdown quitting app!");
			g_bQuittingApp = 1;
			SendMessageA(g_hMainAppWindow, WM_CLOSE, 0, 0);
		}
	}
}

static bool CollisionTest(int zoneindex, XZRectf rect, Sprite* spr1, Sprite* spr2)
{
	if (g_zones[zoneindex].unknown002 && (!spr1 || spr1 != spr2) && (!spr2 || spr2->a1_unknown00011c))
	{
		if (spr1 != g_PlayerSprite
			|| !spr2
			|| !spr2->GetIsCharacterSprite()
			|| IsPointVisibleToPlayer(spr2->GetPosition(), true))
		{
			if (IsRectWithinZone(rect, zoneindex))
				return true;
		}
	}
	return false;
}

bool CollisionTestWithWorld(Sprite* spr, XYZPointf pos, int expandsize, bool option) {

	XZRectf rect;
	rect.m_xmin = pos.m_x - expandsize;
	rect.m_zmin = pos.m_z - expandsize;

	rect.m_xmax = pos.m_x + expandsize;
	rect.m_zmax = pos.m_z + expandsize;

	float gridx, gridz;

	GetGridXZFromWorldXZ(rect.m_xmin, rect.m_zmin, gridx, gridz);
	if (gridx <= 2 || gridz <= 2 || gridx >= 87 || gridz >= 87)
		return true;
	GetGridXZFromWorldXZ(rect.m_xmax, rect.m_zmax, gridx, gridz);
	if (gridx <= 2 || gridz <= 2 || gridx >= 87 || gridz >= 87)
		return true;

	for (int i = 0; i < g_numZones; ++i)
	{
		Sprite* zonespr = g_zones[i].unknown001;
		if ( (option
			|| !zonespr
			|| !zonespr->GetIsCharacterSprite())
			&& CollisionTest(i, rect, spr, zonespr))
		{
			return true;
		}
	}
	return false;
}
static void ClearNavigationFlags() {
	g_bPlayerRotateLeft = g_bPlayerRotateRight =
		g_bPlayerMoveForwards = g_bPlayerMoveBackwards =
		g_bPlayerMoveUpwards = g_bPlayerMoveDownwards =
		false;
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
	if (!CollisionTestWithWorld(g_PlayerSprite, nextpos, 10, true))
		return false;

	XYZPointf playerpos = g_PlayerSprite->GetPosition();
	XYZPointf pointtest = XYZPointf(
		(playerpos.m_x + nextpos.m_x) / 2,
		playerpos.m_y,
		(playerpos.m_z + nextpos.m_x) / 2
	);
	if (!CollisionTestWithWorld(g_PlayerSprite, pointtest, 10, true))
	{
		g_PlayerSprite->SetPosition(nextpos);
		return true;
	}

	return false;
}

bool IsPlayerOutsideApt() {
	return g_PlayerSprite->GetPosition().m_x < -150
		&& g_PlayerSprite->GetPosition().m_y > 120;
}

static bool CollisionTestWithSprite(Sprite* collider1, XYZPointf point, int expandsize, Sprite* collider2) {
	animengine_assert(collider1 && collider2);

	for (int i = 0; i < g_numZones; i++)
	{
		Sprite* spr = g_zones[i].unknown001;
		if (spr != collider2)
			continue;

		XZRectf rect;
		rect.m_xmin = point.m_x - expandsize;
		rect.m_zmin = point.m_z - expandsize;

		rect.m_xmax = point.m_x + expandsize;
		rect.m_zmax = point.m_z + expandsize;
		if(CollisionTest(i, rect, collider1, collider2))
			return true;
	}

	return false;
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

		//if (bAlreadyCollided)
		//temporary :(
		if(1)
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
					bool inarea = playerpos.m_x > -213.0 && playerpos.m_z < 109.0 && playerpos.m_x < -166.0 && playerpos.m_z > 13.0
						|| playerpos.m_x > -161.0 && playerpos.m_z < 148.0 && playerpos.m_x < -83.0 && playerpos.m_z > 74.0;
					
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

#define START_SPRITECREATE(name, mdlframe, mdlinfo, mdlquadpoly, mdlquadlines) \
	static void Create##name##Sprite(ESpriteID spriteid, const char* spritename){ \
		Sprite* sprite = new Sprite(); \
		sprite->m_modelframe = mdlframe; \
		sprite->m_modelinfo = mdlinfo; \
		sprite->m_modelquadpoly = mdlquadpoly; \
		sprite->m_modelquadlines = mdlquadlines; \
		sprite->InitSprite(spriteid, spritename, true, true); \
		sprite->SetVolume(XYZPointf(1, 1, 1)); 

#define END_SPRITECREATE() \
		sprite->SetCollidableType(eCollidableType_Complex); \
	}

START_SPRITECREATE(Couch, 
	g_Couch_modelFrame, 
	g_Couch_modelInfo, 
	g_Couch_modelQuadPolys, 
	nullptr/*g_Couch_modelQuadLines*/)

	sprite->AddSurface(14, 20, 23, 19, eSurfacePickupAngle_unknown002);
	sprite->AddSurface(4, 10, 25, 5, eSurfacePickupAngle_unknown002);
	sprite->AddSurface(8, 27, 12, 7, eSurfacePickupAngle_unknown002);

END_SPRITECREATE()
START_SPRITECREATE(Cabinet,
	g_Cabinet_modelFrame,
	g_Cabinet_modelInfo,
	g_Cabinet_modelQuadPolys,
	nullptr)

	sprite->AddSurface(14, 15, 16, 17, eSurfacePickupAngle_unknown004);
	sprite->AddSurface(18, 19, 20, 21, eSurfacePickupAngle_unknown004);
	sprite->AddSurface(6, 7, 8, 9, eSurfacePickupAngle_unknown004);
	sprite->AddSurface(30, 31, 32, 33, eSurfacePickupAngle_unknown004);
	sprite->AddSurface(34, 35, 36, 37, eSurfacePickupAngle_unknown004);
	sprite->AddSurface(38, 39, 40, 41, eSurfacePickupAngle_unknown004);
	sprite->AddSurface(42, 43, 44, 45, eSurfacePickupAngle_unknown004);

END_SPRITECREATE()
START_SPRITECREATE(SideTable,
	g_SideTable_modelFrame,
	g_SideTable_modelInfo,
	g_SideTable_modelQuadPolys,
	nullptr)

	sprite->AddSurface(33, 36, 38, 34, eSurfacePickupAngle_unknown002);

END_SPRITECREATE()
START_SPRITECREATE(WorkTable,
	g_WorkTable_modelFrame,
	g_WorkTable_modelInfo,
	g_WorkTable_modelQuadPolys,
	nullptr)

	sprite->AddSurface(33, 36, 38, 34, eSurfacePickupAngle_unknown002);

END_SPRITECREATE()
START_SPRITECREATE(Bar,
	g_Bar_modelFrame,
	g_Bar_modelInfo,
	g_Bar_modelQuadPolys,
	nullptr)

	sprite->AddSurface(4, 10, 9, 6, eSurfacePickupAngle_unknown004);
	sprite->AddSurface(2, 4, 10, 11, eSurfacePickupAngle_unknown004);
	sprite->AddSurface(1, 2, 11, 12, eSurfacePickupAngle_unknown001);

END_SPRITECREATE()
START_SPRITECREATE(LuxTable,
	g_SideTable_modelFrame,
	g_SideTable_modelInfo,
	g_SideTable_modelQuadPolys,
	nullptr)

	sprite->AddSurface(33, 36, 38, 34, eSurfacePickupAngle_unknown002);

END_SPRITECREATE()

static int GetFullDrinkLevel(bool isplayer)
{
	if (isplayer)
		return 8;
	return rand2(-2, 2) + 10;
}


static Sprite* CreatePanel(ESpriteID sprid, const char* sprname, bool drawable, EObjectType objtype,
	float x, float y, float rotx, float roty, float rotz,
	bool b1, bool norotation,
	EColor color1,
	ETextureFilmstrip texturefilmstrip,
	EColor color2,
	XYZPointf pos,
	bool collide, bool b3)
{
	static int s_panelID = 54;
	if (g_numPanels >= MAX_PANELS)
	{
		ShouldntBeHere("need to allocate more memory for panels");
		return nullptr;
	}

	bool unknownbool001 = false;

	if (sprid == eSpriteID_NOTASPRITE)
	{
		sprid = (ESpriteID)++s_panelID;
		unknownbool001 = x < 20;
	}

	XYZPointf mdlpoints[5];
	mdlpoints[0] = XYZPointf(0, 0, 0);
	mdlpoints[1] = XYZPointf(-x / 2, -y / 2, 0);
	mdlpoints[2] = XYZPointf(mdlpoints[1].m_x, y / 2, 0);
	mdlpoints[3] = XYZPointf(x / 2, mdlpoints[2].m_y, 0);
	mdlpoints[4] = XYZPointf(mdlpoints[3].m_x, mdlpoints[1].m_y, 0);

	if (!norotation)
		RotateArrayOfPoints(mdlpoints, 5, XYZPointf(rotx, roty, rotz), XYZPointf(0, 0, 0));


	panelmdlframes_t* panelmdlframe = &g_panelModelFrames[g_numPanels];
	panelmdlinfos_t* panelmdlinfos = &g_panelModelInfo[g_numPanels];
	panelmdlquadpolys_t* panelmdlquadpoly = &g_panelModelQuadPolys[g_numPanels];

	for (int i = 0; i < 5; ++i)
		panelmdlframe->frames[i] = mdlpoints[i];
	panelmdlframe->frames[5] = XYZPointf(DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL);

	if (texturefilmstrip == eTextureFilmstrip_NOTATEXTURE)
	{
		animengine_assert(color1 <= eColor_greyshade_45);
	}
	else if (color1 < eColor_black)
	{
		animengine_assert(texturefilmstrip <= eTextureFilmstrip_TipJar);
		color1 = color1 == eColor_unknown ? eColor_white : eColor_black;
	}
	for (int i = 0; i < 5; ++i)
	{
		panelmdlinfos->infos[i].unknown001 = 1.0;
		panelmdlinfos->infos[i].color = color1;
		panelmdlinfos->infos[i].unknown003 = -1.0;
		panelmdlinfos->infos[i].unknown004 = -1.0;
		panelmdlinfos->infos[i].unknown005 = -1.0;
		panelmdlinfos->infos[i].unknown006 = 1.0;
		panelmdlinfos->infos[i].unknown007 = 1.0;
	}
	panelmdlinfos->infos[5].unknown001 = DATA_NULLVAL;
	panelmdlinfos->infos[5].color = DATA_NULLVAL;
	panelmdlinfos->infos[5].unknown003 = DATA_NULLVAL;
	panelmdlinfos->infos[5].unknown004 = DATA_NULLVAL;
	panelmdlinfos->infos[5].unknown005 = DATA_NULLVAL;
	panelmdlinfos->infos[5].unknown006 = DATA_NULLVAL;
	panelmdlinfos->infos[5].unknown007 = DATA_NULLVAL;

	panelmdlquadpoly->polys[0].mdlinfoindex[0] = 4;
	panelmdlquadpoly->polys[0].mdlinfoindex[1] = 1;
	panelmdlquadpoly->polys[0].mdlinfoindex[2] = 2;
	panelmdlquadpoly->polys[0].mdlinfoindex[3] = 3;
	panelmdlquadpoly->polys[0].drawtype = eQuadPolyDraw_quad;
	panelmdlquadpoly->polys[0].facecolor = color1;
	panelmdlquadpoly->polys[0].linecolor1 = color2;
	panelmdlquadpoly->polys[0].linecolor2 = color2;
	panelmdlquadpoly->polys[0].linecolor3 = color2;
	panelmdlquadpoly->polys[0].linecolor4 = color2;
	panelmdlquadpoly->polys[0].linewidth1 = 12;
	panelmdlquadpoly->polys[0].linewidth2 = 12;
	panelmdlquadpoly->polys[0].linewidth3 = 12;
	panelmdlquadpoly->polys[0].linewidth4 = 12;
	panelmdlquadpoly->polys[0].m_eFilmstrip = texturefilmstrip;

	panelmdlquadpoly->polys[1].mdlinfoindex[0] = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].mdlinfoindex[1] = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].mdlinfoindex[2] = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].mdlinfoindex[3] = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].drawtype = (EQuadPolyDraw)DATA_NULLVAL;
	panelmdlquadpoly->polys[1].facecolor =  (EColor)DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linecolor1 = (EColor)DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linecolor2 = (EColor)DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linecolor3 = (EColor)DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linecolor4 = (EColor)DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linewidth1 = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linewidth2 = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linewidth3 = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linewidth4 = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].m_eFilmstrip = (ETextureFilmstrip)DATA_NULLVAL;

	Sprite* spr;
	switch (objtype)
	{
	case eObjectType_Door:				spr = new DoorSprite();				break;
	case eObjectType_Drink:				spr = new DrinkSprite();			break;
	case eObjectType_EightballMessage:	spr = new EightballMessageSprite();	break;
	case eObjectType_Phone:				spr = new PhoneSprite();			break;
	case eObjectType_AnsweringMachine:	spr = new AnsweringMachineSprite();	break;
	default:							spr = new Sprite();					break;
	}

	spr->m_eObjectType = objtype;
	spr->m_modelframe = panelmdlframe->frames;
	spr->m_modelinfo = panelmdlinfos->infos;
	spr->m_modelquadpoly = panelmdlquadpoly->polys;
	spr->a1_unknown00008 = true;
	spr->a1_unknown00010 = b1 && !norotation;
	spr->m_bSpriteFollowsCamera = norotation;
	spr->m_iPanelIndex = g_numPanels;
	spr->a1_unknown00010c = unknownbool001;

	spr->InitSprite(sprid, sprname, true, drawable);
	spr->SetVolume(XYZPointf(1, 1, 1));
	spr->SetPosition(pos);
	if (collide)
		spr->SetCollidableType(eCollidableType_unknown);
	if (b3)
		spr->a1_unknown00011a = true;

	g_numPanels++;

	return spr;
}

void StopAllMP3s()
{
	//undone
}

void PlayerEnteredText()
{
	static int g_waitingToRedisplayCtr;
	strcpy(g_PlayerSprite->m_playerTextInput, g_PlayerSprite->m_ScreenText);
	g_bNewPlayerTextEntered = true;
	g_whenNewPlayerTextEntered = g_numFramesRun;
	g_PlayerSprite->SetScreenText(g_PlayerSprite->m_ScreenText, true);
	g_textDisplayState = 1;
	g_waitingToRedisplayCtr = 0;
	AddEventInfoToLog(eEventLogType_Speech, eSpriteID_player, g_PlayerSprite->m_ScreenText, 0, eSpriteID_NOTASPRITE, 0);
}

void CreateRoom()
{
	Sprite* spr;
	CreateCouchSprite(eSpriteID_couch, "couch");
	CreateCabinetSprite(eSpriteID_cabinet, "cabinet");
	CreateSideTableSprite(eSpriteID_sideTable, "side table");
	CreateWorkTableSprite(eSpriteID_workTable, "work table");
	CreateBarSprite(eSpriteID_bar, "bar");
	CreateLuxTableSprite(eSpriteID_luxTable, "side table 2");

	CreatePanel(eSpriteID_plant,	"plant",											true,	eObjectType_Misc,		120,	120,	0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Plant,			eColor_none,	XYZPointf(160, 60, 65),						false,	false);
	spr = CreatePanel(eSpriteID_ashtray,	"ashtray",										false,	eObjectType_Misc,		10,		5,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Ashtray,			eColor_none,	XYZPointf(165, 60, 10),						false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 5, 10));
	spr = CreatePanel(eSpriteID_phone,			"phone",								true,	eObjectType_Phone,		16,		16,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Phone,			eColor_none,	XYZPointf(145, 60, -60),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 5, 10));
	spr = CreatePanel(eSpriteID_answeringMachine,	"answering machine",					true,	eObjectType_AnsweringMachine,	16,	16,	0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_AnsweringMachine,	eColor_none,	XYZPointf(170, 60, -70),					false,	false);
		spr->SetClickable(true);
		spr->SetVolume(XYZPointf(10, 5, 10));
	spr = CreatePanel(eSpriteID_FIRSTDRINKITEM,	"red wine bottle",						true,	eObjectType_Misc,		25,		25,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_RedWineBottle,	eColor_none,	XYZPointf(-185, 80, -108),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 25, 10));
	spr = CreatePanel(eSpriteID_whitewinebottle,	"white wine bottle",					true,	eObjectType_Misc,		25,		25,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_WhiteWineBottle,	eColor_none,	XYZPointf(-193, 80, -112),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 25, 10));
	spr = CreatePanel(eSpriteID_martiniDisplay1,	"martini glass 1",						true,	eObjectType_Misc,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniEmpty,		eColor_none,	XYZPointf(-184, 41, -274),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = 0;
	spr = CreatePanel(eSpriteID_martiniDisplay2,	"martini glass 2",						true,	eObjectType_Misc,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniEmpty,		eColor_none,	XYZPointf(-183, 41, -286),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = 0;
	spr = CreatePanel(eSpriteID_martiniDisplay3,	"martini glass 3",						true,	eObjectType_Misc,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniEmpty,		eColor_none,	XYZPointf(-183.5, 41, -297),				false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = 0;
	spr = CreatePanel(eSpriteID_martiniDisplay4,	"martini glass 4",						true,	eObjectType_Misc,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniEmpty,		eColor_none,	XYZPointf(-183, 41, -308),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = 0;
	spr = CreatePanel(eSpriteID_bar_redwinebottle,	"bar_redwinebottle",				false,	eObjectType_Misc,		25,		25,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_RedWineBottle,	eColor_none,	XYZPointf(-100, -100, -100),				false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 25, 10));
	spr = CreatePanel(eSpriteID_bar_whitewinebottle,	"bar_whitewinebottle",				false,	eObjectType_Misc,		25,		25,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_WhiteWineBottle,	eColor_none,	XYZPointf(-100, -100, -100),				false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 25, 10));
	spr = CreatePanel(eSpriteID_playerDrink,		"player's drink",						false,	eObjectType_Drink,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniCosmo,		eColor_none,	XYZPointf(-170, 73, -110),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = GetFullDrinkLevel(true);
	spr = CreatePanel(eSpriteID_tripDrink,		"Trip's drink",							false,	eObjectType_Drink,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniCosmo,		eColor_none,	XYZPointf(-100, -100, -100),				false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = GetFullDrinkLevel(false);
	spr = CreatePanel(eSpriteID_graceDrink,		"Grace's drink",						false,	eObjectType_Drink,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniClassic,	eColor_none,	XYZPointf(-100, -100, -100),				false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = GetFullDrinkLevel(false);
	spr = CreatePanel(eSpriteID_bigAbstractPainting,	"big abstract painting",			true,	eObjectType_Misc,		52,		70,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_Painting,			eColor_black,	XYZPointf(190, 100, -380),					false,	false);
		spr->SetVolume(XYZPointf(50, 50, 70));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_littleTreePainting,	"little painting",					true,	eObjectType_Misc,		20,		32,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_LittlePainting,	eColor_black,	XYZPointf(190, 118, -335),					false,	false);
		spr->SetVolume(XYZPointf(20, 20, 30));
		spr->SetClickable(true);
	CreatePanel(eSpriteID_maoPainting,			"maoPainting",							false,	eObjectType_Misc,		40,		50,		0,		270,	0,		true,	false,	eColor_white,			eTextureFilmstrip_LittlePainting,	eColor_black,	XYZPointf(215, 100, 400),					false,	false);
	CreatePanel(eSpriteID_rug,					"rug",									true,	eObjectType_Misc,		50,		100,	90,		0,		0,		false,	false,	eColor_white,			eTextureFilmstrip_Rug,	eColor_black,	XYZPointf(85, 3, -350),									false,	false);
	spr = CreatePanel(eSpriteID_weddingPicture,	"wedding photo",						true,	eObjectType_Misc,		60,		70,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WeddingPicture,	eColor_black,	XYZPointf(190, 90, -145),					false,	false);
		spr->SetVolume(XYZPointf(60, 60, 90));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_italyPicture,		"italy picture",						true,	eObjectType_Misc,		32,		32,		0,		270,	0,		true,	false,	eColor_white,			eTextureFilmstrip_Tuscany_Romance,	eColor_none,	XYZPointf(-199, 90, -60),					false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_gownsketch1,		"wedding gown sketch",					true,	eObjectType_Misc,		32,		32,		0,		90,		0,		true,	false,	eColor_unknown,			eTextureFilmstrip_GownSketch1,		eColor_none,	XYZPointf(190, 90, 0),						false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_gownsketch2,		"wedding gown post its",				true,	eObjectType_Misc,		36,		36,		0,		90,		0,		true,	false,	eColor_unknown,			eTextureFilmstrip_PostIts,			eColor_none,	XYZPointf(190, 50, -28),					false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_gownsketch3,		"wedding gown sketch 3",				false,	eObjectType_Misc,		25,		25,		90,		0,		0,		true,	false,	eColor_unknown,			eTextureFilmstrip_GownSketch1,		eColor_none,	XYZPointf(160, 58, -60),					false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_gownsketch4,		"wedding gown sketch 4",				true,	eObjectType_Misc,		30,		30,		0,		90,		0,		true,	false,	eColor_unknown,			eTextureFilmstrip_GownSketch4,		eColor_none,	XYZPointf(190, 83, -35),					false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_tuxedosketch1,	"tuxedosketch1",						false,	eObjectType_Misc,		32,		32,		0,		90,		0,		true,	false,	eColor_unknown,			eTextureFilmstrip_GownSketch1,		eColor_none,	XYZPointf(190, 95, 25),						false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_tuxedosketch2,	"tuxedosketch2",						false,	eObjectType_Misc,		25,		25,		90,		0,		0,		true,	false,	eColor_unknown,			eTextureFilmstrip_GownSketch1,		eColor_none,	XYZPointf(160, 58, -15),					false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_eightball,		"advice ball",							true,	eObjectType_Misc,		8,		8,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_EightBall,		eColor_none,	XYZPointf(-149, 71, -170),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(8, 8, 8));
		spr->a1_unknown00006 = 1;
		spr->a1_unknown00007 = 6;
	spr = CreatePanel(	(ESpriteID)((int)eSpriteID_LAST_OBJECT|(int)eSpriteID_cabinet),	"advice ball message",	false,	eObjectType_EightballMessage,	5,	5,	0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_EightBallAnims,	eColor_none,	XYZPointf(0, 0, 0),							false,	false);
		spr->SetVolume(XYZPointf(4, 4, 1));
	spr = CreatePanel(eSpriteID_brassbull,		"brass bull",							true,	eObjectType_Misc,		16,		16,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Brassbull,		eColor_none,	XYZPointf(-185, 75, -300),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(16, 16, 10));
	spr = CreatePanel(eSpriteID_trinket1,			"trinket 1",							true,	eObjectType_Misc,		28,		28,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket1,			eColor_none,	XYZPointf(-185, 48, -450),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 28, 10));
	spr = CreatePanel(eSpriteID_trinket2,			"trinket 2",							true,	eObjectType_Misc,		28,		28,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket2,			eColor_none,	XYZPointf(-186, 48, -425),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 28, 10));
	spr = CreatePanel(eSpriteID_trinket3,			"trinket 3",							true,	eObjectType_Misc,		20,		20,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket3,			eColor_none,	XYZPointf(-184.5, 46, -400),				false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 20, 10));
	spr = CreatePanel(eSpriteID_trinket4,			"trinket 4",							true,	eObjectType_Misc,		16,		16,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket4,			eColor_none,	XYZPointf(-185, 75, -443),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 16, 10));
	spr = CreatePanel(eSpriteID_trinket5,			"trinket 5",							true,	eObjectType_Misc,		16,		16,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket5,			eColor_none,	XYZPointf(-186, 75, -405),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 16, 10));
	spr = CreatePanel(eSpriteID_trinket6,			"trinket 6",							true,	eObjectType_Misc,		20,		20,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket6,			eColor_none,	XYZPointf(-186, 98, -399),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 20, 10));
	spr = CreatePanel(eSpriteID_trinket7,			"trinket 7",							true,	eObjectType_Misc,		16,		16,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket7,			eColor_none,	XYZPointf(-186, 96, -424),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 16, 10));
	spr = CreatePanel(eSpriteID_trinket8,			"trinket 8",							true,	eObjectType_Misc,		16,		16,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket8,			eColor_none,	XYZPointf(-186, 96, -450),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 16, 10));
		
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwall1",							true,	eObjectType_Misc,		70,		160,	0,		0,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-165, 80, -500),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwallsliver1",					true,	eObjectType_Misc,		6.5,	160,	0,		0,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-66, 80, -500),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwallsliver2",					true,	eObjectType_Misc,		6.5,	160,	0,		0,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(6, 80, -500),					true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwall3",							true,	eObjectType_Misc,		30,		160,	0,		0,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(85, 80, -500),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwall4",							true,	eObjectType_Misc,		100,	160,	0,		0,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(150, 80, -500),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwalltoppanel",					true,	eObjectType_Misc,		200,	10,		0,		0,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-30, 155, -500),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_leftwall0",							true,	eObjectType_Misc,		70,		160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-200, 80, -5),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_leftwall1",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-200, 80, -90),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_leftwall2",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-200, 80, -190),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_leftwall3",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-200, 80, -290),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_leftwall4",							true,	eObjectType_Misc,		60,		160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-200, 80, -370),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_leftwall5",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-200, 80, -450),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_rightwall0",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(200, 80, 50),					true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_rightwall1",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(200, 80, -50),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_rightwall2",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(200, 80, -150),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_rightwall3",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(200, 80, -250),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_rightwall4",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(200, 80, -350),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_rightwall5",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(200, 80, -450),				true,	false);
	
	spr = CreatePanel(eSpriteID_NOTASPRITE,		"LR_backwindow1",						true,	eObjectType_Misc,		66,		150,	0,		0,		0,		true,	false,	eColor_unnamed030,		eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-101, 77, -498),				true,	false);
		spr->m_bTransparent = true;
	spr = CreatePanel(	eSpriteID_viewWindow,			"LR_backwindow2",						true,	eObjectType_Misc,		66,		150,	0,		0,		0,		true,	false,	eColor_unnamed030,		eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-30, 77, -498),				true,	false);
		spr->m_bTransparent = true;
	spr = CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwindow3",						true,	eObjectType_Misc,		66,		150,	0,		0,		0,		true,	false,	eColor_unnamed030,		eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(41, 77, -498),				true,	false);
		spr->m_bTransparent = true;
		
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_frontwallL",						true,	eObjectType_Misc,		60,		160,	0,		0,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(170, 80, 98),					true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_frontwallR1",						true,	eObjectType_Misc,		66,		160,	0,		0,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(17, 80, 98),					false,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_frontwallR1_real",					true,	eObjectType_Misc,		50,		160,	0,		0,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(2, 80, 108),					true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_frontwallR2",						true,	eObjectType_Misc,		80,		160,	0,		0,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-46, 80, 98),					true,	false);
	
	int zoneid = CreateNewZone(0, 0);
	XZRectf rect;
	rect.m_xmin = -400;
	rect.m_zmin = -600;

	rect.m_xmax = 195;
	rect.m_zmax = 0;

	//ida pro LOOVES to do this fucking shit below, so the above might be wrong.
	// please check this later:
	//*(_QWORD *)&v165.m_xmin = 0xC3C80000LL;
	//*(_QWORD *)&v165.m_xmax = 0xC416000043430000uLL;

	AddRectToZone(zoneid, rect);
	g_roomZoneIDs[0] = zoneid;
	
	CreatePanel(eSpriteID_NOTASPRITE,			"FY_leftwall1",							true,	eObjectType_Misc,		100,	160,	0,		60,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-225, 80, 72),			true,	false);
	
	CreatePanel(eSpriteID_NOTASPRITE,			"FY_rightwall1",						true,	eObjectType_Misc,		100,	160,	0,		60,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-110, 80, 140),			true,	false);
	spr = CreatePanel(	eSpriteID_frontDoor,		"front door",							true,	eObjectType_Door,		60,		130,	0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_FrontDoor,			eColor_black,		XYZPointf(-190, 65, 130),			true,	false);
		spr->a1_unknown00002 = 1;
		spr->SetClickable(true);
		spr->SetVolume(XYZPointf(60, 130, 5));
		spr->SetRotation(XYZPointf(0, 30, 0));
		
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_abovefrontdoor",					true,	eObjectType_Misc,		60,		30,		0,		-30,	0,		true,	false,	eColor_greyshade_20,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-195, 145, 145),			false,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_leftpanel",							true,	eObjectType_Misc,		40,		160,	0,		-30,	0,		true,	false,	eColor_greyshade_20,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-235, 80, 122),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_rightpanel",						true,	eObjectType_Misc,		44,		160,	0,		-30,	0,		true,	false,	eColor_greyshade_20,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-150, 80, 171),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_leftwall1",							true,	eObjectType_Misc,		124,	160,	0,		0,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-311, 80, 115),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_leftwall2",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-370, 80, 150),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_leftwall3",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-370, 80, 250),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_leftwall4",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-370, 80, 350),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_leftwall5",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-370, 80, 450),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_rightwall1",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-135, 80, 232),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_rightwall2",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-135, 80, 332),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_rightwall3",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-135, 80, 432),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_rightwall4",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-135, 80, 532),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_backwall1",							true,	eObjectType_Misc,		80,		160,	0,		0,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-340, 80, 480),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_backwall2",							true,	eObjectType_Misc,		60,		160,	0,		0,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-165, 80, 480),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_backwall_top",						true,	eObjectType_Misc,		110,	 30,	0,		0,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-245, 145, 480),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_elevator1",							true,	eObjectType_Misc,		55,		130,	0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_Elevator,				eColor_black,		XYZPointf(-221, 65, 480.5),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_elevator2",							true,	eObjectType_Misc,		55,		130,	0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_Elevator,				eColor_black,		XYZPointf(-274, 65, 480.5),			true,	false);
	
	spr = CreatePanel(	eSpriteID_elevatorButton,	"elevator button",						true,	eObjectType_Misc,		7,		15,		0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_ElevatorButton,		eColor_black,		XYZPointf(-315, 65, 479.5),			true,	false);
		spr->SetVolume(XYZPointf(10, 10, 20));
		spr->SetClickable(true);
		
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwall1",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(50, 80, 450),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwall2",						true,	eObjectType_Misc,		80,		160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(50, 80, 360),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwall2_sliver",				true,	eObjectType_Misc,		20,		160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(50, 80, 310),				false,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwallabovekitchendoor",		true,	eObjectType_Misc,		60,		40,		0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(50, 140, 270),			false,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwall3",						true,	eObjectType_Misc,		40,		160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(50, 80, 220),				false,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwall4",						true,	eObjectType_Misc,		102,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(50, 80, 149),				false,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwall_3_4_real",				true,	eObjectType_Misc,		105,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(40, 80, 175),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_righthallwall1",					true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(140, 80, 450),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_righthallwall2",					true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(140, 80, 350),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_righthallwall3",					true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(140, 80, 250),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_righthallwall4",					true,	eObjectType_Misc,		102,	160,	0,		90,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(140, 80, 149),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"bedroom door",							true,	eObjectType_Misc,		60,		130,	0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_BedroomDoor,			eColor_black,		XYZPointf(95, 65, 450),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_backwall1",							true,	eObjectType_Misc,		20,		160,	0,		0,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(135, 80, 450),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_backwall2",							true,	eObjectType_Misc,		20,		160,	0,		0,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(55, 80, 450),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_aboveBedroomDoor",					true,	eObjectType_Misc,		60,		30,		0,		0,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(95, 145, 450),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_rightwall1",						true,	eObjectType_Misc,		86,		160,	0,		0,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(7, 80, 200),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_rightwall2",						true,	eObjectType_Misc,		100,	160,	0,		0,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-85, 80, 200),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_backwall1",							true,	eObjectType_Misc,		100,	160,	0,		0,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(0, 80, 450),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_backwall2",							true,	eObjectType_Misc,		90,		160,	0,		0,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-85, 80, 450),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabfront1",					true,	eObjectType_Misc,		60,		50,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WhiteCabinetDoor1,	eColor_black,		XYZPointf(-100, 25, 230),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabfront2",					true,	eObjectType_Misc,		60,		50,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WhiteCabinetDoor1,	eColor_black,		XYZPointf(-100, 25, 290),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabfront3",					true,	eObjectType_Misc,		60,		50,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WhiteCabinetDoor1,	eColor_black,		XYZPointf(-100, 25, 350),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabfront4",					true,	eObjectType_Misc,		20,		50,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WhiteCabinetDoor1,	eColor_black,		XYZPointf(-100, 25, 390),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabtop1",					true,	eObjectType_Misc,		20,		60,		90,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-110, 50, 230),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabtop2",					true,	eObjectType_Misc,		20,		60,		90,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-110, 50, 290),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabtop3",					true,	eObjectType_Misc,		20,		60,		90,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-110, 50, 350),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabtop4",					true,	eObjectType_Misc,		20,		60,		90,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-110, 50, 410),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_backwallcabfront1",					true,	eObjectType_Misc,		20,		50,		0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WhiteCabinetDoor1,	eColor_black,		XYZPointf(-90, 25, 400),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_backwallcabfront2",					true,	eObjectType_Misc,		60,		50,		0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WhiteCabinetDoor1,	eColor_black,		XYZPointf(-50, 25, 400),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_backwallcabtop2",					true,	eObjectType_Misc,		80,		40,		90,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-60, 50, 420),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_fridgeside1",						true,	eObjectType_Misc,		60,		120,	0,		90,		0,		true,	false,	eColor_unnamed034,		eTextureFilmstrip_NOTATEXTURE,			eColor_black,		XYZPointf(-20, 60, 430),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_fridgeside2",						true,	eObjectType_Misc,		60,		120,	0,		90,		0,		true,	false,	eColor_unnamed034,		eTextureFilmstrip_NOTATEXTURE,			eColor_black,		XYZPointf(40, 60, 430),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_fridgefront1",						true,	eObjectType_Misc,		60,		40,		0,		0,		0,		true,	false,	eColor_unnamed034,		eTextureFilmstrip_NOTATEXTURE,			eColor_black,		XYZPointf(10, 100, 400),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_fridgefront2",						true,	eObjectType_Misc,		60,		72,		0,		0,		0,		true,	false,	eColor_unnamed034,		eTextureFilmstrip_NOTATEXTURE,			eColor_black,		XYZPointf(10, 44, 400),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_fridgefrontbottom",					true,	eObjectType_Misc,		60,		8,		0,		0,		0,		true,	false,	eColor_unnamed015,		eTextureFilmstrip_NOTATEXTURE,			eColor_black,		XYZPointf(10, 4, 401),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"ceiling1",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_30,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-250, 160, 300),			false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"ceiling2",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_30,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(250, 160, 300),			false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"ceiling3",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_30,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-250, 160, -300),			false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"ceiling4",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_30,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(250, 160, -300),			false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"floor1",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_45,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-250, 0, 300),			false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"floor2",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_45,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(250, 0, 300),				false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"floor3",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_45,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-250, 0, -300),			false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"floor4",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_45,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(250, 0, -300),			false,	true);
}
void InitEnvironment()
{
	g_numZones = 0;
	for (int i = 0; i < MAXZONES; ++i)
		g_zones[i].numrects = -1;	

	//for (j = 0; j < 90; ++j)
	//{
	//	for (k = 0; k < 90; ++k)
	//		g_fixedPathPlanGrid[90 * j + k] = 0;
	//}
	//g_printMapCtr = -1;
}
void InitCinematography()
{
	g_bCinematographyOn = false;
	g_frameWhenLastCinematographyMove = 0;
	g_framePlayerLastMoved = 0;
	g_bRequestAutoFraming = false;
	g_autoTurnController.InitController(&g_autoTurnAmount, -1000, 1000, true);
}
static void InitPlayerNavigationVars()
{
	g_playerTransAmt = 0;
	g_playerRotAmt = 0;
	g_bPlayerRotateLeft = g_bPlayerRotateRight = 
		g_bPlayerMoveForwards = g_bPlayerMoveBackwards = 
		g_bPlayerMoveUpwards = g_bPlayerMoveDownwards = false;
	g_AutoMovePlayerStatus = 0;
	g_bSeatedOnCouch = false;
	g_bTripCarry = false;
}
static void InitPlayerNavigation()
{
	InitPlayerNavigationVars();
	g_PlayerXController.InitController(&g_autoMovePlayerPt.m_x, -10000, 10000, false);
	g_PlayerYController.InitController(&g_autoMovePlayerPt.m_y, -10000, 10000, false);
	g_PlayerZController.InitController(&g_autoMovePlayerPt.m_z, -10000, 10000, false);
	g_PlayerRotController.InitController(&g_autoMovePlayerRot, -1000, 1000, false);
}
Sprite* CreateCursorSprite()
{
	Sprite* spr = new Sprite();
	spr->InitSprite(eSpriteID_cursor, "cursor", false, true);
	return spr;
}
CharacterSprite* CreateGraceSprite()
{
	CharacterSprite* character = new CharacterSprite();
	//character->m_modelinfo = &g_Grace_modelInfo;
	//character->m_modelquadlines = &g_Grace_modelQuadLines;
	//character->m_modelquadpoly = &g_Grace_modelQuadPolys;
	//character->m_characterarms = &g_Grace_modelArms;
	//character->m_outlineconnectors = &g_Grace_modelOutlineConnectors;
	character->a1_unknown00010d = true;
	character->a1_unknown00001 = true;
	character->InitAnimatingSprite(eSpriteID_grace, "grace", 76, 4533, -5, 5, 35);
	character->SetVolume(XYZPointf(30, 100, 50));
	character->SetCollidableType(eCollidableType_AABB);
	return character;
}
CharacterSprite* CreateTripSprite()
{
	CharacterSprite* character = new CharacterSprite();
	//character->m_modelinfo = &g_Trip_modelInfo;
	//character->m_modelquadlines = &g_Trip_modelQuadLines;
	//character->m_modelquadpoly = &g_Trip_modelQuadPolys;
	//character->m_characterarms = &g_Trip_modelArms;
	//character->m_outlineconnectors = &g_Trip_modelOutlineConnectors;
	character->a1_unknown00010d = true;
	character->a1_unknown00001 = true;
	character->InitAnimatingSprite(eSpriteID_trip, "trip", 73, 7616, -5, 5, 64);
	character->SetVolume(XYZPointf(30, 100, 50));
	character->SetCollidableType(eCollidableType_AABB);
	return character;
}
HeadSprite* CreateGraceHeadSprite(CharacterSprite* grace)
{
	HeadSprite* head = new HeadSprite();
	head->m_modelframe = g_GraceHeadModel_modelFrame;
	head->m_modelinfo = g_GraceHeadModel_modelInfo;
	//head->m_modelquadlines = &g_GraceHeadModel_modelQuadLines;
	//head->m_characterarms = &g_GraceHeadModel_modelArms;
	head->m_modelquadpoly = g_GraceHeadModel_modelQuadPolys;
	head->a1_unknown00010d = true;
	head->a1_unknown00001 = false;
	head->InitSprite((ESpriteID)50, "GraceHead", true, true);
	head->SetVolume(XYZPointf(1, 1, 1));

	return head;
}
HeadSprite* CreateTripHeadSprite(CharacterSprite* trip)
{
	HeadSprite* head = new HeadSprite();
	head->m_modelframe = g_TripHeadModel_modelFrame;
	head->m_modelinfo = g_TripHeadModel_modelInfo;
	//head->m_modelquadlines = &g_TripHeadModel_modelQuadLines;
	//head->m_characterarms = &g_TripHeadModel_modelArms;
	head->m_modelquadpoly = g_TripHeadModel_modelQuadPolys;
	head->a1_unknown00010d = true;
	head->a1_unknown00001 = false;
	head->InitSprite((ESpriteID)52, "TripHead", true, true);
	head->SetVolume(XYZPointf(1, 1, 1));

	return head;
}
void InitGraceSprite()
{
	g_GraceSprite = CreateGraceSprite();
	g_GraceSprite->m_pHeadSprite = CreateGraceHeadSprite(g_GraceSprite);
	g_GraceSprite->a1_unknown00002 = 75;
	g_GraceSprite->animsprite_unknown001 = 75;
	g_GraceSprite->unknown005 = 30;
	g_GraceSprite->unknown006 = -3;
	g_GraceSprite->unknown007 = 8;
	g_GraceSprite->unknown008 = 9;
	g_GraceSprite->InitCharacterSprite(
		-65, 65,
		-20, 35,
		-30, 30,
		-35, 35,
		0.25, 0.75,
		0.25, 0.75,
		0, 1,
		(ESpriteID)51);
	g_GraceSprite->SetEyelidHeight(0.85, true, true, true);
	//g_GraceSprite->unknown039.InitLayerController(eAnimLayer_breathe, g_GraceSprite);  
	//g_GraceSprite->unknown039.SetLayerAlt(0, "breathe",			-1, -1, 2, -1, -1, -1, true,	1, -1);
	//g_GraceSprite->unknown039.SetLayerAlt(1, "breatheMed",		-1, -1, 3, -1, -1, -1, false,	8,  0);
	//g_GraceSprite->unknown039.SetLayerAlt(2, "breatheMedHi",	-1, -1, 4, -1, -1, -1, false,	8,  1);
	//g_GraceSprite->unknown039.SetLayerAlt(3, "breatheHeavy",	-1, -1, 5, -1, -1, -1, false,	8,  2);

	//undone
}
void InitTripSprite()
{
	g_TripSprite = CreateTripSprite();
	g_TripSprite->m_pHeadSprite = CreateTripHeadSprite(g_TripSprite);
	g_TripSprite->a1_unknown00002 = 44;
	g_TripSprite->animsprite_unknown001 = 44;
	g_TripSprite->unknown005 = 30;
	g_TripSprite->unknown006 = -3;
	g_TripSprite->unknown007 = 8;
	g_TripSprite->unknown008 = 9;
	g_TripSprite->InitCharacterSprite(
		-65, 65,
		-20, 35,
		-30, 30,
		-35, 35,
		0.25, 0.75,
		0.25, 0.75,
		0, 1,
		(ESpriteID)53);
	g_TripSprite->SetEyelidHeight(0.85, true, true, true);
	//g_TripSprite->unknown039.InitLayerController(eAnimLayer_breathe, g_TripSprite);
	//g_TripSprite->unknown039.SetLayerAlt(0, "breathe",		-1, -1, 1, -1, -1, -1, true,	1, -1);
	//g_TripSprite->unknown039.SetLayerAlt(1, "breatheMed",	-1, -1, 2, -1, -1, -1, false,	8,  0);
	//g_TripSprite->unknown039.SetLayerAlt(2, "breatheMedHi", -1, -1, 3, -1, -1, -1, false,	8,  1);
	//g_TripSprite->unknown039.SetLayerAlt(3, "breatheHeavy", -1, -1, 4, -1, -1, -1, false,	8,  2);

	//undone
}
void InitTransRotScale(CharacterSprite* spr, bool b1)
{
	static int g_playerMotionNoise;
	if (!spr)
	{
		g_PlayerSprite->SetPosition(XYZPointf(-32, 97, -111));
		XYZPointf playerrot = g_PlayerSprite->GetRotation();
		playerrot.m_y = 24;
		g_PlayerSprite->SetRotation(playerrot);
	}
	g_playerMotionNoise = 0;
	if (!spr && g_numFramesRun > 1 && !b1)
		return;

	if (!spr)
	{
		g_GraceSprite->SetPosition(XYZPointf(-2, -5, -140));
		XYZPointf gracerot = g_GraceSprite->GetRotation();
		gracerot.m_y = 226;
		g_GraceSprite->SetRotation(gracerot);
	}
	g_GraceSprite->SetTorsoRotation(XYZPointf(0, 0, 0));
	g_GraceSprite->m_pHeadSprite->SetPosition(XYZPointf(0, -5, 160));
	g_GraceSprite->SetHeadRotation(XYZPointf(0, 50, 0));

	//g_GraceSprite->m_GestureFxnMgr1.StartGestureFxn(0, 1, 0);
	//g_GraceSprite->m_GestureFxnMgr6.StartGestureFxn(0, 43, 0);
	//g_GraceSprite->m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
	//g_GraceSprite->m_GestureFxnMgr8.StartGestureFxn(0, 1, 0);
	//g_GraceSprite->m_GestureFxnMgr9.StartGestureFxn(0, 1, 0);
	//g_GraceSprite->m_GestureFxnMgr10.StartGestureFxn(0, 1, 0);

	if (spr != g_GraceSprite)
		g_GraceSprite->SetGazeTrackingSprite(g_PlayerSprite, eGazeSpeed_med, eGazeType_normal, eAlterHeadCock_subtle);



	if (!spr)
	{
		g_TripSprite->SetPosition(XYZPointf(-30, -5, -155));
		XYZPointf triprot = g_PlayerSprite->GetRotation();
		triprot.m_y = 178;
		g_TripSprite->SetRotation(triprot);
	}

	g_TripSprite->SetTorsoRotation(XYZPointf(0, 0, 0));
	g_TripSprite->m_pHeadSprite->SetPosition(XYZPointf(0, -5, 170));
	g_TripSprite->SetHeadRotation(XYZPointf(0, 0, 0));

	//g_TripSprite->m_GestureFxnMgr1.StartGestureFxn(0, 1, 0);
	//g_TripSprite->m_GestureFxnMgr6.StartGestureFxn(0, 44, 0);
	//g_TripSprite->m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
	//g_TripSprite->m_GestureFxnMgr8.StartGestureFxn(0, 1, 0);
	//g_TripSprite->m_GestureFxnMgr9.StartGestureFxn(0, 1, 0);
	//g_TripSprite->m_GestureFxnMgr10.StartGestureFxn(0, 1, 0);

	if (spr != g_TripSprite)
		g_TripSprite->SetGazeTrackingSprite(g_PlayerSprite, eGazeSpeed_med, eGazeType_normal, eAlterHeadCock_subtle);



	Sprite* roomspr = g_spriteMgr.GetSpritePtrByName("couch");
		roomspr->SetPosition(XYZPointf(150, 0, 350));
		roomspr->SetRotation(XYZPointf(0, 0, 0));
	roomspr = g_spriteMgr.GetSpritePtrByName("cabinet");
		roomspr->SetPosition(XYZPointf(-177, 0, -350));
		roomspr->SetRotation(XYZPointf(0, 0, 0));
	roomspr = g_spriteMgr.GetSpritePtrByName("side table");
		roomspr->SetPosition(XYZPointf(170, 0, -210));
		roomspr->SetRotation(XYZPointf(0, 0, 0));
	roomspr = g_spriteMgr.GetSpritePtrByName("work table");
		roomspr->SetPosition(XYZPointf(160, 0, -30));
		roomspr->SetRotation(XYZPointf(0, 0, 0));
	roomspr = g_spriteMgr.GetSpritePtrByName("bar");
		roomspr->SetPosition(XYZPointf(-160, 0, -120));
		roomspr->SetRotation(XYZPointf(0, 0, 0));
	roomspr = g_spriteMgr.GetSpritePtrByName("side table 2");
		roomspr->SetPosition(XYZPointf(-180.0, 0.0, -60.0));
		roomspr->SetRotation(XYZPointf(0, 0, 0));

}
void InitCollisionZonesAndPathPlanGrid()
{
	for (int i = 0; i < g_spriteMgr.GetNumSprites(); ++i)
	{
		Sprite* spr = g_spriteMgr.GetSpritePtr(i);
		if (!spr->m_modelframe && !spr->GetIsCharacterSprite()
			|| spr->GetCollidableType() == eCollidableType_NonSolid)
			continue;

		XZRectf rect = ComputeSingleCollisionRectForSprite(spr);

		if (spr->m_eID == eSpriteID_bar)	rect.m_xmax += 15;
		if (spr->m_eID == eSpriteID_workTable)	rect.m_xmax -= 15;

		AddRectToZone(CreateNewZone(spr, true), rect);
		if (!spr->GetIsCharacterSprite()
			&& spr->m_eID == eSpriteID_frontDoor)
		{
			//AddRectToPathPlanGrid(rect);
		}
	}
}
static void SetupSqrtTable() {
	//undone
}
static void SetupDirOffsets() {
	//undone
}
static void SetupHeuristicTable() {
	//undone
}
static void SetupTurnTable() {
	//undone
}
void InitPathPlanning()
{
	SetupSqrtTable();
	SetupDirOffsets();
	SetupHeuristicTable();
	SetupTurnTable();
}
void InitCircleDrawing()
{
	int itertri = 0;
	for (float i = 0; i < MATH_PIDOUBLE; i = MATH_PI / 10.f + i)
	{
		animengine_assert(itertri < TRICIRCLE_ITERATIONS);
		if (itertri == 20)	g_triangleFanSineCache[itertri] = sin(0);
		else				g_triangleFanSineCache[itertri] = sin(i);

		if(itertri == 20)	g_triangleFanSineCache[itertri] = cos(0);
		else				g_triangleFanSineCache[itertri] = cos(i);

		itertri++;
	}
}
void InitQueuedUpRequestedPerformance()
{
	//undone
}

void InitGlobals()
{
	dprintf("\n---------------------------\nInitting globals");
	srand(time(0));

	g_startTime = GetTickCount();
	g_numFramesRun = 0;
	g_bShouldRenderThisFrame = true;
	g_SkipFramesCounter = 0;
	g_bAssertionThrown = false;
	g_launchErrorString[0] = '\0';
	g_bAllQuadLineOutline = false;
	g_bAllCircleOutline = false;

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
	InitTransRotScale(nullptr, false);
	InitCollisionZonesAndPathPlanGrid();
	InitPathPlanning();
	InitCircleDrawing();
	InitQueuedUpRequestedPerformance();

	g_soundSource.InitSoundSource("sounds\\global\\globalsnd.txt");
	g_soundSourceID = g_soundMgr.AddSoundSource(&g_soundSource);

	BuildBitmapFont();

	XCursor::theirCursor = new XCursor();
	XCursor::theirCursor->InitCursor();
	XCursor::theirCursor->JumpCutToCursor(eCursorType_Idle, true);

	LoadIcon(g_hAppInstance, (LPCSTR)IDI_FACADEWINDOWICON);

	for (int i = 0; i < 32; i++)
		g_curTextInput[i] = 0;
	g_curTextCharInput = 0;
}
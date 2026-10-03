#ifndef FACADECOMMON_H
#define FACADECOMMON_H

#include "stdlib.h"
#include "stddef.h"
#include "stdint.h"
#include "stdarg.h"

#include "time.h"
#include "stdio.h"
#include "windows.h"

#include "enums/EAblRequestType.h"
#include "enums/EAlterHeadCock.h"
#include "enums/ECollidableType.h"
#include "enums/EColor.h"
#include "enums/ECursorType.h"
#include "enums/EDrink.h"
#include "enums/EEventLogType.h"
#include "enums/EGazeSpeed.h"
#include "enums/EGazeType.h"
#include "enums/EGlobalSounds.h"
#include "enums/EMiscLittleAction.h"
#include "enums/EObjectType.h"
#include "enums/EMusicAction.h"
#include "enums/EMusicName.h"
#include "enums/EPhoneCallBehavior.h"
#include "enums/EPlayerGestureID.h"
#include "enums/ESetAnimEngineInfo.h"
#include "enums/ESpriteID.h"
#include "enums/ESurfacePickupAngle.h"
#include "enums/ETextureFilmstrip.h"

#include "../../animEngineStarter_decomp/resource.h"

#include "classes/Sprite.h"


void dprintf(const char* format, ...);
void animengine_assert(bool cond);

void AddEventInfoToLog(EEventLogType type, ESpriteID perpetrator,
	const char* text, bool speechinterrupted,
	ESpriteID victim, uint32_t tick);
void OutputEventLog(bool stageplay);
void ShouldntBeHere(const char* asserttext);

bool IsWhitespaceNewLineOrComma(char c);
bool IsNumberCharacter(char c);
bool IsLetterCharacter(char c);
bool IsPunctuationCharacter(char c);

float NormalizeNeg180to180(float ang);
float Normalize0to360(float ang);

int rand2(int min, int max);



#define WM_EDITGRACESCRIPT WM_USER
#define WM_EDITGRACEFACE WM_USER + 1
#define WM_EDITTRIPSCRIPT WM_USER + 2
#define WM_EDITTRIPFACE WM_USER + 3



template <typename T>
class XTPoint
{
public:
	T x;
	T y;
};

template<typename T>
class XYZPoint_template
{
public:
	XYZPoint_template(T x, T y, T z) : m_x(x), m_y(y), m_z(z) {}
	T m_x;
	T m_y;
	T m_z;
};

class XZPointf
{
public:
	float m_x;
	float m_z;
};

class XZRectf
{
public:
	float m_x1;
	float m_z1;

	float m_x2;
	float m_z2;
};

typedef XYZPoint_template<float> XYZPointf;
typedef XTPoint<int> XTPointi;

struct player_namegender_map_t
{
	const char* name;
	const char* gender;
};
extern player_namegender_map_t g_PlayerNameStrings[95];

struct gltexture_t
{
	char name[64];
	uint32_t texid;
};
struct textureentry_t
{
	gltexture_t entries[20];
	bool isalpha;
	int width1;
	int height1;
	int width2;
	int height2;
	bool valid;
	int numentries;
	int unknown;
};
extern textureentry_t g_TextureFilmstrips[eTextureFilmstrip_Max];
extern Sprite* g_PlayerSprite;
extern Sprite* g_CursorSprite;


#define GENDER_MALE 0
#define GENDER_FEMALE 1
extern int g_playerGender;
extern int g_playerNameID;
extern EPlayerGestureID g_playerGesture;
extern ESpriteID g_playerGestureSpriteID;

extern HWND g_hMainAppWindow;
extern HINSTANCE g_hAppInstance;
extern HDC g_hDeviceContext;
extern HANDLE g_ablRequestBufferMutexHandle;

extern uint32_t g_numFramesRun;
extern uint32_t g_SkipFramesCounter;

extern char g_launchErrorString[52];
extern char g_curTextInput[32];
extern char g_curTextCharInput;



extern bool g_bAblRequestingFPSAdjustment;

extern bool g_bAllCircleOutline;
extern bool g_bAllQuadLineOutline;

extern bool g_bAntiAliasing;

extern bool g_bAssertionThrown;

extern bool g_bAtEndOfDrama;
extern bool g_bAtEndOfLoad;

extern bool g_bBlackBackground;
extern bool g_bCinematographyOn;
extern bool g_bDoSoundDropOff;
extern bool g_bDoingFPSAdjustment;
extern bool g_bDoneTension1bMusic;
extern bool g_bExactPointDecel;
extern bool g_bExactRotDecel;
extern bool g_bFacadeHasBeenRunMultipleTimes;

extern bool g_bIgnoreArmLSetPerf;
extern bool g_bIgnoreArmRSetPerf;
extern bool g_bIgnoreGazeSetPerf;
extern bool g_bIgnoreNextWalkAbort;

extern bool g_bIsAblConnected;
extern bool g_bJimDoran;
extern bool g_bKnowScriptNames;
extern bool g_bMuteSounds;
extern bool g_bNewPlayerTextEntered;
extern bool g_bOnCommandLine;
extern bool g_bParsingError;
extern bool g_bPhoneCallBehaviourSubparts[90]; //?

extern bool g_bPlayerChoosingName;
extern bool g_bPlayerInputActive;
extern bool g_bPlayerMoveBackwards;
extern bool g_bPlayerMoveDownwards;
extern bool g_bPlayerMoveForwards;
extern bool g_bPlayerMoveUpwards;
extern bool g_bPlayerRotateLeft;
extern bool g_bPlayerRotateRight;

extern bool g_bPrintBetaInfo;
extern bool g_bQuittingApp;
extern bool g_bRequestAutoFraming;
extern bool g_bSeatedOnCouch;
extern bool g_bShouldRenderThisFrame;
extern bool g_bTextCharWasPressed;
extern bool g_bTripCarry;
extern bool g_bVeryNearCharacter;
extern bool g_bWriteOutput;


#endif // FACADECOMMON_H
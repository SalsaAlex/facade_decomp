#ifndef FACADECOMMON_H
#define FACADECOMMON_H

#include "stdlib.h"
#include "stddef.h"
#include "stdint.h"
#include "stdarg.h"
#include "math.h"

#include "time.h"
#include "stdio.h"


#include "windows.h"
#include "windowsx.h"

#include "enums/EAblRequestType.h"
#include "enums/EAlterHeadCock.h"
#include "enums/EAnimLayer.h"
#include "enums/EBaseScreenMessage.h"
#include "enums/EBlinkBehavior.h"
#include "enums/EBlinkType.h"
#include "enums/EBodyPoseConfig.h"
#include "enums/ECinematographyFraming.h"
#include "enums/ECollidableType.h"
#include "enums/EColor.h"
#include "enums/EControllerType.h"
#include "enums/ECursorType.h"
#include "enums/EDrink.h"
#include "enums/EEventLogType.h"
#include "enums/EFullExpressionBase.h"
#include "enums/EFullExpressionMood.h"
#include "enums/EGazeSpeed.h"
#include "enums/EGazeType.h"
#include "enums/EGestureBodyPart.h"
#include "enums/EGlobalSounds.h"
#include "enums/ELayerAltINdex.h"
#include "enums/EMiscLittleAction.h"
#include "enums/EMusicAction.h"
#include "enums/EMusicName.h"
#include "enums/EObjectType.h"
#include "enums/EPhoneCallBehavior.h"
#include "enums/EPlayerGestureID.h"
#include "enums/ESetAnimEngineInfo.h"
#include "enums/ESpriteID.h"
#include "enums/ESurfacePickupAngle.h"
#include "enums/ETextureFilmstrip.h"
#include "enums/EUScript.h"
#include "enums/EWalkType.h"

#include "../../animEngineStarter/resource.h"


#define WM_EDITGRACESCRIPT WM_USER
#define WM_EDITGRACEFACE WM_USER + 1
#define WM_EDITTRIPSCRIPT WM_USER + 2
#define WM_EDITTRIPFACE WM_USER + 3

#define MATH_PI 3.1415
#define MATH_PIDOUBLE 6.283

#define VAL_SWAP(type, x, y) type temp = x; x = y; y = temp

#define DATA_NULLVAL 666


template <typename T>
class XTPoint
{
public:
	T x;
	T y;
};

template<typename T>
class XYZPoint_Template
{
public:
	XYZPoint_Template() : m_x(0), m_y(0), m_z(0) {}
	XYZPoint_Template(T x, T y, T z) : m_x(x), m_y(y), m_z(z) {}

	bool operator!=(const XYZPoint_Template<T>& point) { return m_x != point.m_x && m_y != point.m_y && m_z != point.m_z; }
	bool operator==(const XYZPoint_Template<T>& point) { return m_x == point.m_x && m_y == point.m_y && m_z == point.m_z; }

	void operator+=(const XYZPoint_Template<T>& point) { m_x += point.m_x; m_y += point.m_y; m_z += point.m_z; }
	XYZPoint_Template operator+(const XYZPoint_Template<T>& point) { 
		XYZPoint_Template<T> temp;
		temp.m_x = m_x + point.m_x;
		temp.m_y = m_y + point.m_y;
		temp.m_z = m_z + point.m_z;
		return temp;
	}
	void operator-=(const XYZPoint_Template<T>& point) { m_x -= point.m_x; m_y -= point.m_y; m_z -= point.m_z; }
	XYZPoint_Template operator-(const XYZPoint_Template<T>& point) {
		XYZPoint_Template<T> temp;
		temp.m_x = m_x - point.m_x;
		temp.m_y = m_y - point.m_y;
		temp.m_z = m_z - point.m_z;
		return temp;
	}

	XYZPoint_Template operator-() {
		XYZPoint_Template<T> temp;
		temp.m_x = -m_x;
		temp.m_y = -m_y;
		temp.m_z = -m_z;
		return temp;
	}

	void operator*=(const float& f) {
		m_x *= f;
		m_y *= f;
		m_z *= f;
	}
	XYZPoint_Template operator*(const float& f) {
		XYZPoint_Template<T> temp;
		temp.m_x = m_x * f;
		temp.m_y = m_y * f;
		temp.m_z = m_z * f;
		return temp;
	}

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

template <typename T, typename U>
class XTRect
{
public:

	void set();

	T m_xmin;
	T m_ymin;

	U m_xmax;
	U m_ymax;

	// T T U U, T U T U, or U U T T ?.. we'll never know..

};

class XZRectf
{
public:
	float m_xmin;
	float m_zmin;

	float m_xmax;
	float m_zmax;
};

typedef XYZPoint_Template<float> XYZPointf;
typedef XTPoint<int> XTPointi;


struct ObjSensePacket
{
	ESpriteID objectID;
	float x;
	float y;
	float z;
};
struct ObjStateSensePacket
{
	ESpriteID objectID;
	int state;
};
struct ObjectActivationSensePacket
{
	int activation;
	ESpriteID activatorid;
};
struct HeldObjectSensePacket
{
	ESpriteID objectID;
	ESpriteID characterID;
	int attachPointID;
	int objectCategory;
};
struct AnimCueSensePacket
{
	int unknown[3][8];
};

struct perfinfo_animapi_input
{
	int a2;
	int a3;
	int a4;
	int a5;
	int a6;
	int a7;
	int a8;
	int a9;
	int a10;
	int a11;
	int a12;
	int a13;
	int a14;
	int a15;
	int a16;
	int a17;
	int a18;
	int a19;
	int a20;
	int a21;
	int a22;
	int a23;
	int a24;
	int a25;
	int a26;
	int a27;
	int a28;
	int a29;
	int a30;
	int a31;
	int a32;
	int a33;
	int a34;
	int a35;
	int a36;
	int a37;
	int a38;
	int a39;
	int a40;
	int a41;
	int a42;
	int a43;
	int a44;
	int a45;
	int a46;
	int a47;
	int a48;
	int a49;
	int a50;
	int a51;
	int a52;
	int a53;
	int a54;
	int a55;
	int a56;
	int a57;
	int a58;
	int a59;
	int a60;
	int a61;
	int a62;
	int a63;
	int a64;
	int a65;
	int a66;
	int a67;
	int a68;
	int a69;
	int a70;
	int a71;
	int a72;
	int a73;
	int a74;
	int a75;
	int a76;
	int a77;
	int a78;
	int a79;
	int a80;
	int a81;
	int a82;
	int a83;
	int a84;
	int a85;
};


struct player_namegender_map_t
{
	const char* name;
	const char* gender;
};
#define NUM_PLAYERNAMES 95
extern player_namegender_map_t g_PlayerNameStrings[NUM_PLAYERNAMES];

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

#include "classes/Sprite/Sprite.h"
#include "classes/Sprite/HeadSprite.h"
#include "classes/Sprite/AnimatingSprite.h"
#include "classes/Sprite/CharacterSprite.h"
#include "classes/SpriteMgr.h"
#include "classes/SoundMgr.h"

#define MAX_PANELS 200

struct panelmdlframes_t
{
	mdlframe_t frames[6]; //frames[5] is DATA_NULLVAL
};
struct panelmdlinfos_t
{
	mdlinfo_t infos[6]; //infos[5] is DATA_NULLVAL
};
struct panelmdlquadpolys_t
{
	mdlquadpoly_t polys[2];
};


extern float g_playerTransAmt;
extern Controller g_PlayerXController;
extern Controller g_PlayerYController;
extern Controller g_PlayerZController;
extern float g_playerRotAmt;
extern Controller g_PlayerRotController;
extern int g_AutoMovePlayerStatus;

extern XYZPointf g_autoMovePlayerPt;
extern float g_autoMovePlayerRot; //yaw

extern Controller g_autoTurnController;
extern float g_autoTurnAmount;

extern int g_numPanels;
extern panelmdlframes_t g_panelModelFrames[MAX_PANELS];
extern panelmdlinfos_t g_panelModelInfo[MAX_PANELS];
extern panelmdlquadpolys_t g_panelModelQuadPolys[MAX_PANELS];

extern CharacterSprite* g_GraceSprite;
extern CharacterSprite* g_TripSprite;
extern Sprite* g_PlayerSprite;
extern Sprite* g_CursorSprite;

extern SpriteMgr g_spriteMgr;

extern SoundMgr g_soundMgr;
extern SoundSource g_soundSource;
extern int g_soundSourceID;

extern EBaseScreenMessage g_baseScreenMessage;

#define GENDER_MALE 0
#define GENDER_FEMALE 1
extern int g_playerGender;
extern int g_playerNameID;
extern EPlayerGestureID g_playerGesture;
extern ESpriteID g_playerGestureSpriteID;

extern HWND g_hMainAppWindow;
extern HWND g_scriptDialogHWnd;
extern HWND g_faceDialogHWnd;

extern HINSTANCE g_hAppInstance;
extern HDC g_hDeviceContext;
extern HANDLE g_ablRequestBufferMutexHandle;

extern uint32_t g_numFramesRun;
extern uint32_t g_SkipFramesCounter;
extern uint32_t g_whenNewPlayerTextEntered;
extern uint32_t g_lastFrameGotAblRequest;

extern char g_launchErrorString[52];
extern char g_curTextInput[32];
extern char g_curTextCharInput;

extern float g_playerTransAmt;
extern float g_playerRotAmt;
extern int g_AutoMovePlayerStatus;

extern int g_ticksIntroStarted;
extern int g_introEventCtr;

extern int g_textDisplayState;

extern int g_framesWhenLastSentPlayerTypedCue;
extern int g_frameWhenLastCinematographyMove;
extern int g_framePlayerLastMoved;
extern int g_blastFlagSignalToAbl;

extern int g_introChoices[8];
extern float g_introTimings[8 + 1];
extern float g_introBaseTimings[8][6];
extern float g_initialIntroDelay;
extern float g_startTitleFadeIn;

extern int g_quitAppCountdown;

#define TRICIRCLE_ITERATIONS 21

extern float g_triangleFanSineCache[TRICIRCLE_ITERATIONS];
extern float g_triangleFanCosineCache[TRICIRCLE_ITERATIONS + 10];

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
extern bool g_bIgnoreNextWalkAbort[2];

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

extern bool g_mouseLButtonClicked;

extern bool g_bDisplayDialogText;






void dprintf(const char* format, ...);
void animengine_assert(bool cond);

void AddEventInfoToLog(EEventLogType type, ESpriteID perpetrator,
	const char* text, bool speechinterrupted,
	ESpriteID victim, uint32_t tick);

void OutputEventLog(bool stageplay);
void ShouldntBeHere(const char* asserttext);

int FloatToInt(float f);

bool IsWhitespaceNewLineOrComma(char c);
bool IsNumberCharacter(char c);
bool IsLetterCharacter(char c);
bool IsPunctuationCharacter(char c);

float NormalizeNeg180to180(float ang);
float Normalize0to360(float ang);

int rand2(int min, int max);
bool rand100(int val);

bool PlayGlobalSound(EGlobalSounds sound);
void StopAllGlobalSounds();

void StopAllMP3s();

bool CollisionTest(int zoneindex, XZRectf rect, Sprite* spr1, Sprite* spr2);
bool CollisionTestWithSprite(Sprite* collider1, XYZPointf point, int expandsize, Sprite* collider2);
bool CollisionTestWithWorld(Sprite* spr, XYZPointf pos, int unknown, bool option);
XZRectf ComputeSingleCollisionRectForSprite(Sprite* spr);

void ClipFloatToRange(float& value, float min, float max);

float GetSecsSinceTicks(int ticks);

float GetXYDistanceBetweenPoints(XYZPointf p1, XYZPointf p2);
float GetXZDistanceBetweenPoints(XYZPointf p1, XYZPointf p2);
float GetXZAngleBetweenPoints(XYZPointf p1, XYZPointf p2);

void GetSubjectivePosition(XYZPointf pos, XYZPointf& out, float& floatout, bool option);
XYZPointf GetObjectivePosition(XYZPointf pos, float factor);

void GetXYBoundingBoxOf4Points(const XYZPointf* a2, XYZPointf* a3);

bool DoXZRectsIntersect(XZRectf rect1, XZRectf rect2);
void GetGridXZFromWorldXZ(float worldx, float worldz,
	float& outgridx, float& outgridz);

bool IsPointVisibleToPlayer(XYZPointf point, bool addoffset);
bool IsPlayerOutsideApt();
void PlayerEnteredText();

float ConvertWorldDegreesToRadians(float ang);
XYZPointf PointAtAngleAndDist(XYZPointf dist, float f1, float f2);

float FrameRateAdjust(float val, bool multiply = true);

void LoadTextureFilmstrip(ETextureFilmstrip filmstrip);

int GetArmSyncSetForScript(CharacterSprite* actor, int unknown);


#endif // FACADECOMMON_H
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

#include "hooking/hooking_allaround.h"

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
DEFINE_HOOKEDARRAYVAR(textureentry_t, g_TextureFilmstrips, 0x1082D5B4)
#define g_TextureFilmstrips						_g_TextureFilmstrips()

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

#define TRICIRCLE_ITERATIONS 21
#define GENDER_MALE 0
#define GENDER_FEMALE 1


DEFINE_HOOKEDARRAYVAR(Controller, g_PlayerXController, 0x106984F8);
DEFINE_HOOKEDARRAYVAR(Controller, g_PlayerYController, 0x10698498);
DEFINE_HOOKEDARRAYVAR(Controller, g_PlayerZController, 0x10698410);
DEFINE_HOOKEDARRAYVAR(Controller, g_PlayerRotController, 0x10698450);
DEFINE_HOOKEDARRAYVAR(XYZPointf, g_autoMovePlayerPt, 0x10698540);
DEFINE_HOOKEDVAR(float, g_autoMovePlayerRot, 0x10698538); //yaw
DEFINE_HOOKEDARRAYVAR(Controller, g_autoTurnController, 0x10200460);
DEFINE_HOOKEDVAR(float, g_autoTurnAmount, 0x10200454);
DEFINE_HOOKEDVAR(int, g_numPanels, 0x103AE850);
DEFINE_HOOKEDARRAYVAR(panelmdlframes_t, g_panelModelFrames, 0x103AB010)
DEFINE_HOOKEDARRAYVAR(panelmdlinfos_t, g_panelModelInfo, 0x1039CF10)
DEFINE_HOOKEDARRAYVAR(panelmdlquadpolys_t, g_panelModelQuadPolys, 0x103A5250)
DEFINE_HOOKEDVAR(CharacterSprite*, g_GraceSprite, 0x1097947C)
DEFINE_HOOKEDVAR(CharacterSprite*, g_TripSprite, 0x10979480)
DEFINE_HOOKEDVAR(Sprite*, g_PlayerSprite, 0x10979474)
DEFINE_HOOKEDVAR(Sprite*, g_CursorSprite, 0x10979478)
DEFINE_HOOKEDARRAYVAR(SpriteMgr, g_spriteMgr, 0x1083F640)
DEFINE_HOOKEDARRAYVAR(SoundMgr, g_soundMgr, 0x10978420)
DEFINE_HOOKEDARRAYVAR(SoundSource, g_soundSource, 0x1083F8C0)
DEFINE_HOOKEDVAR(int, g_soundSourceID, 0x109782F4)
DEFINE_HOOKEDVAR(EBaseScreenMessage, g_baseScreenMessage, 0x106988D4)
DEFINE_HOOKEDVAR(int, g_playerGender, 0x10979488)
DEFINE_HOOKEDVAR(int, g_playerNameID, 0x101F6F18)
DEFINE_HOOKEDVAR(EPlayerGestureID, g_playerGesture, 0x102006F8)
DEFINE_HOOKEDVAR(ESpriteID, g_playerGestureSpriteID, 0x10200700)
DEFINE_HOOKEDVAR(HWND, g_hMainAppWindow, 0x1083F634)
DEFINE_HOOKEDVAR(HWND, g_scriptDialogHWnd, 0x1021B864)
DEFINE_HOOKEDVAR(HWND, g_faceDialogHWnd, 0x10201AA0)
DEFINE_HOOKEDVAR(HINSTANCE, g_hAppInstance, 0x1083F628)
DEFINE_HOOKEDVAR(HDC, g_hDeviceContext, 0x109782F8)
DEFINE_HOOKEDVAR(HANDLE, g_ablRequestBufferMutexHandle, 0x1083F8B8)
DEFINE_HOOKEDVAR(uint32_t, g_numFramesRun, 0x1083F62C)
DEFINE_HOOKEDVAR(uint32_t, g_SkipFramesCounter, 0x1097945C)
DEFINE_HOOKEDVAR(uint32_t, g_whenNewPlayerTextEntered, 0x101C6F9C)
DEFINE_HOOKEDVAR(uint32_t, g_lastFrameGotAblRequest, 0x101FF0E4)
DEFINE_HOOKEDARRAYVAR(char, g_launchErrorString, 0x1083F5F0)
DEFINE_HOOKEDARRAYVAR(char, g_curTextInput, 0x109782D0)
DEFINE_HOOKEDVAR(char, g_curTextCharInput, 0x10978400)
DEFINE_HOOKEDVAR(float, g_playerTransAmt, 0x106984DC)
DEFINE_HOOKEDVAR(float, g_playerRotAmt, 0x106984E4)
DEFINE_HOOKEDVAR(int, g_AutoMovePlayerStatus, 0x10698550)
DEFINE_HOOKEDVAR(int, g_ticksIntroStarted, 0x101C7618)
DEFINE_HOOKEDVAR(int, g_introEventCtr, 0x101C7614)
DEFINE_HOOKEDVAR(int, g_textDisplayState, 0x1069A170)
DEFINE_HOOKEDVAR(int, g_framesWhenLastSentPlayerTypedCue, 0x101FF0E0)
DEFINE_HOOKEDVAR(int, g_frameWhenLastCinematographyMove, 0x102004A8)
DEFINE_HOOKEDVAR(int, g_framePlayerLastMoved, 0x10200450)
DEFINE_HOOKEDVAR(int, g_blastFlagSignalToAbl, 0x101FF0DC)
DEFINE_HOOKEDARRAYVAR(int, g_introChoices, 0x1069891C)
DEFINE_HOOKEDARRAYVAR(float, g_introTimings, 0x1069A13C)
DEFINE_HOOKEDARRAYVAR(float, g_introBaseTimings, 0x101C7554)
DEFINE_HOOKEDVAR(float, g_initialIntroDelay, 0x101C761C)
DEFINE_HOOKEDVAR(float, g_startTitleFadeIn, 0x1069A164)
DEFINE_HOOKEDVAR(int, g_quitAppCountdown, 0x101F6EC8)
DEFINE_HOOKEDARRAYVAR(float, g_triangleFanSineCache, 0x102004C0)
DEFINE_HOOKEDARRAYVAR(float, g_triangleFanCosineCache, 0x10200514)
DEFINE_HOOKEDVAR(bool, g_bAblRequestingFPSAdjustment, 0x1097948C)
DEFINE_HOOKEDVAR(bool, g_bAllCircleOutline, 0x1083F8B5)
DEFINE_HOOKEDVAR(bool, g_bAllQuadLineOutline, 0x1083F8B4)
DEFINE_HOOKEDVAR(bool, g_bAntiAliasing, 0x10979470)
DEFINE_HOOKEDVAR(bool, g_bAssertionThrown, 0x10979460)
DEFINE_HOOKEDVAR(bool, g_bAtEndOfDrama, 0x106988D9)
DEFINE_HOOKEDVAR(bool, g_bAtEndOfLoad, 0x106988D8)
DEFINE_HOOKEDVAR(bool, g_bBlackBackground, 0x101C6BFC)
DEFINE_HOOKEDVAR(bool, g_bCinematographyOn, 0x10200458)
DEFINE_HOOKEDVAR(bool, g_bDoSoundDropOff, 0x1020043C)
DEFINE_HOOKEDVAR(bool, g_bDoingFPSAdjustment, 0x1097948D)
DEFINE_HOOKEDVAR(bool, g_bDoneTension1bMusic, 0x101FF0E9)
DEFINE_HOOKEDVAR(bool, g_bExactPointDecel, 0x10200444)
DEFINE_HOOKEDVAR(bool, g_bExactRotDecel, 0x10200445)
DEFINE_HOOKEDVAR(bool, g_bFacadeHasBeenRunMultipleTimes, 0x10979490)
DEFINE_HOOKEDVAR(bool, g_bIgnoreArmLSetPerf, 0x101FEC20)
DEFINE_HOOKEDVAR(bool, g_bIgnoreArmRSetPerf, 0x101FD598)
DEFINE_HOOKEDVAR(bool, g_bIgnoreGazeSetPerf, 0x101FEC24)
DEFINE_HOOKEDVAR(bool*, g_bIgnoreNextWalkAbort, 0x101F90A8)
DEFINE_HOOKEDVAR(bool, g_bIsAblConnected, 0x10180D90)
DEFINE_HOOKEDVAR(bool, g_bJimDoran, 0x101FF0E8)
DEFINE_HOOKEDVAR(bool, g_bKnowScriptNames, 0x1069A760)
DEFINE_HOOKEDVAR(bool, g_bMuteSounds, 0x1081AB2C)
DEFINE_HOOKEDVAR(bool, g_bNewPlayerTextEntered, 0x1069A168)
DEFINE_HOOKEDVAR(bool, g_bParsingError, 0x1069A659)
DEFINE_HOOKEDARRAYVAR(bool, g_bPhoneCallBehaviourSubparts, 0x103B2094)
DEFINE_HOOKEDVAR(bool, g_bPlayerChoosingName, 0x106988D0)
DEFINE_HOOKEDVAR(bool, g_bPlayerInputActive, 0x101C6F98)
DEFINE_HOOKEDVAR(bool, g_bPlayerMoveBackwards, 0x10698490)
DEFINE_HOOKEDVAR(bool, g_bPlayerMoveDownwards, 0x106984E0)
DEFINE_HOOKEDVAR(bool, g_bPlayerMoveForwards, 0x106984D9)
DEFINE_HOOKEDVAR(bool, g_bPlayerMoveUpwards, 0x10698408)
DEFINE_HOOKEDVAR(bool, g_bPlayerRotateLeft, 0x106984EC)
DEFINE_HOOKEDVAR(bool, g_bPlayerRotateRight, 0x106984D8)
DEFINE_HOOKEDVAR(bool, g_bPrintBetaInfo, 0x10979491)
DEFINE_HOOKEDVAR(bool, g_bQuittingApp, 0x1082D58C)
DEFINE_HOOKEDVAR(bool, g_bRequestAutoFraming, 0x102004A4)
DEFINE_HOOKEDVAR(bool, g_bSeatedOnCouch, 0x1069854C)
DEFINE_HOOKEDVAR(bool, g_bShouldRenderThisFrame, 0x1083F630)
DEFINE_HOOKEDVAR(bool, g_bTextCharWasPressed, 0x101FF0D9)
DEFINE_HOOKEDVAR(bool, g_bTripCarry, 0x10698554)
DEFINE_HOOKEDVAR(bool, g_bVeryNearCharacter, 0x10698555)
DEFINE_HOOKEDVAR(bool, g_bWriteOutput, 0x107D9470)
DEFINE_HOOKEDVAR(bool, g_mouseLButtonClicked, 0x10979468)
DEFINE_HOOKEDVAR(bool, g_bDisplayDialogText, 0x10979471)
#define g_PlayerXController					_g_PlayerXController()
#define g_PlayerYController					_g_PlayerYController()
#define g_PlayerZController					_g_PlayerZController()
#define g_PlayerRotController				_g_PlayerRotController()
#define g_autoMovePlayerPt					_g_autoMovePlayerPt()
#define g_autoMovePlayerRot					_g_autoMovePlayerRot()
#define g_autoTurnController				_g_autoTurnController()
#define g_autoTurnAmount					_g_autoTurnAmount()
#define g_numPanels							_g_numPanels()
#define g_panelModelFrames					_g_panelModelFrames()
#define g_panelModelInfo					_g_panelModelInfo()
#define g_panelModelQuadPolys				_g_panelModelQuadPolys()
#define g_GraceSprite						_g_GraceSprite()
#define g_TripSprite						_g_TripSprite()
#define g_PlayerSprite						_g_PlayerSprite()
#define g_CursorSprite						_g_CursorSprite()
#define g_spriteMgr							_g_spriteMgr()
#define g_soundMgr							_g_soundMgr()
#define g_soundSource						_g_soundSource()
#define g_soundSourceID						_g_soundSourceID()
#define g_baseScreenMessage					_g_baseScreenMessage()
#define g_playerGender						_g_playerGender()
#define g_playerNameID						_g_playerNameID()
#define g_playerGesture						_g_playerGesture()
#define g_playerGestureSpriteID				_g_playerGestureSpriteID()
#define g_hMainAppWindow					_g_hMainAppWindow()
#define g_scriptDialogHWnd					_g_scriptDialogHWnd()
#define g_faceDialogHWnd					_g_faceDialogHWnd()
#define g_hAppInstance						_g_hAppInstance()
#define g_hDeviceContext					_g_hDeviceContext()
#define g_ablRequestBufferMutexHandle		_g_ablRequestBufferMutexHandle()
#define g_numFramesRun						_g_numFramesRun()
#define g_SkipFramesCounter					_g_SkipFramesCounter()
#define g_whenNewPlayerTextEntered			_g_whenNewPlayerTextEntered()
#define g_lastFrameGotAblRequest			_g_lastFrameGotAblRequest()
#define g_launchErrorString					_g_launchErrorString()
#define g_curTextInput						_g_curTextInput()
#define g_curTextCharInput					_g_curTextCharInput()
#define g_playerTransAmt					_g_playerTransAmt()
#define g_playerRotAmt						_g_playerRotAmt()
#define g_AutoMovePlayerStatus				_g_AutoMovePlayerStatus()
#define g_ticksIntroStarted					_g_ticksIntroStarted()
#define g_introEventCtr						_g_introEventCtr()
#define g_textDisplayState					_g_textDisplayState()
#define g_framesWhenLastSentPlayerTypedCue	_g_framesWhenLastSentPlayerTypedCue()
#define g_frameWhenLastCinematographyMove	_g_frameWhenLastCinematographyMove()
#define g_framePlayerLastMoved				_g_framePlayerLastMoved()
#define g_blastFlagSignalToAbl				_g_blastFlagSignalToAbl()
#define g_introChoices						_g_introChoices()
#define g_introTimings						_g_introTimings()
#define g_introBaseTimings					_g_introBaseTimings()
#define g_initialIntroDelay					_g_initialIntroDelay()
#define g_startTitleFadeIn					_g_startTitleFadeIn()
#define g_quitAppCountdown					_g_quitAppCountdown()
#define g_triangleFanSineCache				_g_triangleFanSineCache()
#define g_triangleFanCosineCache			_g_triangleFanCosineCache()
#define g_bAblRequestingFPSAdjustment		_g_bAblRequestingFPSAdjustment()
#define g_bAllCircleOutline					_g_bAllCircleOutline()
#define g_bAllQuadLineOutline				_g_bAllQuadLineOutline()
#define g_bAntiAliasing						_g_bAntiAliasing()
#define g_bAssertionThrown					_g_bAssertionThrown()
#define g_bAtEndOfDrama						_g_bAtEndOfDrama()
#define g_bAtEndOfLoad						_g_bAtEndOfLoad()
#define g_bBlackBackground					_g_bBlackBackground()
#define g_bCinematographyOn					_g_bCinematographyOn()
#define g_bDoSoundDropOff					_g_bDoSoundDropOff()
#define g_bDoingFPSAdjustment				_g_bDoingFPSAdjustment()
#define g_bDoneTension1bMusic				_g_bDoneTension1bMusic()
#define g_bExactPointDecel					_g_bExactPointDecel()
#define g_bExactRotDecel					_g_bExactRotDecel()
#define g_bFacadeHasBeenRunMultipleTimes	_g_bFacadeHasBeenRunMultipleTimes()
#define g_bIgnoreArmLSetPerf				_g_bIgnoreArmLSetPerf()
#define g_bIgnoreArmRSetPerf				_g_bIgnoreArmRSetPerf()
#define g_bIgnoreGazeSetPerf				_g_bIgnoreGazeSetPerf()
#define g_bIgnoreNextWalkAbort				_g_bIgnoreNextWalkAbort()
#define g_bIsAblConnected					_g_bIsAblConnected()
#define g_bJimDoran							_g_bJimDoran()
#define g_bKnowScriptNames					_g_bKnowScriptNames()
#define g_bMuteSounds						_g_bMuteSounds()
#define g_bNewPlayerTextEntered				_g_bNewPlayerTextEntered()
#define g_bParsingError						_g_bParsingError()
#define g_bPhoneCallBehaviourSubparts		_g_bPhoneCallBehaviourSubparts()
#define g_bPlayerChoosingName				_g_bPlayerChoosingName()
#define g_bPlayerInputActive				_g_bPlayerInputActive()
#define g_bPlayerMoveBackwards				_g_bPlayerMoveBackwards()
#define g_bPlayerMoveDownwards				_g_bPlayerMoveDownwards()
#define g_bPlayerMoveForwards				_g_bPlayerMoveForwards()
#define g_bPlayerMoveUpwards				_g_bPlayerMoveUpwards()
#define g_bPlayerRotateLeft					_g_bPlayerRotateLeft()
#define g_bPlayerRotateRight				_g_bPlayerRotateRight()
#define g_bPrintBetaInfo					_g_bPrintBetaInfo()
#define g_bQuittingApp						_g_bQuittingApp()
#define g_bRequestAutoFraming				_g_bRequestAutoFraming()
#define g_bSeatedOnCouch					_g_bSeatedOnCouch()
#define g_bShouldRenderThisFrame			_g_bShouldRenderThisFrame()
#define g_bTextCharWasPressed				_g_bTextCharWasPressed()
#define g_bTripCarry						_g_bTripCarry()
#define g_bVeryNearCharacter				_g_bVeryNearCharacter()
#define g_bWriteOutput						_g_bWriteOutput()
#define g_mouseLButtonClicked				_g_mouseLButtonClicked()
#define g_bDisplayDialogText				_g_bDisplayDialogText()






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


#endif // FACADECOMMON_H
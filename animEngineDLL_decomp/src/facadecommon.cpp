#include "facadecommon.h"

bool g_bAblRequestingFPSAdjustment;

bool g_bAllCircleOutline;
bool g_bAllQuadLineOutline;

bool g_bAntiAliasing;

bool g_bAssertionThrown;

bool g_bAtEndOfDrama;
bool g_bAtEndOfLoad;

bool g_bBlackBackground = true;
bool g_bCinematographyOn;
bool g_bDoSoundDropOff;
bool g_bDoingFPSAdjustment;
bool g_bDoneTension1bMusic;
bool g_bExactPointDecel;
bool g_bExactRotDecel;
bool g_bFacadeHasBeenRunMultipleTimes;

bool g_bIgnoreArmLSetPerf;
bool g_bIgnoreArmRSetPerf;	
bool g_bIgnoreGazeSetPerf;
bool g_bIgnoreNextWalkAbort[2];

bool g_bIsAblConnected;
bool g_bJimDoran;
bool g_bKnowScriptNames;
bool g_bMuteSounds;
bool g_bNewPlayerTextEntered;
bool g_bOnCommandLine;
bool g_bParsingError;
bool g_bPhoneCallBehaviourSubparts[90]; //?

bool g_bPlayerChoosingName;
bool g_bPlayerInputActive = true;
bool g_bPlayerMoveBackwards;
bool g_bPlayerMoveDownwards;
bool g_bPlayerMoveForwards;
bool g_bPlayerMoveUpwards;
bool g_bPlayerRotateLeft;
bool g_bPlayerRotateRight;

bool g_bPrintBetaInfo;
bool g_bQuittingApp;
bool g_bRequestAutoFraming;
bool g_bSeatedOnCouch;
bool g_bShouldRenderThisFrame;
bool g_bTextCharWasPressed;
bool g_bTripCarry;
bool g_bVeryNearCharacter;
bool g_bWriteOutput;

uint32_t g_numFramesRun = 0;
uint32_t g_SkipFramesCounter = 0;
uint32_t g_lastFrameGotAblRequest = 0;

uint32_t g_whenNewPlayerTextEntered = -DATA_NULLVAL;


HWND g_hMainAppWindow;
HWND g_scriptDialogHWnd;
HWND g_faceDialogHWnd;



int g_playerGender;
int g_playerNameID;
EPlayerGestureID g_playerGesture;
ESpriteID g_playerGestureSpriteID;
player_namegender_map_t g_PlayerNameStrings[NUM_PLAYERNAMES] = {
	{	"Adam", 		"m"	},
	{	"Al", 			"m"	},
	{	"Amy", 			"f"	},
	{	"Andrew", 		"m"	},
	{	"Anne", 		"f"	},
	{	"Ashley", 		"f"	},
	{	"Audrey", 		"f"	},
	{	"Ben", 			"m"	},
	{	"Beth", 		"f"	},
	{	"Bob", 			"m"	},
	{	"Brad", 		"m"	},
	{	"Brenda", 		"f"	},
	{	"Brian", 		"m"	},
	{	"Carol", 		"f"	},
	{	"Cathy", 		"f"	},
	{	"Chloe", 		"f"	},
	{	"Chris", 		"f"	},
	{	"Chris", 		"m"	},
	{	"Dan", 			"m"	},
	{	"Dave", 		"m"	},
	{	"Diana", 		"f"	},
	{	"Doug", 		"m"	},
	{	"Ed", 			"m"	},
	{	"Elizabeth", 	"f"	},
	{	"Ellen", 		"f"	},
	{	"Emily", 		"f"	},
	{	"Eric", 		"m"	},
	{	"Erin", 		"f"	},
	{	"Espen", 		"m"	},
	{	"George", 		"m"	},
	{	"Genevra", 		"f"	},
	{	"Gonzalo", 		"m"	},
	{	"Greg", 		"m"	},
	{	"Heather", 		"f"	},
	{	"Ian", 			"m"	},
	{	"Jane", 		"f"	},
	{	"Janet", 		"f"	},
	{	"Jared", 		"m"	},
	{	"Jeff", 		"m"	},
	{	"Jen", 			"f"	},
	{	"Jesper", 		"m"	},
	{	"Jess", 		"f"	},
	{	"Jill", 		"f"	},
	{	"Jim", 			"m"	},
	{	"Jo", 			"f"	},
	{	"Joe", 			"m"	},
	{	"John", 		"m"	},
	{	"Josh", 		"m"	},
	{	"Julie",		"f"	},
	{	"Kate", 		"f"	},
	{	"Keith", 		"m"	},
	{	"Kelly", 		"f"	},
	{	"Kelly", 		"m"	},
	{	"Kevin", 		"m"	},
	{	"Kim", 			"f"	},
	{	"Kha", 			"m"	},
	{	"Lisa", 		"f"	},
	{	"Liz", 			"f"	},
	{	"Mark", 		"m"	},
	{	"Mary", 		"f"	},
	{	"Matt", 		"m"	},
	{	"Mel", 			"f"	},
	{	"Mel", 			"m"	},
	{	"Michael", 		"m"	},
	{	"Nat", 			"f"	},
	{	"Nat", 			"m"	},
	{	"Nick", 		"m"	},
	{	"Nicole", 		"f"	},
	{	"Noah", 		"m"	},
	{	"Olivia", 		"f"	},
	{	"Pat", 			"f"	},
	{	"Pat", 			"m"	},
	{	"Paul", 		"m"	},
	{	"Peter", 		"m"	},
	{	"Rachel", 		"f"	},
	{	"Ray", 			"m"	},
	{	"Rich", 		"m"	},
	{	"Rob", 			"m"	},
	{	"Robin", 		"f"	},
	{	"Ryan", 		"m"	},
	{	"Sam", 			"f"	},
	{	"Sam", 			"m"	},
	{	"Sarah", 		"f"	},
	{	"Scott", 		"m"	},
	{	"Sergio", 		"m"	},
	{	"Stacy", 		"f"	},
	{	"Steph", 		"f"	},
	{	"Tammy", 		"f"	},
	{	"Tania", 		"f"	},
	{	"Ted", 			"m"	},
	{	"Tim", 			"m"	},
	{	"Tom", 			"m"	},
	{	"Walter", 		"m"	},
	{	"Warren", 		"m"	},
	{	"Will", 		"m"	}
};


//wth facade ?
bool writeprintout(const char* print, const char* filename)
{
	DWORD numwrittenbytes;
	HANDLE hfile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE)
		return false;

	if (WriteFile(hfile, print, lstrlenA(print), &numwrittenbytes, 0))
		FlushFileBuffers(hfile);

	CloseHandle(hfile);
	return true;
}
void printf_step2(const char* text)
{
	int tries = 0;
	while (!writeprintout(text, "\\\\.\\mailslot\\WinLog"))
	{
		if (++tries > 0)
		{
			OutputDebugString(text);
			return;
		}
		Sleep(10);
	}
}
void printf_step1(const char* format, ...)
{
	static char s_printbuffer[2048];
	va_list va;
	va_start(va, format);
	vsnprintf(s_printbuffer, sizeof(s_printbuffer) - 1, format, va);
	printf_step2(s_printbuffer);
}
void dprintf(const char* format, ...)
{
	static char s_varbuffer[2048];
	va_list va;
	va_start(va, format);
	vsnprintf(s_varbuffer, sizeof(s_varbuffer) - 1, format, va);
	printf_step1("%s\n", s_varbuffer);
}
void ShouldntBeHere(const char* asserttext)
{
	g_bAssertionThrown = true;

	dprintf("\n#####\nASSERTION: %s\n#####\n", asserttext);
	AddEventInfoToLog(eEventLogType_Miscellaneous, eSpriteID_NOTASPRITE, asserttext, false, eSpriteID_NOTASPRITE, 0);

	g_bAssertionThrown = false;
}
void animengine_assert(bool cond)
{
	g_bAssertionThrown = true;

	if (!cond)
	{
		dprintf("\n#####\nASSERTION FAILED, see callstack for details\n#####\n");
		AddEventInfoToLog(eEventLogType_Miscellaneous, eSpriteID_NOTASPRITE, "Generic AnimEngine assertion failed", false, eSpriteID_NOTASPRITE, 0);
		//StopAllMP3s();
	}

	g_bAssertionThrown = false;
}

int FloatToInt(float f)
{
	int i = f;
	float roundingfloat;
	int roundingint;
	if (f < 0.0)
		roundingfloat = -f;
	else
		roundingfloat = f;
	if (i < 0)
		roundingint = -i;
	else
		roundingint = f;
	float roundfinal =  roundingfloat - roundingint;
	animengine_assert(roundfinal >= 0.0);
	if (roundfinal > 0.5)
	{
		if (f > 0.0)
			++i;
		if (f < 0.0)
			--i;
	}
	return i;
}


bool IsWhitespaceNewLineOrComma(char c){
	return c == ' ' 
		|| c == '\t' 
		|| c == '\n' 
		|| c == ',' 
		|| c == '\r';
}
bool IsNumberCharacter(char c) {
	return c >= '0' && c <= '9';
}
bool IsLetterCharacter(char c) {
	return (c >= 'a' && c <= 'z') 
		|| (c >= 'A' && c <= 'Z');
}
bool IsPunctuationCharacter(char c) {
	return c == '!'
		|| c == '?'
		|| c == '.'
		|| c == ','
		|| c == '"'
		|| c == '\''
		|| c == ';'
		|| c == ':'
		|| c == '('
		|| c == ')'
		|| c == '-';
}

float NormalizeNeg180to180(float ang)
{
	while (ang > 180)	ang -= 180;
	while (ang < -180)	ang += 180;
	return ang;
}
float Normalize0to360(float ang)
{
	while (ang > 360)	ang -= 360;
	while (ang < - 360)	ang += 360;
	return ang;
}

int rand2(int min, int max)
{
	if (max - min <= 0) return min;
	
	return min + rand() % (max - min + 1);
}
bool rand100(int val)
{
	animengine_assert(val <= 100);
	return rand() % 100 < val;
}

void ClipFloatToRange(float& value, float min, float max)
{
	value = fminf(fmaxf(value, min), max);
}

float GetSecsSinceTicks(int ticks)
{
	return (GetTickCount() - ticks) / 1000.f;
}

float GetXYDistanceBetweenPoints(XYZPointf p1, XYZPointf p2)
{
	float xdist = p1.m_x - p2.m_y;
	float ydist = p1.m_y - p2.m_y;
	float sqrdist = xdist * xdist + ydist * ydist;
	animengine_assert(sqrdist >= 0);
	return sqrt(sqrdist);
}

float GetXZDistanceBetweenPoints(XYZPointf p1, XYZPointf p2)
{
	float xdist = p1.m_x - p2.m_y;
	float zdist = p1.m_z - p2.m_z;
	float sqrdist = xdist * xdist + zdist * zdist;
	animengine_assert(sqrdist >= 0);
	return sqrt(sqrdist);
}

float GetXZAngleBetweenPoints(XYZPointf p1, XYZPointf p2)
{
	return NormalizeNeg180to180(atan2(p2.m_z - p1.m_z, p2.m_x - p1.m_x) * (180.0/MATH_PI) + 90.0);
}

void GetSubjectivePosition(XYZPointf pos, XYZPointf& out, float& floatout, bool option){
	out = pos - g_PlayerSprite->GetPosition();

	float v1 = atan2(out.m_z, out.m_x);
	float v2 = sqrt(out.m_z * out.m_z + out.m_x * out.m_x);
	float v3 = g_PlayerSprite->GetRotation().m_y * MATH_PI / 180.f;

	out.m_x = cos(v1 - v3) * v2;
	out.m_z = sin(v1 - v3) * v2;

	if (option)
	{
		float v11 = -out.m_z;
		if (v11 < 0.1)	v11 = 0.1;

		floatout = 100.0 / v11;
		out.m_x *= floatout * 1.25;
		out.m_y *= floatout * 1.25;
	}
}
XYZPointf GetObjectivePosition(XYZPointf pos, float factor) {
	pos.m_x /= (factor * 1.25);
	pos.m_y /= (factor * 1.25);

	float v1 = atan2(pos.m_z, pos.m_x);
	float v2 = sqrt(pos.m_z * pos.m_z + pos.m_x * pos.m_x);
	float v3 = g_PlayerSprite->GetRotation().m_y * (MATH_PI / 180);

	pos.m_x = cos(v1 + v3) * v2;
	pos.m_z = sin(v1 + v3) * v2;

	return pos + g_PlayerSprite->GetPosition();
}

static float bbox_max(float a1, float a2, float a3, float a4)
{
	float v1; // [esp+50h] [ebp-Ch]
	float v2; // [esp+54h] [ebp-8h]

	if (a1 <= a2)
		v2 = a2;
	else
		v2 = a1;

	if (v2 <= a3)
		v1 = a3;
	else
		v1 = v2;

	if (v1 <= a4)
		return a4;
	else
		return v1;
}
static float bbox_min(float a1, float a2, float a3, float a4)
{
	float v1; // [esp+50h] [ebp-Ch]
	float v2; // [esp+54h] [ebp-8h]

	if (a1 >= a2)
		v2 = a2;
	else
		v2 = a1;

	if (v2 >= a3)
		v1 = a3;
	else
		v1 = v2;

	if (v1 >= a4)
		return a4;
	else
		return v1;
}

void GetXYBoundingBoxOf4Points(const XYZPointf* a2, XYZPointf* a3)
{
	float x, y, z;

	z = bbox_max(a2[0].m_z, a2[1].m_z, a2[2].m_z, a2[3].m_z);
	y = bbox_min(a2[0].m_y, a2[1].m_y, a2[2].m_y, a2[3].m_y);
	x = bbox_min(a2[0].m_x, a2[1].m_x, a2[2].m_x, a2[3].m_x);
	a3[0] = XYZPointf(x, y, z);

	z = bbox_max(a2[0].m_z, a2[1].m_z, a2[2].m_z, a2[3].m_z);
	y = bbox_min(a2[0].m_y, a2[1].m_y, a2[2].m_y, a2[3].m_y);
	x = bbox_max(a2[0].m_x, a2[1].m_x, a2[2].m_x, a2[3].m_x);
	a3[1] = XYZPointf(x, y, z);

	z = bbox_max(a2[0].m_z, a2[1].m_z, a2[2].m_z, a2[3].m_z);
	y = bbox_max(a2[0].m_y, a2[1].m_y, a2[2].m_y, a2[3].m_y);
	x = bbox_max(a2[0].m_x, a2[1].m_x, a2[2].m_x, a2[3].m_x);
	a3[2] = XYZPointf(x, y, z);

	z = bbox_max(a2[0].m_z, a2[1].m_z, a2[2].m_z, a2[3].m_z);
	y = bbox_max(a2[0].m_y, a2[1].m_y, a2[2].m_y, a2[3].m_y);
	x = bbox_min(a2[0].m_x, a2[1].m_x, a2[2].m_x, a2[3].m_x);
	a3[3] = XYZPointf(x, y, z);
}

static bool point_inrect_check(XZPointf p, XZPointf rectmin, XZPointf rectmax)
{
	if (rectmin.m_x > rectmax.m_x)
	{
		VAL_SWAP(float, rectmin.m_x, rectmax.m_x);
	}
	if (rectmin.m_z > rectmax.m_z)
	{
		VAL_SWAP(float, rectmin.m_z, rectmax.m_z);
	}

	return rectmin.m_x <= p.m_x && rectmax.m_x >= p.m_x 
		&& rectmin.m_z <= p.m_z && rectmax.m_z >= p.m_z;
}

bool DoXZRectsIntersect(XZRectf rect1, XZRectf rect2)
{
	return point_inrect_check({rect2.m_xmin, rect2.m_zmin}, {rect1.m_xmin, rect1.m_zmin}, {rect1.m_xmax, rect1.m_zmax})
		|| point_inrect_check({rect2.m_xmin, rect2.m_zmax}, {rect1.m_xmin, rect1.m_zmin}, {rect1.m_xmax, rect1.m_zmax})
		|| point_inrect_check({rect2.m_xmax, rect2.m_zmin}, {rect1.m_xmin, rect1.m_zmin}, {rect1.m_xmax, rect1.m_zmax})
		|| point_inrect_check({rect2.m_xmax, rect2.m_zmax}, {rect1.m_xmin, rect1.m_zmin}, {rect1.m_xmax, rect1.m_zmax})
		|| point_inrect_check({rect1.m_xmin, rect1.m_zmin}, {rect2.m_xmin, rect2.m_zmin}, {rect2.m_xmax, rect2.m_zmax})
		|| point_inrect_check({rect1.m_xmin, rect1.m_zmax}, {rect2.m_xmin, rect2.m_zmin}, {rect2.m_xmax, rect2.m_zmax})
		|| point_inrect_check({rect1.m_xmax, rect1.m_zmin}, {rect2.m_xmin, rect2.m_zmin}, {rect2.m_xmax, rect2.m_zmax})
		|| point_inrect_check({rect1.m_xmax, rect1.m_zmax}, {rect2.m_xmin, rect2.m_zmin}, {rect2.m_xmax, rect2.m_zmax});
}

void GetGridXZFromWorldXZ(float worldx, float worldz,
	float& outgridx, float& outgridz)
{
	outgridx = worldx / 15.f + 45.f;
	outgridz = worldz / 15.f + 45.f;
}

bool IsPointVisibleToPlayer(XYZPointf point, bool addoffset)
{
	XYZPointf playerpos = g_PlayerSprite->GetPosition();
	XYZPointf playerrot = g_PlayerSprite->GetRotation();

	float factor = NormalizeNeg180to180(GetXZAngleBetweenPoints(playerpos, point) - playerrot.m_y);
	if (factor < 0.0)
		factor = -factor;

	if (addoffset)
		return 35 > factor;
	else
		return 20 + 35 > factor;
}

float ConvertWorldDegreesToRadians(float ang)
{
	return Normalize0to360(ang - 90.0) * (MATH_PI/180.f);
}

XYZPointf PointAtAngleAndDist(XYZPointf dist, float f1, float f2)
{
	float radangle = ConvertWorldDegreesToRadians(f1);
	dist.m_x = cos(radangle) * f2 + dist.m_x;
	dist.m_z = sin(radangle) * f2 + dist.m_z;

	return dist;
}


float FrameRateAdjust(float val, bool multiply)
{
	if (multiply)
		return val * 2;
	else
		return val / 2;
}
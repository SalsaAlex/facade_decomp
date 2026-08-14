#include "facadecommon.h"

bool g_bAblRequestingFPSAdjustment;

bool g_bAllCircleOutline;
bool g_bAllQuadLineOutline;

bool g_bAntiAliasing;

bool g_bAssertionThrown;

bool g_bAtEndOfDrama;
bool g_bAtEndOfLoad;

bool g_bBlackBackground;
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
bool g_bIgnoreNextWalkAbort;

bool g_bIsAblConnected;
bool g_bJimDoran;
bool g_bKnowScriptNames;
bool g_bMuteSounds;
bool g_bNewPlayerTextEntered;
bool g_bOnCommandLine;
bool g_bParsingError;
bool g_bPhoneCallBehaviourSubparts[90]; //?

bool g_bPlayerChoosingName;
bool g_bPlayerInputActive;
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


HWND g_hMainAppWindow;



int g_playerGender;
int g_playerNameID;
EPlayerGestureID g_playerGesture;
ESpriteID g_playerGestureSpriteID;
extern player_namegender_map_t g_PlayerNameStrings[95] = {
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
	vsnprintf(s_printbuffer, sizeof(s_printbuffer) - 1, format, va);
	printf_step2(s_printbuffer);
}
void dprintf(const char* format, ...)
{
	static char s_varbuffer[2048];
	va_list va;
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
		StopAllMP3s();
	}

	g_bAssertionThrown = false;
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
}
float Normalize0to360(float ang)
{
	while (ang > 360)	ang -= 360;
	while (ang < - 360)	ang += 360;
}

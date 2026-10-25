#include "facadecommon.h"
#include "glfuncs.h"
#include "zone.h"

bool g_bOnCommandLine;


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
	_set_g_bAssertionThrown(true);

	dprintf("\n#####\nASSERTION: %s\n#####\n", asserttext);
	AddEventInfoToLog(eEventLogType_Miscellaneous, eSpriteID_NOTASPRITE, asserttext, false, eSpriteID_NOTASPRITE, 0);

	_set_g_bAssertionThrown(false);
}
void animengine_assert(bool cond)
{
	_set_g_bAssertionThrown(true);

	if (!cond)
	{
		dprintf("\n#####\nASSERTION FAILED, see callstack for details\n#####\n");
		AddEventInfoToLog(eEventLogType_Miscellaneous, eSpriteID_NOTASPRITE, "Generic AnimEngine assertion failed", false, eSpriteID_NOTASPRITE, 0);
		//StopAllMP3s();
	}

	_set_g_bAssertionThrown(false);
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

bool PlayGlobalSound(EGlobalSounds sound)
{
	return g_soundMgr->PlaySound(g_soundSourceID, sound, true);
}

bool CollisionTest(int zoneindex, XZRectf rect, Sprite* spr1, Sprite* spr2)
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

bool CollisionTestWithSprite(Sprite* collider1, XYZPointf point, int expandsize, Sprite* collider2) {
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
		if (CollisionTest(i, rect, collider1, collider2))
			return true;
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
		if ((option
			|| !zonespr
			|| !zonespr->GetIsCharacterSprite())
			&& CollisionTest(i, rect, spr, zonespr))
		{
			return true;
		}
	}
	return false;
}

XZRectf ComputeSingleCollisionRectForSprite(Sprite* spr) {
	XZRectf rect;

	XYZPointf spritepos = spr->GetPosition();
	if (spr->GetCollidableType() == eCollidableType_SimpleBox)
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

		if (spr->GetCollidableType() == eCollidableType_Panel)
		{
			float rectsizex = fabs(rect.m_xmin - rect.m_xmax);
			float rectsizez = fabs(rect.m_zmin - rect.m_zmax);

			if (rectsizex <= 0.001)
			{
				rect.m_xmin -= 10;
				rect.m_xmax += 10;
			}
			else if (rectsizez <= 0.001)
			{
				rect.m_zmin -= 10;
				rect.m_zmax += 10;
			}
		}
	}

	return rect;
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
	float xdist = p1.m_x - p2.m_x;
	float ydist = p1.m_y - p2.m_y;
	float sqrdist = xdist * xdist + ydist * ydist;
	animengine_assert(sqrdist >= 0);
	return sqrt(sqrdist);
}

float GetXZDistanceBetweenPoints(XYZPointf p1, XYZPointf p2)
{
	float xdist = p1.m_x - p2.m_x;
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

	float factor = fabs( NormalizeNeg180to180(GetXZAngleBetweenPoints(playerpos, point) - playerrot.m_y) );

	if (addoffset)
		return 35 > factor;
	else
		return 20 + 35 > factor;
}
bool IsPlayerOutsideApt() {
	return g_PlayerSprite->GetPosition().m_x < -150
		&& g_PlayerSprite->GetPosition().m_y > 120;
}
void PlayerEnteredText()
{
	static int g_waitingToRedisplayCtr;
	strcpy(g_PlayerSprite->m_playerTextInput, g_PlayerSprite->m_ScreenText);
	_set_g_bNewPlayerTextEntered(true);
	_set_g_whenNewPlayerTextEntered(g_numFramesRun);
	g_PlayerSprite->SetScreenText(g_PlayerSprite->m_ScreenText, true);
	_set_g_textDisplayState(1);
	g_waitingToRedisplayCtr = 0;
	AddEventInfoToLog(eEventLogType_Speech, eSpriteID_player, g_PlayerSprite->m_ScreenText, 0, eSpriteID_NOTASPRITE, 0);
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
#include "facadecommon.h"
#include "glfuncs.h"
#include "classes/AblRequestBuffer.h"

#include "classes/Sprite/Sprite.h"
#include "classes/Sprite/DoorSprite.h"
#include "classes/Sprite/DrinkSprite.h"
#include "classes/Sprite/AnsweringMachineSprite.h"
#include "classes/Sprite/PhoneSprite.h"

static int g_whichSoundtrack = -1;
static EMusicName g_curMusicType;
static bool g_bUnused;

static MP3Manager g_MP3Manager_0;
static MP3Manager g_MP3Manager_1;

static bool g_bIgnoreNextWalkAbort[2] = { false, false };
static bool g_bIgnoreArmRSetPerf[2];
static bool g_bIgnoreArmLSetPerf[2];
static bool g_bIgnoreGazeSetPerf[2];

extern void GetAccessToRequestBuffer(const char* name);
extern void AblBufferPostSemaphore();

extern void PlayerEnteredText();

static void FakePlayerEnteredText(const char* text)
{
	strcpy(g_PlayerSprite->m_playerTextInput, text);
	g_bNewPlayerTextEntered = true;
	g_whenNewPlayerTextEntered = g_numFramesRun;
}

bool IsAblConnected(){
	return g_bIsAblConnected;
}
static void SetAblConnected(bool option){
	g_bIsAblConnected = option;
}
static void StartTitleSequence()
{
	g_introChoices[0] = 0;
	g_introChoices[1] = rand2(0, 4);
	g_introChoices[2] = rand2(0, 4);
	g_introChoices[3] = rand2(0, 3);
	g_introChoices[4] = rand2(0, 3);
	g_introChoices[5] = rand2(0, 5);
	g_introChoices[6] = rand2(0, 5);
	g_introChoices[7] = 0;

	for (int i = 0; i < 8; ++i)
	{
		g_introTimings[i + 1] = g_introBaseTimings[i][g_introChoices[i]] + 1.0;
		if (i == 0)
			g_introTimings[1] += g_initialIntroDelay;
		if (i == 3)
			g_introTimings[4] = g_introTimings[4] + 1.0;
	}
	for (int i = 1; i < 8; ++i)
		g_introTimings[i + 1] += g_introTimings[i];
	g_startTitleFadeIn = g_introTimings[8] + 2.0;
	g_ticksIntroStarted = GetTickCount();
	g_introEventCtr = 0;
}

extern void StartPlayerMoveControllers(XYZPointf point, float f1, float f2, bool option); //main.cpp
static void StartTripCarryingPlayer()
{
	//undone
	XYZPointf playerpos = g_PlayerSprite->GetPosition();
	XYZPointf playerrot = g_PlayerSprite->GetRotation();
	playerpos.m_y = 94.8;

	g_bSeatedOnCouch = false;

	if (g_TripSprite->GetLocomoting())
		g_TripSprite->StopLocomotion(true);

	XYZPointf point1 = PointAtAngleAndDist(playerpos, playerrot.m_y + 180, 50);
	point1.m_y = g_TripSprite->GetPosition().m_y;

	g_TripSprite->SetPosition(point1);
	g_TripSprite->SetRotation(playerrot);

	XYZPointf point2 = PointAtAngleAndDist(point1, playerrot.m_y, 40.0);
	point2.m_y = playerpos.m_y;

	StartPlayerMoveControllers(point2, playerrot.m_y + 160, 7.0, 0);
	g_AutoMovePlayerStatus = 2;
	g_tripCarryDist = 0.0;
}
static void EndTripCarryingPlayer()
{
	//undone
	XYZPointf trippos = g_TripSprite->GetPosition();
	float tripyaw = g_TripSprite->GetRotation().m_y;

	XYZPointf point = PointAtAngleAndDist(trippos, tripyaw, 120.0);
	point.m_y = g_PlayerSprite->GetPosition().m_y;

	StartPlayerMoveControllers(point, randFloat2(-10.0, 10.0, 1000.0) + tripyaw, 7.0, 0);
	g_AutoMovePlayerStatus = 3;
	g_bTripCarry = false;
}

static void StartMP3(const char* filepath, int vol, int fadevol)
{
	MP3Manager* pChosenManager;
	dprintf("Starting %s\n", filepath);
	if (g_pMP3Manager_0)
	{
		if (g_pMP3Manager_1)
		{
			g_pMP3Manager_0->StopAllMP3();
			pChosenManager = g_pMP3Manager_0;
			g_pMP3Manager_0 = g_pMP3Manager_1;
			g_pMP3Manager_1 = pChosenManager;
		}
		else
		{
			g_pMP3Manager_1 = &g_MP3Manager_0;
			pChosenManager = &g_MP3Manager_0;
		}
	}
	else
	{
		g_pMP3Manager_0 = &g_MP3Manager_1;
		pChosenManager = &g_MP3Manager_1;
	}
	pChosenManager->PlayFromMP3File(filepath);
	if (fadevol <= 0)
	{
		pChosenManager->SetVolume(vol);
	}
	else
	{
		pChosenManager->SetVolume(0.0);
		pChosenManager->m_fFade_TargetVol = vol;
		pChosenManager->m_fFade_Factor = pChosenManager->m_fFade_TargetVol / (float)fadevol;
		if (pChosenManager != g_pMP3Manager_0)
		{
			g_pMP3Manager_0->m_fFade_TargetVol = 0.0;
			g_pMP3Manager_0->m_fFade_Factor = -g_pMP3Manager_0->m_fVolume / (float)fadevol;
		}
	}
}
static void FadeOutCurrentMP3(int time)
{
	if (g_pMP3Manager_0)
	{
		g_pMP3Manager_0->m_fFade_TargetVol = 0.0;
		g_pMP3Manager_0->m_fFade_Factor = -g_pMP3Manager_0->m_fVolume / (double)time;
		dprintf("Fading out current mp3\n");
	}
	else
	{
		dprintf("No mp3 to fade out\n");
	}
}


bool PerformCallbackWhenFullExpressionMoodDone(ESpriteID actor, AblRequest* request)
{
	animengine_assert(actor <= eSpriteID_LAST_CHARACTER);

	CharacterSprite* dramaactor = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(actor);
	if (dramaactor->AreWeDoingFullExpression())
		return false;

	extern bool (*AblCallbackAPI_FullExpressionMoodFinished)(ESpriteID, bool); //AblRequestBuffer.cpp

	AblBufferPostSemaphore();
	int result = AblCallbackAPI_FullExpressionMoodFinished(actor, true);
	GetAccessToRequestBuffer("PerformAllRequests PerformCallbackWhenFullExpressionMoodDone");
	g_ablrequestbuffer->SaveRequestInTrace(request);
	if (result == 0)
		dprintf("ERROR: ASSERTFAIL_ABL_CALLBACK_CONFUSION");

	return true;
}
bool PerformCallbackWhenFullExpressionBaseDone(ESpriteID actor, AblRequest* request)
{
	animengine_assert(actor <= eSpriteID_LAST_CHARACTER);

	CharacterSprite* dramaactor = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(actor);
	if (dramaactor->AreWeDoingFullExpression())
		return false;

	extern bool (*AblCallbackAPI_FullExpressionBaseFinished)(ESpriteID, bool); //AblRequestBuffer.cpp

	AblBufferPostSemaphore();
	int result = AblCallbackAPI_FullExpressionBaseFinished(actor, true);
	GetAccessToRequestBuffer("PerformAllRequests PerformCallbackWhenFullExpressionBaseDone");
	g_ablrequestbuffer->SaveRequestInTrace(request);
	if (result == 0)
		dprintf("ERROR: ASSERTFAIL_ABL_CALLBACK_CONFUSION");

	return true;
}
bool PerformCallbackWhenGestureDone(ESpriteID actor, EGestureBodyPart bodypartindex, AblRequest* request)
{
	animengine_assert(bodypartindex < eNumGestureBodyPartIndexes);
	animengine_assert(actor <= eSpriteID_LAST_CHARACTER);

	CharacterSprite* character = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(actor);
	//	undone

	return true;
}
bool PerformCallbackWhenAnimationDone(ESpriteID actor, EAnimLayer layer, AblRequest* request)
{
	//animengine_assert(layer < eNumAnimLayers);
	//animengine_assert(actor <= eSpriteID_LAST_CHARACTER);
	//
	//CharacterSprite* character = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(actor);
	//
	//if (layer == eAnimLayer_base && !character->m_animMgr.AreBaseScriptsAndFramesDone())
	//	return false;
	//
	//if (layer < eAnimLayer_base || character->m_animMgr.IsLayerDone(layer))
	//	return false;
	//
	//if (layer == eAnimLayer_dialog)
	//{
	//	if (character->IsDialogQueued() || character->AreWeSpeaking())
	//		return false;
	//}
	//AblBufferPostSemaphore();
	//bool scriptfinished = AblCallbackAPI_ScriptFinished(actor, layer);
	//GetAccessToRequestBuffer("PerformAllRequests PerformCallbackWhenAnimationDone");
	//g_ablrequestbuffer->SaveRequestInTrace(request);
	//if (!scriptfinished)
	//	dprintf("ERROR: ASSERTFAIL_ABL_CALLBACK_CONFUSION");
	//
	return true;

}
bool PerformCallbackWhenWalkIsDone(uint32_t unused, ESpriteID actor, bool b1, AblRequest* request)
{
	//	undone

	return true;
}


void PerformGetMouthShape(ESpriteID spriteid, int& mouthshape)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* spr = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	
	mouthshape = spr->m_pHeadSprite->GetMouthShape();
}
void PerformSetMouthShape(ESpriteID spriteid, int mouthshape)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* spr = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	
	spr->m_pHeadSprite->SetMouthShape(mouthshape, -1);
}

void PerformGetEyelidHeightR(ESpriteID spriteid, float& height)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* spr = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	
	height = spr->m_eyelidHeightR.GetVal();
}
void PerformGetEyelidHeightL(ESpriteID spriteid, float& height)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* spr = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	
	height = spr->m_eyelidHeightL.GetVal();
}
void PerformSetEyelidHeightR(ESpriteID spriteid, float height, float unused)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* spr = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	
	animengine_assert(height >= 0 && height <= 1);
	spr->SetEyelidHeight(height, true, true, true);
}
void PerformSetEyelidHeightL(ESpriteID spriteid, float height, float unused)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* spr = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	
	animengine_assert(height >= 0 && height <= 1);
	spr->SetEyelidHeight(height, true, true, true);
}

void PerformGetBrowShapeR(ESpriteID spriteid, int& browshape)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* spr = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	
	browshape = spr->m_pHeadSprite->GetBrowR();
}
void PerformGetBrowShapeL(ESpriteID spriteid, int& browshape)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* spr = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	
	browshape = spr->m_pHeadSprite->GetBrowL();
}
void PerformSetBrowShapeR(ESpriteID spriteid, int browshape)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* spr = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	
	spr->m_pHeadSprite->SetBrowR(browshape, -1);
}
void PerformSetBrowShapeL(ESpriteID spriteid, int browshape)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* spr = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	
	spr->m_pHeadSprite->SetBrowL(browshape, -1);
}

void PerformGetEyesShape(ESpriteID spriteid, int& eyeshape)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* spr = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	
	eyeshape = spr->m_pHeadSprite->GetEyesShape();
}
void PerformSetEyesShape(ESpriteID spriteid, int eyeshape)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* spr = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	
	spr->m_pHeadSprite->SetEyesShape(eyeshape, -1);
}

void PerformGetObjectPosition(ESpriteID spriteid, float& x, float& y, float& z)
{
	animengine_assert(spriteid <= eSpriteID_LAST_OBJECT);
	Sprite* spr = g_spriteMgr.GetSpritePtrByID(spriteid);
	XYZPointf pos = spr->GetPosition();
	x = pos.m_x;
	y = pos.m_y;
	z = pos.m_z;
}
void PerformSetObjectPosition(ESpriteID spriteid, float x, float y, float z)
{
	animengine_assert(spriteid <= eSpriteID_LAST_OBJECT);
	Sprite* spr = g_spriteMgr.GetSpritePtrByID(spriteid);
	spr->SetPosition(XYZPointf(x, y, z));
}

void PerformGetObjectRotation(ESpriteID spriteid, float& x, float& y, float& z)
{
	animengine_assert(spriteid <= eSpriteID_LAST_OBJECT);
	Sprite* spr = g_spriteMgr.GetSpritePtrByID(spriteid);
	XYZPointf rot = spr->GetRotation();
	x = rot.m_x;
	y = rot.m_y;
	z = rot.m_z;
}
void PerformSetObjectRotation(ESpriteID spriteid, float x, float y, float z)
{
	animengine_assert(spriteid <= eSpriteID_LAST_OBJECT);
	Sprite* spr = g_spriteMgr.GetSpritePtrByID(spriteid);
	if (spr->m_yawController.GetIsMoving())
		spr->m_yawController.StopMoving();
	spr->SetRotation(XYZPointf(x, y, z));
}

void PerformGetGazeTracking(ESpriteID gazetracker, ESpriteID& gazetracked)
{
	animengine_assert(gazetracker <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* character = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(gazetracker);
	Sprite* spr = character->GetGazeTracking();
	gazetracked = g_spriteMgr.GetSpriteIDBySpritePtr(spr);
}
void PerformSetGazeTracking(ESpriteID gazetracker, ESpriteID gazetracked, float x, float y, float z, EGazeSpeed gazespeed, EGazeType gazetype, EAlterHeadCock headcock, bool b1)
{
	animengine_assert(gazetracker <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* character = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(gazetracker);
	
	int entry = gazetracker != eSpriteID_grace;
	if (!b1)
		g_bIgnoreGazeSetPerf[entry] = true;

	if (g_bIgnoreGazeSetPerf[entry] && b1)
		return;

	if (gazetracked == eSpriteID_NOTASPRITE)
	{
		character->SetGazeTrackingPoint(XYZPointf(x, y, z), gazespeed, gazetype, headcock);
	}
	else
	{
		animengine_assert(gazetracked <= eSpriteID_NUMOBJECTS);

		Sprite* spr = nullptr;
		if (gazetracked != gazetracker)
			spr = g_spriteMgr.GetSpritePtrByID(gazetracked);
		character->SetGazeTrackingSprite(spr, gazespeed, gazetype, headcock);
	}
}

void PerformSetObjectToHold(ESpriteID holder, int i1, ESpriteID victim)
{
	animengine_assert(holder <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* character = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(holder);
	animengine_assert(victim <= eSpriteID_LAST_OBJECT);
	Sprite* object = g_spriteMgr.GetSpritePtrByID(victim);
	if (object)
		character->SetSpriteToHold(object, i1, -DATA_NULLVAL);
}

void PerformGetAllObjectPickupRotations(ESpriteID spriteid, ObjSensePacket* sensepacket)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	Sprite* actor = g_spriteMgr.GetSpritePtrByID(spriteid);
	for (int i = eSpriteID_FIRST_OBJECT; i <= eSpriteID_elevatorButton; ++i)
	{
		XZPointf xz;
		float rot;

		Sprite* obj = g_spriteMgr.GetSpritePtrByID((ESpriteID)i);
		if (i < eSpriteID_LAST_AGENT || i > eSpriteID_luxTable)
			obj->GetPickupPositionAndRotation(xz, rot);
		else
			obj->GetSurfaceDropPositionAndRotation(0, xz, rot);

		sensepacket[i].objectID = (ESpriteID)i;
		sensepacket[i].x = 0;
		sensepacket[i].y = rot;
		sensepacket[i].z = 0;
	}
}
void PerformGetAllObjectPickupPositions(ESpriteID spriteid, ObjSensePacket* sensepacket)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	Sprite* actor = g_spriteMgr.GetSpritePtrByID(spriteid);
	for (int i = eSpriteID_FIRST_OBJECT; i <= eSpriteID_elevatorButton; ++i)
	{
		XZPointf xz;
		float rot;

		Sprite* obj = g_spriteMgr.GetSpritePtrByID((ESpriteID)i);
		if (i < eSpriteID_LAST_AGENT || i > eSpriteID_luxTable)
			obj->GetPickupPositionAndRotation(xz, rot);
		else
			obj->GetSurfaceDropPositionAndRotation(0,xz, rot);

		//sanity check
		animengine_assert(xz.m_x > -10000 && xz.m_x < 10000 
			&& xz.m_z > -10000 && xz.m_z < 10000);

		sensepacket[i].objectID = (ESpriteID)i;
		sensepacket[i].x = xz.m_x;
		sensepacket[i].y = actor->GetPosition().m_y;
		sensepacket[i].z = xz.m_z;
	}
}
void PerformGetAllObjectRotations(ObjSensePacket* sensepacket)
{
	for (int i = eSpriteID_FIRST_OBJECT; i <= eSpriteID_LAST_OBJECT; ++i)
	{
		Sprite* spr = g_spriteMgr.GetSpritePtrByID((ESpriteID)i);
		XYZPointf rot = spr->GetRotation();
		sensepacket[i].objectID = (ESpriteID)i;
		sensepacket[i].x = rot.m_x;
		sensepacket[i].y = rot.m_y;
		sensepacket[i].z = rot.m_z;
	}
}
void PerformGetAllObjectPositions(ObjSensePacket* sensepacket)
{
	if (!IsAblConnected())
		SetAblConnected(true);
	for (int i = eSpriteID_FIRST_OBJECT; i <= eSpriteID_LAST_OBJECT; ++i)
	{
		Sprite* spr = g_spriteMgr.GetSpritePtrByID((ESpriteID)i);
		XYZPointf pos = spr->GetPosition();
		sensepacket[i].objectID = (ESpriteID)i;
		sensepacket[i].x = pos.m_x;
		sensepacket[i].y = pos.m_y;
		sensepacket[i].z = pos.m_z;
	}
}
void PerformGetAllObjectStates(ObjStateSensePacket* statesensepacket)
{
	for (int i = eSpriteID_FIRST_OBJECT; i <= eSpriteID_LAST_OBJECT; ++i)
	{
		Sprite* spr = g_spriteMgr.GetSpritePtrByID((ESpriteID)i);
		statesensepacket[i].objectID = (ESpriteID)i;
		statesensepacket[i].state = spr->m_iDrinkLevel;
	}
}
void PerformGetAllObjectStagingRotations(ObjSensePacket* sensepacket)
{
	//	undone
}
void PerformGetAllHeldObjects(HeldObjectSensePacket* heldpacket)
{
	for (int i = 0; i < eSpriteID_NUMOBJECTS+1; ++i)
	{
		heldpacket[i].objectID = eSpriteID_NOTASPRITE;
		heldpacket[i].characterID = eSpriteID_NOTASPRITE;
	}

	int packet_iter = 0;

	for (int i = eSpriteID_FIRST_CHARACTER; i <= eSpriteID_LAST_CHARACTER; ++i)
	{
		Sprite* spr;
		if (i == eSpriteID_player)
			spr = g_spriteMgr.GetSpritePtrByID(eSpriteID_elevatorButton);
		else
			spr = g_spriteMgr.GetSpritePtrByID((ESpriteID)i);

		int numheldsprites = spr->GetNumHeldSprites();
		Sprite** heldspritelist = spr->GetHeldSpritesList();
		for (int j = 0; j < numheldsprites; ++j)
		{
			heldpacket[packet_iter].characterID = (ESpriteID)i;
			heldpacket[packet_iter].objectID = g_spriteMgr.GetSpriteIDBySpritePtr(heldspritelist[j]);
			if (i == eSpriteID_player)
				heldpacket[packet_iter].attachPointID = 0;
			else
				heldpacket[packet_iter].attachPointID = spr->GetNthHeldSpriteAttachModelPoint(j);

			heldpacket[packet_iter].objectCategory = heldspritelist[j]->m_eObjectType;
			packet_iter++;
			if (packet_iter >= 49)
				return;
		}
	}
}

void PerformGetArmGestureBases(ESpriteID actor, int& i1out, int& i2out, int& i3out)
{
	//	undone
}

void PerformGetEyesMove(ESpriteID spriteid, float& x, float& y)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);
	((CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid))->GetEyesMove(x, y);
}

void PerformGetObjectPickupPosition(ESpriteID obj, ESpriteID holder, float& x, float& y, float& z)
{
	animengine_assert(obj >= eSpriteID_ashtray && obj <= eSpriteID_trinket8);
	animengine_assert(holder <= eSpriteID_LAST_CHARACTER);

	XZPointf xz;
	float rot;

	g_spriteMgr.GetSpritePtrByID(obj)->GetPickupPositionAndRotation(xz, rot);

	x = xz.m_x;
	y = g_spriteMgr.GetSpritePtrByID(holder)->GetPosition().m_y;
	z = xz.m_z;
}

void PerformSetPerformanceInfo(ESpriteID spriteid, perfinfo_animapi_input* perfinfo)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);

	((CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid))->SetPerformanceInfo(perfinfo);

	int entry = spriteid != eSpriteID_grace;
	g_bIgnoreArmRSetPerf[entry] = false;
	g_bIgnoreArmLSetPerf[entry] = false;
	g_bIgnoreGazeSetPerf[entry] = false;
}
void PerformSetAnimEngineInfo(ESetAnimEngineInfoType type, int info)
{
#define MAKEDRINK(spriteid, drinktype) \
	animengine_assert(drinktype > eDrink_NOTADRINK && drinktype <= eDrink_lastDrink); \
	((DrinkSprite*)g_spriteMgr.GetSpritePtrByID(spriteid))->SetDrinkType((EDrink)drinktype)


	PhoneSprite* phone;
	AnsweringMachineSprite* answeringmachine;
	CharacterSprite* character;

	dprintf("setanimengineinfo %d", (int)type);


	switch (type)
	{
		case eSetAnimEngineInfo_setPlayerGenderID:		g_playerGender = info;						break;
		case eSetAnimEngineInfo_setPlayerNameID:		g_playerNameID = info;						break;
		case eSetAnimEngineInfo_startTitleSequence:		StartTitleSequence();						break;
		case eSetAnimEngineInfo_choosePlayerName:
			g_bPlayerChoosingName = true;
			g_curTextCharInput = 0;
			break;
		case eSetAnimEngineInfo_setPlayerInputActive:	g_bPlayerInputActive = info;				break;
		case eSetAnimEngineInfo_startFade:				StartFade(info * 0.01);						break;
		case eSetAnimEngineInfo_ignoreNextWalkAbort:
			if (info && info == eSpriteID_trip)
				g_bUnused = true;
			else
				g_bIgnoreNextWalkAbort[0] = true;
			break;
		case eSetAnimEngineInfo_createNewPlayerDrink:	MAKEDRINK(eSpriteID_playerDrink, info);		break;
		case eSetAnimEngineInfo_createNewTripDrink:		MAKEDRINK(eSpriteID_tripDrink, info);		break;
		case eSetAnimEngineInfo_createNewGraceDrink:	MAKEDRINK(eSpriteID_graceDrink, info);		break;
		case eSetAnimEngineInfo_tripGrabsPlayer:		StartTripCarryingPlayer();					break;
		case eSetAnimEngineInfo_tripThrowsPlayer:		EndTripCarryingPlayer();					break;
		case eSetAnimEngineInfo_magicallyOpenDoor:
			animengine_assert(info <= eSpriteID_LAST_CHARACTER);
			character = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID((ESpriteID)info);
			((DoorSprite*)g_spriteMgr.GetSpritePtrByID(eSpriteID_frontDoor))->OpenDoor(character, 3);
			break;
		case eSetAnimEngineInfo_magicallyCloseDoor:
			animengine_assert(info <= eSpriteID_LAST_CHARACTER);
			character = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID((ESpriteID)info);
			((DoorSprite*)g_spriteMgr.GetSpritePtrByID(eSpriteID_frontDoor))->CloseDoor(character, 3);
			break;
		case eSetAnimEngineInfo_startFPSAdjustment:		g_bAblRequestingFPSAdjustment = true;		break;
		case eSetAnimEngineInfo_muteSounds:				g_bMuteSounds = info;						break;
		case eSetAnimEngineInfo_setDoSoundDropOff:		g_bDoSoundDropOff = info;					break;
		case eSetAnimEngineInfo_doPhoneCallBehavior:
			phone = (PhoneSprite*)g_spriteMgr.GetSpritePtrByName("phone");
			answeringmachine = (AnsweringMachineSprite*)g_spriteMgr.GetSpritePtrByName("answering machine");
			if (info <= ePhoneCallBehavior_none)
			{
				phone->SetBehavior(ePhoneCallBehavior_none);
				answeringmachine->SetBehavior(ePhoneCallBehavior_none);
			}
			else if (info >= ePhoneCallBehavior_answeringMachineOutgoing)
			{
				phone->SetBehavior(ePhoneCallBehavior_none);
				answeringmachine->SetBehavior((EPhoneCallBehavior)info);
			}
			else
			{
				phone->SetBehavior((EPhoneCallBehavior)info);
				answeringmachine->SetBehavior(ePhoneCallBehavior_none);
			}
			break;
		case eSetAnimEngineInfo_warmUpNLP:
			FakePlayerEnteredText("warm up nlu");
			if (!g_bAtEndOfLoad) g_bAtEndOfLoad = true;
			break;
		case eSetAnimEngineInfo_dramaComplete:
			g_bAtEndOfDrama = true;
			strcpy(g_PlayerSprite->m_ScreenText, "q");
			PlayerEnteredText();
			OutputEventLog(true);
			g_baseScreenMessage = eBaseScreenMessage_EndGameMenu;
			break;
		case eSetAnimEngineInfo_doBreakingGlassSfx:	PlayGlobalSound(eGlobalSound_breakingGlass);	break;
		case eSetAnimEngineInfo_doBlenderSfx:		PlayGlobalSound(eGlobalSound_blender);			break;
	}
#undef MAKEDRINK
}

void PerformPushMultipleAnimationScripts(ESpriteID actor, int script1Index, int script2Index, int script3Index, int script4Index, EAnimLayer layer)
{
	animengine_assert(layer < eNumAnimLayers);
	animengine_assert(actor <= eSpriteID_LAST_CHARACTER);

	CharacterSprite* character = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(actor);
	if (script1Index < 0 || script1Index >= character->m_animMgr.unknown[80872])
	{
		dprintf("###/n### ERROR for char %d in PerformPushMultipleAnimationScripts: script1Index = %d", actor, script1Index);
		script1Index = 0;
	}
	if (script2Index < 0 || script2Index >= character->m_animMgr.unknown[80872])
	{
		dprintf("###/n### ERROR for char %d in PerformPushMultipleAnimationScripts: script2Index = %d", actor, script2Index);
		script2Index = 0;
	}
	if (script3Index < 0 || script3Index >= character->m_animMgr.unknown[80872])
	{
		dprintf("###/n### ERROR for char %d in PerformPushMultipleAnimationScripts: script3Index = %d", actor, script3Index);
		script3Index = 0;
	}
	if (script4Index < 0 || script4Index >= character->m_animMgr.unknown[80872])
	{
		dprintf("###/n### ERROR for char %d in PerformPushMultipleAnimationScripts: script4Index = %d", actor, script4Index);
		script4Index = 0;
	}
	if (g_ablrequestbuffer->CheckForCallbacksPending(eAblRequestType_CallbackWhenAnimationDone, actor, layer))
	{
		dprintf("Error:  Abl wants to push anim on layer %d before a previous anim/pose is done", layer);
		ShouldntBeHere("Error:  Abl wants to push anim before a previous anim/pose is done, see Winlog");
	}
	if (layer != eAnimLayer_base)
	{
		if (layer == eAnimLayer_dialog)
		{
			character->QueueDialogScript(script1Index, true);
			if (script2Index >= 0) character->QueueDialogScript(script2Index, false);
			if (script3Index >= 0) character->QueueDialogScript(script3Index, false);
			if (script4Index >= 0) character->QueueDialogScript(script4Index, false);
		}
	}
	else
	{
		if (character->GetAreWeBodyPosing())
			ShouldntBeHere("Error: Abl is trying to push a base animation while posing!");
		//character->m_animMgr.PushScript(script1Index, -1);
	}
}
void PerformPushPoseScript(ESpriteID spriteid, EBodyPoseConfig config)
{
	animengine_assert(config <= eBodyPoseConfig_charged);
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);

	CharacterSprite* dramaactor = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);

	if (g_ablrequestbuffer->CheckForCallbacksPending(eAblRequestType_CallbackWhenAnimationDone, spriteid, 0))
		ShouldntBeHere("Error:  Abl wants to push anim/pose before a previous anim/pose is done");
	if (dramaactor->GetLocomoting())
		ShouldntBeHere("Error: Abl is trying to push pose script while locomoting!");
	dramaactor->StartBodyPosing(config);
}

void PerformMusic(EMusicAction action, EMusicName name)
{
	if (g_whichSoundtrack == -1)
	{
		g_whichSoundtrack = false;

		if (g_bFacadeHasBeenRunMultipleTimes)
			g_whichSoundtrack = !rand100(79);

		g_bJimDoran = rand100(50);
		dprintf("Chose soundtrack %d, JD %d\n", g_whichSoundtrack, g_bJimDoran);
	}

	if (action == eMusicAction_stop)
	{
		StopAllMP3s();
		g_curMusicType = (EMusicName)-1;
		return;
	}
	else if (action != eMusicAction_start)
	{
		ShouldntBeHere("illegal musicAction");
		return;
	}

	if (g_whichSoundtrack)
	{
		switch (name)
		{
		case eMusicName_title:			StartMP3("sounds\\mp3_2\\1.mp3", 100, 60);	break;
		case eMusicName_tension1:		StartMP3("sounds\\mp3_2\\2.mp3", 50, 120);	break;
		case eMusicName_tension1b:													break;
		case eMusicName_tension2:		StartMP3("sounds\\mp3_2\\3.mp3", 50, 120);	break;
		case eMusicName_crisis:			StartMP3("sounds\\mp3_2\\4.mp3", 50, 120);	break;
		case eMusicName_postCrisis:		StartMP3("sounds\\mp3_2\\5.mp3", 50, 120);	break;
		case eMusicName_therapyGame:	StartMP3("sounds\\mp3_2\\6.mp3", 50, 120);	break;
		case eMusicName_revBuildup:		StartMP3("sounds\\mp3_2\\7.mp3", 50, 120);	break;
		case eMusicName_revelations:	StartMP3("sounds\\mp3_2\\8.mp3", 50, 120);	break;
		case eMusicName_endingNoRevs:	StartMP3("sounds\\mp3_2\\9.mp3", 50, 120);	break;
		case eMusicName_endingOneLeaves:
			if (rand100(50))
				StartMP3("sounds\\mp3_2\\10.mp3", 50, 120);
			else
				StartMP3("sounds\\mp3_2\\10b.mp3", 50, 120);
			break;
		case eMusicName_endingBest:		StartMP3("sounds\\mp3_2\\11.mp3", 35, 120);	break;
		default:						ShouldntBeHere("unknown music name");		break;
		}
	}
	else
	{
		int random = rand2(0, 3);
		switch (name)
		{
		case eMusicName_title:
			switch (random)
			{
			case 0: StartMP3("sounds\\mp3_1\\1a.mp3", 100, 60); break;
			case 1: StartMP3("sounds\\mp3_1\\1b.mp3", 100, 60); break;
			case 2: StartMP3("sounds\\mp3_1\\1c.mp3", 25, 60);	break;
			default: StartMP3("sounds\\mp3_1\\1d.mp3", 50, 60); break;
			}
			break;
		case eMusicName_tension1:
		case eMusicName_postCrisis:
		case eMusicName_revBuildup:
			FadeOutCurrentMP3(120);
			break;
		case eMusicName_tension1b:
			if (!g_bDoneTension1bMusic && g_curMusicType < eMusicName_tension2)
			{
				g_bDoneTension1bMusic = true;
				if (g_bJimDoran)
					StartMP3("sounds\\mp3_1\\2b.mp3", 75, 120);
				else
					StartMP3("sounds\\mp3_1\\2a.mp3", 60, 120);
			}
			break;
		case eMusicName_tension2:
			if (g_bJimDoran)
			{
				if (!g_bDoneTension1bMusic)
					StartMP3("sounds\\mp3_1\\2b.mp3", 75, 120);
			}
			else
			{
				StartMP3("sounds\\mp3_1\\3a.mp3", 60, 120);
			}
			break;
		case eMusicName_crisis:			StartMP3("sounds\\mp3_1\\4.mp3", 25, 120);	break;
		case eMusicName_therapyGame:	StartMP3("sounds\\mp3_1\\6.mp3", 60, 120);	break;
		case eMusicName_revelations:												break;
		case eMusicName_endingNoRevs:	StartMP3("sounds\\mp3_1\\9.mp3", 50, 120);	break;
		case eMusicName_endingOneLeaves:
			if (rand100(50))
				StartMP3("sounds\\mp3_1\\1d.mp3", 25, 120);
			else
				StartMP3("sounds\\mp3_1\\10b.mp3", 25, 120);
			break;
		case eMusicName_endingBest:		StartMP3("sounds\\mp3_1\\2a.mp3", 40, 120);	break;
		default:						ShouldntBeHere("unknown music name");		break;
		}
	}
	g_curMusicType = name;
}
void PerformPlaySoundEffect(EGlobalSounds sound)
{
	PlayGlobalSound(sound);
}


void PerformReactiveWalkStep(uint32_t unknown, ESpriteID actor, ESpriteID spriteid, bool stop)
{
	animengine_assert(actor <= eSpriteID_LAST_CHARACTER);
	animengine_assert(spriteid <= eSpriteID_LAST_OBJECT);

	CharacterSprite* dramaactor = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(actor);

	if (g_ablrequestbuffer->CheckForCallbacksPending(eAblRequestType_CallbackWhenAnimationDone, actor, 0))
		ShouldntBeHere("Error:  Abl wants to reactive walkstep before a previous anim/pose is done");
	if (g_ablrequestbuffer->HasQueuedAnimOrPose(actor))
		ShouldntBeHere("Error:  Abl wants to reactive walkstep while a queued up anim/pose is waiting to start");
	if (stop)
	{
		dramaactor->StopLocomotion(false);
	}
	else
	{
		dramaactor->DoLocomotion(XYZPointf(0, 0, 0), -DATA_NULLVAL, g_spriteMgr.GetSpritePtrByID(spriteid), eWalkType_normal);
	}
}
void PerformWalkStep(uint32_t unknown, ESpriteID actor, EWalkType type, float x, float y, float z, float f1, float& xout, float& yout, float& zout)
{
	//	undone
}

void PerformAbortWalkStep(ESpriteID walkstepper)
{
	animengine_assert(walkstepper <= eSpriteID_LAST_CHARACTER);

	if (g_bIgnoreNextWalkAbort[walkstepper != eSpriteID_grace])
		g_bIgnoreNextWalkAbort[walkstepper != eSpriteID_grace] = false;
	else
		((CharacterSprite*)g_spriteMgr.GetSpritePtrByID(walkstepper))->StopLocomotion(false);
}
void PerformAbortAnimationScripts(ESpriteID actor, EAnimLayer layer)
{
	animengine_assert(layer == -1 || layer < eAnimLayer_dialog);
	animengine_assert(actor <= eSpriteID_LAST_CHARACTER);

	CharacterSprite* character = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(actor);
	if (layer == -1)
	{
		animengine_assert(!character->GetLocomoting() || !character->GetAreWeBodyPosing());
		if(character->GetLocomoting())
		{
			character->StopLocomotion(true);
			//character->m_animMgr.Reset(true, false, true);
		}
		if (character->GetAreWeBodyPosing())
		{
			character->StopCurrentBodyPosing();
			//character->m_animMgr.Reset(true, false, true);
		}
		//undone
	}
	else if (layer)
	{
		if (layer == eAnimLayer_dialog) character->StopDialog();
	}
	else if (!character->GetLocomoting())
	{
		if (character->GetAreWeBodyPosing())
			character->StopCurrentBodyPosing();

		//character->m_animMgr.Reset(false, true, true);
	}
}

void PerformDoGestureAnimation(ESpriteID spriteid, int i1, int a2, bool b1)
{
	//	undone
}
void PerformDoPickupObjGestureAnimation(ESpriteID spriteid, EGestureBodyPart bodypartindex, int i1, int i2, int i3)
{
	//	undone
}
void PerformDoFullExpressionBaseAnimation(ESpriteID actor, EFullExpressionMood mood, EFullExpressionBase base, ESpriteID obj1, ESpriteID obj2)
{
	//	undone
}
void PerformDoPutdownObjGestureAnimation(ESpriteID spriteid, int i1, int i2, int i3, int i4, float x, float y, float z, int i6)
{
	//	undone
}

void PerformMiscLittleAction(ESpriteID spriteid, EMiscLittleAction action, int i1,  float f1)
{
	animengine_assert(spriteid <= eSpriteID_LAST_CHARACTER);

	CharacterSprite* actor = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(spriteid);
	switch (action)
	{
	case eMiscLittleAction_eyeLookAway:
		actor->SetEyeballLookAway(true, false, 0.0, i1, f1);
		break;
	case eMiscLittleAction_eyeLookAwayAndUp:
		actor->SetEyeballLookAway(true, false, -60.0, i1, f1);
		break;
	case eMiscLittleAction_eyeLookAwayAndWayUp:
		actor->SetEyeballLookAway(true, false, -100.0, i1, f1);
		break;
	case eMiscLittleAction_eyeLookAwayAndDown:
		actor->SetEyeballLookAway(true, false, 40.0, i1, f1);
		break;
	case eMiscLittleAction_eyeLookAwayAndWayDown:
		actor->SetEyeballLookAway(true, false, 130.0, i1, f1);
		break;
	case eMiscLittleAction_eyeLookUp:
		actor->SetEyeballLookAway(true, true, -120.0, i1, f1);
		break;
	case eMiscLittleAction_eyeQuiver:
		animengine_assert(i1 <= 100);
		actor->SetEyeballQuiver(i1 / 1000.0, f1, 0.20943333);
		break;
	case eMiscLittleAction_headQuiver:
		animengine_assert(i1 <= 100);
		actor->SetHeadQuiver(i1 / 10.0, f1, 0.31415001);
		break;
	case eMiscLittleAction_briefEyelidHeight:
		animengine_assert(i1 <= 100);
		actor->SetBriefEyelidHeight(i1 / 100.0, f1, true, true);
		break;
	case eMiscLittleAction_wink:
		actor->SetBriefEyelidHeight(0, 0.40000001, false, true);
		break;
	case eMiscLittleAction_blinkTwice:
		actor->SetBlinkBehavior(eBlinkBehavior_blinkTwice, true);
		break;
	case eMiscLittleAction_blinkFlutter:
		actor->SetBlinkBehavior(eBlinkBehavior_blinkFlutter, true);
		break;
	case eMiscLittleAction_blinkFaster:
		actor->SetBlinkBehavior(eBlinkBehavior_blinkFaster, true);
		break;
	case eMiscLittleAction_raiseEyebrows:
		actor->StartHeadEmphasis(4, false, 0.0, -1, i1);
		break;
	case eMiscLittleAction_lowerEyebrows:
		actor->StartHeadEmphasis(5, false, 0.0, -1, i1);
		break;
	case eMiscLittleAction_facialModNone:
	case eMiscLittleAction_facialModSerious:
	case eMiscLittleAction_facialModHappy:
	case eMiscLittleAction_facialModAnxious:
	case eMiscLittleAction_facialModAnxiousMed:
	case eMiscLittleAction_facialModAnxiousHigh:
	case eMiscLittleAction_facialModAnnoyed:
	case eMiscLittleAction_facialModAngry:
	case eMiscLittleAction_facialModFurious:
	case eMiscLittleAction_facialModSurprised:
	case eMiscLittleAction_facialModImpatient:
		actor->DoFullExpressionMod(action, f1);
		break;
	case eMiscLittleAction_turnHead:
		actor->StartHeadEmphasis(2, false, i1, f1 * 30, 0);
		break;
	case eMiscLittleAction_turnHeadAndBriefHold:
		actor->StartHeadEmphasis(2, false, i1, f1 * 30, fabs(i1) >= 15 ? 60 : 30);
		break;
	case eMiscLittleAction_turnHeadAndLongHold:
		actor->StartHeadEmphasis(2, false, i1, f1 * 30, 30 * rand2(3, 4));
		break;
	case eMiscLittleAction_turnHeadAndVeryLongHold:
		actor->StartHeadEmphasis(2, false, i1, f1 * 30, 30 * rand2(6, 8));
		break;
	case eMiscLittleAction_tiltHead:
		actor->StartHeadEmphasis(1, false, i1, f1 * 30, 0);
		break;
	case eMiscLittleAction_tiltHeadAndBriefHold:
		actor->StartHeadEmphasis(1, false, i1, f1 * 30, 90);
		break;
	case eMiscLittleAction_cockHead:
		actor->StartHeadEmphasis(3, false, i1, f1 * 30, 0);
		break;
	case eMiscLittleAction_breathe:
		actor->SetBreathingBehavior((ELayerAltIndex)i1);
		break;
	case eMiscLittleAction_lookAtEightball:
		actor->SetGazeTrackingSprite(actor, eGazeSpeed_med, eGazeType_normal, eAlterHeadCock_subtle);
		actor->SetEyeballLookAway(false, false, 0.0, 1, 2.0);
		actor->m_headRotYControl.SetTargetByFrames(-6, 30, eControllerType_unknown02);
		actor->m_headRotTiltControl.SetTargetByFrames(11, 30, eControllerType_unknown02);
		AddEventInfoToLog(eEventLogType_DoInteraction, eSpriteID_trip, nullptr, false, eSpriteID_eightball, 0);
		break;
	case eMiscLittleAction_clearThroat:
		actor->SetGazeTrackingSprite(actor, eGazeSpeed_med, eGazeType_normal, eAlterHeadCock_subtle);
		actor->SetEyeballLookAway(false, false, 0.0, 1, 2.0);
		float target;
		if (actor->charsprite_bool1)
			target = randFloat2(3.0, 10.0, 1000.0);
		else
			target = randFloat2(-10.0, -3.0, 1000.0);
		actor->m_headRotYControl.SetTargetByFrames(target, 30, eControllerType_unknown02);
		target = randFloat2(1.0, 2.0, 1000.0);
		actor->m_headRotTiltControl.SetTargetByFrames(target, 30, eControllerType_unknown02);
		break;
	case eMiscLittleAction_temporaryAnnoyedSpouseGlance:
		Sprite* glancetarget = g_spriteMgr.GetSpritePtrByID((ESpriteID)actor->charsprite_bool1);
		if (actor->m_pCurrentGazeTarget != glancetarget)
		{
			actor->m_pSavedGazeTarget = actor->m_pCurrentGazeTarget;
			actor->m_pSavedGazeSpeed = actor->m_eCurrentGazeSpeed;
			actor->m_pSavedGazeType = actor->m_eCurrentGazeType;
			actor->SetGazeTrackingSprite(glancetarget, eGazeSpeed_med, eGazeType_eyesOnly, eAlterHeadCock_subtle);
			actor->unknown067_1170 = rand2(15, 30);
			actor->DoFullExpressionMod(eMiscLittleAction_facialModAnnoyed, actor->unknown067_1170 / 30.0);
		}
		break;
	}
}

void PerformGetMyCopyOfAllAnimCues(int animcueindex, AnimCueSensePacket* animcuepacket)
{
	//	undone
}
void PerformGetMyCopyOfAllObjectActivations(int index, ObjectActivationSensePacket* objectactivationpacket)
{
	//	undone
}

void PerformResetMyCopyOfAllAnimCues(int i1)
{
	//	undone
}
void PerformResetMyCopyOfAllObjectActivations(int i1)
{
	//	undone
}
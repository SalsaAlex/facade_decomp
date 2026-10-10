#include "facadecommon.h"
#include "classes/AblRequestBuffer.h"

static int g_whichSoundtrack = -1;
static EMusicName g_curMusicType;
static bool g_bUnused;

bool PerformCallbackWhenFullExpressionMoodDone(ESpriteID actor, AblRequest* request)
{

}
bool PerformCallbackWhenFullExpressionBaseDone(ESpriteID actor, AblRequest* request)
{
	
}
bool PerformCallbackWhenGestureDone(ESpriteID actor, EGestureBodyPart bodypartindex, AblRequest* request)
{
	animengine_assert(bodypartindex < eNumGestureBodyPartIndexes);
	animengine_assert(actor <= eSpriteID_LAST_CHARACTER);

	CharacterSprite* character = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(actor);
	//	undone
}
bool PerformCallbackWhenAnimationDone(ESpriteID actor, EAnimLayer layer, AblRequest* request)
{
	animengine_assert(layer < eNumAnimLayers);
	animengine_assert(actor <= eSpriteID_LAST_CHARACTER);

	CharacterSprite* character = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(actor);

	if (layer == eAnimLayer_base && !character->m_animMgr.AreBaseScriptsAndFramesDone())
		return false;

	if (layer < eAnimLayer_base || character->m_animMgr.IsLayerDone(layer))
		return false;

	if (layer == eAnimLayer_dialog)
	{
		if (character->IsDialogQueued() || character->AreWeSpeaking())
			return false;
	}
	AblBufferPostSemaphore();
	bool scriptfinished = AblCallbackAPI_ScriptFinished(actor, layer);
	GetAccessToRequestBuffer("PerformAllRequests PerformCallbackWhenAnimationDone");
	g_ablrequestbuffer->SaveRequestInTrace(request);
	if (!scriptfinished)
		dprintf("ERROR: ASSERTFAIL_ABL_CALLBACK_CONFUSION");

	return true;

}
bool PerformCallbackWhenWalkIsDone(uint32_t unused, ESpriteID actor, bool b1, AblRequest* request)
{
	//	undone
}


void PerformGetMouthShape(ESpriteID spriteid, int& mouthshape)
{
	//	undone
}
void PerformSetMouthShape(ESpriteID spriteid, int mouthshape)
{
	//	undone
}

void PerformGetEyelidHeightR(ESpriteID spriteid, float& height)
{
	//	undone
}
void PerformGetEyelidHeightL(ESpriteID spriteid, float& height)
{
	//	undone
}
void PerformSetEyelidHeightR(ESpriteID spriteid, float height)
{
	//	undone
}
void PerformSetEyelidHeightL(ESpriteID spriteid, float height)
{
	//	undone
}

void PerformGetBrowShapeR(ESpriteID spriteid, int& browshape)
{
	//	undone
}
void PerformGetBrowShapeL(ESpriteID spriteid, int& browshape)
{
	//	undone
}
void PerformSetBrowShapeR(ESpriteID spriteid, int browshape)
{
	//	undone
}
void PerformSetBrowShapeL(ESpriteID spriteid, int browshape)
{
	//	undone
}

void PerformGetEyesShape(ESpriteID spriteid, int& eyeshape)
{
	//	undone
}
void PerformSetEyesShape(ESpriteID spriteid, int eyeshape)
{
	//	undone
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
	spr->SetPosition(XYZPointf(x, y, z), -1);
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
	if (spr->unknown003[20311].GetIsMoving())
		spr->unknown003[20311].StopMoving();
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
	//undone
}

void PerformSetObjectToHold(ESpriteID holder, int i1, ESpriteID victim)
{
	animengine_assert(holder <= eSpriteID_LAST_CHARACTER);
	CharacterSprite* character = (CharacterSprite*)g_spriteMgr.GetSpritePtrByID(holder);
	animengine_assert(victim <= eSpriteID_LAST_OBJECT);
	Sprite* object = g_spriteMgr.GetSpritePtrByID(victim);
	if (object)
		character->SetSpriteToHold(object, i1, -666);
}

void PerformGetAllObjectPickupRotations(ESpriteID spriteid, ObjSensePacket* sensepacket)
{
	//	undone
}
void PerformGetAllObjectPickupPositions(ESpriteID spriteid, ObjSensePacket* sensepacket)
{
	//	undone
}
void PerformGetAllObjectRotations(ObjSensePacket* sensepacket)
{
	//	undone
}
void PerformGetAllObjectPositions(ObjSensePacket* sensepacket)
{
	//	undone
}
void PerformGetAllObjectStates(ObjStateSensePacket* statesensepacket)
{
	//	undone
}
void PerformGetAllObjectStagingRotations(ObjSensePacket* sensepacket)
{
	//	undone
}
void PerformGetAllHeldObjects(HeldObjectSensePacket* heldpacket)
{
	//	undone
}

void PerformGetArmGestureBases(ESpriteID actor, int& i1out, int& i2out, int& i3out)
{
	//	undone
}

void PerformGetEyesMove(ESpriteID spriteid, float& x, float& y)
{
	//	undone
}

void PerformGetObjectPickupPosition(ESpriteID holder, ESpriteID object, float& x, float& y, float& z)
{
	//	undone
}

void PerformSetPerformanceInfo(ESpriteID spriteid, void* perfinfo)
{
	//	undone
}
void PerformSetAnimEngineInfo(ESetAnimEngineInfoType type, int info)
{
#define MAKEDRINK(spriteid, drinktype) \
	animengine_assert(drinktype > eDrink_NOTADRINK && drinktype <= eDrink_lastDrink); \
	((DrinkSprite*)g_spriteMgr.GetSpritePtrByID(spriteid))->SetDrinkType(drinktype)

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
			CharacterSprite* character = g_SpriteMgr->GetSpriteByID(info);
			((DoorSprite*)g_spriteMgr->GetSpritePtrByID(eSpriteID_frontDoor))->OpenDoor(character, 3.0);
			break;
		case eSetAnimEngineInfo_magicallyCloseDoor:
			animengine_assert(info <= eSpriteID_LAST_CHARACTER);
			CharacterSprite* character = g_SpriteMgr->GetSpriteByID(info);
			((DoorSprite*)g_spriteMgr->GetSpritePtrByID(eSpriteID_frontDoor))->CloseDoor(character, 3.0);
			break;
		case eSetAnimEngineInfo_startFPSAdjustment:		g_bAblRequestingFPSAdjustment = true;		break;
		case eSetAnimEngineInfo_muteSounds:				g_bMuteSounds = info;						break;
		case eSetAnimEngineInfo_setDoSoundDropOff:		g_bDoSoundDropOff = info;					break;
		case eSetAnimEngineInfo_doPhoneCallBehavior:
			PhoneSprite* phone = g_spriteMgr.GetSpritePtrByName("phone");
			AnsweringMachineSprite* answeringmachine = g_spriteMgr.GetSpritePtrByName("answering machine");
			if (info <= ePhoneCallBehavior_none)
			{
				phone->SetBehaviour(-1);
				answeringmachine->SetBehaviour(-1);
			}
			else if (info >= ePhoneCallBehavior_answeringMachineOutgoing)
			{
				phone->SetBehaviour(-1);
				answeringmachine->SetBehaviour(info);
			}
			else
			{
				phone->SetBehaviour(info);
				answeringmachine->SetBehaviour(-1);
			}
			break;
		case eSetAnimEngineInfo_warmUpNLP:
			FakePlayerEnteredText("warm up nlu");
			if (!g_bAtEndOfLoad) g_bAtEndOfLoad = true;
			break;
		case eSetAnimEngineInfo_dramaComplete:
			g_bAtEndOfDrama = true;
			strcpy((char*)&g_PlayerSprite->unknown003[20335], "q");
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
	if (script1Index < 0 || script1Index >= character[4].data.unknown003[17762])
	{
		dprintf("###/n### ERROR for char %d in PerformPushMultipleAnimationScripts: script1Index = %d", actor, script1Index);
		script1Index = 0;
	}
	if (script2Index < 0 || script2Index >= character[4].data.unknown003[17762])
	{
		dprintf("###/n### ERROR for char %d in PerformPushMultipleAnimationScripts: script2Index = %d", actor, script2Index);
		script2Index = 0;
	}
	if (script3Index < 0 || script3Index >= character[4].data.unknown003[17762])
	{
		dprintf("###/n### ERROR for char %d in PerformPushMultipleAnimationScripts: script3Index = %d", actor, script3Index);
		script3Index = 0;
	}
	if (script4Index < 0 || script4Index >= character[4].data.unknown003[17762])
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
		character->m_animMgr.PushScript(script1Index, -1);
	}
}
void PerformPushPoseScript(ESpriteID spriteid, EBodyPoseConfig config)
{
	//	undone
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
		switch (name)
		{
		case eMusicName_title:
			int random = rand2(0, 3);
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
	//	undone
}
void PerformWalkStep(uint32_t unknown, ESpriteID actor, EWalkType type, float x, float y, float z, float f1, float& xout, float& yout, float& zout)
{
	//	undone
}

void PerformAbortWalkStep(ESpriteID walkstepper)
{
	//	undone
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
			character->m_animMgr.Reset(true, false, true);
		}
		if (character->GetAreWeBodyPosing())
		{
			character->StopCurrentBodyPosing();
			character->m_animMgr.Reset(true, false, true);
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

		character->m_animMgr.Reset(false, true, true);
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

void PerformMiscLittleAction(ESpriteID spriteid, EMiscLittleAction action, int i1, int i2)
{
	//	undone
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
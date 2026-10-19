#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"
#include "classes/Sprite/DoorSprite.h"

DoorSprite::DoorSprite()
	: Sprite()
{
	m_eObjectType = eObjectType_Door;
}
void DoorSprite::InitDoorSprite()
{
	unknown001 = 0;
	unknown002 = false;
	unknown003 = false;
}

void DoorSprite::UpdateSprite()
{
	Sprite::UpdateSprite();

	//undone
}




void DoorSprite::OpenDoor(Sprite* actor, float speed)
{
	b1_unknown20299.SetTargetByRate(-65, speed, eControllerType_unknown02, -DATA_NULLVAL);
	PlayGlobalSound((EGlobalSounds)((int)eGlobalSound_opendoor1 + rand2(0, 1)));
	//UpdateFrontDoorCollisionZone(true);
	unknown002 = true;
	AddEventInfoToLog(eEventLogType_EnterInteraction,
		g_spriteMgr.GetSpriteIDBySpritePtr(actor),
		nullptr, false,
		g_spriteMgr.GetSpriteIDBySpritePtr(this),
		0);
}
void DoorSprite::CloseDoor(Sprite* actor, float speed)
{
	b1_unknown20299.SetTargetByRate(30, speed, eControllerType_unknown02, -DATA_NULLVAL);
	unknown003 = false;
	AddEventInfoToLog(eEventLogType_StopInteraction,
		g_spriteMgr.GetSpriteIDBySpritePtr(actor),
		nullptr, false,
		g_spriteMgr.GetSpriteIDBySpritePtr(this),
		0);
}

bool DoorSprite::IsKnockOccurring()
{
	return g_numFramesRun < unknown001 + 60;
}
bool DoorSprite::IsKnockPossible()
{
	XYZPointf playerpoint = g_PlayerSprite->GetPosition();
	return !IsDoorOpen() && !IsKnockOccurring() && playerpoint.m_x < -150 && playerpoint.m_z > 140;
}
bool DoorSprite::IsDoorOpen()
{
	return GetRotation().m_y != 30;
}
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

extern void UpdateFrontDoorCollisionZone(bool dooropen);

void DoorSprite::UpdateSprite()
{
	Sprite::UpdateSprite();

	if (b1_unknown20299.GetTarget() == 30
		&& b1_unknown20299.GetVal() < 30
		&& b1_unknown20299.GetVal() > 10)
	{
		UpdateFrontDoorCollisionZone(false);
		if (!this->unknown003)
		{
			this->unknown003 = true;
			PlayGlobalSound((EGlobalSounds)(rand2(0, 1) + eGlobalSound_closedoor1));
		}
	}

	if (GetClickedThisFrame())
	{
		b1_unknown20584 = 4;
		m_pInteractor = g_PlayerSprite;
		if (IsKnockPossible())
		{
			PlayGlobalSound((EGlobalSounds)rand2(eGlobalSound_doorknock1, eGlobalSound_doorknock3));
			this->unknown001 = g_numFramesRun;
			AddEventInfoToLog(eEventLogType_DoInteraction, eSpriteID_player, nullptr, false, g_spriteMgr.GetSpriteIDBySpritePtr(this), 0);
		}
		else if (!IsKnockOccurring())
		{
			if (IsDoorOpen())
			{
				if (!IsPlayerOutsideApt())
					CloseDoor(g_PlayerSprite, 1);
			}
			else
			{
				OpenDoor(g_PlayerSprite, 1);
			}
		}
	}
}

void DoorSprite::OpenDoor(Sprite* actor, float speed)
{
	b1_unknown20299.SetTargetByRate(-65, speed, eControllerType_unknown02, -DATA_NULLVAL);
	PlayGlobalSound((EGlobalSounds)((int)eGlobalSound_opendoor1 + rand2(0, 1)));
	UpdateFrontDoorCollisionZone(true);
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
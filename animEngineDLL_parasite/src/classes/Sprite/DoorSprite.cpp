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
	m_iTimeKnocked = 0;
	m_bOpening = false;
	m_bClosing = false;
}

extern void UpdateFrontDoorCollisionZone(bool dooropen);

void DoorSprite::UpdateSprite()
{
	Sprite::UpdateSprite();

	if (m_yawController.GetTarget() == 30
		&& m_yawController.GetVal() < 30
		&& m_yawController.GetVal() > 10)
	{
		UpdateFrontDoorCollisionZone(false);
		if (!m_bClosing)
		{
			m_bClosing = true;
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
			m_iTimeKnocked = g_numFramesRun;
			AddEventInfoToLog(eEventLogType_DoInteraction, eSpriteID_player, nullptr, false, g_spriteMgr->GetSpriteIDBySpritePtr(this), 0);
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
	m_yawController.SetTargetByRate(-65, speed, eControllerType_unknown02, -DATA_NULLVAL);
	PlayGlobalSound((EGlobalSounds)((int)eGlobalSound_opendoor1 + rand2(0, 1)));
	UpdateFrontDoorCollisionZone(true);
	m_bOpening = true;
	AddEventInfoToLog(eEventLogType_EnterInteraction,
		g_spriteMgr->GetSpriteIDBySpritePtr(actor),
		nullptr, false,
		g_spriteMgr->GetSpriteIDBySpritePtr(this),
		0);
}
void DoorSprite::CloseDoor(Sprite* actor, float speed)
{
	m_yawController.SetTargetByRate(30, speed, eControllerType_unknown02, -DATA_NULLVAL);
	m_bClosing = false;
	AddEventInfoToLog(eEventLogType_StopInteraction,
		g_spriteMgr->GetSpriteIDBySpritePtr(actor),
		nullptr, false,
		g_spriteMgr->GetSpriteIDBySpritePtr(this),
		0);
}

bool DoorSprite::IsKnockOccurring()
{
	return g_numFramesRun < m_iTimeKnocked + 60;
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
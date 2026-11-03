#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"
#include "classes/Sprite/EightballMessageSprite.h"

const char* g_EightballMessageStrings[] = {
	"Maybe Later",
	"Who Knows",
	"Could Be",
	"Yes",
	"No",
	"It's Definite",
	"Never",
	"Perhaps Soon",
	"Why Not",
	"Sure Thing",
	"I Doubt It"
};

EightballMessageSprite::EightballMessageSprite()
	: Sprite()
{
	m_eObjectType = eObjectType_EightballMessage;
}
void EightballMessageSprite::InitEightballMessageSprite()
{
	a1_unknown00005 = 1;
	m_iEightballMessageIndex = -1;
	m_iLastEightballMessageIndex = -1;
}

void EightballMessageSprite::UpdateSprite()
{
	Sprite::UpdateSprite();
	Sprite* eightball = g_spriteMgr->GetSpritePtrByID(eSpriteID_eightball);

	eightball = g_spriteMgr->GetSpritePtrByID(eSpriteID_eightball);
	if (eightball->GetSpriteHoldingMe() != g_CursorSprite
		|| eightball->GetAmIStillGrabDropAnimating())
	{
		if (GetDrawMe())
		{
			SetDrawMe(false);
			SetSpriteHoldingMe(nullptr);
		}
		return;
	}

	if (GetDrawMe())
	{
		if (m_iEightballMessageIndex == -1 && m_iSpriteFrame == 61)
		{
			m_iEightballMessageIndex = rand2(0, 10);

			//cheap way to avoid repetition 
			if (m_iLastEightballMessageIndex == m_iEightballMessageIndex)
				m_iEightballMessageIndex = rand2(0, 10);

			m_iLastEightballMessageIndex = m_iEightballMessageIndex;
			SetTextureFrameAnimation(
				m_iEightballMessageIndex,
				m_iEightballMessageIndex,
				1,
				false);
			AddEventInfoToLog(
				eEventLogType_DoInteraction,
				eSpriteID_player,
				g_EightballMessageStrings[m_iEightballMessageIndex],
				false,
				eSpriteID_eightball,
				0);
		}
		else if (m_iEightballMessageIndex == -1 || g_numFramesRun % 10)
		{
			if (m_iEightballMessageIndex == -1 && m_iSpriteFrame == 33 && rand100(2))
				SetDrawMe(false);
		}
		else if (IsCursorWiggling())
		{
			m_iEightballMessageIndex = -1;
			SetTextureFrameAnimation(
				60,
				33,
				1,
				false);
			PlayGlobalSound((EGlobalSounds)(rand2(0, 2) + eGlobalSound_eightballShake1));
			AddEventInfoToLog(eEventLogType_DoInteraction, eSpriteID_player, 0, 0, eSpriteID_eightball, 0);
		}
	}
	else
	{
		SetDrawMe(true);
		SetTextureFrameAnimation(
			34,
			61,
			3,
			false);
		m_iEightballMessageIndex = -1;
	}

	XYZPointf ballpos = eightball->GetPosition();
	ballpos.m_y += 0.4;
	SetPosition(ballpos);
	m_fFixedDepth = eightball->m_renderPoints1[0].m_z + 1.0;
}
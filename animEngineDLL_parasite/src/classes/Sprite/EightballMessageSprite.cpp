#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"
#include "classes/Sprite/EightballMessageSprite.h"

EightballMessageSprite::EightballMessageSprite()
	: Sprite()
{
	m_eObjectType = eObjectType_EightballMessage;
}
void EightballMessageSprite::InitEightballMessageSprite()
{
	a1_unknown00005 = 1;
	unknown001 = -1;
	unknown002 = -1;
}

void EightballMessageSprite::UpdateSprite()
{
	Sprite::UpdateSprite();
	Sprite* eightball = g_spriteMgr->GetSpritePtrByID(eSpriteID_eightball);

	//undone
}
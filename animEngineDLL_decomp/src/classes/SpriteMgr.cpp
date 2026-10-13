#include "facadecommon.h"
#include "classes/SpriteMgr.h"

void SpriteMgr::AddSprite(Sprite* spr)
{
	if (!spr)
		return;

	if (m_iNumSprites >= 149)
	{
		ShouldntBeHere("Sprite manager is full");
		return;
	}

	int i = 0;
	for (; i < this->m_iNumSprites; ++i)
	{
		if (m_pSpriteList[i] == spr)
		{
			ShouldntBeHere("Sprite is already in sprite manager in AddSprite()");
			return;
		}
		if (m_pSpriteList[i] && m_pSpriteList[i]->m_eID == spr->m_eID)
		{
			ShouldntBeHere("Sprite index already taken in AddSprite()");
			return;
		}
		if (m_pSpriteList[i] && !strcmp(m_pSpriteList[i]->m_name, spr->m_name))
		{
			ShouldntBeHere("Sprite name already taken in AddSprite()");
			return;
		}
	}
	m_pSpriteList[m_iNumSprites++] = spr;
}

Sprite* SpriteMgr::GetSpritePtr(int listindex)
{
	if (listindex >= 0 && listindex < m_iNumSprites)
		return m_pSpriteList[listindex];

	return nullptr;
}
Sprite* SpriteMgr::GetSpritePtrByName(const char* name)
{
	for (int i = 0; i < m_iNumSprites; i++)
		if (!strcmp(m_pSpriteList[i]->m_name, name))
			return m_pSpriteList[i];

	return nullptr;
}
Sprite* SpriteMgr::GetSpritePtrByID(ESpriteID spriteid)
{
	for (int i = 0; i < m_iNumSprites; i++)
		if (m_pSpriteList[i]->m_eID == spriteid)
			return m_pSpriteList[i];

	return nullptr;
}
ESpriteID SpriteMgr::GetSpriteIDBySpritePtr(Sprite* sprite)
{
	for(int i = 0; i < m_iNumSprites; i++)
		if (m_pSpriteList[i] == sprite)
			return sprite->m_eID;

	return eSpriteID_NOTASPRITE;
}
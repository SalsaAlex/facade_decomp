#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include "Sprite.h"

class SpriteMgr
{
public:
	virtual ~SpriteMgr() = default;

	void AddSprite(Sprite* spr);

	int GetNumSprites() { return m_iNumSprites; }
	Sprite* GetSpritePtr(int listindex);
	Sprite* GetSpritePtrByName(const char* name);
	Sprite* GetSpritePtrByID(ESpriteID spriteid);
	ESpriteID GetSpriteIDBySpritePtr(Sprite* sprite);


private:
	Sprite* m_pSpriteList[150];
	int m_iNumSprites;
protected:

};

static_assert(sizeof(SpriteMgr) == 608, "");

#endif // SPRITEMANAGER_H
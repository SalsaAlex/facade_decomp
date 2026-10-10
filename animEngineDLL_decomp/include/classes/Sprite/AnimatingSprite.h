#ifndef ANIMATINGSPRITE_H
#define ANIMATINGSPRITE_H

#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"
#include "classes/AnimMgr.h"
#include "classes/SoundSource.h"


class AnimatingSprite : public Sprite							//proof: libnativeAnimInterface.jnilib:__const_coal:002068E0
{
public:

	AnimMgr m_animMgr;

private:
	int unknown001;
	XYZPointf pointunknown001;
	XYZPointf pointunknown002;
	SoundSource m_soundSrc;
	int unknown002;
protected:

};

static_assert(sizeof(AnimatingSprite) == 1688412, ""); //estimate

#endif // ANIMATINGSPRITE_H
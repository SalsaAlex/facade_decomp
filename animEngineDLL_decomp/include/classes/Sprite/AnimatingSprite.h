#ifndef ANIMATINGSPRITE_H
#define ANIMATINGSPRITE_H

#include "classes/AnimMgr.h"
#include "classes/SoundSource.h"


class AnimatingSprite : public Sprite							//proof: libnativeAnimInterface.jnilib:__const_coal:002068E0
{
public:

	void UpdatePositionAndRotation(float* f1);
	void InitAnimatingSprite(ESpriteID id, const char* name, int nummodelpoints, int, int, int, int);

	void SetOrigPosition(XYZPointf pos) { m_originalPos = pos; }
	void SetOrigRotation(XYZPointf rot);

	void SetPosition(XYZPointf pos, float unknown) override;
	void SetRotation(XYZPointf rot) override;


	bool GetIsAnimatingSprite() override { return true; }

	AnimMgr m_animMgr;
	int animsprite_unknown001 = -1;
private:
	XYZPointf m_originalPos;
	XYZPointf m_originalRot;
	SoundSource m_soundSrc;
	int animsprite_unknown002 = -1;
protected:

};

static_assert(sizeof(AnimatingSprite) == 1688412, ""); //estimate

#endif // ANIMATINGSPRITE_H
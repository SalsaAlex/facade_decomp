#ifndef DOORSPRITE_H
#define DOORSPRITE_H

#include "Sprite.h"

class DoorSprite : public Sprite								//proof: libnativeAnimInterface.jnilib:__const_coal:002067E0
{
public:
	DoorSprite();
	void InitDoorSprite();

	void UpdateSprite() override;

	void OpenDoor(Sprite* actor, float speed);
	void CloseDoor(Sprite* actor, float speed);

	bool IsKnockOccurring();
	bool IsKnockPossible();
	bool IsDoorOpen();


private:
	int m_iTimeKnocked = 0;
	bool m_bOpening = false;
	bool m_bClosing = false;

protected:

};

static_assert(sizeof(DoorSprite) == 84124, "");					//proof: animEngineDLL.dll:.text:10076816

#endif // DOORSPRITE_H
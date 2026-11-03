#ifndef EIGHTBALLMESSAGESPRITE_H
#define EIGHTBALLMESSAGESPRITE_H

#include "Sprite.h"

class EightballMessageSprite : public Sprite					//proof: libnativeAnimInterface.jnilib:__const_coal:00206820
{
public:
	EightballMessageSprite();
	void InitEightballMessageSprite();

	void UpdateSprite() override;

private:
	int m_iEightballMessageIndex;
	int m_iLastEightballMessageIndex;

protected:

};

static_assert(sizeof(EightballMessageSprite) == 84124, "");		//proof: animEngineDLL.dll:.text:100768D2

#endif // EIGHTBALLMESSAGESPRITE_H
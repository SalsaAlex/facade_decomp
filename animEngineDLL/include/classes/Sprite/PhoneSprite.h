#ifndef PHONESPRITE_H
#define PHONESPRITE_H

#include "Sprite.h"

class PhoneSprite : public Sprite								//proof: libnativeAnimInterface.jnilib:__const_coal:00206840
{
public:
	PhoneSprite();
	void InitPhoneSprite();

	void UpdateSprite() override;

	void Speak(const char* speech, EGlobalSounds sound);

	void SetBehavior(EPhoneCallBehavior behavior);


private:
	EPhoneCallBehavior m_eBehavior;
	int unknown;

protected:

};

static_assert(sizeof(PhoneSprite) == 84124, "");				//proof: animEngineDLL.dll:.text:10076933

#endif // PHONESPRITE_H
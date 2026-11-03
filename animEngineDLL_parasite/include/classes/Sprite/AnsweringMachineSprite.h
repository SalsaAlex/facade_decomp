#ifndef ANSWERINGMACHINESPRITE_H
#define ANSWERINGMACHINESPRITE_H

#include "Sprite.h"

class AnsweringMachineSprite : public Sprite					//proof: libnativeAnimInterface.jnilib:__const_coal:00206860
{
public:
	AnsweringMachineSprite();
	void InitAnsweringMachineSprite();

	void UpdateSprite() override;

	void Speak(const char* speech, EGlobalSounds sound);

	void SetBehavior(EPhoneCallBehavior behavior);

	void SayMessage(EPhoneCallBehavior behavior, float time);

private:
	EPhoneCallBehavior m_eBehavior;
	int m_iTickCallInitiated;
	EPhoneCallBehavior m_eAltBehavior;
	int m_iTickVar1;
	int m_iTickVar2;

protected:

};

static_assert(sizeof(AnsweringMachineSprite) == 84136, "");		//proof: animEngineDLL.dll:.text:10076994

#endif // ANSWERINGMACHINESPRITE_H
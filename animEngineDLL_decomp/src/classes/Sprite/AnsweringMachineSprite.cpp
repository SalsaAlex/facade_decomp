#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"
#include "classes/Sprite/AnsweringMachineSprite.h"

AnsweringMachineSprite::AnsweringMachineSprite()
	: AnsweringMachineSprite()
{
	m_eObjectType = eObjectType_AnsweringMachine;
}
void AnsweringMachineSprite::InitAnsweringMachineSprite()
{
	m_eBehavior = ePhoneCallBehavior_none;
	unknown001 = 0;
	unknown002 = -1;
	unknown003 = 0;
	unknown004 = 0;
}

void AnsweringMachineSprite::UpdateSprite()
{
	Sprite::UpdateSprite();

	//undone
}

void AnsweringMachineSprite::Speak(const char* speech, EGlobalSounds sound)
{
	bool played = PlayGlobalSound(sound);
	//undone
}

void AnsweringMachineSprite::SetBehavior(EPhoneCallBehavior behavior)
{
	m_eBehavior = behavior;
	if (behavior != ePhoneCallBehavior_none)
		unknown001 = 0;
}

void AnsweringMachineSprite::SayMessage(EPhoneCallBehavior behavior, float time)
{
	//undone
}
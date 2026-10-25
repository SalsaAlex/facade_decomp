#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"
#include "classes/Sprite/PhoneSprite.h"

PhoneSprite::PhoneSprite()
	: Sprite()
{
	m_eObjectType = eObjectType_Phone;
}
void PhoneSprite::InitPhoneSprite()
{
	m_eBehavior = ePhoneCallBehavior_none;
	unknown = 0;

	//undone
}

void PhoneSprite::UpdateSprite()
{
	Sprite::UpdateSprite();

	//undone
}

void PhoneSprite::Speak(const char* speech, EGlobalSounds sound)
{
	bool played = PlayGlobalSound(sound);
	//undone
}

void PhoneSprite::SetBehavior(EPhoneCallBehavior behavior)
{
	m_eBehavior = behavior;
	if (behavior != ePhoneCallBehavior_none)
		unknown = 0;
}
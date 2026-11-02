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
	m_iTickCallInitiated = 0;

	for (int i = 0; i < ePhoneCallBehavior_max; ++i)
	{
		for (int j = 0; j < 10; ++j)
			g_bPhoneCallBehaviorSubparts[i][j] = false;
	}
}

void PhoneSprite::UpdateSprite()
{
	Sprite::UpdateSprite();

	static bool s_bRespondToPlayerOppPast = false;
	static bool s_bPlayerTalkedToCaller = false;

	if (m_eBehavior != ePhoneCallBehavior_none && m_iTickCallInitiated == 0)
		m_iTickCallInitiated = GetTickCount();
	if (m_eBehavior == ePhoneCallBehavior_none && m_iTickCallInitiated > 0)
	{
		if (g_bDisplayDialogText || g_phoneAnsM_latestGlobalSound == -1)
			SetScreenText((char*)" ", true);
		StopAllGlobalSounds();
		m_iTickCallInitiated = 0;
	}

	bool ringed = false;
	switch (m_eBehavior)
	{
	case ePhoneCallBehavior_ringPhone:
		if (GetSecsSinceTicks(m_iTickCallInitiated) >= 0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][0]){
			g_bPhoneCallBehaviorSubparts[m_eBehavior][0] = ringed = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 5 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][1]){
			g_bPhoneCallBehaviorSubparts[m_eBehavior][1] = ringed = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 10 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][2]){
			g_bPhoneCallBehaviorSubparts[m_eBehavior][2] = ringed = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 15 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][3]){
			g_bPhoneCallBehaviorSubparts[m_eBehavior][3] = ringed = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 200 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][4])
		{
			g_bPhoneCallBehaviorSubparts[m_eBehavior][4] = ringed = true;
			SetBehavior(ePhoneCallBehavior_none);
		}

		if (ringed)	Speak("** RING **", eGlobalSound_phoneRing);

		break;
	case ePhoneCallBehavior_tripsMomMsgInterrupted:
		StopAllGlobalSounds();
		if (GetSecsSinceTicks(m_iTickCallInitiated) >= 0.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][0]){
			Speak("Oh uh...", eGlobalSound_tripMomPhone1);
			g_bPhoneCallBehaviorSubparts[m_eBehavior][0] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) < 0.0 || g_bPhoneCallBehaviorSubparts[m_eBehavior][0])
		{
			if (GetSecsSinceTicks(m_iTickCallInitiated) >= 4.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][1])
			{
				g_bPhoneCallBehaviorSubparts[m_eBehavior][1] = true;
				SetBehavior(ePhoneCallBehavior_tripsMomOnPhone);
			}
		}
		break;
	case ePhoneCallBehavior_tripsMomOnPhone:
		if (!s_bRespondToPlayerOppPast && g_numFramesRun - g_framesWhenLastSentPlayerTypedCue <= 1)
			s_bPlayerTalkedToCaller = true;
		if (GetSecsSinceTicks(m_iTickCallInitiated) >= 0.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][0]){
			Speak("Hello?  Hello? ", eGlobalSound_tripMomPhone2);
			g_bPhoneCallBehaviorSubparts[m_eBehavior][0] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 4.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][1]) {
			Speak("Who is this?  Travis, is that you?  This is your mother.", eGlobalSound_tripMomPhone3);
			g_bPhoneCallBehaviorSubparts[m_eBehavior][1] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 9.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][2]) {
			s_bRespondToPlayerOppPast = true;
			g_bPhoneCallBehaviorSubparts[m_eBehavior][2] = true;
			if (s_bPlayerTalkedToCaller)
				Speak("I don't recognize your voice...", eGlobalSound_tripMomPhone4);
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 11 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][3]) {
			Speak("I -- I think I have the wrong number... ** click **", eGlobalSound_tripMomPhone5);
			g_bPhoneCallBehaviorSubparts[m_eBehavior][3] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 16 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][4]) {
			g_bPhoneCallBehaviorSubparts[m_eBehavior][4] = true;
			SetBehavior(ePhoneCallBehavior_none);
		}
		break;
	case ePhoneCallBehavior_gracesDadMsgInterrupted:
		StopAllGlobalSounds();
		if (GetSecsSinceTicks(m_iTickCallInitiated) >= 0.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][0]) {
			Speak("Ah, Grace? ...", eGlobalSound_graceDadPhone1);
			g_bPhoneCallBehaviorSubparts[m_eBehavior][0] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) < 0.0 || g_bPhoneCallBehaviorSubparts[m_eBehavior][0])
		{
			if (GetSecsSinceTicks(m_iTickCallInitiated) >= 4.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][1])
			{
				g_bPhoneCallBehaviorSubparts[m_eBehavior][1] = true;
				SetBehavior(ePhoneCallBehavior_none);
				m_eBehavior = ePhoneCallBehavior_gracesDadOnPhone;
			}
		}
		break;
	case ePhoneCallBehavior_gracesDadOnPhone:
		if (!s_bRespondToPlayerOppPast && g_numFramesRun - g_framesWhenLastSentPlayerTypedCue <= 1)
			s_bPlayerTalkedToCaller = true;
		if (GetSecsSinceTicks(m_iTickCallInitiated) >= 0.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][0]){
			Speak("Grace?  Hello? ", eGlobalSound_graceDadPhone2);
			g_bPhoneCallBehaviorSubparts[m_eBehavior][0] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 4.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][1]){
			Speak( "Who is this?  Grace is that you?", eGlobalSound_graceDadPhone3);
			g_bPhoneCallBehaviorSubparts[m_eBehavior][1] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 8.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][2]){
			s_bRespondToPlayerOppPast = true;
			g_bPhoneCallBehaviorSubparts[m_eBehavior][2] = true;
			if (s_bPlayerTalkedToCaller)
				Speak("I don't recognize your voice...", eGlobalSound_graceDadPhone4);
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 10.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][3]){
			Speak("Excuse me, I must have the wrong number... ** click **", eGlobalSound_graceDadPhone5);
			g_bPhoneCallBehaviorSubparts[m_eBehavior][3] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 14.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior][4]){
			g_bPhoneCallBehaviorSubparts[m_eBehavior][4] = true;
			SetBehavior(ePhoneCallBehavior_none);
		}
		break;
	}

}

void PhoneSprite::Speak(const char* speech, EGlobalSounds sound)
{
	bool played = PlayGlobalSound(sound);
	g_phoneAnsM_latestGlobalSound = sound;
	if (g_bDisplayDialogText || !played)
		SetScreenText((char*)speech, true);
	AddEventInfoToLog(eEventLogType_Speech, eSpriteID_phone, speech, false, eSpriteID_NOTASPRITE, 0);
}

void PhoneSprite::SetBehavior(EPhoneCallBehavior behavior)
{
	m_eBehavior = behavior;
	if (behavior != ePhoneCallBehavior_none)
		m_iTickCallInitiated = 0;
}
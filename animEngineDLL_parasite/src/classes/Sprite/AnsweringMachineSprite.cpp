#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"
#include "classes/Sprite/AnsweringMachineSprite.h"
#include "classes/Sprite/PhoneSprite.h"

AnsweringMachineSprite::AnsweringMachineSprite()
	: Sprite()
{
	m_eObjectType = eObjectType_AnsweringMachine;
}
void AnsweringMachineSprite::InitAnsweringMachineSprite()
{
	m_eBehavior = ePhoneCallBehavior_none;
	m_iTickCallInitiated = 0;
	m_eAltBehavior = ePhoneCallBehavior_none;
	m_iTickVar1 = 0;
	m_iTickVar2 = 0;
}

void AnsweringMachineSprite::UpdateSprite()
{
	Sprite::UpdateSprite();

	if (m_eBehavior != ePhoneCallBehavior_none && m_iTickCallInitiated == 0)
		m_iTickCallInitiated = GetTickCount();
	if (m_eBehavior == ePhoneCallBehavior_none && m_iTickCallInitiated > 0)
	{
		if (g_bDisplayDialogText || g_phoneAnsM_latestGlobalSound == -1)
			SetScreenText((char*)" ", true);
		StopAllGlobalSounds();
		m_iTickCallInitiated = 0;
	}
	switch (m_eBehavior)
	{
	case ePhoneCallBehavior_answeringMachineOutgoing:
		if (GetSecsSinceTicks(m_iTickCallInitiated) >= 0.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 0])
		{
			Speak("** click **", eGlobalSound_answeringMachineClick);
			g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 0] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 1.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 1])
		{
			Speak("You've reached the fabulous new home of Grace and Trip.  Leave us a message!", eGlobalSound_answeringMachineOutgoing);
			g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 1] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 5.5 && !g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 2])
		{
			Speak("** beep **", eGlobalSound_answeringMachineBeep);
			g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 2] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 7.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 3])
		{
			g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 3] = 1;
			SetBehavior(ePhoneCallBehavior_none);
		}
		break;
	case ePhoneCallBehavior_answeringMachineMessage:
		if (m_eAltBehavior == ePhoneCallBehavior_none) {
			if (GetSecsSinceTicks(m_iTickCallInitiated) >= 0.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 0]) {
				Speak("No new messages", eGlobalSound_answeringMachineNoNewMsgs);
				g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 0] = true;
			}
			else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 3.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 1]) {
				g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 0] = false;
				SetBehavior(ePhoneCallBehavior_none);
			}
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= m_iTickVar1 + 3.0) {
			g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 0] = false;
			g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 1] = false;
			SetBehavior(ePhoneCallBehavior_none);
			m_eAltBehavior = ePhoneCallBehavior_none;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 0.0 && !g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 0]) {
			Speak("One new message *beep*", eGlobalSound_answeringMachineOneNewMsg);
			g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 0] = true;
			for (int i = 0; i < 10; ++i)
				g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + i] = false;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= 3.0) {
			SayMessage(m_eAltBehavior, 3.0);
		}
		break;
	case ePhoneCallBehavior_tripsMomAnsweringMachine:
		if (GetSecsSinceTicks(m_iTickCallInitiated) >= 0.0
			&& !g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 0])
		{
			m_eAltBehavior = m_eBehavior;
			m_iTickVar2 = GetTickCount();
			g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 0] = true;
		}
		SayMessage(m_eBehavior, 0.0);
		m_iTickVar1 = GetSecsSinceTicks(m_iTickVar2);
		break;
	case ePhoneCallBehavior_gracesDadAnsweringMachine:
		if (GetSecsSinceTicks(m_iTickCallInitiated) >= 0.0
			&& !g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 0])
		{
			m_eAltBehavior = m_eBehavior;
			m_iTickVar2 = GetTickCount();
			g_bPhoneCallBehaviorSubparts[m_eBehavior * 9 + 0] = true;
		}
		SayMessage(m_eBehavior, 0.0);
		m_iTickVar1 = GetSecsSinceTicks(m_iTickVar2);
		break;
	}
	if (GetClickedThisFrame())
	{
		b1_unknown20584 = (m_eAltBehavior == ePhoneCallBehavior_none) + 4;
		m_pInteractor = g_PlayerSprite;
		if (m_eBehavior == ePhoneCallBehavior_none
			&& ((PhoneSprite*)g_spriteMgr->GetSpritePtrByName("phone"))->m_eBehavior == -1)
		{
			SetBehavior(ePhoneCallBehavior_answeringMachineMessage);
			g_spriteMgr->GetSpriteIDBySpritePtr(this);
			AddEventInfoToLog(eEventLogType_DoInteraction, eSpriteID_player, nullptr, false, g_spriteMgr->GetSpriteIDBySpritePtr(this), 0);
		}
	}
}

void AnsweringMachineSprite::Speak(const char* speech, EGlobalSounds sound)
{
	bool played = PlayGlobalSound(sound);
	_set_g_phoneAnsM_latestGlobalSound(sound);
	if (g_bDisplayDialogText || !played)
		SetScreenText((char*)speech, true);
	AddEventInfoToLog(eEventLogType_Speech, eSpriteID_answeringMachine, speech, false, eSpriteID_NOTASPRITE, 0);
}

void AnsweringMachineSprite::SetBehavior(EPhoneCallBehavior behavior)
{
	m_eBehavior = behavior;
	if (behavior != ePhoneCallBehavior_none)
		m_iTickCallInitiated = 0;
}

void AnsweringMachineSprite::SayMessage(EPhoneCallBehavior behavior, float time)
{
	if (behavior == ePhoneCallBehavior_tripsMomAnsweringMachine)
	{
		if (GetSecsSinceTicks(m_iTickCallInitiated) >= time + 0.0 && !g_bPhoneCallBehaviorSubparts[behavior * 9 + 1]) {
			Speak("Travis, are you there, it's your mother, I haven't heard from you and Grace in a while,", eGlobalSound_tripMomMsg1);
			g_bPhoneCallBehaviorSubparts[behavior * 9 + 1] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= time + 7.0 && !g_bPhoneCallBehaviorSubparts[behavior * 9 + 2]) {
			Speak("I'm just calling to find out if you've settled in to your new apartment, that's all...", eGlobalSound_tripMomMsg2);
			g_bPhoneCallBehaviorSubparts[behavior * 9 + 2] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= time + 12.0 && !g_bPhoneCallBehaviorSubparts[behavior * 9 + 3]) {
			Speak("Give me a call if you're not travelling, I know you're travelling a lot these days.", eGlobalSound_tripMomMsg3);
			g_bPhoneCallBehaviorSubparts[behavior * 9 + 3] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= time + 17.0 && !g_bPhoneCallBehaviorSubparts[behavior * 9 + 4]) {
			Speak("Okay, that's all, bye Travis.", eGlobalSound_tripMomMsg4);
			g_bPhoneCallBehaviorSubparts[behavior * 9 + 4] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= time + 22.0 && !g_bPhoneCallBehaviorSubparts[behavior * 9 + 5]) {
			SetBehavior(ePhoneCallBehavior_none);
			g_bPhoneCallBehaviorSubparts[behavior * 9 + 5] = true;
		}
	}
	else if (behavior == ePhoneCallBehavior_gracesDadAnsweringMachine)
	{
		if (GetSecsSinceTicks(m_iTickCallInitiated) >= time + 0.0 && !g_bPhoneCallBehaviorSubparts[behavior * 9 + 1]) {
			Speak("Grace, this is your father, I thought I'd call to see how my lovely daughter is doing.",
				eGlobalSound_graceDadMsg1);
			g_bPhoneCallBehaviorSubparts[behavior * 9 + 1] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= time + 5.5 && !g_bPhoneCallBehaviorSubparts[behavior * 9 + 2]) {
			Speak("I want you and Trip to come up to the house this weekend, your mother and I are throwing a party for a senior VP at the firm, and you should be there.",
				eGlobalSound_graceDadMsg2);
			g_bPhoneCallBehaviorSubparts[behavior * 9 + 2] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= time + 13.0 && !g_bPhoneCallBehaviorSubparts[behavior * 9 + 3]) {
			Speak("Oh, and be sure to arrive early, I want to talk to you about this business about why you won't be attending your mother's Christmas Eve gala.",
				eGlobalSound_graceDadMsg3);
			g_bPhoneCallBehaviorSubparts[behavior * 9 + 3] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= time + 19.5 && !g_bPhoneCallBehaviorSubparts[behavior * 9 + 4]) {
			Speak("(clear throat) So, that's all.  Uh, goodbye.", eGlobalSound_graceDadMsg4);
			g_bPhoneCallBehaviorSubparts[behavior * 9 + 4] = true;
		}
		else if (GetSecsSinceTicks(m_iTickCallInitiated) >= time + 27.0 && !g_bPhoneCallBehaviorSubparts[behavior * 9 + 5]) {
			SetBehavior(ePhoneCallBehavior_none);
			g_bPhoneCallBehaviorSubparts[behavior * 9 + 5] = true;
		}
	}
}
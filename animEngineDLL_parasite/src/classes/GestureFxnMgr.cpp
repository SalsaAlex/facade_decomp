#include "facadecommon.h"

#include "classes/GestureFxnMgr.h"

void GestureFxnMgr::StartGestureFxn(int, int, bool)
{
	//undone
}

bool GestureFxnMgr::StandardBodyGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo)
{
	if (msg == eGestureFxnMsg_unknown000)
	{
		actor->m_animMgr.PushLayeredScript(msginfo, 1, 3, -1);
	}
	else if (msg == 3)
	{
		return actor->m_animMgr.IsLayerDone(3);
	}

	return true;
}
bool GestureFxnMgr::StandardArmLGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo)
{
	//undone
	return true;
}
bool GestureFxnMgr::StandardArmRGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo)
{
	//undone
	return true;
}
bool GestureFxnMgr::StandardEyesMoveGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo)
{
	if (msg == eGestureFxnMsg_unknown000)
	{
		actor->unknown051[18] = msginfo;
		actor->m_pHeadSprite->SetEyeExpressionIndex(actor->unknown051[18], true);
		actor->unknown051[19] = GetRandomFramesFromSecPercs(25, 100);
	}
	else if (msg == 1)
	{
		actor->m_pHeadSprite->SetEyeExpressionIndex(0, false);
	}
	else if (msg == 2 && --actor->unknown051[19] <= 0 && !actor->m_Controller17.GetIsMoving())
	{
		actor->m_pHeadSprite->SetEyeExpressionIndex(actor->unknown051[18], true);
		actor->unknown051[19] = GetRandomFramesFromSecPercs(25, 100);
	}

	return true;
}
bool GestureFxnMgr::StandardBrowLGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo)
{
	if (msg == eGestureFxnMsg_unknown000)
	{
		actor->m_pHeadSprite->SetBrowL(msginfo, -1);
	}
	else if (msg == 1)
	{
		actor->m_pHeadSprite->SetBrowL(1, -1);
	}
	return true;
}
bool GestureFxnMgr::StandardBrowRGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo)
{
	if (msg == eGestureFxnMsg_unknown000)
	{
		actor->m_pHeadSprite->SetBrowR(msginfo, -1);
	}
	else if (msg == 1)
	{
		actor->m_pHeadSprite->SetBrowR(1, -1);
	}
	return true;
}
bool GestureFxnMgr::StandardMouthGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo)
{
	if (msg == eGestureFxnMsg_unknown000)
	{
		actor->m_pHeadSprite->SetMouthShape(msginfo, -1);
	}
	else if (msg == 1)
	{
		actor->m_pHeadSprite->SetMouthShape(1, -1);
	}
	return true;
}
bool GestureFxnMgr::StandardEyesShapeGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo)
{
	if (msg == eGestureFxnMsg_unknown000)
	{
		actor->m_pHeadSprite->SetEyesShape(msginfo, -1);
	}
	else if (msg == 1)
	{
		actor->m_pHeadSprite->SetEyesShape(1, -1);
	}

	return true;
}
bool GestureFxnMgr::StandardHeadGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo)
{
	if (msg)
	{
		if (msg == 1)
		{
			actor->m_pHeadSprite->SetHeadExpressionIndex(1, 0, -1);
		}
		else if (msg == 2
			&& --actor->unknown051[9] <= 0
			&& !actor->m_Controller9.GetIsMoving()
			&& !actor->AreWeInAnEditor(false))
		{
			actor->m_pHeadSprite->SetHeadExpressionIndex(actor->unknown051[8], 1, -1);
			actor->unknown051[9] = GetRandomFramesFromSecPercs(25, 100);
		}
	}
	else
	{
		actor->unknown051[8] = msginfo;
		actor->m_pHeadSprite->TweakHeadExpressionRotations(actor->unknown051[8]);
		actor->m_pHeadSprite->SetHeadExpressionIndex(actor->unknown051[8], 1, -1);
		actor->unknown051[9] = GetRandomFramesFromSecPercs(25, 100);
	}
	return true;
}
bool GestureFxnMgr::StandardArmsBothGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo)
{
	//undone
	return true;
}
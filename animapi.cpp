#include <windows.h>
#include "AblRequestBuffer.h"
#include "assert.h"

//sizeof(request_struct) = 3256 ?
//request_struct[1] = eAblRequestType

#define GET_ABLREQUEST(entry)							\
	AblRequestEntry* ##entry;							\
														\
	AnimAPI_AssertFunc(__func__);						\
														\
	GetAblRequestBuffer()->GetFreeRequestPtr(&##entry);	\
	if (!##entry)										\
		assert(0);

extern int printfunc(const char* format, ...); //dllmain.cpp

void AnimAPI_AssertFunc(const char* funcname)
{
	assert(funcname);
	assert(strlen(funcname) < 100);
	while (int i = WaitForSingleObject(hMutex, 10000))
	{
		printfunc("%s couldn't get req buffer mutex; owner %s", funcname);
		MessageBeep(MB_OK);
	}
	return strcpy(AblRequestBuffer::m_pCurAnimFuncName, funcname);
}

void AnimAPI_SetObjectPosition(int objectindex, float x, float y, float z)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_iObjectIndex = objectindex;
	entry->m_FloatVarX = x;
	entry->m_FloatVarY = y;
	entry->m_FloatVarZ = z;
	entry->m_eRequestType = eAblRequestType_SetObjectPosition;
	entry->m_bRequestActive = 1;
}
void AnimAPI_SetObjectRotation(int objectindex, float x, float y, float z)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_iObjectIndex = objectindex;
	entry->m_FloatVarX = x;
	entry->m_FloatVarY = y;
	entry->m_FloatVarZ = z;
	entry->m_eRequestType = eAblRequestType_SetObjectRotation;
	entry->m_bRequestActive = 1;
}
void AnimAPI_PushMultipleAnimationScripts(int characterID, int script1, int script2, int script3, int script4, int layerIndex)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_iCharacterID = characterID;
	entry->unknown013 = script1;
	entry->unknown014 = script2;
	entry->unknown015 = script3;
	entry->unknown016 = script4;
	entry->m_iAnimLayerIndex = layerIndex;
	entry->m_eRequestType = eAblRequestType_PushMultipleAnimationScripts;
	entry->m_bRequestActive = 1;
}
void AnimAPI_PushAnimationUScript(int characterID, int script1, int layerIndex)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_iCharacterID = characterID;
	entry->unknown013 = script1;
	entry->m_iAnimLayerIndex = layerIndex;
	entry->m_eRequestType = eAblRequestType_PushAnimationUScript;
	entry->m_bRequestActive = 1;
}
void AnimAPI_PushPoseScript(int characterID, int script2)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_iCharacterID = characterID;
	entry->m_iAnimLayerIndex = 0;
	entry->unknown013 = script2;
	entry->m_eRequestType = eAblRequestType_PushPoseScript;
	entry->m_bRequestActive = 1;
}
void AnimAPI_PushPoseScript(int characterID, int posescript)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_iCharacterID = characterID;
	entry->m_iAnimLayerIndex = 0;
	entry->unknown013 = posescript;
	entry->m_eRequestType = eAblRequestType_PushPoseScript;
	entry->m_bRequestActive = 1;
}
void AnimAPI_AbortAnimationScripts(int characterID, int layerIndex)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_iCharacterID = characterID;
	entry->m_iAnimLayerIndex = layerIndex;
	entry->m_eRequestType = eAblRequestType_AbortAnimationScripts;
	entry->m_bRequestActive = 1;
}
void AnimAPI_DoGestureAnimation(int characterID, int gestureBodyPartIndex, int unknown1)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_iCharacterID = characterID;
	entry->m_iAnimLayerIndex = gestureBodyPartIndex;
	entry->unknown013 = unknown1;
	entry->m_eRequestType = eAblRequestType_DoGestureAnimation;
	entry->m_bRequestActive = 1;
}

void AnimAPI_DoPickupObjGestureAnimation(int characterID, int gestureBodyPartIndex, int unknown1, int objectIndex, int unknown2)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_iCharacterID = characterID;
	entry->m_iAnimLayerIndex = gestureBodyPartIndex;
	entry->unknown013 = unknown1;
	entry->m_iObjectIndex = objectIndex;
	entry->unknown116 = unknown2;
	entry->m_eRequestType = eAblRequestType_DoPickupObjGestureAnimation;
	entry->m_bRequestActive = 1;
}
void AnimAPI_DoPutdownObjGestureAnimation(int characterID, int gestureBodyPartIndex, int unknown1, int objectIndex, int unknown2, float x, float y, float z, int unknown3)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_iCharacterID = characterID;
	entry->m_iAnimLayerIndex = gestureBodyPartIndex;
	entry->unknown013 = unknown1;
	entry->m_iObjectIndex = objectIndex;
	entry->m_FloatVarX = x;
	entry->m_FloatVarY = y;
	entry->m_FloatVarZ = z;
	entry->unknown029 = unknown3;
	entry->m_eRequestType = eAblRequestType_DoPutdownObjGestureAnimation;
	entry->m_bRequestActive = 1;
}
void AnimAPI_DoFullExpressionBaseAnimation(int characterID, int unknown1, int unknown2, int unknown3)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_iCharacterID = characterID;
	entry->unknown013 = unknown1;
	entry->unknown029 = unknown2;
	entry->unknown030 = unknown3;
	entry->m_eRequestType = eAblRequestType_DoFullExpressionBaseAnimation;
	entry->m_bRequestActive = 1;
}
void AnimAPI_ResetMyCopyOfAllAnimCues(int characterID)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_eRequestType = eAblRequestType_ResetMyCopyOfAllAnimCues;
	entry->m_iCharacterID = characterID;
	entry->m_bRequestActive = 1;
	OpenEvent(EVENT_ALL_ACCESS, 0, entry->m_RequestName);
}
void AnimAPI_ResetMyCopyOfAllObjectActivations(int characterID)
{
	GET_ABLREQUEST(entry)

	entry->m_iRequestTick = GetTickCount();
	entry->m_eRequestType = eAblRequestType_ResetMyCopyOfAllObjectActivations;
	entry->m_iCharacterID = characterID;
	entry->m_bRequestActive = 1;
	OpenEvent(EVENT_ALL_ACCESS, 0, entry->m_RequestName);
}

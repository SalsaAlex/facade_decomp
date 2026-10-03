#include "facadecommon.h"
#include "classes/AblRequestBuffer.h"

#define GET_ABLREQUEST(req, reqtype)								\
	AblRequest* ##req;												\
																	\
	GetAccessToRequestBuffer(__func__);								\
																	\
	int reqindex = g_ablrequestbuffer->GetFreeRequestPtr(##req);	\
	if (!##req)														\
	{																\
		animengine_assert(0);										\
		return;														\
	}																\
	req->type = reqtype;											\
	req->active = 1;												\
	req->requesttime = GetTickCount()


static void GetAccessToRequestBuffer(const char* name)
{
	animengine_assert(name != nullptr);
	animengine_assert(strlen(name) < 100);

	int waitresponse = -1;
	while (waitresponse)
	{
		if (waitresponse = WaitForSingleObject(g_ablRequestBufferMutexHandle, 10000))
		{
			dprintf("%s couldn't get req buffer mutex; owner %s", name, g_ablrequestbuffer->unknown001->name);
			MessageBeep(MB_OK);
		}
	}

	strcpy(g_ablrequestbuffer->unknown001->name, name);
}

static void AblBufferPostSemaphore()
{
	g_ablrequestbuffer->unknown001->name[0] = '\0';
	ReleaseMutex(g_ablRequestBufferMutexHandle);
}
static bool AblBufferWaitSemaphore(HANDLE hevent, const char* name)
{
	int event_answer = -1;
	while (event_answer)
	{
		event_answer = WaitForSingleObject(hevent, 1 * 1000);//wait for 1 sec
		if (event_answer)
		{
			dprintf("%s never got answer event", name);
			MessageBeep(MB_OK);
			return true;
		}
	}
	CloseHandle(hevent);
	return false;
}


extern "C" {

void AnimAPI_AbortAnimationScripts()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_AbortAnimationScripts);



	AblBufferPostSemaphore();
}
void AnimAPI_AbortTurnInPlace()
{
	ShouldntBeHere("AnimAPI_AbortTurnInPlace is an obsolete function, use PerformAbortWalkStep");
}
void AnimAPI_AbortWalkStep()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_AbortWalkStep);



	AblBufferPostSemaphore();
}
void AnimAPI_AddEventInfoToLog()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_AddEventInfoToLog);



	AblBufferPostSemaphore();
}
void AnimAPI_DoFullExpressionBaseAnimation()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_DoFullExpressionBaseAnimation);



	AblBufferPostSemaphore();
}
void AnimAPI_DoFullExpressionMoodAnimation()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_DoFullExpressionMoodAnimation);



	AblBufferPostSemaphore();
}
void AnimAPI_DoGestureAnimation()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_DoGestureAnimation);



	AblBufferPostSemaphore();
}
void AnimAPI_DoPickupObjGestureAnimation()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_DoPickupObjGestureAnimation);



	AblBufferPostSemaphore();
}
void AnimAPI_DoPutdownObjGestureAnimation()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_DoPutdownObjGestureAnimation);



	AblBufferPostSemaphore();
}
void AnimAPI_MiscLittleAction(ESpriteID spriteid, EMiscLittleAction action, float param1, float param2)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_MiscLittleAction);

	req->unknown001 = spriteid;
	req->unknown002 = action;
	req->unknown003 = param1;
	req->unknown004 = param2;

	AblBufferPostSemaphore();
}
void AnimAPI_Music(int musicaction, int musicid)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_Music);

	req->unknown000 = musicaction;
	req->unknown016 = musicid;

	AblBufferPostSemaphore();
}
void AnimAPI_PlaySoundEffect(int sfx)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_PlaySoundEffect);

	req->unknown000 = sfx;

	AblBufferPostSemaphore();
}
void AnimAPI_PushAnimationUScript()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_PushAnimationUScript);



	AblBufferPostSemaphore();
}
void AnimAPI_PushMultipleAnimationScripts()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_PushMultipleAnimationScripts);



	AblBufferPostSemaphore();
}
void AnimAPI_PushPoseScript()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_PushPoseScript);



	AblBufferPostSemaphore();
}
void AnimAPI_ReactiveWalkStep()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_ReactiveWalkStep);



	AblBufferPostSemaphore();
}
void AnimAPI_ResetMyCopyOfAllAnimCues()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_ResetMyCopyOfAllAnimCues);



	AblBufferPostSemaphore();
}
void AnimAPI_ResetMyCopyOfAllObjectActivations()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_ResetMyCopyOfAllObjectActivations);



	AblBufferPostSemaphore();
}
void AnimAPI_SetAnimEngineInfo()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetAnimEngineInfo);



	AblBufferPostSemaphore();
}
void AnimAPI_SetBrowShapeL()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetBrowShapeL);



	AblBufferPostSemaphore();
}
void AnimAPI_SetBrowShapeR()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetBrowShapeR);



	AblBufferPostSemaphore();
}
void AnimAPI_SetEyelidHeightL()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetEyelidHeightL);



	AblBufferPostSemaphore();
}
void AnimAPI_SetEyelidHeightR()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetEyelidHeightR);



	AblBufferPostSemaphore();
}
void AnimAPI_SetEyesMove()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetEyesMove);



	AblBufferPostSemaphore();
}
void AnimAPI_SetEyesShape()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetEyesShape);



	AblBufferPostSemaphore();
}
void AnimAPI_SetGazeTracking()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetGazeTracking);



	AblBufferPostSemaphore();
}
void AnimAPI_SetMouthShape(ESpriteID actor, int shape)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetMouthShape);

	req->unknown001 = actor;
	req->unknown007 = shape;

	AblBufferPostSemaphore();
}
void AnimAPI_SetObjectPosition(ESpriteID object, float x, float y, float z)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetObjectPosition);

	req->unknown000 = object;
	req->unknown009 = x;
	req->unknown010 = y;
	req->unknown011 = z;

	AblBufferPostSemaphore();
}
void AnimAPI_SetObjectRotation(ESpriteID object, float x, float y, float z)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetObjectRotation);

	req->unknown000 = object;
	req->unknown009 = x;
	req->unknown010 = y;
	req->unknown011 = z;

	AblBufferPostSemaphore();
}
void AnimAPI_SetObjectToHold(ESpriteID actor, int a2, ESpriteID object)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetObjectToHold);

	req->unknown001 = actor;
	req->unknown008 = a2;
	req->unknown000 = object;

	AblBufferPostSemaphore();
}
void AnimAPI_SetPerformanceInfo()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetPerformanceInfo);



	AblBufferPostSemaphore();
}
void AnimAPI_TurnInPlace()
{
	ShouldntBeHere("AnimAPI_TurnInPlace is an obsolete function, use PerformWalkStep");
}
void AnimAPI_WalkStep(
	int a1, int a2, int a3, int a4, int a5, int a6, //in
	int* a7, int* a8, int* a9)//out
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_WalkStep);

	req->unknown001 = a1;
	req->unknown002 = a2;
	req->unknown009 = a3;
	req->unknown010 = a4;
	req->unknown011 = a5;
	req->unknown012 = a6;

	g_ablrequestbuffer->SaveRequestInTrace(req);

	HANDLE hevent = OpenEvent(EVENT_ALL_ACCESS, FALSE, req->name);

	AblBufferPostSemaphore();
	AblBufferWaitSemaphore(hevent, __func__);

	GetAccessToRequestBuffer("AnimAPI_WalkStep 2");
	g_ablrequestbuffer->GetRequestPtrByIndex(reqindex, req);
	animengine_assert(req->active == 2);

	*a7 = req->unknown013;
	*a8 = req->unknown014;
	*a9 = req->unknown015;

	req->active = 0;

	AblBufferPostSemaphore();
}


int AnimAPI_GetAllHeldObjects()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetAllHeldObjects);



	AblBufferPostSemaphore();
}
int AnimAPI_GetAllObjectPickupPositions()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetAllObjectPickupPositions);



	AblBufferPostSemaphore();
}
int AnimAPI_GetAllObjectPickupRotations()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetAllObjectPickupRotations);



	AblBufferPostSemaphore();
}
int AnimAPI_GetAllObjectPositions()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetAllObjectPositions);



	AblBufferPostSemaphore();
}
int AnimAPI_GetAllObjectRotations()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetAllObjectRotations);



	AblBufferPostSemaphore();
}
int AnimAPI_GetAllObjectStagingRotations()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetAllObjectStagingRotations);



	AblBufferPostSemaphore();
}
int AnimAPI_GetAllObjectStates()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetAllObjectStates);

	AblBufferPostSemaphore();
}
int AnimAPI_GetArmGestureBases()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetArmGestureBases);

	AblBufferPostSemaphore();
}
int AnimAPI_GetBrowShapeL()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetBrowShapeL);

	AblBufferPostSemaphore();
}
int AnimAPI_GetBrowShapeR()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetBrowShapeR);

	AblBufferPostSemaphore();
}
int AnimAPI_GetEyelidHeightL()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetEyelidHeightL);

	AblBufferPostSemaphore();
}
int AnimAPI_GetEyelidHeightR()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetEyelidHeightR);

	AblBufferPostSemaphore();
}
int AnimAPI_GetEyesMove()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetEyesMove);

	AblBufferPostSemaphore();
}
int AnimAPI_GetEyesShape()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetEyesShape);

	AblBufferPostSemaphore();
}
int AnimAPI_GetGazeTracking()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetGazeTracking);

	AblBufferPostSemaphore();
}
int AnimAPI_GetMouthShape()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetMouthShape);

	AblBufferPostSemaphore();
}
int AnimAPI_GetMyCopyOfAllAnimCues()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetMyCopyOfAllAnimCues);

	AblBufferPostSemaphore();
}
int AnimAPI_GetMyCopyOfAllObjectActivations()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetMyCopyOfAllObjectActivations);

	AblBufferPostSemaphore();
}
int AnimAPI_GetObjectPickupPosition()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetObjectPickupPosition);



	AblBufferPostSemaphore();
}
int AnimAPI_GetObjectPosition()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetObjectPosition);



	AblBufferPostSemaphore();
}
int AnimAPI_GetObjectRotation()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_GetObjectRotation);



	AblBufferPostSemaphore();
}


}
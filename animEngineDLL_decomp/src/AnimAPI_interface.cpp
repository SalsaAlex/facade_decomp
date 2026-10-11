#include "facadecommon.h"
#include "classes/AblRequestBuffer.h"

#include <string>

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

#define GET_ABLREQUEST_RET(req, reqtype)							\
	AblRequest* ##req;												\
																	\
	GetAccessToRequestBuffer(__func__);								\
																	\
	int reqindex = g_ablrequestbuffer->GetFreeRequestPtr(##req);	\
	if (!##req)														\
	{																\
		animengine_assert(0);										\
		return 1;													\
	}																\
	req->type = reqtype;											\
	req->active = 1;												\
	req->requesttime = GetTickCount()



#define WAIT_ABLREQUEST()												\
	HANDLE semaphore = OpenEvent(EVENT_ALL_ACCESS, 0, req->name);		\
	AblBufferPostSemaphore();											\
	bool waitresult = AblBufferWaitSemaphore(semaphore, __func__);\
	if (waitresult)\
		return waitresult;\
	\
	GetAccessToRequestBuffer((std::string(__func__) + " 2").c_str());\
	g_ablrequestbuffer->GetRequestPtrByIndex(reqindex, req);\
	animengine_assert(req->active == 2);



void GetAccessToRequestBuffer(const char* name)
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

void AblBufferPostSemaphore()
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

	//undone

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

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_AddEventInfoToLog()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_AddEventInfoToLog);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_DoFullExpressionBaseAnimation()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_DoFullExpressionBaseAnimation);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_DoFullExpressionMoodAnimation()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_DoFullExpressionMoodAnimation);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_DoGestureAnimation()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_DoGestureAnimation);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_DoPickupObjGestureAnimation()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_DoPickupObjGestureAnimation);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_DoPutdownObjGestureAnimation()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_DoPutdownObjGestureAnimation);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_MiscLittleAction(ESpriteID spriteid, EMiscLittleAction action, float param1, float param2)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_MiscLittleAction);

	req->unknown001 = spriteid;
	req->unknown002 = action;
	req->unknown003 = param1;
	req->unknown009 = param2;

	AblBufferPostSemaphore();
}
void AnimAPI_Music(int musicaction, int musicid)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_Music);

	req->unknown002 = musicaction;
	req->unknown018 = musicid;

	AblBufferPostSemaphore();
}
void AnimAPI_PlaySoundEffect(int sfx)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_PlaySoundEffect);

	req->unknown002 = sfx;

	AblBufferPostSemaphore();
}
void AnimAPI_PushAnimationUScript(ESpriteID actor, int param1, int param2, int param3)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_PushAnimationUScript);

	req->unknown001 = actor;
	req->unknown002 = param2;
	req->unknown006 = param3;

	g_ablrequestbuffer->SaveRequestInTrace(req);
	AblBufferPostSemaphore();
}
void AnimAPI_PushMultipleAnimationScripts(ESpriteID actor, 
	int script1Index, int script2Index, int script3Index, int script4Index,
	EAnimLayer layer)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_PushMultipleAnimationScripts);

	req->unknown001 = actor;
	req->unknown002 = script1Index;
	req->unknown003 = script2Index;
	req->unknown004 = script3Index;
	req->unknown005 = script4Index;
	req->unknown006 = layer;

	AblBufferPostSemaphore();
}
void AnimAPI_PushPoseScript(ESpriteID actor, int param1)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_PushPoseScript);

	req->unknown001 = actor;
	req->unknown006 = 0;
	req->unknown002 = param1;

	AblBufferPostSemaphore();
}
void AnimAPI_ReactiveWalkStep(ESpriteID actor, ESpriteID spriteid, bool stop)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_ReactiveWalkStep);

	req->unknown001 = actor;
	req->unknown000 = spriteid;
	req->unknown018 = stop;

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
void AnimAPI_SetAnimEngineInfo(ESetAnimEngineInfoType type, int info)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetAnimEngineInfo);

	req->unknown018 = type;
	req->unknown019 = info;

	AblBufferPostSemaphore();
}
void AnimAPI_SetBrowShapeL(ESpriteID actor, int shape)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetBrowShapeL);

	req->unknown001 = actor;
	req->unknown007 = shape;

	AblBufferPostSemaphore();
}
void AnimAPI_SetBrowShapeR(ESpriteID actor, int shape)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetBrowShapeR);

	req->unknown001 = actor;
	req->unknown007 = shape;

	AblBufferPostSemaphore();
}
void AnimAPI_SetEyelidHeightL(ESpriteID actor, float height, float unused)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetEyelidHeightL);

	req->unknown001 = actor;
	req->unknown010 = height;
	req->unknown016 = unused;

	AblBufferPostSemaphore();
}
void AnimAPI_SetEyelidHeightR(ESpriteID actor, float height, float unused)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetEyelidHeightR);

	req->unknown001 = actor;
	req->unknown010 = height;
	req->unknown016 = unused;

	AblBufferPostSemaphore();
}
void AnimAPI_SetEyesMove(ESpriteID actor, float x, float y)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetEyesMove);

	req->unknown001 = actor;
	req->unknown009 = x;
	req->unknown010 = y;

	AblBufferPostSemaphore();
}
void AnimAPI_SetEyesShape(ESpriteID actor, int shape)
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetEyesShape);

	req->unknown001 = actor;
	req->unknown007 = shape;

	AblBufferPostSemaphore();
}
void AnimAPI_SetGazeTracking()
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_SetGazeTracking);

	//undone

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

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_TurnInPlace()
{
	ShouldntBeHere("AnimAPI_TurnInPlace is an obsolete function, use PerformWalkStep");
}
void AnimAPI_WalkStep(
	ESpriteID actor, EWalkType type, float x, float y, float z, float f1, //in
	float* xout, float* yout, float* zout)//out
{
	AblRequest* req;
	GET_ABLREQUEST(req, eAblRequestType_WalkStep);

	req->unknown001 = actor;
	req->unknown002 = type;
	req->unknown009 = x;
	req->unknown010 = y;
	req->unknown011 = z;
	req->unknown012 = f1;

	g_ablrequestbuffer->SaveRequestInTrace(req);

	HANDLE hevent = OpenEvent(EVENT_ALL_ACCESS, FALSE, req->name);

	AblBufferPostSemaphore();
	AblBufferWaitSemaphore(hevent, __func__);

	GetAccessToRequestBuffer("AnimAPI_WalkStep 2");
	g_ablrequestbuffer->GetRequestPtrByIndex(reqindex, req);
	animengine_assert(req->active == 2);

	*xout = req->unknown013;
	*yout = req->unknown014;
	*zout = req->unknown015;

	req->active = 0;

	AblBufferPostSemaphore();
}


int AnimAPI_GetAllHeldObjects()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetAllHeldObjects);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetAllObjectPickupPositions()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetAllObjectPickupPositions);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetAllObjectPickupRotations()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetAllObjectPickupRotations);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetAllObjectPositions()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetAllObjectPositions);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetAllObjectRotations()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetAllObjectRotations);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetAllObjectStagingRotations()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetAllObjectStagingRotations);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetAllObjectStates()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetAllObjectStates);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetArmGestureBases()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetArmGestureBases);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetBrowShapeL()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetBrowShapeL);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetBrowShapeR()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetBrowShapeR);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetEyelidHeightL()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetEyelidHeightL);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetEyelidHeightR()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetEyelidHeightR);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetEyesMove()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetEyesMove);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetEyesShape()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetEyesShape);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetGazeTracking()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetGazeTracking);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetMouthShape()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetMouthShape);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetMyCopyOfAllAnimCues()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetMyCopyOfAllAnimCues);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetMyCopyOfAllObjectActivations()
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetMyCopyOfAllObjectActivations);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetObjectPickupPosition(ESpriteID actor, ESpriteID object, float* x, float* y, float* z)
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetObjectPickupPosition);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetObjectPosition(ESpriteID object, float* x, float* y, float* z)
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetObjectPosition);
	WAIT_ABLREQUEST();

	*x = req->unknown009;
	*y = req->unknown010;
	*z = req->unknown011;
	req->active = 0;

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetObjectRotation(ESpriteID object, float* x, float* y, float* z)
{
	AblRequest* req;
	GET_ABLREQUEST_RET(req, eAblRequestType_GetObjectRotation);
	WAIT_ABLREQUEST();

	*x = req->unknown009;
	*y = req->unknown010;
	*z = req->unknown011;
	req->active = 0;

	AblBufferPostSemaphore();

	return 0;
}


}
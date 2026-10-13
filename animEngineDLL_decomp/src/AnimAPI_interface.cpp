#include "facadecommon.h"
#include "classes/AblRequestBuffer.h"

#include <string>

#define GET_ABLREQUEST(reqtype)										\
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

#define GET_ABLREQUEST_RET(reqtype)									\
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

struct perfinfo_animapi_input
{
	int a2;
	int a3;
	int a4;
	int a5;
	int a6;
	int a7;
	int a8;
	int a9;
	int a10;
	int a11;
	int a12;
	int a13;
	int a14;
	int a15;
	int a16;
	int a17;
	int a18;
	int a19;
	int a20;
	int a21;
	int a22;
	int a23;
	int a24;
	int a25;
	int a26;
	int a27;
	int a28;
	int a29;
	int a30;
	int a31;
	int a32;
	int a33;
	int a34;
	int a35;
	int a36;
	int a37;
	int a38;
	int a39;
	int a40;
	int a41;
	int a42;
	int a43;
	int a44;
	int a45;
	int a46;
	int a47;
	int a48;
	int a49;
	int a50;
	int a51;
	int a52;
	int a53;
	int a54;
	int a55;
	int a56;
	int a57;
	int a58;
	int a59;
	int a60;
	int a61;
	int a62;
	int a63;
	int a64;
	int a65;
	int a66;
	int a67;
	int a68;
	int a69;
	int a70;
	int a71;
	int a72;
	int a73;
	int a74;
	int a75;
	int a76;
	int a77;
	int a78;
	int a79;
	int a80;
	int a81;
	int a82;
	int a83;
	int a84;
	int a85;
};


extern "C" {

void AnimAPI_AbortAnimationScripts(ESpriteID charid, EAnimLayer layer)
{
	GET_ABLREQUEST(eAblRequestType_AbortAnimationScripts);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_AbortTurnInPlace()
{
	ShouldntBeHere("AnimAPI_AbortTurnInPlace is an obsolete function, use PerformAbortWalkStep");
}
void AnimAPI_AbortWalkStep(ESpriteID charid)
{
	GET_ABLREQUEST(eAblRequestType_AbortWalkStep);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_AddEventInfoToLog()
{
	GET_ABLREQUEST(eAblRequestType_AddEventInfoToLog);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_DoFullExpressionBaseAnimation(ESpriteID charid, EFullExpressionBase base, int i1, int i2)
{
	GET_ABLREQUEST(eAblRequestType_DoFullExpressionBaseAnimation);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_DoFullExpressionMoodAnimation(ESpriteID charid, EFullExpressionMood mood, int i1, int i2)
{
	GET_ABLREQUEST(eAblRequestType_DoFullExpressionMoodAnimation);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_DoGestureAnimation(ESpriteID charid, EGestureBodyPart bodypart, 
	int i1)
{
	GET_ABLREQUEST(eAblRequestType_DoGestureAnimation);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_DoPickupObjGestureAnimation(ESpriteID charid, EGestureBodyPart bodypart, 
	int i1, int i2, int i3)
{
	GET_ABLREQUEST(eAblRequestType_DoPickupObjGestureAnimation);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_DoPutdownObjGestureAnimation(ESpriteID charid, EGestureBodyPart bodypart, 
	int i1, int i2, int i3, int i4, int i5, int i6, int i7)
{
	GET_ABLREQUEST(eAblRequestType_DoPutdownObjGestureAnimation);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_MiscLittleAction(ESpriteID spriteid, EMiscLittleAction action, float param1, float param2)
{
	GET_ABLREQUEST(eAblRequestType_MiscLittleAction);

	req->unknown001 = spriteid;
	req->unknown002 = action;
	req->unknown003 = param1;
	req->unknown009 = param2;

	AblBufferPostSemaphore();
}
void AnimAPI_Music(int musicaction, int musicid)
{
	GET_ABLREQUEST(eAblRequestType_Music);

	req->unknown002 = musicaction;
	req->unknown018 = musicid;

	AblBufferPostSemaphore();
}
void AnimAPI_PlaySoundEffect(int sfx)
{
	GET_ABLREQUEST(eAblRequestType_PlaySoundEffect);

	req->unknown002 = sfx;

	AblBufferPostSemaphore();
}
void AnimAPI_PushAnimationUScript(ESpriteID actor, int param1, int param2)
{
	GET_ABLREQUEST(eAblRequestType_PushAnimationUScript);

	req->unknown001 = actor;
	req->unknown002 = param1;
	req->unknown006 = (EAnimLayer)param2;

	g_ablrequestbuffer->SaveRequestInTrace(req);
	AblBufferPostSemaphore();
}
void AnimAPI_PushMultipleAnimationScripts(ESpriteID actor, 
	int script1Index, int script2Index, int script3Index, int script4Index,
	EAnimLayer layer)
{
	GET_ABLREQUEST(eAblRequestType_PushMultipleAnimationScripts);

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
	GET_ABLREQUEST(eAblRequestType_PushPoseScript);

	req->unknown001 = actor;
	req->unknown006 = eAnimLayer_base;
	req->unknown002 = param1;

	AblBufferPostSemaphore();
}
void AnimAPI_ReactiveWalkStep(ESpriteID actor, ESpriteID spriteid, bool stop)
{
	GET_ABLREQUEST(eAblRequestType_ReactiveWalkStep);

	req->unknown001 = actor;
	req->unknown000 = spriteid;
	req->unknown018 = stop;

	AblBufferPostSemaphore();
}
void AnimAPI_ResetMyCopyOfAllAnimCues(ESpriteID actor)
{
	GET_ABLREQUEST(eAblRequestType_ResetMyCopyOfAllAnimCues);



	AblBufferPostSemaphore();
}
void AnimAPI_ResetMyCopyOfAllObjectActivations(ESpriteID actor)
{
	AblRequest* req;												
		
	GetAccessToRequestBuffer(__func__);								
	
	int reqindex = g_ablrequestbuffer->GetFreeRequestPtr(req);	
	if (!req)														
	{																
		animengine_assert(0);										
		return;														
	}																
	req->type = eAblRequestType_ResetMyCopyOfAllObjectActivations;
	req->active = 1;												
	req->requesttime = GetTickCount();



	AblBufferPostSemaphore();
}
void AnimAPI_SetAnimEngineInfo(ESetAnimEngineInfoType type, int info)
{
	GET_ABLREQUEST(eAblRequestType_SetAnimEngineInfo);

	req->unknown018 = type;
	req->unknown019 = info;

	AblBufferPostSemaphore();
}
void AnimAPI_SetBrowShapeL(ESpriteID actor, int shape)
{
	GET_ABLREQUEST(eAblRequestType_SetBrowShapeL);

	req->unknown001 = actor;
	req->unknown007 = shape;

	AblBufferPostSemaphore();
}
void AnimAPI_SetBrowShapeR(ESpriteID actor, int shape)
{
	GET_ABLREQUEST(eAblRequestType_SetBrowShapeR);

	req->unknown001 = actor;
	req->unknown007 = shape;

	AblBufferPostSemaphore();
}
void AnimAPI_SetEyelidHeightL(ESpriteID actor, float height, float unused)
{
	GET_ABLREQUEST(eAblRequestType_SetEyelidHeightL);

	req->unknown001 = actor;
	req->unknown010 = height;
	req->unknown016 = unused;

	AblBufferPostSemaphore();
}
void AnimAPI_SetEyelidHeightR(ESpriteID actor, float height, float unused)
{
	GET_ABLREQUEST(eAblRequestType_SetEyelidHeightR);

	req->unknown001 = actor;
	req->unknown010 = height;
	req->unknown016 = unused;

	AblBufferPostSemaphore();
}
void AnimAPI_SetEyesMove(ESpriteID actor, float x, float y)
{
	GET_ABLREQUEST(eAblRequestType_SetEyesMove);

	req->unknown001 = actor;
	req->unknown009 = x;
	req->unknown010 = y;

	AblBufferPostSemaphore();
}
void AnimAPI_SetEyesShape(ESpriteID actor, int shape)
{
	GET_ABLREQUEST(eAblRequestType_SetEyesShape);

	req->unknown001 = actor;
	req->unknown007 = shape;

	AblBufferPostSemaphore();
}
void AnimAPI_SetGazeTracking(ESpriteID actor, ESpriteID obj, 
	int a3, int a4, int a5, int a6, int a7, int a8)
{
	GET_ABLREQUEST(eAblRequestType_SetGazeTracking);

	//undone

	AblBufferPostSemaphore();
}
void AnimAPI_SetMouthShape(ESpriteID actor, int shape)
{
	GET_ABLREQUEST(eAblRequestType_SetMouthShape);

	req->unknown001 = actor;
	req->unknown007 = shape;

	AblBufferPostSemaphore();
}
void AnimAPI_SetObjectPosition(ESpriteID object, float x, float y, float z)
{
	GET_ABLREQUEST(eAblRequestType_SetObjectPosition);

	req->unknown000 = object;
	req->unknown009 = x;
	req->unknown010 = y;
	req->unknown011 = z;

	AblBufferPostSemaphore();
}
void AnimAPI_SetObjectRotation(ESpriteID object, float x, float y, float z)
{
	GET_ABLREQUEST(eAblRequestType_SetObjectRotation);

	req->unknown000 = object;
	req->unknown009 = x;
	req->unknown010 = y;
	req->unknown011 = z;

	AblBufferPostSemaphore();
}
void AnimAPI_SetObjectToHold(ESpriteID actor, int a2, ESpriteID object)
{
	GET_ABLREQUEST(eAblRequestType_SetObjectToHold);

	req->unknown001 = actor;
	req->unknown008 = a2;
	req->unknown000 = object;

	AblBufferPostSemaphore();
}
void AnimAPI_SetPerformanceInfo(ESpriteID actor, perfinfo_animapi_input input)
{
	GET_ABLREQUEST(eAblRequestType_SetPerformanceInfo);

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
	GET_ABLREQUEST(eAblRequestType_WalkStep);

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


int AnimAPI_GetAllHeldObjects(void* dest)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetAllHeldObjects);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetAllObjectPickupPositions(ESpriteID actor, void* dest)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetAllObjectPickupPositions);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetAllObjectPickupRotations(ESpriteID actor, void* dest)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetAllObjectPickupRotations);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetAllObjectPositions(void* dest)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetAllObjectPositions);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetAllObjectRotations(void* dest)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetAllObjectRotations);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetAllObjectStagingRotations(void* dest)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetAllObjectStagingRotations);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetAllObjectStates(void* dest)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetAllObjectStates);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetArmGestureBases(ESpriteID actor, int* a1, int* a2, int* a3)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetArmGestureBases);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetBrowShapeL(ESpriteID actor, int* a1)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetBrowShapeL);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetBrowShapeR(ESpriteID actor, int* a1)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetBrowShapeR);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetEyelidHeightL(ESpriteID actor, float* a1)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetEyelidHeightL);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetEyelidHeightR(ESpriteID actor, float* a1)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetEyelidHeightR);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetEyesMove(ESpriteID actor, float* a1, float* a2)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetEyesMove);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetEyesShape(ESpriteID actor, int* a1)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetEyesShape);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetGazeTracking(ESpriteID actor, int* a1)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetGazeTracking);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetMouthShape(ESpriteID actor, int* a1)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetMouthShape);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetMyCopyOfAllAnimCues(ESpriteID actor, void* dest)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetMyCopyOfAllAnimCues);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetMyCopyOfAllObjectActivations(ESpriteID actor, void* dest)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetMyCopyOfAllObjectActivations);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetObjectPickupPosition(ESpriteID actor, ESpriteID object, float* x, float* y, float* z)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetObjectPickupPosition);
	WAIT_ABLREQUEST();

	//undone

	AblBufferPostSemaphore();

	return 0;
}
int AnimAPI_GetObjectPosition(ESpriteID object, float* x, float* y, float* z)
{
	GET_ABLREQUEST_RET(eAblRequestType_GetObjectPosition);
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
	GET_ABLREQUEST_RET(eAblRequestType_GetObjectRotation);
	WAIT_ABLREQUEST();

	*x = req->unknown009;
	*y = req->unknown010;
	*z = req->unknown011;
	req->active = 0;

	AblBufferPostSemaphore();

	return 0;
}


}
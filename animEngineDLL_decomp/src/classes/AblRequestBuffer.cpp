#include "facadecommon.h"
#include "classes/AblRequestBuffer.h"
#include "ABLRequestsPerform_impl.h"

static bool g_ToldAblPlayerInfo = false;

void AblRequestBuffer::InitBuffer(void* mappedmemptr, bool resetmemory)
{
	m_pAblRequests = (AblRequest*)mappedmemptr;
	unknown001 = (unknownablstructure1*)(&m_pAblRequests[REQUESTBUFFERSIZE]);
	unknown002 = (unknownablstructure2*)(&unknown001[1]);

	if (resetmemory)
	{
		for (int i = 0; i < REQUESTBUFFERSIZE; i++)
		{
			sprintf(m_pAblRequests[i].name, "FacadeEvent%i", i);
			HANDLE event = CreateEvent(NULL, FALSE, FALSE, m_pAblRequests[i].name);
			animengine_assert(event != 0);
			m_pAblRequests[i].active = 0;
		}
		for (int i = 0; i < 1000; i++)
		{
			unknown002[i].requesttime = 0;
		}
		for (int i = 0; i < 3; i++)
		{
			PerformResetMyCopyOfAllAnimCues(i);
			PerformResetMyCopyOfAllObjectActivations(i);
		}
	}
}

int AblRequestBuffer::PerformAllRequests()
{
	int performedrequests = 0;

	//for (int i = 0; i < 2; ++i)
	//{
	//	for (j = 0; j < 8; ++j)
	//		*((_BYTE*)&unknown003 + 8 * i + j) = 0;
	//	for (j = 0; j < 4; ++j)
	//		*((_BYTE*)&unknown007 + 4 * i + j) = 0;
	//	*((_BYTE*)&unknown009 + i) = 0;
	//	*((_BYTE*)&unknown009 + i + 2) = 0;
	//	*((_BYTE*)&unknown010 + i) = 0;
	//	*((_BYTE*)&unknown010 + i + 2) = 0;
	//}

	for (int i = 0; i < REQUESTBUFFERSIZE; ++i)	performedrequests += PerformRequest(i, true);
	for (int i = 0; i < REQUESTBUFFERSIZE; ++i)	performedrequests += PerformRequest(i, false);

	PerformAnyRequestedCueTriggeredPerformance(eSpriteID_grace);
	PerformAnyRequestedCueTriggeredPerformance(eSpriteID_trip);

	//if (!g_ToldAblPlayerInfo && IsAblConnected() && g_playerNameID >= 0)
	//{
	//	g_ToldAblPlayerInfo = true;
	//	AblBufferPostSemaphore();
	//	bool playerentered = AblCallbackAPI_PlayerGestured(ePlayerGesture_playerInfoEntered, g_playerNameID, g_playerGender);
	//	GetAccessToRequestBuffer("PerformAllRequests playergestured");
	//	if (!playerentered)
	//		dprintf("ERROR: ASSERTFAIL_ABL_CALLBACK_CONFUSION");
	//}

	//undone

	return performedrequests;
}

bool AblRequestBuffer::CheckForBaseAnimRequestsQueuedUp(int unknown)
{
	//undone
	return false;
}
bool AblRequestBuffer::CheckForCallbacksPending(EAblRequestType type, int unknown1, int unknown2)
{
	//undone
	return false;
}
bool AblRequestBuffer::DoesAnimNeedToWaitForLocomotionToFinish(ESpriteID sprite, int unknown)
{
	//undone
	return false;
}

int AblRequestBuffer::GetFreeRequestPtr(AblRequest*& out)
{
	for (int i = 0; i < REQUESTBUFFERSIZE; ++i)
	{
		if (!m_pAblRequests[i].active)
		{
			out = &m_pAblRequests[i];
			return i;
		}
	}
	ShouldntBeHere("no more free request slots in AblRequestBuffer::GetFreeRequestPtr()");
	out = nullptr;
	return -1;
}
void AblRequestBuffer::GetRequestPtrByIndex(int i, AblRequest*& out)
{
	animengine_assert(i < REQUESTBUFFERSIZE);
	out = &m_pAblRequests[i];
}
void AblRequestBuffer::DeactiveWalkstepCallbacks(ESpriteID spriteid)
{
	for (int i = 0; i < REQUESTBUFFERSIZE; ++i)
	{
		AblRequest* buf = &this->m_pAblRequests[i];
		if (buf->type == eAblRequestType_CallbackWhenWalkIsDone && buf->unknown001 == spriteid && buf->active == 1)
			buf->active = 0;
	}
}

void AblRequestBuffer::PerformAnyRequestedCueTriggeredPerformance(ESpriteID spriteid)
{
	//undone
}
int AblRequestBuffer::PerformRequest(int reqindex, bool abortonly)
{
	AblRequest* req = &m_pAblRequests[reqindex];
	if (abortonly
		&& req->type != eAblRequestType_AbortAnimationScripts
		&& req->type != eAblRequestType_AbortWalkStep)
		return 0;

	int performs = 0;
    bool didcallback = false;
    bool pushedanim = false;
    AblRequest* reqptr;

	if (req->active == 1)
	{
		switch (req->type)
		{
		case eAblRequestType_SetObjectPosition: 
			PerformSetObjectPosition(req->unknown000, req->unknown009, req->unknown010, req->unknown011);
			break;
		case eAblRequestType_SetObjectRotation:
			PerformSetObjectRotation(req->unknown000, req->unknown009, req->unknown010, req->unknown011);
			break;
        case eAblRequestType_PushMultipleAnimationScripts:
        case eAblRequestType_PushAnimationUScript:
        case eAblRequestType_PushPoseScript:
            if (DoesAnimNeedToWaitForLocomotionToFinish(req->unknown001, req->unknown006))
            {
                PerformAbortWalkStep(req->unknown001);
                DeactiveWalkstepCallbacks(req->unknown001);
                pushedanim = true;
            }
            switch (req->type)
            {
            case eAblRequestType_PushMultipleAnimationScripts:
                PerformPushMultipleAnimationScripts(
                    req->unknown001,
                    req->unknown002,
                    req->unknown003,
                    req->unknown004,
                    req->unknown005,
                    req->unknown006);
                break;
            case eAblRequestType_PushAnimationUScript:
                //PushAnimationUScript(req->unknown001, req->unknown002, req->unknown006);
                break;
            case eAblRequestType_PushPoseScript:
                PerformPushPoseScript(req->unknown001, (EBodyPoseConfig)req->unknown002);
                break;
            }
            ++performs;
            GetFreeRequestPtr(reqptr);
            if (reqptr)
            {
                reqptr->requesttime = req->requesttime;
                reqptr->unknown001 = req->unknown001;
                reqptr->unknown006 = req->unknown006;
                reqptr->type = eAblRequestType_CallbackWhenAnimationDone;
                reqptr->active = 1;
            }
            else
            {
                animengine_assert(0);
                return performs;
            }
            break;
        case eAblRequestType_CallbackWhenAnimationDone:
            didcallback = 1;
            animengine_assert(req->unknown001 <= eSpriteID_plant);
            animengine_assert(req->unknown006 < eNumAnimLayers);

            //animengine_assert(*((_BYTE*)&v22->unknown003 + 8 * req->unknown001 + req->unknown006) == 0);
            //*((_BYTE*)&v22->unknown003 + 8 * req->unknown001 + req->unknown006) = 1;
            //if (PerformCallbackWhenAnimationDone(req->unknown001, req->unknown006, (int)v21))
            //    req->active = 0;
            break;
        case eAblRequestType_AbortAnimationScripts:
            PerformAbortAnimationScripts(req->unknown001, req->unknown006);
            ++performs;
            for (int i = 0; i < 100; ++i)
            {
                reqptr = &m_pAblRequests[i];
                if ((reqptr->type == eAblRequestType_CallbackWhenAnimationDone
                    || reqptr->type == eAblRequestType_PushMultipleAnimationScripts
                    || reqptr->type == eAblRequestType_PushAnimationUScript
                    || reqptr->type == eAblRequestType_PushPoseScript)
                    && reqptr->unknown001 == req->unknown001
                    && (reqptr->unknown006 == req->unknown006 || req->unknown006 == -1)
                    && reqptr->active == 1)
                {
                    reqptr->active = 0;
                }
            }
            break;
        case eAblRequestType_DoGestureAnimation:
        case eAblRequestType_DoPickupObjGestureAnimation:
        case eAblRequestType_DoPutdownObjGestureAnimation:
            //undone
            break;
        case eAblRequestType_CallbackWhenGestureDone:
            //undone
            break;
        case eAblRequestType_DoFullExpressionBaseAnimation:
            //undone
            break;
        case eAblRequestType_DoFullExpressionMoodAnimation:
            //undone
            break;
        case eAblRequestType_CallbackWhenFullExpressionBaseDone:
            //undone
            break;
        case eAblRequestType_CallbackWhenFullExpressionMoodDone:
            //undone
            break;
        case eAblRequestType_SetGazeTracking:
            PerformSetGazeTracking(
                req->unknown001,
                req->unknown000,
                req->unknown009,
                req->unknown010,
                req->unknown011,
                (EGazeSpeed)req->unknown018,
                (EGazeType)req->unknown019,
                (EAlterHeadCock)req->unknown020,
                false);
            ++performs;
            break;
        case eAblRequestType_SetEyelidHeightL:
            PerformSetEyelidHeightL(req->unknown001, req->unknown010/*, req->unknown016*/);
            ++performs;
            break;
        case eAblRequestType_SetEyelidHeightR:
            PerformSetEyelidHeightR(req->unknown001, req->unknown010/*, req->unknown016*/);
            ++performs;
            break;
        case eAblRequestType_SetBrowShapeL:
            PerformSetBrowShapeL(req->unknown001, req->unknown007);
            ++performs;
            break;
        case eAblRequestType_SetBrowShapeR:
            PerformSetBrowShapeR(req->unknown001, req->unknown007);
            ++performs;
            break;
        case eAblRequestType_SetMouthShape:
            PerformSetMouthShape(req->unknown001, req->unknown007);
            ++performs;
            break;
        case eAblRequestType_SetEyesShape:
            PerformSetEyesShape(req->unknown001, req->unknown007);
            ++performs;
            break;
        case eAblRequestType_MiscLittleAction:
            PerformMiscLittleAction(
                req->unknown001,
                (EMiscLittleAction)req->unknown002,
                req->unknown003,
                req->unknown009);
            ++performs;
            break;
        case eAblRequestType_SetObjectToHold:
            PerformSetObjectToHold(req->unknown001, req->unknown008, req->unknown000);
            ++performs;
            break;
        case eAblRequestType_ReactiveWalkStep:
            PerformReactiveWalkStep(req->requesttime, req->unknown001, req->unknown000, req->unknown018);
            ++performs;
            GetFreeRequestPtr(reqptr);
            animengine_assert(reqptr != 0);
            reqptr->requesttime = req->requesttime;
            reqptr->unknown001 = req->unknown001;
            reqptr->type = eAblRequestType_CallbackWhenWalkIsDone;
            reqptr->active = 1;
            break;
        case eAblRequestType_AbortWalkStep:
            PerformAbortWalkStep(req->unknown001);
            ++performs;
            DeactiveWalkstepCallbacks(req->unknown001);
            break;
        case eAblRequestType_CallbackWhenWalkIsDone:
            //undone
            break;
        case eAblRequestType_PlaySoundEffect:
            PerformPlaySoundEffect((EGlobalSounds)req->unknown002);
            ++performs;
            break;
        case eAblRequestType_Music:
            PerformMusic((EMusicAction)req->unknown002, (EMusicName)req->unknown018);
            ++performs;
            break;
        case eAblRequestType_ResetMyCopyOfAllAnimCues:
            PerformResetMyCopyOfAllAnimCues(req->unknown001);
            break;
        case eAblRequestType_ResetMyCopyOfAllObjectActivations:
            PerformResetMyCopyOfAllObjectActivations(req->unknown001);
            break;
        case eAblRequestType_SetAnimEngineInfo:
            PerformSetAnimEngineInfo((ESetAnimEngineInfoType)req->unknown018, req->unknown019);
            ++performs;
            break;
        case eAblRequestType_SetPerformanceInfo:
            PerformSetPerformanceInfo(req->unknown001, &req->unknown021);
            ++performs;
            break;
        case eAblRequestType_AddEventInfoToLog:
            ++performs;
            break;
        case eAblRequestType_GetObjectPosition:
            PerformGetObjectPosition(req->unknown000, req->unknown009, req->unknown010, req->unknown011);
            ++performs;
            break;
        case eAblRequestType_GetObjectRotation:
            PerformGetObjectRotation(req->unknown000, req->unknown009, req->unknown010, req->unknown011);
            ++performs;
            break;
        case eAblRequestType_GetObjectPickupPosition:
            PerformGetObjectPickupPosition(
                req->unknown000,
                req->unknown001,
                req->unknown009,
                req->unknown010,
                req->unknown011);
            ++performs;
            break;
        case eAblRequestType_GetGazeTracking:
            PerformGetGazeTracking(req->unknown001, req->unknown000);
            ++performs;
            break;
        case eAblRequestType_GetEyelidHeightL:
            PerformGetEyelidHeightL(req->unknown001, req->unknown010);
            ++performs;
            break;
        case eAblRequestType_GetEyelidHeightR:
            PerformGetEyelidHeightR(req->unknown001, req->unknown010);
            ++performs;
            break;
        case eAblRequestType_GetBrowShapeL:
            PerformGetBrowShapeL(req->unknown001, req->unknown007);
            ++performs;
            break;
        case eAblRequestType_GetBrowShapeR:
            PerformGetBrowShapeR(req->unknown001, req->unknown007);
            ++performs;
            break;
        case eAblRequestType_GetMouthShape:
            PerformGetMouthShape(req->unknown001, req->unknown007);
            ++performs;
            break;
        case eAblRequestType_GetEyesShape:
            PerformGetEyesShape(req->unknown001, req->unknown007);
            ++performs;
            break;
        case eAblRequestType_GetEyesMove:
            PerformGetEyesMove(req->unknown001, req->unknown009, req->unknown010);
            ++performs;
            break;
        case eAblRequestType_GetArmGestureBases:
            PerformGetArmGestureBases(req->unknown001, req->unknown018, req->unknown019, req->unknown020);
            ++performs;
            break;
        case eAblRequestType_WalkStep:
            PerformWalkStep(
                req->requesttime,
                req->unknown001,
                (EWalkType)req->unknown002,
                req->unknown009,
                req->unknown010,
                req->unknown011,
                *(float*)&req->unknown012,
                req->unknown013,
                req->unknown014,
                req->unknown015);
            ++performs;
            GetFreeRequestPtr(reqptr);
            animengine_assert(reqptr != 0);
            reqptr->requesttime = req->requesttime;
            reqptr->unknown001 = req->unknown001;
            reqptr->type = eAblRequestType_CallbackWhenWalkIsDone;
            reqptr->active = 1;
            break;
        case eAblRequestType_GetAllObjectPositions:
            g_lastFrameGotAblRequest = g_numFramesRun;
            PerformGetAllObjectPositions(req->unknown106);
            break;
        case eAblRequestType_GetAllObjectRotations:
            PerformGetAllObjectRotations(req->unknown106);
            break;
        case eAblRequestType_GetAllObjectStagingRotations:
            PerformGetAllObjectStagingRotations(req->unknown106);
            break;
        case eAblRequestType_GetAllObjectPickupPositions:
            PerformGetAllObjectPickupPositions(req->unknown001, req->unknown106);
            break;
        case eAblRequestType_GetAllObjectPickupRotations:
            PerformGetAllObjectPickupRotations(req->unknown001, req->unknown106);
            break;
        case eAblRequestType_GetAllObjectStates:
            PerformGetAllObjectStates(req->unknown302);
            break;
        case eAblRequestType_GetAllHeldObjects:
            PerformGetAllHeldObjects(req->unknown400);
            break;
        case eAblRequestType_GetMyCopyOfAllAnimCues:
            PerformGetMyCopyOfAllAnimCues(req->unknown001, req->unknown596);
            break;
        case eAblRequestType_GetMyCopyOfAllObjectActivations:
            PerformGetMyCopyOfAllObjectActivations(req->unknown001, req->unknown620);
            break;
        default:
            ShouldntBeHere("unknown abl request type in PerformAllRequests()");
            break;
        }

        if (!didcallback && !pushedanim)
        {
            if (req->type > (unsigned int)eAblRequestType_AddEventInfoToLog)
            {
                req->active = 2;
                HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, req->name);
                SetEvent(hEvent);
                CloseHandle(hEvent);
            }
            else
            {
                req->active = 0;
            }
        }
	}

	return performs;
}

void AblRequestBuffer::PrintTraceBuffer()
{
	//undone
}
void AblRequestBuffer::SaveRequestInTrace(AblRequest* req)
{
	//undone
}

void AblRequestBuffer::QueueUpRequestedPerformance(ESpriteID spriteid, /*PerformanceInfo*/void* performanceinfo, int randomnum)
{
	//undone
}
void AblRequestBuffer::TickDownAndPerformRequestedPerformances(ESpriteID spriteid)
{
	//undone
}
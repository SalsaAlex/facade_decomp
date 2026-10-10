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

	if (!g_ToldAblPlayerInfo && IsAblConnected() && g_playerNameID >= 0)
	{
		g_ToldAblPlayerInfo = true;
		AblBufferPostSemaphore();
		bool playerentered = AblCallbackAPI_PlayerGestured(ePlayerGesture_playerInfoEntered, g_playerNameID, g_playerGender);
		GetAccessToRequestBuffer("PerformAllRequests playergestured");
		if (!playerentered)
			dprintf("ERROR: ASSERTFAIL_ABL_CALLBACK_CONFUSION");
	}

	//undone
}
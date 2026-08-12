#include "facadecommon.h"
#include "classes/AblRequestBuffer.h"

void AblRequestBuffer::InitBuffer(void* mappedmemptr, bool resetmemory)
{
	m_pAblRequests = (AblRequest*)mappedmemptr;
	unknown001 = (unknownablstructure1*)(m_pAblRequests + REQUESTBUFFERSIZE);
	unknown002 = (unknownablstructure2*)(unknown001 + 1);

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
			//PerformResetMyCopyOfAllAnimCues(k);
			//PerformResetMyCopyOfAllObjectActivations(k);
		}
	}
}
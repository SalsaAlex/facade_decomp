#include "AblRequestBuffer.h"

AblRequestBuffer* GetAblRequestBuffer()
{
	static AblRequestBuffer* reqbuffer = nullptr;
	if (!reqbuffer)
		reqbuffer = new AblRequestBuffer();

	return reqbuffer;
}

int AblRequestBuffer::GetFreeRequestPtr(AblRequestEntry** pRequest)
{
	for (int i = 0; i < MAX_ABL_REQUESTS; i++)
	{
		if (!m_Requests[i].m_bRequestActive)
		{
			*pRequest = &m_Requests[i];
			return i;
		}
	}
	printfunc("no more free request slots in AblRequestBuffer::GetFreeRequestPtr()");
	*pRequest = nullptr;
	return -1;
}
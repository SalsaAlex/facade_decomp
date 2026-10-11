#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"

HANDLE g_nativeInterfaceStateHandle;
HANDLE hFileMapping;

LPCVOID activeAnimationLayers;



int DllMain(HINSTANCE instDLL, int reason)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		//undone
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		dprintf("Abl native interface detaching");
		UnmapViewOfFile(activeAnimationLayers);
		CloseHandle(hFileMapping);
	}
	return 1;
}
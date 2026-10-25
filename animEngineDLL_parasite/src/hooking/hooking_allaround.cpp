#include "facadecommon.h"

#include "minhook/include/MinHook.h"

#define IDAPRO_IMAGEBASE 0x10000000 //ex: 0x400000

static void* s_baseprocaddr;

void HookFunc(void* funcaddr, void* ourfunc)
{
    MH_CreateHook((void*)((uint32_t)s_baseprocaddr+((uint32_t)funcaddr - IDAPRO_IMAGEBASE)), ourfunc, NULL);
    MH_EnableHook((void*)((uint32_t)s_baseprocaddr+((uint32_t)funcaddr - IDAPRO_IMAGEBASE)));
}

void* GetVarAddr(void* varaddr){
	return (void*)((uint32_t)s_baseprocaddr+((uint32_t)varaddr - IDAPRO_IMAGEBASE));
}


void HookingInit(void* baseprocaddr)
{
    MH_Initialize();
    s_baseprocaddr = baseprocaddr;
}
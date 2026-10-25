#include "windows.h"
#include "stdio.h"

typedef int(__cdecl*t_AnimEngineWinMain)(HINSTANCE, HINSTANCE, LPSTR, int);

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
	HMODULE moduledll = LoadLibrary("animEngineDLL.dll");
    HMODULE parasitedll = LoadLibrary("animEngineDLL_parasite.dll"); //it'll automatically hook to animenginedll.dll
	t_AnimEngineWinMain winmain = (t_AnimEngineWinMain)GetProcAddress(moduledll, "?AnimEngineWinMain@@YAHPAUHINSTANCE__@@0PADH@Z");
    winmain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
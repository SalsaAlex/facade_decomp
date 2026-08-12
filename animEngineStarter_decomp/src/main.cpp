#include "windows.h"
#include "stdio.h"

typedef WPARAM(*t_AnimEngineWinMain)(HINSTANCE hInstance);

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
	HMODULE moduledll = LoadLibrary("animEngineDLL.dll");
	t_AnimEngineWinMain winmain = (t_AnimEngineWinMain)GetProcAddress(moduledll, "?AnimEngineWinMain@@YAHPAUHINSTANCE__@@0PADH@Z");
    winmain(hInstance);
}
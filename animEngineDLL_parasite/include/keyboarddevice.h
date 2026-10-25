#ifndef KEYBOARDDEVICE_H
#define KEYBOARDDEVICE_H

#include "facadecommon.h"
#include "dinput.h"

HRESULT Keyboard_Init(void);
void Keyboard_Shutdown(void);

HRESULT ReadBufferedKeyboardData();

DEFINE_HOOKEDVAR(IDirectInput8*, g_pDI, 0x10979494)
DEFINE_HOOKEDVAR(IDirectInputDevice8*, g_pKeyboard, 0x10979498)

#define g_pDI _g_pDI()
#define g_pKeyboard _g_pKeyboard()


#endif // KEYBOARDDEVICE_H
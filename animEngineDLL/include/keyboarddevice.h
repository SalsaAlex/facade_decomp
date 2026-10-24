#ifndef KEYBOARDDEVICE_H
#define KEYBOARDDEVICE_H

#include "facadecommon.h"
#include "dinput.h"

HRESULT Keyboard_Init(void);
void Keyboard_Shutdown(void);

HRESULT ReadBufferedKeyboardData();

extern IDirectInputDevice8* g_pKeyboard;


#endif // KEYBOARDDEVICE_H
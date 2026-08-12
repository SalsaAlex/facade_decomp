#ifndef KEYBOARDDEVICE_H
#define KEYBOARDDEVICE_H

#include "facadecommon.h"

HRESULT Keyboard_Init(void);
void FreeDirectInput(void);

HRESULT ReadBufferedKeyboardData();


#endif // KEYBOARDDEVICE_H
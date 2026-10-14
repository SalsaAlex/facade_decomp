#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H

#include "facadecommon.h"

bool DSound_Init();

int DSound_Load_WAV(const char* filename, uint32_t soundflags);

void DSound_Play(int sndbufferindex, int flags);

void DSound_Set_Volume(int soundindex, uint32_t vol);
bool DSound_Set_Pan(int soundindex, uint32_t pan);

bool DSound_Stop_Sound(int soundindex);
bool DSound_Delete_Sound(int soundindex);

bool DSound_Shutdown();


#endif // SOUNDDEVICE_H
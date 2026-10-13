#include "facadecommon.h"
#include "classes/SoundMgr.h"
#include "sounddevice.h"

bool SoundMgr::PlaySound(int, EGlobalSounds gsoundindex, bool)
{
    //undone

    return true;
}
int SoundMgr::AddSoundSource(SoundSource* src)
{
    for (int i = 0; i < 10; ++i)
    {
        if (!sndsources[i])
        {
            sndsources[i] = src;
            return i;
        }
    }

    ShouldntBeHere("Error: Too many sound sources");
    return -1;
}

void SoundMgr::SetSoundPan(int sourceid, int pan, int)
{
    //undone
}
void SoundMgr::SetSoundVolume(int sourceid, int volume, int)
{
    //undone
}

void SoundMgr::StopAllSounds(int sourceid)
{
    for (int i = 0; i < SOUNDMGR_MAXSOUNDS && sounds[i].soundsource_id != -1; ++i)
    {
        if (sounds[i].soundsource_id == sourceid)
            DSound_Stop_Sound(sounds[i].soundindex);
    }
}
void SoundMgr::StopSound(int sourceid, int unknown)
{
    for (int i = 0; i < SOUNDMGR_MAXSOUNDS && sounds[i].soundsource_id != -1; ++i)
    {
        if (sounds[i].soundsource_id == sourceid && sounds[i].unknown001 == unknown)
            DSound_Stop_Sound(sounds[i].soundindex);
    }
}

void SoundMgr::PlaySoundPart2(int)
{
    //undone
}
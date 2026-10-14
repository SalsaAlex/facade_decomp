#include "facadecommon.h"
#include "classes/SoundMgr.h"
#include "sounddevice.h"

#include "dsound.h"

bool SoundMgr::PlaySound(int sourceid, EGlobalSounds gsoundindex, bool option)
{
    unknown03 = sourceid;

    int i = 0;
    int ticknow = GetTickCount();
    int freeindex = 0;
    for (; i < 256 && sounds[i].soundsource_id != -1; ++i)
    {
        if (sounds[i].soundsource_id == sourceid && sounds[i].globalsndindex == gsoundindex)
        {
            PlaySoundPart2(i);
            return true;
        }
        if (sounds[i].sound_timestart < ticknow)
        {
            ticknow = sounds[i].sound_timestart;
            freeindex = i;
        }
    }


    if (i >= 256)
    {
        i = freeindex;
        DSound_Delete_Sound(sounds[freeindex].soundindex);
    }

    animengine_assert(gsoundindex >= eGlobalSound_FIRST_SOUND && gsoundindex <= eGlobalSound_LAST_SOUND);


    char filenamebuffer[256];
    sndsources[sourceid]->GetSoundFilename(gsoundindex, filenamebuffer, true);
    int dsound_index = DSound_Load_WAV(filenamebuffer, DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY);
    if (option)
        unknown04 = false;
    else
        unknown05 = false;

    if (dsound_index == -1)
        return 0;

    if (option)
        unknown04 = true;
    else
        unknown05 = true;

    sounds[i].soundindex = dsound_index;
    sounds[i].soundsource_id = sourceid;
    sounds[i].globalsndindex = gsoundindex;
    PlaySoundPart2(i);

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

void SoundMgr::SetSoundPan(int sourceid, int pan, EGlobalSounds gsoundindex)
{
    animengine_assert(pan <= 100 && pan >= -100);

    unknown02 = pan;
    if (gsoundindex == -1)
    {
        if (unknown03 == -1)
            return;
        gsoundindex = (EGlobalSounds)unknown03;
    }
    for (int i = 0; i < SOUNDMGR_MAXSOUNDS && sounds[i].soundsource_id != -1; ++i)
    {
        if (sounds[i].soundsource_id == sourceid && sounds[i].globalsndindex == gsoundindex)
        {
            pan *= 20;
            DSound_Set_Volume(sounds[i].soundindex, pan);
        }
    }
    //undone
}
void SoundMgr::SetSoundVolume(int sourceid, int volume, EGlobalSounds gsoundindex)
{
    animengine_assert(volume <= 100);

    unknown01 = volume;
    if (gsoundindex == -1)
    {
        if (unknown03 == -1)
            return;
        gsoundindex = (EGlobalSounds)unknown03;
    }
    for (int i = 0; i < SOUNDMGR_MAXSOUNDS && sounds[i].soundsource_id != -1; ++i)
    {
        if (sounds[i].soundsource_id == sourceid && sounds[i].globalsndindex == gsoundindex)
            DSound_Set_Volume(sounds[i].soundindex, volume);
    }
}

void SoundMgr::StopAllSounds(int sourceid)
{
    for (int i = 0; i < SOUNDMGR_MAXSOUNDS && sounds[i].soundsource_id != -1; ++i)
    {
        if (sounds[i].soundsource_id == sourceid)
            DSound_Stop_Sound(sounds[i].soundindex);
    }
}
void SoundMgr::StopSound(int sourceid, EGlobalSounds gsoundindex)
{
    for (int i = 0; i < SOUNDMGR_MAXSOUNDS && sounds[i].soundsource_id != -1; ++i)
    {
        if (sounds[i].soundsource_id == sourceid && sounds[i].globalsndindex == gsoundindex)
            DSound_Stop_Sound(sounds[i].soundindex);
    }
}

void SoundMgr::PlaySoundPart2(int sourceid)
{
    if (g_bMuteSounds)
        return;

    soundmgr_soundentry_t* entry = &sounds[sourceid];

    DSound_Play(entry->soundindex, 0);
    entry->sound_timestart = GetTickCount();
    SetSoundVolume(entry->soundsource_id, unknown01);
    SetSoundPan(entry->soundsource_id, unknown02);

}
#ifndef SOUNDMGR_H
#define SOUNDMGR_H

#undef PlaySound
#include "enums/EGlobalSounds.h"

class SoundSource;

struct soundmgr_soundentry_t
{
	int soundsource_id;
	int unknown001;
	int soundindex;
	int sound_timestart;
};


class SoundMgr
{
public:
	virtual ~SoundMgr() = default;

	void PlaySound(int, EGlobalSounds gsoundindex , bool);
	int AddSoundSource(SoundSource* src);

	void SetSoundPan(int sourceid, int pan, int);
	void SetSoundVolume(int sourceid, int volume, int);

	void StopAllSounds(int sourceid);
private:

	void PlaySoundPart2(int);

	SoundSource* sndsources[10];
	soundmgr_soundentry_t sounds[256];
	int unknown02[3];
	bool unknown03;
	bool unknown04;

protected:

};

static_assert(sizeof(SoundMgr) == 4156, "");					//proof: animEngineDLL.dll:.data:10978420 (g_soundMgr is 4156 bytes long)

#endif // SOUNDMGR_H
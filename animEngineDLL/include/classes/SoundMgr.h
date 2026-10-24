#ifndef SOUNDMGR_H
#define SOUNDMGR_H

#undef PlaySound

class SoundSource;

struct soundmgr_soundentry_t
{
	int soundsource_id;
	EGlobalSounds globalsndindex;
	int soundindex;
	int sound_timestart;
};

#define SOUNDMGR_MAXSOUNDS 256


class SoundMgr
{
public:
	virtual ~SoundMgr() = default;

	bool PlaySound(int sourceid, EGlobalSounds gsoundindex , bool);
	int AddSoundSource(SoundSource* src);

	void SetSoundPan(int sourceid, int pan, EGlobalSounds gsoundindex = (EGlobalSounds)-1);
	void SetSoundVolume(int sourceid, int volume, EGlobalSounds gsoundindex = (EGlobalSounds)-1);

	void StopAllSounds(int sourceid);
	void StopSound(int sourceid, EGlobalSounds gsoundindex);
private:

	void PlaySoundPart2(int sourceid);

	SoundSource* sndsources[10];
	soundmgr_soundentry_t sounds[SOUNDMGR_MAXSOUNDS];
	int unknown01;
	int unknown02;
	int unknown03;
	bool unknown04;
	bool unknown05;

protected:

};

static_assert(sizeof(SoundMgr) == 4156, "");					//proof: animEngineDLL.dll:.data:10978420 (g_soundMgr is 4156 bytes long)

#endif // SOUNDMGR_H
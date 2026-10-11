#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

struct soundsrc_nameentry_t
{
	char name[256];
};

#define MAX_SOUNDENTRIES 5000

class SoundSource
{
public:
	virtual ~SoundSource() = default;

	int GetNumSounds() { return m_iNumSounds; }
	void GetSoundFilename(int soundindex, char* outbuffer, bool listpath);
	void InitSoundSource(const char* filepath);


private:

	char m_listname[256];
	char m_listpath[256];
	soundsrc_nameentry_t soundentries[MAX_SOUNDENTRIES];
	int m_iNumSounds;

protected:

};

static_assert(sizeof(SoundSource) == 1280520, "");				//proof: animEngineDLL.dll:.data:1083F8C0 (g_soundSource is 1280520 bytes long)

#endif // SOUNDSOURCE_H
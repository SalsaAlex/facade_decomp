#ifndef MP3MANAGER_H
#define MP3MANAGER_H

#ifndef MAX_PATH
	#define MAX_PATH 260
#endif

#include <dshow.h>

enum PLAYSTATE { Stopped, Paused, Running, Init };

class MP3Manager
{
public:
	virtual ~MP3Manager() {};

	void PlayFromMP3File(const char* path);
	void SetVolume(float vol);

	void UpdateFades();
	void HandleGraphEvent();
	void StopAllMP3();

	void ToggleMute(); //unused

private:

	HRESULT PlayAudio(const char* path);
	void CheckVisibility();

	void CloseInterfaces();

	char m_mp3Path[MAX_PATH];
	BOOL m_bAudioOnly = FALSE;
	LONG m_lVolume = 0;
	int unknown2 = 0;
	PLAYSTATE m_psCurrent = Stopped;
	IGraphBuilder* pGB = NULL;
	IMediaControl* pMC = NULL;
	IMediaEventEx* pME = NULL;
	IVideoWindow* pVW = NULL;
	IBasicAudio* pBA = NULL;
	IBasicVideo* pBV = NULL;
	IMediaSeeking* pMS = NULL;
	float m_fVolume;
	float m_fFade_Factor = 0.0;
	float m_fFade_TargetVol;

protected:

};

static_assert(sizeof(MP3Manager) == 320, "");

#endif // MP3MANAGER_H
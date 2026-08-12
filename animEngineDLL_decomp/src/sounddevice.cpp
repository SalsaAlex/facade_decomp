#include "facadecommon.h"

#include "dsound.h"
#include "mmiscapi.h"

#include "sounddevice.h"

#define MAX_SOUNDBUFFERS 256

struct dsoundentry_t
{
	IDirectSoundBuffer* dsoundbuffer;
	int validsound; //ida insists this is a integer but its only ever assigned 0 and 1.. sure
	int unknown2;
	uint32_t buffersize;
	int index;
};

IDirectSound8* g_ppDS;

dsoundentry_t g_SoundBuffers[MAX_SOUNDBUFFERS];

bool DSound_Init()
{
	HRESULT hr;
	static bool s_dsoundinitonce_guard = false;
	if (!s_dsoundinitonce_guard)
	{
		if (FAILED(hr = DirectSoundCreate8(NULL, &g_ppDS, NULL)))
			return false;

		if (FAILED(hr = g_ppDS->SetCooperativeLevel(g_hMainAppWindow, DSSCL_PRIORITY)))
			return false;
	}

	for (int i = 0; i < MAX_SOUNDBUFFERS; i++)
	{
		if (g_SoundBuffers[i].dsoundbuffer)
		{
			g_SoundBuffers[i].dsoundbuffer->Stop();
			g_SoundBuffers[i].dsoundbuffer->Release();
		}
		memset(&g_SoundBuffers[i], 0, sizeof(dsoundentry_t));
		g_SoundBuffers[i].index = i;
	}
	return true;
}

void DSound_Set_Volume(int soundindex, uint32_t vol)
{
	animengine_assert(vol <= 100);
	g_SoundBuffers[soundindex].dsoundbuffer->SetVolume(-30 * (100 - vol)); //is this right..?
}

//the return value is never used.. but fine
bool DSound_Set_Pan(int soundindex, uint32_t pan)
{
	return g_SoundBuffers[soundindex].dsoundbuffer->SetPan(pan) == 0;
}

bool DSound_Stop_Sound(int soundindex)
{
	if (g_SoundBuffers[soundindex].dsoundbuffer)
	{
		g_SoundBuffers[soundindex].dsoundbuffer->Stop();
		g_SoundBuffers[soundindex].dsoundbuffer->SetCurrentPosition(NULL);
	}
	return true;
}

//same with this one
bool DSound_Delete_Sound(int soundindex)
{
	if (!DSound_Stop_Sound(soundindex))
		return false;
	if (g_SoundBuffers[soundindex].dsoundbuffer)
	{
		g_SoundBuffers[soundindex].dsoundbuffer->Release();
		g_SoundBuffers[soundindex].dsoundbuffer = NULL;
		g_SoundBuffers[soundindex].validsound = 0;
	}
	return 1;
}


int DSound_Load_WAV(const char* filename, uint32_t soundflags)
{
	int soundindex = -1;

	static WAVEFORMATEX s_waveformatdesc;
	static DSBUFFERDESC s_soundbufferdesc;

	for (int i = 0; i < MAX_SOUNDBUFFERS; i++)
	{
		if (g_SoundBuffers[i].validsound)
			continue;

		soundindex = i;
		break;
	}

	if (soundindex == -1)
		return -1;

	HMMIO inputfile = mmioOpen((char*)filename, NULL, MMIO_ALLOCBUF);
	if (!inputfile)
		return -1;


	MMCKINFO mminfo1, mminfo2;
	memset(&mminfo1, 0, sizeof(mminfo1));
	memcpy(&mminfo2, &mminfo1, sizeof(MMCKINFO));

	mminfo1.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mminfo2.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (MMSYSERR_NOERROR != mmioDescend(inputfile, &mminfo1, NULL, NULL)
		|| MMSYSERR_NOERROR != mmioDescend(inputfile, &mminfo2, &mminfo1, NULL) )
	{
		
		goto close_n_fail;
	}

	char pch[4];
	uint8_t* rawdata;
	void* dsoundmemorymap1, *dsoundmemorymap2;
	DWORD lockedsize1, lockedsize2;

	if (mmioRead(inputfile, pch, 18) != 18)
		goto close_n_fail;

	if (pch[0] != 1)
		goto close_n_fail;

	if(mmioAscend(inputfile, &mminfo2, NULL))
		goto close_n_fail;

	mminfo2.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if(mmioDescend(inputfile, &mminfo2, &mminfo1, MMIO_FINDCHUNK))	
		goto close_n_fail;

	rawdata = (uint8_t*)malloc(mminfo2.cksize);
	mmioRead(inputfile, (HPSTR)rawdata, mminfo2.cksize);
	mmioClose(inputfile, NULL);
	//g_SoundBuffers[soundindex].unknown2 = v2;// this local variable is never assigned a value..?
	g_SoundBuffers[soundindex].buffersize = mminfo2.cksize;
	g_SoundBuffers[soundindex].validsound = 1;

	s_waveformatdesc.wFormatTag = WAVE_FORMAT_PCM;
	s_waveformatdesc.nChannels = 1;
	s_waveformatdesc.nSamplesPerSec = 22050;
	s_waveformatdesc.nBlockAlign = 2;
	s_waveformatdesc.nAvgBytesPerSec = 44100;
	s_waveformatdesc.wBitsPerSample = 16;
	s_waveformatdesc.cbSize = 0;

	s_soundbufferdesc.dwSize = sizeof(DSBUFFERDESC);
	s_soundbufferdesc.dwFlags = soundflags | (DSBCAPS_LOCSOFTWARE | DSBCAPS_STATIC | DSBCAPS_GLOBALFOCUS);
	s_soundbufferdesc.dwBufferBytes = mminfo2.cksize;
	s_soundbufferdesc.lpwfxFormat = &s_waveformatdesc;
	if (0 > g_ppDS->CreateSoundBuffer(&s_soundbufferdesc, &g_SoundBuffers[soundindex].dsoundbuffer, NULL))
	{
		free(rawdata);
		return -1;
	}

	if (S_OK != g_SoundBuffers[soundindex].dsoundbuffer->Lock(
		0,
		mminfo2.cksize,
		&dsoundmemorymap1,
		&lockedsize1,
		&dsoundmemorymap2,
		&lockedsize2,
		DSBLOCK_FROMWRITECURSOR))
	{
		return 0;
	}

	memcpy(dsoundmemorymap1, rawdata, lockedsize1);
	memcpy(dsoundmemorymap2, &rawdata[lockedsize1], lockedsize2);

	if (0 > g_SoundBuffers[soundindex].dsoundbuffer->Unlock(
		&dsoundmemorymap1,
		lockedsize1,
		dsoundmemorymap2,
		lockedsize2))
	{
		return 0;
	}

	free(rawdata);
	return soundindex;


close_n_fail:
	mmioClose(inputfile, NULL);
	return -1;
}


static void StopAllSounds()
{
	for (int i = 0; i < MAX_SOUNDBUFFERS; i++)
		DSound_Stop_Sound(i);
}

bool DSound_Shutdown()
{
	StopAllSounds();
	for (int i = 0; i < MAX_SOUNDBUFFERS; i++)
	{
		if (g_SoundBuffers[i].dsoundbuffer)
			g_SoundBuffers[i].dsoundbuffer->Release();
	}
	g_ppDS->Release();
}
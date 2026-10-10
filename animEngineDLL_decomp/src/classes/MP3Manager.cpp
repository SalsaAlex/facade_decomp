#include "facadecommon.h"
#include "classes/MP3Manager.h"
#include <commctrl.h>
#include <commdlg.h>

//
// Macros
//
#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }

#define JIF(x) if (FAILED(hr=(x))) \
{Msg(TEXT("FAILED(hr=0x%x) in ") TEXT(#x) TEXT("\n"), hr); return hr;}

#define LIF(x) if (FAILED(hr=(x))) \
{Msg(TEXT("FAILED(hr=0x%x) in ") TEXT(#x) TEXT("\n"), hr);}

//
// Constants
//
#define VOLUME_FULL     0L
#define VOLUME_SILENCE  -10000L


void Msg(const char* szFormat, ...)
{
	TCHAR szBuffer[512];  // Large buffer for very long filenames (like with HTTP)

	va_list pArgs;
	va_start(pArgs, szFormat);
	vsprintf(szBuffer, szFormat, pArgs);
	va_end(pArgs);

	MessageBox(NULL, szBuffer, "PlayWnd Sample", MB_OK);
}

void MP3Manager::PlayFromMP3File(const char* path)
{
	StopAllMP3();
	strcpy(m_mp3Path, path);
	m_psCurrent = Stopped;
	m_lVolume = 0;
	if (PlayAudio(m_mp3Path) < 0)
		StopAllMP3();
}
void MP3Manager::SetVolume(float vol)
{
	HRESULT hr;

	m_fVolume = vol;
	int ivol = vol;
	animengine_assert(ivol <= 100);
	if (ivol)
		ivol = 20 * (ivol - 100);
	else
		ivol = VOLUME_SILENCE;
	animengine_assert(ivol >= VOLUME_SILENCE && ivol <= VOLUME_FULL);
	if (!pGB || !pBA)
		return;

	m_lVolume = ivol;
	LIF(pBA->put_Volume(m_lVolume));
}

void MP3Manager::UpdateFades()
{
	float newvol;
	if (m_fFade_Factor != 0.0)
	{
		newvol = (m_fVolume + m_fFade_Factor);
		if ((m_fFade_Factor > 0.0) && newvol >= m_fFade_TargetVol
			|| (m_fFade_Factor < 0.0) && newvol <= m_fFade_TargetVol)
		{
			newvol = m_fFade_TargetVol;
			m_fFade_Factor = 0.0;
		}
	}
	SetVolume(newvol);
}
void MP3Manager::HandleGraphEvent()
{
	LONG evCode, evParam1, evParam2;
	HRESULT hr = S_OK;

	// Process all queued events
	while (SUCCEEDED(pME->GetEvent(&evCode, (LONG_PTR*)&evParam1,
		(LONG_PTR*)&evParam2, 0)))
	{
		// Free memory associated with callback, since we're not using it
		hr = pME->FreeEventParams(evCode, evParam1, evParam2);

		// If this is the end of the clip, reset to beginning
		if (EC_COMPLETE == evCode)
		{
			LONGLONG pos = 0;

			// Reset to first frame of movie
			hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning,
				NULL, AM_SEEKING_NoPositioning);
			if (FAILED(hr))
			{
				// Some custom filters (like the Windows CE MIDI filter)
				// may not implement seeking interfaces (IMediaSeeking)
				// to allow seeking to the start.  In that case, just stop
				// and restart for the same effect.  This should not be
				// necessary in most cases.
				if (FAILED(hr = pMC->Stop()))
				{
					Msg(TEXT("Failed(0x%08lx) to stop media clip!\r\n"), hr);
					break;
				}

				if (FAILED(hr = pMC->Run()))
				{
					Msg(TEXT("Failed(0x%08lx) to reset media clip!\r\n"), hr);
					break;
				}
			}
		}
	}
}
void MP3Manager::StopAllMP3()
{
	if (pMC)
		pMC->Stop();

	m_psCurrent = Stopped;
	m_bAudioOnly = TRUE;

	CloseInterfaces();

	m_mp3Path[0] = '\0';
	m_psCurrent = Init;
}


HRESULT MP3Manager::PlayAudio(const char* path)
{
	WCHAR wFile[MAX_PATH];
	HRESULT hr;

#ifndef UNICODE
	MultiByteToWideChar(CP_ACP, 0, path, -1, wFile, MAX_PATH);
#else
	lstrcpy(wFile, path);
#endif

	// Get the interface for DirectShow's GraphBuilder
	JIF(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGB));

	if (SUCCEEDED(hr))
	{
		// Have the graph construct its the appropriate graph automatically
		JIF(pGB->RenderFile(wFile, NULL));

		// QueryInterface for DirectShow interfaces
		JIF(pGB->QueryInterface(IID_IMediaControl, (void**)&pMC));
		JIF(pGB->QueryInterface(IID_IMediaEventEx, (void**)&pME));
		JIF(pGB->QueryInterface(IID_IMediaSeeking, (void**)&pMS));

		// Query for video interfaces, which may not be relevant for audio files
		JIF(pGB->QueryInterface(IID_IVideoWindow, (void**)&pVW));
		JIF(pGB->QueryInterface(IID_IBasicVideo, (void**)&pBV));

		// Query for audio interfaces, which may not be relevant for video-only files
		JIF(pGB->QueryInterface(IID_IBasicAudio, (void**)&pBA));

		// Is this an audio-only file (no video component)?
		CheckVisibility();

		if (!m_bAudioOnly)
		{
			ShouldntBeHere("must play audio only");
		}

		// Run the graph to play the media file
		JIF(pMC->Run());
		m_psCurrent = Running;
	}
	return hr;
}
void MP3Manager::CheckVisibility()
{
	long lVisible;
	HRESULT hr;

	if ((!pVW) || (!pBV))
	{
		// Audio-only files have no video interfaces.  This might also
		// be a file whose video component uses an unknown video codec.
		m_bAudioOnly = TRUE;
		return;
	}
	else
	{
		// Clear the global flag
		m_bAudioOnly = FALSE;
	}

	hr = pVW->get_Visible(&lVisible);
	if (FAILED(hr))
	{
		// If this is an audio-only clip, get_Visible() won't work.
		//
		// Also, if this video is encoded with an unsupported codec,
		// we won't see any video, although the audio will work if it is
		// of a supported format.
		//
		if (hr == E_NOINTERFACE)
		{
			m_bAudioOnly = TRUE;
		}
		else
		{
			Msg(TEXT("Failed(%08lx) in pVW->get_Visible()!\r\n"), hr);
		}
	}
}

void MP3Manager::CloseInterfaces()
{
	// Relinquish ownership (IMPORTANT!) after hiding video window
	if (pVW)
	{
		pVW->put_Visible(OAFALSE);
		pVW->put_Owner(NULL);
	}

	// Release and zero DirectShow interfaces
	SAFE_RELEASE(pME);
	SAFE_RELEASE(pMS);
	SAFE_RELEASE(pMC);
	SAFE_RELEASE(pBA);
	SAFE_RELEASE(pBV);
	SAFE_RELEASE(pVW);
	SAFE_RELEASE(pGB);
}

void MP3Manager::ToggleMute()
{
	HRESULT hr = S_OK;

	if ((!pGB) || (!pBA))
		return;

	// Read current volume
	hr = pBA->get_Volume(&m_lVolume);
	if (hr == E_NOTIMPL)
	{
		// Fail quietly if this is a video-only media file
		return;
	}
	else if (FAILED(hr))
	{
		Msg(TEXT("Failed to read audio volume!  hr=0x%x\r\n"), hr);
		return;
	}

	// Switch volume levels
	if (m_lVolume == VOLUME_FULL)
		m_lVolume = VOLUME_SILENCE;
	else
		m_lVolume = VOLUME_FULL;

	// Set new volume
	LIF(pBA->put_Volume(m_lVolume));
}
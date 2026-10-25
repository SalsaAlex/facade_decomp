#include "facadecommon.h"

#include "dinput.h"

#include "keyboarddevice.h"
//its pretty clear they used a directinput sample from directx sdk for this.
//see https://github.com/LiveMirror/DX9SDK/blob/master/Samples/C%2B%2B/DirectInput/Keyboard/keyboard.cpp


#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define SAMPLE_BUFFER_SIZE 32  // arbitrary number of buffer elements

HKL g_keyboardlayout = GetKeyboardLayout(0);

HRESULT Keyboard_Init(void)
{
	HRESULT hr;
	uint32_t dwCoopFlags;
	bool bExclusive = false;
	bool bForeground = true;
	bool bImmediate = false;

	if (bExclusive)
		dwCoopFlags = DISCL_EXCLUSIVE;
	else
		dwCoopFlags = DISCL_NONEXCLUSIVE;

	if (bForeground)
		dwCoopFlags |= DISCL_FOREGROUND;
	else
		dwCoopFlags |= DISCL_BACKGROUND;

	// Create a DInput object
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)_rawaddr_g_pDI(), NULL)))
		return hr;

	// Obtain an interface to the system keyboard device.
	if (FAILED(hr = g_pDI->CreateDevice(GUID_SysKeyboard, _rawaddr_g_pKeyboard(), NULL)))
		return hr;

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.
	if (FAILED(hr = g_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return hr;

	// Set the cooperativity level to let DirectInput know how
	// this device should interact with the system and with other
	// DirectInput applications.
	hr = g_pKeyboard->SetCooperativeLevel(g_hMainAppWindow, dwCoopFlags);
	if (hr == DIERR_UNSUPPORTED && !bForeground && bExclusive)
	{
		Keyboard_Shutdown();
		dprintf("SetCooperativeLevel() returned DIERR_UNSUPPORTED.\n"
			"For security reasons, background exclusive keyboard\n"
			"access is not allowed.");
		return S_OK;
	}

	if (FAILED(hr))
		return hr;

	if (!bImmediate)
	{
		// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
		//
		// DirectInput uses unbuffered I/O (buffer size = 0) by default.
		// If you want to read buffered data, you need to set a nonzero
		// buffer size.
		//
		// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
		//
		// The buffer size is a DWORD property associated with the device.
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = SAMPLE_BUFFER_SIZE; // Arbitary buffer size

		if (FAILED(hr = g_pKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
			return hr;
	}

	g_pKeyboard->Acquire();

	return S_OK;
}

void Keyboard_Shutdown()
{
	// Unacquire the device one last time just in case 
// the app tried to exit while the device is still acquired.
	if (g_pKeyboard)
		g_pKeyboard->Unacquire();

	// Release any DirectInput objects.
	{ if (g_pKeyboard) { (g_pKeyboard)->Release(); _set_g_pKeyboard(NULL); } }
	{ if (g_pDI) { (g_pDI)->Release(); _set_g_pDI(NULL); } }
}



//	!!UNFINISHED!!
HRESULT ReadBufferedKeyboardData()
{
	TCHAR              strNewText[256 * 5 + 1] = TEXT("");
	TCHAR              strLetter[10];
	DIDEVICEOBJECTDATA didod[SAMPLE_BUFFER_SIZE];  // Receives buffered data 
	DWORD              dwElements;
	DWORD              i;
	HRESULT            hr;

	static int unknown = 67983; //?

	if (NULL == g_pKeyboard)
		return S_OK;

	dwElements = SAMPLE_BUFFER_SIZE;
	hr = g_pKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),
		didod, &dwElements, 0);
	if (hr != DI_OK)
	{
		// We got an error or we got DI_BUFFEROVERFLOW.
		//
		// Either way, it means that continuous contact with the
		// device has been lost, either due to an external
		// interruption, or because the buffer overflowed
		// and some events were lost.
		//
		// Consequently, if a button was pressed at the time
		// the buffer overflowed or the connection was broken,
		// the corresponding "up" message might have been lost.
		//
		// But since our simple sample doesn't actually have
		// any state associated with button up or down events,
		// there is no state to reset.  (In a real game, ignoring
		// the buffer overflow would result in the game thinking
		// a key was held down when in fact it isn't; it's just
		// that the "up" event got lost because the buffer
		// overflowed.)
		//
		// If we want to be cleverer, we could do a
		// GetDeviceState() and compare the current state
		// against the state we think the device is in,
		// and process all the states that are currently
		// different from our private state.
		hr = g_pKeyboard->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = g_pKeyboard->Acquire();

		// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
		// may occur when the app is minimized or in the process of 
		// switching, so just try again later 
		return S_OK;
	}

	// Study each of the buffer elements and process them.
	//
	// Since we really don't do anything, our "processing"
	// consists merely of squirting the name into our
	// local buffer.
	bool NewInput = false;
	int strindex = strlen(g_curTextInput);
	for (i = 0; i < dwElements; i++)
	{
		if (strindex >= 32 || !(didod[i].dwData & 0x80))
			continue;


		BYTE keyboardstate[256];
		WORD Char[10];
		DWORD virtualkey = didod[i].dwOfs;

		GetKeyboardState(keyboardstate);
		ToAsciiEx(MapVirtualKeyEx(virtualkey, MAPVK_VSC_TO_VK, g_keyboardlayout), 
			virtualkey, keyboardstate, Char, NULL, g_keyboardlayout);

		char CorrectedChar = Char[0] == '\b' ? '<' : Char[0];
		if (virtualkey == DIK_NUMPADENTER)
			CorrectedChar = '\r';

		if (IsWhitespaceNewLineOrComma(CorrectedChar)
			|| IsNumberCharacter(CorrectedChar)
			|| IsLetterCharacter(CorrectedChar)
			|| IsPunctuationCharacter(CorrectedChar))
		{
			g_curTextInput[strindex++] = CorrectedChar;
			NewInput = true;
		}
	}

	if (NewInput || !(GetAsyncKeyState(VK_BACK) & 0x8000) || strindex >= 32)
	{
		unknown = 0;
	}
	else if (++unknown > 15 && !(g_numFramesRun % 2))
	{
		g_curTextInput[strindex++] = '<';
	}
	_set_g_curTextCharInput(g_curTextInput[0]);
	for (int i = 0; i < sizeof(g_curTextInput)-1; ++i)
		g_curTextInput[i] = g_curTextInput[i + 1];
	g_curTextInput[31] = 0;

	return S_OK;
}
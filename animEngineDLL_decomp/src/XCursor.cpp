#include "facadecommon.h"

#include "classes/XCursor.h"

void XCursor::ChangeCursorImage(ECursorType cursor)
{
	if (m_bLockedState || cursor == m_eCursorType)
		return;

	HCURSOR cursorhandle1, cursorhandle2;

	switch (cursor)
	{
		case eCursorType_IDC_ARROW:		cursorhandle1 = LoadCursor(NULL, IDC_ARROW); break;
		case eCursorType_IDC_WAIT:		cursorhandle1 = LoadCursor(NULL, IDC_WAIT); break;
		case eCursorType_IDC_SIZEWE:	cursorhandle1 = LoadCursor(NULL, IDC_SIZEWE); break;
		case eCursorType_IDC_SIZENS:	cursorhandle1 = LoadCursor(NULL, IDC_SIZENS); break;
		case eCursorType_IDC_SIZENWSE:	cursorhandle1 = LoadCursor(NULL, IDC_SIZENWSE); break;
		case eCursorType_IDC_SIZENESW:	cursorhandle1 = LoadCursor(NULL, IDC_SIZENESW); break;
		default:						cursorhandle2 = LoadImage(g_hAppInstance, (uint32_t)cursor + IDC_HANDCURSOR_IDLE, IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR); break;
	}

	if (!cursorhandle1 && !cursorhandle2)
		return;

	if (cursorhandle1)
	{
		SetClassLong(g_hMainAppWindow, GCL_HCURSOR, (LONG)cursorhandle1);
		SetCursor(cursorhandle1);
	}
	if (cursorhandle2)
	{
		SetClassLong(g_hMainAppWindow, GCL_HCURSOR, (LONG)cursorhandle2);
		SetCursor(cursorhandle2);
	}
	m_eCursorType = cursor;
}

void XCursor::Override(ECursorType cursor)
{
	m_bLockedState = false;
	ChangeCursorImage(cursor);
	m_bLockedState = true;
}

void XCursor::SetRate(int rate)
{
	m_iRate = rate;
}
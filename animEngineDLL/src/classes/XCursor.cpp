#include "facadecommon.h"

#include "classes/XCursor.h"

XCursor* XCursor::theirCursor;

void XCursor::InitCursor()
{
	cursorstack = new Stack();
	cursorstack->InitStack(4096);

	//undone
}

void XCursor::JumpCutToCursor(ECursorType cursor, bool clear)
{
	ResetStack(false);
	if (clear)
		this->m_eCursorType = (ECursorType)-1;
	this->unknown4[0] = cursor;
	unknown2[4].unknown[1] = cursor;
	ChangeCursorImage(cursor);
}

void XCursor::ResetStack(bool option)
{
	if (option)
	{
		this->unknown4[1] = 0;
	}
	else
	{
		cursorstack->Reset();
		this->unknown4[2] = -1;
		this->unknown4[1] = 0;
		this->unknown4[0] = -1;
	}
}

bool XCursor::IsCursorStackEmpty()
{
	return cursorstack->IsStackEmpty();
}

void XCursor::ChangeCursorImage(ECursorType cursor)
{
	if (m_bLockedState || cursor == m_eCursorType)
		return;

	HCURSOR cursorhandle1 = NULL;
	HANDLE cursorhandle2 = NULL;

	switch (cursor)
	{
		case eCursorType_IDC_ARROW:		cursorhandle1 = LoadCursor(NULL, IDC_ARROW); break;
		case eCursorType_IDC_WAIT:		cursorhandle1 = LoadCursor(NULL, IDC_WAIT); break;
		case eCursorType_IDC_SIZEWE:	cursorhandle1 = LoadCursor(NULL, IDC_SIZEWE); break;
		case eCursorType_IDC_SIZENS:	cursorhandle1 = LoadCursor(NULL, IDC_SIZENS); break;
		case eCursorType_IDC_SIZENWSE:	cursorhandle1 = LoadCursor(NULL, IDC_SIZENWSE); break;
		case eCursorType_IDC_SIZENESW:	cursorhandle1 = LoadCursor(NULL, IDC_SIZENESW); break;
		default:						cursorhandle2 = LoadImage(g_hAppInstance, (LPCSTR)(IDC_HANDCURSOR_IDLE + cursor), IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR); break;
		//default: cursorhandle2 = LoadCursor(NULL, IDC_SIZENESW); break;
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
		SetCursor((HCURSOR)cursorhandle2);
	}
	m_eCursorType = cursor;
}

void XCursor::Override(ECursorType cursor)
{
	m_bLockedState = false;
	ChangeCursorImage(cursor);
	m_bLockedState = true;
}

void XCursor::Push(CursorSequence seq)
{
	//cursorstack->Compress();
	//for (int i = 1; i < unknown2[0].unknown[seq]; ++i)
	//{
	//	if (cursorstack->GetStackPtr() < 4096)
	//		cursorstack->Push(theCursorFrame[30 * seq + 1 + i]);
	//	else
	//		dprintf("###WARNING:  Exceeding cursor push stack size");
	//}
}

void XCursor::SetRate(int rate)
{
	m_iRate = rate;
}

void XCursor::Update()
{
	if (!(g_numFramesRun % 2))
		return;

	//if (cursorstack->IsStackEmpty())
	//{
	//	if (--unknown4[1] > 0 && unknown4[2] >= 0)
	//		cursorstack->SetStackPtr(unknown4[2]);
	//}
	//else if (++this->unknown5 >= this->m_iRate)
	//{
	//	ECursorType cursortype = cursorstack->Pop();
	//	unknown2[9] = cursortype;
	//	ChangeCursorImage(cursortype);
	//	unknown5 = 0;
	//}
	//if (unknown4[1] < 0)
	//	unknown4[1] = 0;
}
void XCursor::SetCursorPosition(const XTPointi& point)
{
	ClipCursor(NULL);
	SetCursorPos(point.x, point.y);
}

void XCursor::PushTransitionToNeutral(ECursorType cursor)
{
	//if (unknown4[0] == -1)
	//salsatobias: temporary :(
	if(1)
	{
		JumpCutToCursor(cursor, false);
		return;
	}

	if (cursor == unknown4[0])
		return;

	int i = 0;
	for (; i < 3; ++i)
	{
		if (unknown2[i + 1].unknown[1] == unknown4[0] && unknown2[i + 2].unknown[0] == cursor)
		{
			Push((CursorSequence)i);
			unknown4[0] = cursor;
			unknown4[1] = 0;
			unknown5 = m_iRate;
			return;
		}
	}
	for (i = 0; i < 3; ++i)
	{
		if (unknown2[i + 1].unknown[1] != unknown4[0])
			continue;
		for (int j = 0; j < 3; ++j)
		{
			if (unknown2[i + 2].unknown[0] == unknown2[j + 1].unknown[1]
				&& unknown2[j + 2].unknown[0] == cursor)
			{
				Push((CursorSequence)i);
				Push((CursorSequence)j);
				unknown4[0] = cursor;
				unknown4[1] = 0;
				unknown5 = m_iRate;
				return;
			}
		}
	}
	for (i = 0; i < 3; ++i)
	{
		if (unknown2[i + 1].unknown[1] != unknown4[0])
			continue;
		for (int j = 0; j < 3; ++j)
		{
			if (unknown2[i + 2].unknown[0] == unknown2[j + 1].unknown[1]
				&& unknown2[j + 2].unknown[0] == cursor)
			{
				for (int k = 0; k < 3; ++k)
				{
					if (unknown2[j + 2].unknown[0] == unknown2[k + 1].unknown[1]
						&& unknown2[k + 2].unknown[0] == cursor)
					{
						Push((CursorSequence)i);
						Push((CursorSequence)j);
						Push((CursorSequence)k);
						unknown4[0] = cursor;
						unknown4[1] = 0;
						unknown5 = m_iRate;
						return;
					}
				}
			}
		}

	}
}
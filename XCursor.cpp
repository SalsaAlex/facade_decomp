#include <windows.h>
#include "Stack.h"

class XCursor
{
public:
	XCursor(void);
	~XCursor(void);

	int InitCursor(void)
	{
		m_pStack = new Stack();
		m_pStack->InitStack(4096);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				//if (dword_101F6F24[30 * i + j] == -1)
				//{
				//	*(_DWORD*)(this + 4 * i + 8) = j;
				//	break;
				//}
			}
		}
		//for (int k = 0; k < 3; ++k)
		//{
		//	*(DWORD*)(this + 8 * k + 20) = dword_101F6F24[30 * k];
		//	*(DWORD*)(this + 8 * k + 24) = dword_101F6F20[30 * k + *(DWORD*)(this + 4 * k + 8)];
		//}
		//*(DWORD*)(this + 60) = 0;
		//*(DWORD*)(this + 56) = 0;
	}

	HCURSOR ChangeCursorImage()
	{
		
	}


	HCURSOR m_CurCursor;
	Stack* m_pStack; // this + 13 | this + 52 | this[13]
};
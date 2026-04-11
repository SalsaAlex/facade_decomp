#pragma once

class Stack
{
public:
	Stack(void);
	~Stack(void);

	void InitStack(int iStackSize);
	void Reset();


	int GetLastPushedNeutralPos();
	void SetLastPushedNeutralPos(int lastpushed);

	void* GetStackPtr();
	void SetStackPtr(void* ptr);

	int GetStackSize();
	void SetStackSize(int stacksize);

	int* m_pStack = nullptr;//this + 39 | this + 156 | this[39]
	void* m_pStackptr = nullptr; //this + 38 | this + 152 | this[38]
	int m_iStacksize = 0; //this + 36 | this + 144 | this[36]
	int m_iStackCapacity = 0; //this + 37 | this + 148 | this[37]
	int m_iLastpushedneutralpos = 0; //this + 34 | this + 136 | this[34]

};
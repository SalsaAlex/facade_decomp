
#include <windows.h>
#include "Stack.h"


Stack::Stack(void) 
{
}
Stack::~Stack(void)
{
}


void Stack::InitStack(int iStackSize)
{
	m_pStack = new int[iStackSize];
	m_iStackCapacity = iStackSize;
}
void Stack::Reset()
{
	m_pStackptr = nullptr;
	m_iStacksize = 0;
}


int Stack::GetLastPushedNeutralPos() 
{ 
	return m_iLastpushedneutralpos; 
}
void Stack::SetLastPushedNeutralPos(int lastpushed) 
{ 
	m_iLastpushedneutralpos = lastpushed; 
}

void* Stack::GetStackPtr() 
{ 
	return m_pStackptr; 
}
void Stack::SetStackPtr(void* ptr) 
{ 
	m_pStackptr = ptr; 
}

int Stack::GetStackSize() 
{ 
	return m_iStacksize;
}
void Stack::SetStackSize(int stacksize)
{
	m_iStacksize = stacksize;
	if (m_iStackCapacity < m_iStacksize)
		m_iStackCapacity = m_iStacksize;
}
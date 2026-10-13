#include "facadecommon.h"
#include "classes/Stack.h"

Stack::Stack()
{
    for (int i = 0; i < 10; ++i)
    {
        unknown002[i] = 0;
        unknown012[i] = 0;
    }
}

void Stack::InitStack(int size)
{
    m_pMem = new int[size];
    m_iMemSize = size;
}
void Stack::Reset()
{
    unknown038 = 0;
    unknown036 = 0;
}
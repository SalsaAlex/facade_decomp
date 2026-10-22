#ifndef STACK_H
#define STACK_H

class Stack
{
public:
	Stack();
	virtual ~Stack() = default;

	void InitStack(int size);
	void Reset();

	bool IsStackEmpty();

private:

	int unknown001 = 0;
	int unknown002[10];
	int unknown012[10];
	int unknown022;
	int unknown023;
	int unknown024;
	int unknown025;
	int unknown026;
	int unknown027;
	int unknown028;
	int unknown029;
	int unknown030;
	int unknown031;
	int unknown032;
	int unknown033 = 0;
	int unknown034 = -1;
	int unknown035 = -1;
	int unknown036 = 0;
	int m_iMemSize = 0;
	int unknown038 = 0;
	void* m_pMem = nullptr;
	int unknown040;

protected:

};

static_assert(sizeof(Stack) == 164, "");						//proof: animEngineDLL.dll:.text:100B4352

#endif // STACK_H
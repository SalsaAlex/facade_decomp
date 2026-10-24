#ifndef XCURSOR_H
#define XCURSOR_H

#include "facadecommon.h"
#include "Stack.h"

struct unknownxcursorstruct_t
{
	int unknown[2];
};

enum CursorSequence
{

};

class XCursor
{
public:

	virtual ~XCursor() = default;

	void InitCursor();

	void JumpCutToCursor(ECursorType cursor, bool clear);
	void Override(ECursorType cursor);

	void Push(CursorSequence seq);

	void Restore();
	void Update();
	void SetRate(int rate);
	void SetCursorPosition(const XTPointi& point);

	void PushTransitionToNeutral(ECursorType cursor);

	void ResetStack(bool option);

	bool IsCursorStackEmpty();


	static XCursor* theirCursor;
private:

	void ChangeCursorImage(ECursorType cursor);
	void OffsetPointALittle(XTPointi& point) {};




	//data
	bool m_bLockedState;
	unknownxcursorstruct_t unknown2[5];
	ECursorType m_eCursorType;
	Stack* cursorstack;
	int unknown4[3];
	int m_iRate;
	int unknown5;

protected:

};

static_assert(sizeof(XCursor) == 76, "");						//proof: animEngineDLL.dll:.text:100B2001

#endif // XCURSOR_H
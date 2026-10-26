#ifndef GESTUREFXNMGR_H
#define GESTUREFXNMGR_H

class GestureFxnMgr
{
public:
	virtual ~GestureFxnMgr() = default;

	void StartGestureFxn(int, int, bool);

	int unknown[692];
};

static_assert(sizeof(GestureFxnMgr) == 2772, "");				//proof: animEngineDLL.dll:.text:1001AD19 (19E25C - 19ED30 = AD4 = 2772)

#endif // GESTUREFXNMGR_H
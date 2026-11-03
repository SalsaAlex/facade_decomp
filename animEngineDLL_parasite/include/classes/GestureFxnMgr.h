#ifndef GESTUREFXNMGR_H
#define GESTUREFXNMGR_H

enum EGestureFxnMsg
{
	eGestureFxnMsg_unknown000 = 0,
};

class GestureFxnMgr
{
public:
	virtual ~GestureFxnMgr() = default;

	void StartGestureFxn(int, int, bool);

	bool StandardBodyGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo);
	bool StandardArmLGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo);
	bool StandardArmRGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo);
	bool StandardEyesMoveGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo);
	bool StandardBrowLGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo);
	bool StandardBrowRGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo);
	bool StandardMouthGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo);
	bool StandardEyesShapeGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo);
	bool StandardHeadGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo);
	bool StandardArmsBothGestureHandler(CharacterSprite* actor, EGestureFxnMsg msg, int msginfo);

	int unknown[692];
};

static_assert(sizeof(GestureFxnMgr) == 2772, "");				//proof: animEngineDLL.dll:.text:1001AD19 (19E25C - 19ED30 = AD4 = 2772)

#endif // GESTUREFXNMGR_H
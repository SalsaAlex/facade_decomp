#ifndef CHARACTERSPRITE_H
#define CHARACTERSPRITE_H

class LayerController
{
public:
	virtual ~LayerController() = default;

	int unknown[754];
};

class GestureFxnMgr
{
public:
	virtual ~GestureFxnMgr() = default;

	int unknown[692];
};

class CharacterSprite : public AnimatingSprite					//proof: libnativeAnimInterface.jnilib:__const_coal:00206620
{
public:

	void InitCharacterSprite(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, ESpriteID id);

	void EndArmSync();
	void ResetArmSyncSet(int i1);

	void SetHeadRotation(XYZPointf rot);
	XYZPointf GetHeadRotation();

	ECinematographyFraming GetCinematographyFraming();
	void SetCinematographyFraming(ECinematographyFraming cineframing);

	void SetTorsoRotation(XYZPointf rot);

	void SetHeadRotY(float y);
	void SetHeadTilt(float tilt);
	void SetHeadCock(float cock);

	void SetBreathingBehavior(ELayerAltIndex index);

	void SetHeadQuiver(float f1, float f2, float f3);
	void SetBlinkBehavior(EBlinkBehavior behavior, bool b1);

	void SetEyesMove(float f1, float f2);
	void GetEyesMove(float& f1, float& f2);

	void SetEyesToLookAtPlayer();
	void SetEyeballLookAway(bool b1, bool b2, float f1, int i1, float i2);
	void SetEyeballQuiver(float f1, float f2, float f3);
	void SetEyelidHeight(float f1, bool b1, bool b2, bool b3);
	void SetBriefEyelidHeight(float f1, float f2, bool b1, bool b2);

	void SetGazeTrackingSprite(Sprite* sprite, EGazeSpeed speed, EGazeType type, EAlterHeadCock cock);
	void SetGazeTrackingPoint(XYZPointf point, EGazeSpeed speed, EGazeType type, EAlterHeadCock cock);
	void SetCueFromSpeakDialog(int i1);
	void SetPerformanceInfo(int* p1);

	void SetVolumeAndPan();

	void SetLocomoting(bool option);
	bool GetLocomoting();

	XYZPointf GetNorm0to360HeadRotation();
	XYZPointf GetNorm0to360TorsoRotation();
	XYZPointf GetHeadPlusTorsoRotation();

	Sprite* GetGazeTracking();

	void GetCurrentBrowRLowerEmphasis(int&, int&);
	void GetCurrentBrowLRaiseEmphasis(int&, int&);
	void GetCurrentBrowRRaiseEmphasis(int&, int&);
	void GetCurrentBrowLLowerEmphasis(int&, int&);
	int GetVolumeInterpolation(int, int, int, int, int);

	XYZPointf GetLocomotionTargetPoint();
	XYZPointf GetLocomotionTweakedFinalPoint();
	int GetNextWalkUScript(EUScript script);

	XYZPointf GetConversationPosition();
	float GetWalkEndLength();
	bool GetIsCharacterSprite() override;

	bool IsPelvisModelPointIndex(int i1);
	bool IsLowerBodyModelPointIndex(int i1);
	bool IsLeftHandHoldingSomething();
	bool IsRightHandHoldingSomething();
	bool IsDialogQueued();

	bool AreWeInAnEditor(bool option);
	bool AreWeDoingFullExpression();
	bool AreWeDoingTenseFullExpression();
	bool AreWeSpeaking();
	bool AreWePositionedForConversation(bool option);
	bool AreWeTurningInPlace();
	bool AreWeWalkingStraight();
	bool GetAreWeBodyPosing();

	void PossiblyWalkBackwards();

	void UpdateArmSync();
	void UpdateSprite() override;
	void UpdateBreathing();
	void UpdateCurrentPose();
	void UpdateHeadEmphasis(int i1);
	void UpdateFaceVariation();
	void UpdateGazeTracking();
	void UpdateEyeballQuiver();
	void UpdateHeadQuiverAndCock();
	void UpdateBlinking();
	void UpdateFullExpression();
	void UpdateSpeaking(bool option);
	void UpdateLocomotionKinematicTargets();
	void UpdateExactPointDeleceration();
	void UpdateLocomotion();
	void UpdateGripForCertainObjects();

	void SetupNextWordToSpeak();

	void StartBlink(int i1, int i2, int i3);
	void StartBlinkType(EBlinkType type);
	void StartHeadEmphasisStuff(int i1, int i2, int i3, float f1, float f2, float f3, bool b1, bool b2);
	void StartHeadEmphasis(int i1, bool b1, float f1, int i2, int i3);
	void StartBodyPosing(EBodyPoseConfig config);

	void StopCurrentBodyPosing();

	void DoLocomotion(XYZPointf point, float f1, Sprite* spr, EWalkType type);
	void StopLocomotion(bool option);

	void DoFullExpression(EFullExpressionMood mood, EFullExpressionBase base, Sprite* spr1, Sprite *spr2, int i1, bool b1);
	void DoFullExpressionMod(EMiscLittleAction action, float f1);

	void StopDialog();
	void SpeakDialog(int, int*, char*, int);

	float ShortestAngleToTurnTo(float target);

	void AlterPathPlanToGetInOrOutOfBar(bool b1, bool b2, XYZPointf& pointout);

	void PickNewBasePoseScript(int i1, int& i2, int& i3);
	void OffsetTorsoTracking(float f1);
	void RefreshGazeTracking();
	void QueueDialogScript(int i1, bool b1);
	void ClearDialogQueue();
	void ConversationPositionComputation(float f1, float f2);
	void ReduceVolumeIfFacingAway(int i1, float f1);
	void PossiblyDoCorrespondingLittleActionsForDialog(int i1);


	static const char* GetFullExpressionBaseName(EFullExpressionBase base);
	static const char* GetFullExpressionMoodName(EFullExpressionMood mood);


	Sprite* unknown001;
	HeadSprite* m_pHeadSprite;
	XYZPointf unknown004;
	int unknown005;
	int unknown006;
	int unknown007;
	int unknown008;
	int unknown009;
	int unknown010;
	int unknown011;
	int unknown012;
	int unknown013;
	int unknown014;
	int unknown015;
	int unknown016;
	Controller m_Controller1;
	Controller m_Controller2;
	Controller m_Controller3;
	Controller m_Controller4;
	Controller m_Controller5;
	Controller m_Controller6;
	Controller m_Controller7;
	Controller m_Controller8;
	Controller m_Controller9;
	Controller m_Controller10;
	Controller m_Controller11;
	Controller m_Controller12;
	Controller m_Controller13;
	Controller m_Controller14;
	Controller m_Controller15;
	Controller m_Controller16;
	Controller m_Controller17;
	Controller m_Controller18;
	Controller m_Controller19;
	Controller m_Controller20;
	Controller m_Controller21;
	Controller m_Controller22;
	LayerController m_LayerController1;
	LayerController m_LayerController2;
	int unknown039[14];
	int unknown040[14];
	int unknown041[14];
	int unknown042[12];
	Controller m_Controller23;
	float unknown043;
	Controller m_Controller24;
	float unknown044;
	Controller m_Controller25;
	float unknown045;
	GestureFxnMgr m_GestureFxnMgr1;
	GestureFxnMgr m_GestureFxnMgr2;
	GestureFxnMgr m_GestureFxnMgr3;
	GestureFxnMgr m_GestureFxnMgr4;
	GestureFxnMgr m_GestureFxnMgr5;
	GestureFxnMgr m_GestureFxnMgr6;
	GestureFxnMgr m_GestureFxnMgr7;
	GestureFxnMgr m_GestureFxnMgr8;
	GestureFxnMgr m_GestureFxnMgr9;
	GestureFxnMgr m_GestureFxnMgr10;
	int unknown046;
	bool unknown047;
	int unknown048;
	int unknown049;
	int unknown050;
	int unknown051[28];
	int unknown052;
	int unknown053;
	int unknown054[3];
	int unknown055;
	int unknown056;
	int unknown057;
	int unknown058[3];
	int unknown059;
	float funknown001;
	float funknown002;
	int unknown061;
	int unknown062;
	int unknown063;
	int unknown064;
	int unknown065;
	int unknown066;
	float unknown067[1808];
	ECinematographyFraming m_eCineFraming;
	int unknown068[16];
	bool charsprite_bool1;

private:
protected:

};

static_assert(sizeof(CharacterSprite) == 1731584, "");			//proof: animEngineDLL.dll:.text:1006C2AD

#endif // CHARACTERSPRITE_H
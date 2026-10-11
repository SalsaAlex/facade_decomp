#ifndef CHARACTERSPRITE_H
#define CHARACTERSPRITE_H

#include "AnimatingSprite.h"

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

	void GetCurrentBrowRLowerEmphasis(int&, int&);
	void GetCurrentBrowLRaiseEmphasis(int&, int&);
	void GetCurrentBrowRRaiseEmphasis(int&, int&);
	void GetCurrentBrowLLowerEmphasis(int&, int&);
	const char* GetFullExpressionBaseName(EFullExpressionBase base);
	const char* GetFullExpressionMoodName(EFullExpressionMood mood);
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



	int unknown001;
	void* m_pHeadSprite;
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
	Controller unknown017;
	Controller unknown018;
	Controller unknown019;
	Controller unknown020;
	Controller unknown021;
	Controller unknown022;
	Controller unknown023;
	Controller unknown024;
	Controller unknown025;
	Controller unknown026;
	Controller unknown027;
	Controller unknown028;
	Controller unknown029;
	Controller unknown030;
	Controller unknown031;
	Controller unknown032;
	Controller unknown033;
	Controller unknown034;
	Controller unknown035;
	Controller unknown036;
	Controller unknown037;
	Controller unknown038;
	int unknown039[10424];

private:
protected:

};

static_assert(sizeof(CharacterSprite) == 1731584, "");			//proof: animEngineDLL.dll:.text:1006C2AD

#endif // CHARACTERSPRITE_H
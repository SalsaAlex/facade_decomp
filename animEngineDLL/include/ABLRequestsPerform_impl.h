#pragma once

#include "facadecommon.h"
struct AblRequest;

bool PerformCallbackWhenFullExpressionMoodDone(ESpriteID actor, AblRequest* request);
bool PerformCallbackWhenFullExpressionBaseDone(ESpriteID actor, AblRequest* request);
bool PerformCallbackWhenGestureDone(ESpriteID actor, EGestureBodyPart bodypartindex, AblRequest* request);
bool PerformCallbackWhenAnimationDone(ESpriteID actor, EAnimLayer layer, AblRequest* request);
bool PerformCallbackWhenWalkIsDone(uint32_t unused, ESpriteID actor, bool b1, AblRequest* request);


void PerformGetMouthShape(ESpriteID spriteid, int& mouthshape);
void PerformSetMouthShape(ESpriteID spriteid, int mouthshape);

void PerformGetEyelidHeightR(ESpriteID spriteid, float& height);
void PerformGetEyelidHeightL(ESpriteID spriteid, float& height);
void PerformSetEyelidHeightR(ESpriteID spriteid, float height);
void PerformSetEyelidHeightL(ESpriteID spriteid, float height);

void PerformGetBrowShapeR(ESpriteID spriteid, int& browshape);
void PerformGetBrowShapeL(ESpriteID spriteid, int& browshape);
void PerformSetBrowShapeR(ESpriteID spriteid, int browshape);
void PerformSetBrowShapeL(ESpriteID spriteid, int browshape);

void PerformGetEyesShape(ESpriteID spriteid, int& eyeshape);
void PerformSetEyesShape(ESpriteID spriteid, int eyeshape);

void PerformGetObjectPosition(ESpriteID spriteid, float& x, float& y, float& z);
void PerformSetObjectPosition(ESpriteID spriteid, float x, float y, float z);

void PerformGetObjectRotation(ESpriteID spriteid, float& x, float& y, float& z);
void PerformSetObjectRotation(ESpriteID spriteid, float x, float y, float z);

void PerformGetGazeTracking(ESpriteID gazetracker, ESpriteID& gazetracked);
void PerformSetGazeTracking(ESpriteID gazetracker, ESpriteID gazetracked, float x, float y, float z, EGazeSpeed gazespeed, EGazeType gazetype, EAlterHeadCock headcock, bool b1);

void PerformSetObjectToHold(ESpriteID holder, int i1, ESpriteID victim);

void PerformGetAllObjectPickupRotations(ESpriteID spriteid, ObjSensePacket* sensepacket);
void PerformGetAllObjectPickupPositions(ESpriteID spriteid, ObjSensePacket* sensepacket);
void PerformGetAllObjectRotations(ObjSensePacket* sensepacket);
void PerformGetAllObjectPositions(ObjSensePacket* sensepacket);
void PerformGetAllObjectStates(ObjStateSensePacket* statesensepacket);
void PerformGetAllObjectStagingRotations(ObjSensePacket* sensepacket);
void PerformGetAllHeldObjects(HeldObjectSensePacket* heldpacket);

void PerformGetArmGestureBases(ESpriteID actor, int& i1out, int& i2out, int& i3out);

void PerformGetEyesMove(ESpriteID spriteid, float& x, float& y);

void PerformGetObjectPickupPosition(ESpriteID holder, ESpriteID object, float& x, float& y, float& z);

void PerformSetPerformanceInfo(ESpriteID spriteid, void* perfinfo);
void PerformSetAnimEngineInfo(ESetAnimEngineInfoType type, int info);

void PerformPushMultipleAnimationScripts(ESpriteID actor, int script1Index, int script2Index, int script3Index, int script4Index, EAnimLayer layer);
void PerformPushPoseScript(ESpriteID spriteid, EBodyPoseConfig config);

void PerformMusic(EMusicAction action, EMusicName name);
void PerformPlaySoundEffect(EGlobalSounds sound);


void PerformReactiveWalkStep(uint32_t unknown, ESpriteID actor, ESpriteID spriteid, bool stop);
void PerformWalkStep(uint32_t unknown, ESpriteID actor, EWalkType type, float x, float y, float z, float f1, float& xout, float& yout, float& zout);

void PerformAbortWalkStep(ESpriteID walkstepper);
void PerformAbortAnimationScripts(ESpriteID actor, EAnimLayer layer);

void PerformDoGestureAnimation(ESpriteID spriteid, int i1, int a2, bool b1);
void PerformDoPickupObjGestureAnimation(ESpriteID spriteid, EGestureBodyPart bodypartindex, int i1, int i2, int i3);
void PerformDoFullExpressionBaseAnimation(ESpriteID actor, EFullExpressionMood mood, EFullExpressionBase base, ESpriteID obj1, ESpriteID obj2);
void PerformDoPutdownObjGestureAnimation(ESpriteID spriteid, int i1, int i2, int i3, int i4, float x, float y, float z, int i6);

void PerformMiscLittleAction(ESpriteID spriteid, EMiscLittleAction action, int i1, int i2);

void PerformGetMyCopyOfAllAnimCues(int animcueindex, AnimCueSensePacket* animcuepacket);
void PerformGetMyCopyOfAllObjectActivations(int index, ObjectActivationSensePacket* objectactivationpacket);

void PerformResetMyCopyOfAllAnimCues(int i1);
void PerformResetMyCopyOfAllObjectActivations(int i1);
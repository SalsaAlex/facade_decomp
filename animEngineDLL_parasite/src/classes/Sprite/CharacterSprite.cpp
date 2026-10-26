#include "facadecommon.h"
#include "ABLRequestsPerform_impl.h"
#include "classes/Sprite/CharacterSprite.h"

static XYZPointf GetEyeXYDeltaForLookAtUser(float yaw, float pitch)
{
	float v5; // [esp+60h] [ebp-38h]
	XYZPointf xydelta; // [esp+64h] [ebp-34h]
	float v7; // [esp+70h] [ebp-28h]
	float v8; // [esp+74h] [ebp-24h]
	float v9; // [esp+78h] [ebp-20h]
	float v10; // [esp+7Ch] [ebp-1Ch]
	float v11; // [esp+80h] [ebp-18h]
	float v12; // [esp+84h] [ebp-14h]
	float v13; // [esp+88h] [ebp-10h]
	float v14; // [esp+8Ch] [ebp-Ch]
	float v15; // [esp+90h] [ebp-8h]
	float v16; // [esp+94h] [ebp-4h]


	v5 = fabs(yaw);

#define SWITCHCASE(minval, maxval, valv14, valv13, valv12, valv11) (v5 >= minval && v5 < maxval) \
	{  v14 = valv14; v13 = valv13; v12 = valv12; v11 = valv11; }


				     //min max	//v14 v13 v12 v11
	if SWITCHCASE(		0, 10,		0,	2,	0, 0)
	else if SWITCHCASE(10, 20,		2,	3,	0, 0)
	else if SWITCHCASE(20, 30,		3,	5,	0, 0)
	else if SWITCHCASE(30, 40,		5,	8,	0, 0)
	else if SWITCHCASE(40, 50,		8,	13, 0, 2)
	else if SWITCHCASE(50, 60,		13, 16, 2, 6)
	else if SWITCHCASE(60, 70,		16, 20, 6, 6)
	else { ShouldntBeHere("bug"); }

	if (yaw < 0.0)
	{
		v14 = -v14;
		v13 = -v13;
	}

	if (pitch < -15){
		v10 = 25;
		v9 = 25;
	}
	else if (pitch < -10) {
		v10 = 16;
		v9 = 25;
	}
	else if (pitch < -5) {
		v10 = 7;
		v9 = 16;
	}
	else if (pitch < 0) {
		v10 = 0;
		v9 = 7;
	}
	else if (pitch < 5) {
		v10 = 0;
		v9 = -7;
	}
	else if (pitch < 10) {
		v10 = -7;
		v9 = -11;
	}
	else if (pitch < 15) {
		v10 = -11;
		v9 = -18;
	}
	else if (pitch < 20) {
		v10 = -18;
		v9 = -25;
	}
	else{
		v10 = -25;
		v9 = -25;
	}

	pitch = fabs(pitch);

	v15 = ((int)pitch % 5) + pitch - pitch;
	v16 = ((int)v5 % 10) + v5 - v5;

	v8 = v16 / 10.0;
	v7 = v15 / 5.0;

	xydelta.m_x = (v13 - v14) * v8 + v14;
	xydelta.m_y = (v11 - v12) * v8 + v12 + v10 + (v9 - v10) * v7;

	return xydelta;
}


void CharacterSprite::InitCharacterSprite(float f1, float f2, float f3, 
	float f4, float f5, float f6, 
	float f7, float f8, float f9, 
	float f10, float f11, float f12, float f13, float f14, 
	ESpriteID id)
{
	animengine_assert(g_GraceSprite != nullptr);
	charsprite_bool1 = this == g_GraceSprite;
	m_headRotYControl.InitController(&m_pHeadSprite->headrot.m_y, f1, f2, false);
	m_headRotTiltControl.InitController(&m_pHeadSprite->headrot.m_x, f3, f4, false);
	m_headRotCockControl.InitController(&m_pHeadSprite->headrot.m_z, f5, f6, false);
	m_Controller4.InitController(&unknown004.m_y, f7, f8, 0);
	m_Controller5.InitController(&m_pHeadSprite->unknown007, f9, f10, false);
	m_Controller6.InitController(&m_pHeadSprite->unknown009, f11, f12, false);
	m_eyelidHeightL.InitController(&m_pHeadSprite->unknown005, f13, f14, false);
	m_eyelidHeightR.InitController(&m_pHeadSprite->unknown006, f13, f14, false);
	m_Controller9.InitController(&m_pHeadSprite->unknown[13], f1, f2, false);
	m_Controller10.InitController(&m_pHeadSprite->unknown[12], f3, f4, false);
	m_Controller11.InitController(&m_pHeadSprite->unknown[14], f5, f6, false);
	m_Controller12.InitController(&m_pHeadSprite->unknown[17], f9, f10, false);
	m_Controller13.InitController(&m_pHeadSprite->unknown[19], f11, f12, false);
	m_Controller14.InitController(&m_pHeadSprite->unknown[15], f13, f14,0);
	m_Controller15.InitController(&m_pHeadSprite->unknown[16], f13, f14,0);
	m_Controller16.InitController(&m_pHeadSprite->unknown[20], f7, f8, 0);
	m_Controller17.InitController(&m_pHeadSprite->unknown[28], f9, f10,0);
	m_Controller18.InitController(&m_pHeadSprite->unknown[30], f11, f12, 0);
	m_Controller19.InitController(&m_pHeadSprite->unknown[26], f13, f14, 0);
	m_Controller20.InitController(&m_pHeadSprite->unknown[27], f13, f14, 0);
	m_Controller21.InitController(&unknown067_1171, -100, 100, 0);
	m_Controller22.InitController(&unknown067_1172, -100, 130, 0);
	m_Controller23.InitController(&unknown043, -5000, 5000, 0);
	m_Controller24.InitController(&unknown044, -5000, 5000, 0);
	m_Controller25.InitController(&unknown045, -1000, 1000, 0);

	//undone

	char namebuffer[256];
	strcpy(namebuffer, GetName());
	strcat(namebuffer, "GazeUtil");
	m_pGazeHelper = new Sprite();
	m_pGazeHelper->InitSprite(id, namebuffer, false, true);

	m_pGazeHelper->SetUpdateMe(false);
}


void CharacterSprite::EndArmSync()
{
	if (unknown048 == -1)
	{
		if (unknown049 == -1)
		{
			if (unknown050 == -1)
				ShouldntBeHere("error in EndArmSync");
			else
				unknown046 = GetArmSyncSetForScript(this, unknown050);
		}
		else
		{
			unknown046 = GetArmSyncSetForScript(this, unknown049);
		}
	}
	else
	{
		unknown046 = GetArmSyncSetForScript(this, unknown048);
	}
	unknown048 = -1;
	unknown049 = -1;
	unknown050 = -1;
	unknown047 = 0;
}
void CharacterSprite::ResetArmSyncSet(int i1)
{
	unknown046 = GetArmSyncSetForScript(this, i1);
	unknown048 = -1;
	unknown049 = -1;
	unknown050 = -1;
	unknown047 = 0;
}

void CharacterSprite::SetHeadRotation(XYZPointf rot)
{
	m_headRotYControl.SetVal(rot.m_y, false);
	m_headRotTiltControl.SetVal(rot.m_x, false);
	m_headRotCockControl.SetVal(rot.m_z, false);
}
XYZPointf CharacterSprite::GetHeadRotation()
{
	return XYZPointf(m_headRotYControl.GetVal(), m_headRotTiltControl.GetVal(), m_headRotCockControl.GetVal());
}

ECinematographyFraming CharacterSprite::GetCinematographyFraming()
{
	return m_eCineFraming;
}
void CharacterSprite::SetCinematographyFraming(ECinematographyFraming cineframing)
{
	m_eCineFraming = cineframing;
}

void CharacterSprite::SetTorsoRotation(XYZPointf rot)
{
	unknown004.m_x = 0;
	m_Controller4.SetVal(rot.m_y, false);
	unknown004.m_z = 0;
}

void CharacterSprite::SetHeadRotY(float y)
{
	m_headRotYControl.SetVal(y, false);
}
void CharacterSprite::SetHeadTilt(float tilt)
{
	m_headRotTiltControl.SetVal(tilt, false);
}
void CharacterSprite::SetHeadCock(float cock)
{
	m_headRotCockControl.SetVal(cock, false);
}

void CharacterSprite::SetBreathingBehavior(ELayerAltIndex index)
{
	if (index >= eLayerAltIndex_Max)
	{
		ShouldntBeHere("Illegal breatheAlt in SetBreathingBehavior");
	}
	else
	{
		//m_LayerController1.AnimateLayer(index);
		unknown067_1267 = 30 * rand2(8, 12);
	}
}

void CharacterSprite::SetHeadQuiver(float f1, float f2, float f3)
{
	animengine_assert(f1 >= 0);
	animengine_assert(f3 > 0);

	if (f1 > 0.0)
	{
		if (unknown067_1184 == 0.0)
		{
			unknown067_1182 = 0.0;
			unknown067_1185 = 0.0;
		}
		unknown067_1183 = f1;
		unknown067_1184 = FrameRateAdjust(f3, 1);
		unknown067_1186 = FloatToInt(30.0 * f2);
	}
	else
	{
		unknown067_1182 = 0.0;
		unknown067_1183 = 0.0;
		unknown067_1184 = 0.0;
	}
}
void CharacterSprite::SetBlinkBehavior(EBlinkBehavior behavior, bool b1)
{
	m_eBlinkBehavior = behavior;
	switch (m_eBlinkBehavior)
	{
	case 1:
	case 3:
		unknown066 = FrameRateAdjust(rand2(6, 8), 0);
		if (b1)
		{
			if (m_eBlinkBehavior == 1)
				m_blinkFactor1 = 2;
			else
				m_blinkFactor1 = rand2(2, 4);
		}
		else if (--m_blinkFactor1 <= 0)
		{
			m_eBlinkBehavior = (EBlinkBehavior)0;
			unknown066 *= 2;
		}
		break;
	case 2:
		unknown066 = FrameRateAdjust(rand2(1, 3), 0);
		if (b1)
		{
			m_blinkFactor1 = rand2(2, 4);
		}
		else if (m_blinkFactor1 <= 0)
		{
			m_eBlinkBehavior = (EBlinkBehavior)0;
			unknown066 *= 2;
		}
		break;
	case 4:
		unknown066 = 30 * rand2(15, 30);
		break;
	default:
		unknown066 = 30 * rand2(3, 6);
		break;
	}
}

void CharacterSprite::SetEyesMove(float f1, float f2)
{
	float target = 0.02;
	if (unknown067_1161 == 2)
		target = 0.04;

	if (!unknown067_1161)
		target = 0.01;

	if (m_Controller5.GetIsMoving())
		m_Controller5.UpdateTarget(f1);
	else
		m_Controller5.SetTargetByRate(f1, target, eControllerType_unknown02, -DATA_NULLVAL);

	if (m_Controller6.GetIsMoving())	m_Controller6.UpdateTarget(f2);
	else
		m_Controller6.SetTargetByRate(f2, target, eControllerType_unknown02, -DATA_NULLVAL);
}
void CharacterSprite::GetEyesMove(float& f1, float& f2)
{
	f1 = m_Controller5.GetVal();
	f2 = m_Controller6.GetVal();
}

void CharacterSprite::SetEyesToLookAtPlayer()
{
	XYZPointf xydelta = GetEyeXYDeltaForLookAtUser(m_pHeadSprite->unknownrot1.m_y - 180.0, m_pHeadSprite->unknownrot1.m_x);
	SetEyesMove((xydelta.m_x + 50.0) / 100.0, (xydelta.m_y + 50.0) / 100.0);
}
void CharacterSprite::SetEyeballLookAway(bool b1, bool b2, float f1, int i1, float i2)
{
	if (b1)
	{
		//undone
	}
	else
	{
		m_Controller21.SetTargetByFrames(0, 15, eControllerType_unknown02);
		m_Controller22.SetTargetByFrames(0, 15, eControllerType_unknown02);
	}
}
void CharacterSprite::SetEyeballQuiver(float f1, float f2, float f3)
{
	animengine_assert(f1 >= 0);
	animengine_assert(f3 > 0);

	//undone
}
void CharacterSprite::SetEyelidHeight(float f1, bool b1, bool b2, bool b3)
{
	if (b2)
		m_eyelidHeightL.SetTargetByFrames(f1, 15, eControllerType_unknown02);
	if (b3)
		m_eyelidHeightR.SetTargetByFrames(f1, 15, eControllerType_unknown02);
	funknown001 = f1;
	funknown002 = f1;
	if (b1)
		unknown067_1188 = 0;
	unknown063 = 0;
	unknown064 = 0;
	unknown066 = 30 * rand2(3, 6);
	m_eBlinkBehavior = eBlinkBehavior_unknown;
}
void CharacterSprite::SetBriefEyelidHeight(float f1, float f2, bool b1, bool b2)
{
	if (unknown067_1188 <= 0)
		unknown067_1187 = funknown001;

	unknown067_1188 = FloatToInt(30.0 * f2);

	SetEyelidHeight(f1, 0, b1, b2);
}

void CharacterSprite::SetGazeTrackingSprite(Sprite* sprite, EGazeSpeed speed, EGazeType type, EAlterHeadCock cock)
{
	//undone
}
void CharacterSprite::SetGazeTrackingPoint(XYZPointf point, EGazeSpeed speed, EGazeType type, EAlterHeadCock cock)
{
	animengine_assert(m_pGazeHelper != nullptr);
	m_pGazeHelper->SetPosition(point);
	SetGazeTrackingSprite(m_pGazeHelper, speed, type, cock);
}
void CharacterSprite::SetCueFromSpeakDialog(int i1)
{
	//undone
}
void CharacterSprite::SetPerformanceInfo(perfinfo_animapi_input* p1)
{
	unknown067_1058 = p1->a2;
	unknown067_1059 = p1->a3;
	unknown067_1061 = p1->a4;
	unknown067_1062 = p1->a5;
	unknown067_1063 = p1->a6;
	unknown067_1064 = p1->a7;
	unknown067_1065 = p1->a8;
	unknown067_1066 = p1->a9;
	unknown067_1067 = p1->a10;
	unknown067_1068 = p1->a11;
	unknown067_1069 = p1->a12;
	unknown067_1070 = p1->a13;
	unknown067_1071 = p1->a14;
	unknown067_1072 = p1->a15;
	unknown067_1073 = p1->a16;
	unknown067_1074 = p1->a17;
	unknown067_1076 = p1->a18;
	unknown067_1077 = p1->a19;
	unknown067_1078 = p1->a20;
	unknown067_1079 = p1->a21;
	unknown067_1080 = p1->a22;
	unknown067_1081 = p1->a23;
	unknown067_1082 = p1->a24;
	unknown067_1083 = p1->a25;
	unknown067_1084 = p1->a26;
	unknown067_1085 = p1->a27;
	unknown067_1086 = p1->a28;
	unknown067_1087 = p1->a29;
	unknown067_1088 = p1->a30;
	unknown067_1089 = p1->a31;
	unknown067_1091 = p1->a32;
	unknown067_1092 = p1->a33;
	unknown067_1093 = p1->a34;
	unknown067_1094 = p1->a35;
	unknown067_1095 = p1->a36;
	unknown067_1096 = p1->a37;
	unknown067_1097 = p1->a38;
	unknown067_1098 = p1->a39;
	unknown067_1099 = p1->a40;
	unknown067_1100 = p1->a41;
	unknown067_1101 = p1->a42;
	unknown067_1102 = p1->a43;
	unknown067_1103 = p1->a44;
	unknown067_1104 = p1->a45;
	unknown067_1106 = p1->a46;
	unknown067_1107 = p1->a47;
	unknown067_1108 = p1->a48;
	unknown067_1109 = p1->a49;
	unknown067_1110 = p1->a50;
	unknown067_1111 = p1->a51;
	unknown067_1112 = p1->a52;
	unknown067_1113 = p1->a53;
	unknown067_1114 = p1->a54;
	unknown067_1115 = p1->a55;
	unknown067_1116 = p1->a56;
	unknown067_1117 = p1->a57;
	unknown067_1118 = p1->a58;
	unknown067_1119 = p1->a59;
	unknown067_1121 = p1->a60;
	unknown067_1122 = p1->a61;
	unknown067_1123 = p1->a62;
	unknown067_1124 = p1->a63;
	unknown067_1125 = p1->a64;
	unknown067_1126 = p1->a65;
	unknown067_1127 = p1->a66;
	unknown067_1128 = p1->a67;
	unknown067_1129 = p1->a68;
	unknown067_1130 = p1->a69;
	unknown067_1131 = p1->a70;
	unknown067_1132 = p1->a71;
	unknown067_1133 = p1->a72;
	unknown067_1134 = p1->a73;
	unknown067_1136 = p1->a74;
	unknown067_1137 = p1->a75;
	unknown067_1138 = p1->a76;
	unknown067_1139 = p1->a77;
	unknown067_1140 = p1->a78;
	unknown067_1141 = p1->a79;
	unknown067_1142 = p1->a80;
	unknown067_1143 = p1->a81;
	unknown067_1144 = p1->a82;
	unknown067_1145 = p1->a83;
	unknown067_1146 = p1->a84;
	unknown067_1147 = p1->a85;
}

void CharacterSprite::SetVolumeAndPan()
{
	//undone
}

void CharacterSprite::SetLocomoting(bool option)
{
	unknown067_1777 = option;
	if (!unknown067_1777)
	{
		//undone
	}
}
bool CharacterSprite::GetLocomoting()
{
	return unknown067_1777;
}

XYZPointf CharacterSprite::GetNorm0to360HeadRotation()
{
	XYZPointf headrot = XYZPointf(m_headRotYControl.GetVal(), m_headRotTiltControl.GetVal(), m_headRotCockControl.GetVal());
	return XYZPointf(Normalize0to360(headrot.m_x), Normalize0to360(headrot.m_y), Normalize0to360(headrot.m_z));
}
XYZPointf CharacterSprite::GetNorm0to360TorsoRotation()
{
	XYZPointf torsorot;
	torsorot.m_x = Normalize0to360(unknown004.m_x);
	torsorot.m_y = Normalize0to360(m_Controller16.GetVal() + unknown004.m_y);
	torsorot.m_z = Normalize0to360(unknown004.m_z);

	return torsorot;
}
XYZPointf CharacterSprite::GetHeadPlusTorsoRotation()
{
	XYZPointf torsorot = GetNorm0to360TorsoRotation();
	XYZPointf headrot = GetNorm0to360HeadRotation();

	return headrot + torsorot;
}

Sprite* CharacterSprite::GetGazeTracking()
{
	return unknown067_1160;
}

void CharacterSprite::GetCurrentBrowRLowerEmphasis(int&, int&)
{
	//undone
}
void CharacterSprite::GetCurrentBrowLRaiseEmphasis(int&, int&)
{
	//undone
}
void CharacterSprite::GetCurrentBrowRRaiseEmphasis(int&, int&)
{
	//undone
}
void CharacterSprite::GetCurrentBrowLLowerEmphasis(int&, int&)
{
	//undone
}
int CharacterSprite::GetVolumeInterpolation(int, int, int, int, int)
{
	//undone
	return 0;
}

XYZPointf CharacterSprite::GetLocomotionTargetPoint()
{
	if (unknown067_1776)
		return unknown067_1776->GetPosition();
	else
		return unknown067_1772;
}
XYZPointf CharacterSprite::GetLocomotionTweakedFinalPoint()
{
	//undone
	return XYZPointf(0, 0, 0);
}
int CharacterSprite::GetNextWalkUScript(EUScript script)
{
	//undone
	return 0;
}

XYZPointf CharacterSprite::GetConversationPosition()
{
	//undone
	return XYZPointf(0, 0, 0);
}
float CharacterSprite::GetWalkEndLength()
{
	if (unknown067_1786 == 2 || unknown067_1786 == 4)
		return 30.0;
	else
		return 15.0;
}
bool CharacterSprite::GetIsCharacterSprite()
{
	return true;
}

bool CharacterSprite::IsPelvisModelPointIndex(int i1)
{
	if (charsprite_bool1)
	{
		switch (i1)
		{
		case 0:
		case 1:
		case 16:
		case 17:
		case 71:
		case 72:
		case 73:
		case 74:
		case 75:
			return true;
		default:
			return false;
		}
	}
	else
	{
		switch (i1)
		{
		case 0:
		case 1:
		case 6:
		case 7:
		case 16:
		case 17:
		case 44:
		case 71:
		case 72:
			return true;
		default:
			return false;
		}
	}
}
bool CharacterSprite::IsLowerBodyModelPointIndex(int i1)
{
	bool result;
	if (charsprite_bool1)
	{
		switch (i1)
		{
		case 18:
		case 19:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
			return true;
		default:
			return false;
		}
	}
	else
	{
		switch (i1)
		{
		case 18:
		case 19:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
			return true;
		default:
			return false;
		}
	}
}
bool CharacterSprite::IsLeftHandHoldingSomething()
{
	return GetFirstSpriteAttachedToModelPoint(50) != nullptr;
}
bool CharacterSprite::IsRightHandHoldingSomething()
{
	return GetFirstSpriteAttachedToModelPoint(66) != nullptr;
}
bool CharacterSprite::IsDialogQueued()
{
	return unknown067_1158 > 0;
}

bool CharacterSprite::AreWeInAnEditor(bool option)
{
	//undone
	return false;
}
bool CharacterSprite::AreWeDoingFullExpression()
{
	return !unknown068_003;
}
bool CharacterSprite::AreWeDoingTenseFullExpression()
{
	return false;
}
bool CharacterSprite::AreWeSpeaking()
{
	return unknown067_0002 != -1;
}
bool CharacterSprite::AreWePositionedForConversation(bool option)
{
	//undone
	return false;
}
bool CharacterSprite::AreWeTurningInPlace()
{
	//undone
	return false;
}
bool CharacterSprite::AreWeWalkingStraight()
{
	//undone
	return false;
}
bool CharacterSprite::GetAreWeBodyPosing()
{
	return unknown067_1268 != -1;;
}

void CharacterSprite::PossiblyWalkBackwards()
{
	//undone
}

void CharacterSprite::UpdateArmSync()
{
	//undone
}
void CharacterSprite::UpdateSprite()
{
	Sprite::UpdateSprite();
	if (g_numFramesRun > 1)
	{
		//undone
	}

	//undone
}
void CharacterSprite::UpdateBreathing()
{
	//undone
}
void CharacterSprite::UpdateCurrentPose()
{
	//undone
}
void CharacterSprite::UpdateHeadEmphasis(int i1)
{
	//undone
}
void CharacterSprite::UpdateFaceVariation()
{
	//undone
}
void CharacterSprite::UpdateGazeTracking()
{
	//undone
}
void CharacterSprite::UpdateEyeballQuiver()
{
	//undone
}
void CharacterSprite::UpdateHeadQuiverAndCock()
{
	//undone
}
void CharacterSprite::UpdateBlinking()
{
	//undone
}
void CharacterSprite::UpdateFullExpression()
{
	//undone
}
void CharacterSprite::UpdateSpeaking(bool option)
{
	//undone
}
void CharacterSprite::UpdateLocomotionKinematicTargets()
{
	//undone
}
void CharacterSprite::UpdateExactPointDeleceration()
{
	//undone
}
void CharacterSprite::UpdateLocomotion()
{
	//undone
}
void CharacterSprite::UpdateGripForCertainObjects()
{
	unknown009 = -1;
	unknown013 = -1;
	Sprite* spr1 = GetFirstSpriteAttachedToModelPoint(50);
	Sprite* spr2 = GetFirstSpriteAttachedToModelPoint(66);
	if (spr1 && spr1->m_eID == eSpriteID_eightball)
	{
		unknown009 = 50;
		unknown010 = XYZPointf(0.1, 0.3, -0.3);
	}
	if (spr2 && spr2->m_eID == eSpriteID_eightball)
	{
		unknown013 = 66;
		unknown014 = XYZPointf(-0.1, 0.3, -0.3);
	}
}

void CharacterSprite::SetupNextWordToSpeak()
{
	//undone
}

void CharacterSprite::StartBlink(int i1, int i2, int i3)
{
	//undone
}
void CharacterSprite::StartBlinkType(EBlinkType type)
{
	int v1;
	switch (type)
	{
	case 1:
		StartBlink(2, 4, rand2(10, 20));
		break;
	case 2:
		StartBlink(3, 12, rand2(20, 30));
		break;
	case 3:
		StartBlink(3, 8, rand2(10, 20));
		break;
	case 4:
		v1 = rand2(10, 20);
		StartBlink(v1, v1, rand2(20, 40));
		break;
	case 5:
		StartBlink(2, 3, rand2(3, 4));
		break;
	default:
		StartBlink(2, 3, rand2(5, 8));
		break;
	}
}
void CharacterSprite::StartHeadEmphasisStuff(int i1, int i2, int i3, float f1, float f2, float f3, bool b1, bool b2)
{
	//undone
}
void CharacterSprite::StartHeadEmphasis(int i1, bool b1, float f1, int i2, int i3)
{
	//undone
}
void CharacterSprite::StartBodyPosing(EBodyPoseConfig config)
{
	//undone
}

void CharacterSprite::StopCurrentBodyPosing()
{
	//undone
}

void CharacterSprite::DoLocomotion(XYZPointf point, float f1, Sprite* spr, EWalkType type)
{
	//undone
}
void CharacterSprite::StopLocomotion(bool option)
{
	//undone
}

void CharacterSprite::DoFullExpression(EFullExpressionMood mood, EFullExpressionBase base, Sprite* spr1, Sprite* spr2, int i1, bool b1)
{
	animengine_assert(mood <= eFEMood_turnedOn || base <= eFEBase_shocked);

	if (mood > eFE_NONE && mood != m_currentMoodExpression)
	{
		m_currentMoodExpression = mood;
		unknown068_003 = false;
	}
	if (base > eFE_NONE && base != m_currentBaseExpression)
	{
		m_currentBaseExpression = base;
		unknown068_003 = false;
	}

	if (i1 >= 0 && i1 != unknown068_004 || b1)
		unknown068_003 = 0;

	if (!unknown068_003)
	{
		unknown068_004 = i1;
		unknown068_005 = 0;
		unknown068_006 = 0;
		unknown068_007 = spr1;
		unknown068_008 = spr2;
		unknown068_009 = false;
		unknown068_012 = 0;
	}

}
void CharacterSprite::DoFullExpressionMod(EMiscLittleAction action, float farg)
{
	if (action == eMiscLittleAction_facialModNone)
	{
		if (unknown068_012 > 0)
			unknown068_012 = 1;
		return;
	}

	unknown068_012 = farg * 30;

	if (charsprite_bool1)
	{
		switch (action)
		{
		case eMiscLittleAction_facialModSerious:
			m_GestureFxnMgr10.StartGestureFxn(0, 2, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
			break;
		case eMiscLittleAction_facialModHappy:
			m_GestureFxnMgr10.StartGestureFxn(0, 5, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
			break;
		case eMiscLittleAction_facialModAnxious:
			m_GestureFxnMgr10.StartGestureFxn(0, 17, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
			break;
		case eMiscLittleAction_facialModAnxiousMed:
			m_GestureFxnMgr10.StartGestureFxn(0, 21, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 9, 0);
			break;
		case eMiscLittleAction_facialModAnxiousHigh:
			m_GestureFxnMgr10.StartGestureFxn(0, 24, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 7, 0);
			break;
		case eMiscLittleAction_facialModAnnoyed:
		case eMiscLittleAction_facialModImpatient:
			m_GestureFxnMgr10.StartGestureFxn(0, 18, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
			break;
		case eMiscLittleAction_facialModAngry:
			m_GestureFxnMgr10.StartGestureFxn(0, 19, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 4, 0);
			break;
		case eMiscLittleAction_facialModFurious:
			m_GestureFxnMgr10.StartGestureFxn(0, 22, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 5, 0);
			break;
		case eMiscLittleAction_facialModSurprised:
			m_GestureFxnMgr10.StartGestureFxn(0, 1, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
			break;
		default:
			return;
		}
	}
	else
	{
		switch (action)
		{
		case eMiscLittleAction_facialModSerious:
			m_GestureFxnMgr10.StartGestureFxn(0, 2, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
			break;
		case eMiscLittleAction_facialModHappy:
			m_GestureFxnMgr10.StartGestureFxn(0, 4, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
			break;
		case eMiscLittleAction_facialModAnxious:
			m_GestureFxnMgr10.StartGestureFxn(0, 9, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
			break;
		case eMiscLittleAction_facialModAnxiousMed:
			m_GestureFxnMgr10.StartGestureFxn(0, 15, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 7, 0);
			break;
		case eMiscLittleAction_facialModAnxiousHigh:
			m_GestureFxnMgr10.StartGestureFxn(0, 16, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 5, 0);
			break;
		case eMiscLittleAction_facialModAnnoyed:
		case eMiscLittleAction_facialModImpatient:
			m_GestureFxnMgr10.StartGestureFxn(0, 11, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
			break;
		case eMiscLittleAction_facialModAngry:
			m_GestureFxnMgr10.StartGestureFxn(0, 12, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 2, 0);
			break;
		case eMiscLittleAction_facialModFurious:
			m_GestureFxnMgr10.StartGestureFxn(0, 17, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 3, 0);
			break;
		case eMiscLittleAction_facialModSurprised:
			m_GestureFxnMgr10.StartGestureFxn(0, 1, 0);
			m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
			break;
		default:
			return;
		}
	}


}

void CharacterSprite::StopDialog()
{
	//undone
}
void CharacterSprite::SpeakDialog(int, int*, char*, int)
{
	//undone
}

float CharacterSprite::ShortestAngleToTurnTo(float yawtarget)
{
	float yawtarget_360 = yawtarget + 360.0;
	float yawtarget_neg360 = yawtarget - 360.0;

	XYZPointf charrot = GetRotation();

	float yawnormalized = Normalize0to360(charrot.m_y);

	float v3 = fabs(yawnormalized - yawtarget);
	float v2 = fabs(yawnormalized - yawtarget_360);
	float v1 = fabs(yawnormalized - yawtarget_neg360);

	if (v3 <= v2 && v3 <= v1)
		return yawtarget;
	if (v2 <= v3 && v2 <= v1)
		return yawtarget_360;
	if (v1 <= v3 && v1 <= v2)
		return yawtarget_neg360;

	ShouldntBeHere("bug in ShortestAngleToTurnTo()");
	return yawtarget;
}

void CharacterSprite::AlterPathPlanToGetInOrOutOfBar(bool b1, bool b2, XYZPointf& pointout)
{
	//undone
}

void CharacterSprite::PickNewBasePoseScript(int i1, int& i2, int& i3)
{
	//undone
}
void CharacterSprite::OffsetTorsoTracking(float f1)
{
	//undone
}
void CharacterSprite::RefreshGazeTracking()
{
	//undone
}
void CharacterSprite::QueueDialogScript(int i1, bool b1)
{
	//undone
}
void CharacterSprite::ClearDialogQueue()
{
	unknown067_1158 = 0;
}
void CharacterSprite::ConversationPositionComputation(float f1, float f2)
{
	//undone
}
void CharacterSprite::ReduceVolumeIfFacingAway(int i1, float f1)
{
	//undone
}
void CharacterSprite::PossiblyDoCorrespondingLittleActionsForDialog(int i1)
{
	if (m_eID == eSpriteID_trip && i1 == 302)
	{
		PerformMiscLittleAction(m_eID, eMiscLittleAction_tiltHead, -10, 1.0);
		PerformMiscLittleAction(m_eID, eMiscLittleAction_briefEyelidHeight, 0, 0.5);
	}
	else if (m_eID == eSpriteID_trip	&& (i1 == 281 || i1 == 286 || i1 == 266 || i1 == 300)
		||	m_eID == eSpriteID_grace	&& (i1 == 175 || i1 == 181 || i1 == 187 || i1 == 188 || i1 == 167))
	{
		PerformMiscLittleAction(m_eID, eMiscLittleAction_tiltHead, 1, 1.0);
		PerformMiscLittleAction(m_eID, eMiscLittleAction_briefEyelidHeight, 0, 0.3);
	}
	else if (m_eID == eSpriteID_trip && (i1 == 283 || i1 == 288)
		|| m_eID == eSpriteID_grace && (i1 == 178 || i1 == 183))
	{
		PerformMiscLittleAction(m_eID, eMiscLittleAction_tiltHead, 2, 1.0);
		PerformMiscLittleAction(m_eID, eMiscLittleAction_briefEyelidHeight, 0, 1.0);
	}
}



const char* CharacterSprite::GetFullExpressionBaseName(EFullExpressionBase base)
{
	switch (base)
	{
		case eFEBase_pleasant:	return "pleasant";
		case eFEBase_smile:		return "smile";
		case eFEBase_impatient: return "impatient";
		case eFEBase_hopeful:	return "hopeful";
		case eFEBase_skeptical: return "skeptical";
		case eFEBase_serious:	return "serious";
		case eFEBase_frown:		return "frown";
		case eFEBase_surprised: return "surprised";
		case eFEBase_shocked:	return "shocked";
		default: 
			ShouldntBeHere("unknown full expression in GetFullExpressionName()");
			return nullptr;
	}
}
const char* CharacterSprite::GetFullExpressionMoodName(EFullExpressionMood mood)
{
	switch (mood)
	{
	case eFEMood_neutral:			return "neutral";
	case eFEMood_angryBarely:		return "angryBarely";
	case eFEMood_angryLow:			return "angryLow";
	case eFEMood_angryMedium:		return "angryMedium";
	case eFEMood_angryHigh:			return "angryHigh";
	case eFEMood_rejectedLow:		return "rejectedLow";
	case eFEMood_rejectedMedium:	return "rejectedMedium";
	case eFEMood_crying:			return "crying";
	case eFEMood_anxiousBarely:		return "anxiousBarely";
	case eFEMood_anxiousLow:		return "anxiousLow";
	case eFEMood_anxiousMedium:		return "anxiousMedium";
	case eFEMood_happyLow:			return "happyLow";
	case eFEMood_happyMedium:		return "happyMedium";
	case eFEMood_bored:				return "bored";
	case eFEMood_turnedOn:			return "turnedOn";
	default:
		ShouldntBeHere("unknown full expression in GetFullExpressionName()");
		return nullptr;
	}
}
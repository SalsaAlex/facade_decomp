#include "facadecommon.h"
#include "classes/Sprite/CharacterSprite.h"

void CharacterSprite::InitCharacterSprite(float f1, float f2, float f3, 
	float f4, float f5, float f6, 
	float f7, float f8, float f9, 
	float f10, float f11, float f12, float f13, float f14, 
	ESpriteID id)
{
	extern CharacterSprite* g_GraceSprite;
	animengine_assert(g_GraceSprite != nullptr);
	//LOBYTE(unknown067[1825]) = this == g_GraceSprite;
	m_Controller1.InitController(&m_pHeadSprite->unknown003, f1, f2, false);
	m_Controller2.InitController(&m_pHeadSprite->unknown002, f3, f4, false);
	m_Controller3.InitController(&m_pHeadSprite->unknown004, f5, f6, false);
	m_Controller4.InitController(&unknown004.m_y, f7, f8, 0);
	m_Controller5.InitController(&m_pHeadSprite->unknown007, f9, f10, false);
	m_Controller6.InitController(&m_pHeadSprite->unknown009, f11, f12, false);
	m_Controller7.InitController(&m_pHeadSprite->unknown005, f13, f14, false);
	m_Controller8.InitController(&m_pHeadSprite->unknown006, f13, f14, false);
	m_Controller9.InitController(&m_pHeadSprite->unknown[25], f1, f2, false);
	m_Controller10.InitController(&m_pHeadSprite->unknown[24], f3, f4, false);
	m_Controller11.InitController(&m_pHeadSprite->unknown[26], f5, f6, false);
	m_Controller12.InitController(&m_pHeadSprite->unknown[29], f9, f10, false);
	m_Controller13.InitController(&m_pHeadSprite->unknown[31], f11, f12, false);
	m_Controller14.InitController(&m_pHeadSprite->unknown[27], f13, f14,0);
	m_Controller15.InitController(&m_pHeadSprite->unknown[28], f13, f14,0);
	m_Controller16.InitController(&m_pHeadSprite->unknown[32], f7, f8, 0);
	m_Controller17.InitController(&m_pHeadSprite->unknown[40], f9, f10,0);
	m_Controller18.InitController(&m_pHeadSprite->unknown[42], f11, f12, 0);
	m_Controller19.InitController(&m_pHeadSprite->unknown[38], f13, f14, 0);
	m_Controller20.InitController(&m_pHeadSprite->unknown[39], f13, f14, 0);
	m_Controller21.InitController(&unknown067[1172], -100.0, 100.0, 0);
	m_Controller22.InitController(&unknown067[1173], -100.0, 130.0, 0);
	m_Controller23.InitController(&unknown043, -5000.0, 5000.0, 0);
	m_Controller24.InitController(&unknown044, -5000.0, 5000.0, 0);
	m_Controller25.InitController(&unknown045, -1000.0, 1000.0, 0);

	//undone
}


void CharacterSprite::EndArmSync()
{
	//undone
}
void CharacterSprite::ResetArmSyncSet(int i1)
{
	//undone
}

void CharacterSprite::SetHeadRotation(XYZPointf rot)
{
	//undone
}
XYZPointf CharacterSprite::GetHeadRotation()
{
	return XYZPointf(m_Controller1.GetVal(), m_Controller2.GetVal(), m_Controller3.GetVal());
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
	m_Controller1.SetVal(y, false);
}
void CharacterSprite::SetHeadTilt(float tilt)
{
	m_Controller2.SetVal(tilt, false);
}
void CharacterSprite::SetHeadCock(float cock)
{
	m_Controller3.SetVal(cock, false);
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
		unknown067[1268] = 30 * rand2(8, 12);
	}
}

void CharacterSprite::SetHeadQuiver(float f1, float f2, float f3)
{
	animengine_assert(f1 >= 0);
	animengine_assert(f3 > 0);

	//undone
}
void CharacterSprite::SetBlinkBehavior(EBlinkBehavior behavior, bool b1)
{
	unknown067[0] = behavior;

	//undone
}

void CharacterSprite::SetEyesMove(float f1, float f2)
{
	float target = 0.02;
	if (unknown067[1162] == 2)
		target = 0.04;

	if (!unknown067[1162])
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
	//undone
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
		m_Controller7.SetTargetByFrames(f1, 15, eControllerType_unknown02);
	if (b3)
		m_Controller8.SetTargetByFrames(f1, 15, eControllerType_unknown02);
	funknown001 = f1;
	funknown002 = f1;
	if (b1)
		unknown067[1189] = 0;
	unknown063 = 0;
	unknown064 = 0;
	unknown066 = 30 * rand2(3, 6);
	unknown067[0] = 0;
}
void CharacterSprite::SetBriefEyelidHeight(float f1, float f2, bool b1, bool b2)
{
	//if (unknown067[1189] <= 0)
	//	unknown067[1188] = LODWORD(funknown001);

	unknown067[1189] = FloatToInt(30.0 * f2);

	SetEyelidHeight(f1, 0, b1, b2);
}

void CharacterSprite::SetGazeTrackingSprite(Sprite* sprite, EGazeSpeed speed, EGazeType type, EAlterHeadCock cock)
{
	//undone
}
void CharacterSprite::SetGazeTrackingPoint(XYZPointf point, EGazeSpeed speed, EGazeType type, EAlterHeadCock cock)
{
	animengine_assert(unknown001 != 0);
	unknown001->SetPosition(point);
	SetGazeTrackingSprite(unknown001, speed, type, cock);
}
void CharacterSprite::SetCueFromSpeakDialog(int i1)
{
	//undone
}
void CharacterSprite::SetPerformanceInfo(int* p1)
{
	unknown067[1059] = *p1;
	unknown067[1060] = p1[1];
	unknown067[1062] = p1[2];
	unknown067[1063] = p1[3];
	unknown067[1064] = p1[4];
	unknown067[1065] = p1[5];
	unknown067[1066] = p1[6];
	unknown067[1067] = p1[7];
	unknown067[1068] = p1[8];
	unknown067[1069] = p1[9];
	unknown067[1070] = p1[10];
	unknown067[1071] = p1[11];
	unknown067[1072] = p1[12];
	unknown067[1073] = p1[13];
	unknown067[1074] = p1[14];
	unknown067[1075] = p1[15];
	unknown067[1077] = p1[16];
	unknown067[1078] = p1[17];
	unknown067[1079] = p1[18];
	unknown067[1080] = p1[19];
	unknown067[1081] = p1[20];
	unknown067[1082] = p1[21];
	unknown067[1083] = p1[22];
	unknown067[1084] = p1[23];
	unknown067[1085] = p1[24];
	unknown067[1086] = p1[25];
	unknown067[1087] = p1[26];
	unknown067[1088] = p1[27];
	unknown067[1089] = p1[28];
	unknown067[1090] = p1[29];
	unknown067[1092] = p1[30];
	unknown067[1093] = p1[31];
	unknown067[1094] = p1[32];
	unknown067[1095] = p1[33];
	unknown067[1096] = p1[34];
	unknown067[1097] = p1[35];
	unknown067[1098] = p1[36];
	unknown067[1099] = p1[37];
	unknown067[1100] = p1[38];
	unknown067[1101] = p1[39];
	unknown067[1102] = p1[40];
	unknown067[1103] = p1[41];
	unknown067[1104] = p1[42];
	unknown067[1105] = p1[43];
	unknown067[1107] = p1[44];
	unknown067[1108] = p1[45];
	unknown067[1109] = p1[46];
	unknown067[1110] = p1[47];
	unknown067[1111] = p1[48];
	unknown067[1112] = p1[49];
	unknown067[1113] = p1[50];
	unknown067[1114] = p1[51];
	unknown067[1115] = p1[52];
	unknown067[1116] = p1[53];
	unknown067[1117] = p1[54];
	unknown067[1118] = p1[55];
	unknown067[1119] = p1[56];
	unknown067[1120] = p1[57];
	unknown067[1122] = p1[58];
	unknown067[1123] = p1[59];
	unknown067[1124] = p1[60];
	unknown067[1125] = p1[61];
	unknown067[1126] = p1[62];
	unknown067[1127] = p1[63];
	unknown067[1128] = p1[64];
	unknown067[1129] = p1[65];
	unknown067[1130] = p1[66];
	unknown067[1131] = p1[67];
	unknown067[1132] = p1[68];
	unknown067[1133] = p1[69];
	unknown067[1134] = p1[70];
	unknown067[1135] = p1[71];
	unknown067[1137] = p1[72];
	unknown067[1138] = p1[73];
	unknown067[1139] = p1[74];
	unknown067[1140] = p1[75];
	unknown067[1141] = p1[76];
	unknown067[1142] = p1[77];
	unknown067[1143] = p1[78];
	unknown067[1144] = p1[79];
	unknown067[1145] = p1[80];
	unknown067[1146] = p1[81];
	unknown067[1147] = p1[82];
	unknown067[1148] = p1[83];
}

void CharacterSprite::SetVolumeAndPan()
{
	//undone
}

void CharacterSprite::SetLocomoting(bool option)
{
	//undone
}
bool CharacterSprite::GetLocomoting()
{
	return unknown067[1778];
}

XYZPointf CharacterSprite::GetNorm0to360HeadRotation()
{
	XYZPointf headrot = XYZPointf(m_Controller1.GetVal(), m_Controller2.GetVal(), m_Controller3.GetVal());
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
	return (Sprite*)&unknown067[1161];
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
	//undone
	return XYZPointf(0, 0, 0);
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
	//undone
	return 0;
}
bool CharacterSprite::GetIsCharacterSprite()
{
	return true;
}

bool CharacterSprite::IsPelvisModelPointIndex(int i1)
{
	//undone
	return false;
}
bool CharacterSprite::IsLowerBodyModelPointIndex(int i1)
{
	//undone
	return false;
}
bool CharacterSprite::IsLeftHandHoldingSomething()
{
	//undone
	return false;
}
bool CharacterSprite::IsRightHandHoldingSomething()
{
	//undone
	return false;
}
bool CharacterSprite::IsDialogQueued()
{
	//undone
	return false;
}

bool CharacterSprite::AreWeInAnEditor(bool option)
{
	//undone
	return false;
}
bool CharacterSprite::AreWeDoingFullExpression()
{
	//undone
	return false;
}
bool CharacterSprite::AreWeDoingTenseFullExpression()
{
	//undone
	return false;
}
bool CharacterSprite::AreWeSpeaking()
{
	//undone
	return false;
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
	//undone
	return false;
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
	//undone
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
	//undone
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
	//undone
}
void CharacterSprite::DoFullExpressionMod(EMiscLittleAction action, float f1)
{
	//undone
}

void CharacterSprite::StopDialog()
{
	//undone
}
void CharacterSprite::SpeakDialog(int, int*, char*, int)
{
	//undone
}

float CharacterSprite::ShortestAngleToTurnTo(float target)
{
	//undone
	return 0;
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
	//undone
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
	//undone
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
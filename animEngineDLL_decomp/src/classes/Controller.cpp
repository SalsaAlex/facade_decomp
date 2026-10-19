#include "facadecommon.h"
#include "classes/Controller.h"

void Controller::InitController(float* valptr, float valmin, float valmax, bool b1)
{
	animengine_assert(valmin <= valmax);
	m_valPtr = valptr;
	m_valMin = valmin;
	m_valMax = valmax;
	unknown11 = b1;
}

void Controller::SetTargetByFrames(float target, int numframes, EControllerType controltype)
{
	animengine_assert(this->m_eControllerType != eControllerType_unknown03);
	if (numframes > 0)
	{
		float RateByFrames = GetRateByFrames(target, numframes);
		if (RateByFrames != -666.0)
			SetTargetByRate(target, RateByFrames, controltype, -666);
	}
	else
	{
		SetVal(target, true);
	}
}
void Controller::SetTargetByRate(float target, float rate, EControllerType controltype, float f1)
{
	//undone
}
float Controller::GetRateByFrames(float f1, int numframes)
{
	animengine_assert(m_valPtr != nullptr);
	if (numframes <= 0)
	{
		ShouldntBeHere("numFrames must be > 0 in GetRateByFrames()");
		return -666;
	}

	//undone

	return -666.0;
}


void Controller::SetVal(float value, bool keepMoving)
{
	animengine_assert(m_valPtr != nullptr);
	ClipFloatToRange(value, m_valMin, m_valMax);
	*m_valPtr = value;
	if (!keepMoving)
		m_bIsMoving = false;
}
float Controller::GetVal()
{
	animengine_assert(m_valPtr != nullptr);
	return *m_valPtr;
}

void Controller::SetMaxVal(float val)
{
	m_valMax = val;
}
void Controller::SetMinVal(float val)
{
	m_valMin = val;
}

bool Controller::GetIsMoving()
{
	return m_bIsMoving;
}
void Controller::StopMoving()
{
	m_bIsMoving = false;
}

void Controller::UpdateTarget(float target)
{
	animengine_assert(m_valPtr != nullptr);
	animengine_assert(m_bIsMoving);

	ClipFloatToRange(target, m_valMin, m_valMax);
	m_valTarget = target;
}
void Controller::UpdateController()
{
	//undone
}
void Controller::Offset(float offs)
{
	SetVal(GetVal() + offs, true);
	UpdateTarget(m_valTarget + offs);
}
#include "facadecommon.h"
#include "classes/Sprite/HeadSprite.h"

void HeadSprite::StartFaceAnimMorph(FaceFeatureMorph* morph, FaceAnimSet* animset, int animindex, float f1, bool b1, float f2)
{
	animengine_assert(animindex < animset->numsets);

	if (f2 < 0.0)	f2 = randFloat2(0.80000001, 1.05, 1000.0);

	//undone
}

void HeadSprite::TweakHeadExpressionRotations(float val)
{
	//undone
}


void HeadSprite::SetEyeExpressionIndex(int i1, bool option)
{
	m_iCurrentEyeExprIndex = i1;
	m_pCurrentEyeExprPtr = m_HeadExpressionData[i1].faceanimdata;

	//undone
}
void HeadSprite::SetHeadExpressionIndex(int i1, bool option, int i2)
{
	m_iCurrentHeadExprIndex = i1;
	m_pCurrentHeadExprPtr = m_HeadExpressionData[i1].faceanimdata;

	//undone
}
void HeadSprite::SetEyesShape(int i1, float val)
{
	m_EyeShapeMorphInfo = i1;
	StartFaceAnimMorph(
		&m_EyeShapeMorph,
		&m_EyesShapeData,
		m_EyeShapeMorphInfo,
		randFloat2(0.75, 1.2, 1000.0) * 0.25,
		0,
		val);
}
void HeadSprite::SetMouthShape(int i1, float val)
{
	m_MouthShapeMorphInfo = i1;
	StartFaceAnimMorph(
		&m_MouthShapeMorph,
		&m_MouthShapeData,
		m_MouthShapeMorphInfo,
		randFloat2(0.75, 1.2, 1000.0) * 0.25,
		0,
		val);
}
void HeadSprite::SetMouthViseme(int i1, float val)
{
	m_MouthVisemeMorphInfo = i1;

	//undone
}
void HeadSprite::SetBrowR(int i1, float val)
{
	m_BrowRMorphInfo = i1;
	StartFaceAnimMorph(
		&m_BrowRMorph,
		&m_RBrowData,
		m_BrowRMorphInfo,
		randFloat2(0.75, 1.2, 1000.0) * 0.25,
		0,
		val);
}
void HeadSprite::SetBrowL(int i1, float val)
{
	m_BrowLMorphInfo = i1;
	StartFaceAnimMorph(
		&m_BrowLMorph,
		&m_LBrowData,
		m_BrowLMorphInfo,
		randFloat2(0.75, 1.2, 1000.0) * 0.25,
		0,
		val);
}
#ifndef HEADSPRITE_H
#define HEADSPRITE_H

class CharacterSprite;

struct animset_t
{
	char name[64];
	void* faceanimdata;
};
struct FaceAnimSet
{
	animset_t sets[35];
	int numsets;
	int info;
};
struct FaceFeatureMorph
{
	int unknown[278];
};

#define MAX_HEADEXPRESSION 50

class HeadSprite : public Sprite								//proof: libnativeAnimInterface.jnilib:__const_coal:00206780
{
public:

	void StartFaceAnimMorph(FaceFeatureMorph* morph, FaceAnimSet* animset, int animindex, float f1, bool b1, float f2);

	void TweakHeadExpressionRotations(float val);

	void SetEyeExpressionIndex(int i1, bool option);
	void SetHeadExpressionIndex(int i1, bool option, int i2);
	void SetEyesShape(int i1, float val);
	void SetMouthShape(int i1, float val);
	void SetMouthViseme(int i1, float val);
	void SetBrowR(int i1, float val);
	void SetBrowL(int i1, float val);

	int GetEyesShape() { return m_EyeShapeMorphInfo; };
	int GetMouthShape() { return m_MouthShapeMorphInfo; };
	int GetBrowR() { return m_BrowRMorphInfo; };
	int GetBrowL() { return m_BrowLMorphInfo; };

	CharacterSprite* owner;
	XYZPointf headrot;
	float unknown005;
	float unknown006;
	float unknown007;
	float unknown008;
	float unknown009;
	XYZPointf unknownrot1;
	float unknown010[10];
	void* m_pCurrentHeadExprPtr;
	int m_iCurrentHeadExprIndex;
	float unknown010a[9];
	void* m_pCurrentEyeExprPtr;
	int m_iCurrentEyeExprIndex;
	float unknown010b[117];
	FaceAnimSet m_LBrowData;
	FaceAnimSet m_RBrowData;
	FaceAnimSet m_MouthShapeData;
	FaceAnimSet m_MouthVisemeData;
	FaceAnimSet m_EyesShapeData;
	animset_t m_HeadExpressionData[MAX_HEADEXPRESSION];
	int m_iNumHeadExpressionData;
	FaceFeatureMorph m_BrowLMorph;
	int m_BrowLMorphInfo;
	FaceFeatureMorph m_BrowRMorph;
	int m_BrowRMorphInfo;
	FaceFeatureMorph m_MouthShapeMorph;
	int m_MouthShapeMorphInfo;
	FaceFeatureMorph m_MouthVisemeMorph;
	int m_MouthVisemeMorphInfo;
	FaceFeatureMorph m_EyeShapeMorph;
	int m_EyeShapeMorphInfo;

private:
protected:

};

static_assert(sizeof(HeadSprite) == 105648, "");				//proof: animEngineDLL.dll:.text:1006C45D

#endif // HEADSPRITE_H
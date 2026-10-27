#ifndef HEADSPRITE_H
#define HEADSPRITE_H

class CharacterSprite;

struct head_facedata_t
{
	char name[64];
	void* faceanimdata;
};

#define MAX_LBROW 35
#define MAX_RBROW 35
#define MAX_MOUTHSHAPE 35
#define MAX_MOUTHVISEME 35
#define MAX_EYESSHAPE 35
#define MAX_HEADEXPRESSION 50

class HeadSprite : public Sprite								//proof: libnativeAnimInterface.jnilib:__const_coal:00206780
{
public:

	CharacterSprite* owner;
	XYZPointf headrot;
	float unknown005;
	float unknown006;
	float unknown007;
	float unknown008;
	float unknown009;
	XYZPointf unknownrot1;
	float unknown010[140];
	head_facedata_t m_LBrowData[MAX_LBROW];
	int m_iNumLBrowData;
	int m_iLBrowInfo;
	head_facedata_t m_RBrowData[MAX_RBROW];
	int m_iNumRBrowData;
	int m_iRBrowInfo;
	head_facedata_t m_MouthShapeData[MAX_MOUTHSHAPE];
	int m_iNumMouthShapeData;
	int m_iMouthShapeInfo;
	head_facedata_t m_MouthVisemeData[MAX_MOUTHVISEME];
	int m_iNumMouthVisemeData;
	int m_iMouthVisemeInfo;
	head_facedata_t m_EyesShapeData[MAX_EYESSHAPE];
	int m_iNumEyesShapeData;
	int m_iEyesShapeInfo;
	head_facedata_t m_HeadExpressionData[MAX_HEADEXPRESSION];
	int m_iNumHeadExpressionData;
	int unknown011[1395];

private:
protected:

};

static_assert(sizeof(HeadSprite) == 105648, "");				//proof: animEngineDLL.dll:.text:1006C45D

#endif // HEADSPRITE_H
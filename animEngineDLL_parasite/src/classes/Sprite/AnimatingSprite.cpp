#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"
#include "classes/Sprite/AnimatingSprite.h"
#include "classes/SoundMgr.h"


void AnimatingSprite::UpdatePositionAndRotation(float* f1)
{
	float yaw = m_originalRot.m_y / (180 / MATH_PI);

	XYZPointf vector = XYZPointf(sin(yaw), cos(yaw), yaw);

	XYZPointf vec2; 
	vec2.m_x = vector.m_y * f1[0] + vector.m_x * f1[2];
	vec2.m_y = f1[1];
	vec2.m_z = -f1[2] * vector.m_y + vector.m_x * f1[0];


	//undone
}

void AnimatingSprite::InitAnimatingSprite(ESpriteID id, const char* name, int nummodelpoints, int a5, int a6, int a7, int a8)
{
	char pathbuffer[256];

	Sprite::InitSprite(id, name, true, true, nummodelpoints);
	m_animMgr.unknown[80934] = a7;
	//m_animMgr.InitAnimationManager(this, name, a5, a8);
	m_animMgr.unknown[80933] = a6;
	sprintf(pathbuffer, "sounds\\%s\\%ssnd.txt", name, name);
	m_soundSrc.InitSoundSource(pathbuffer);
	animsprite_unknown002 = g_soundMgr->AddSoundSource(&m_soundSrc);
}

void AnimatingSprite::SetOrigRotation(XYZPointf rot)
{
	rot.m_y = Normalize0to360(rot.m_y);
	m_originalRot = rot;
}

void AnimatingSprite::SetPosition(XYZPointf pos, float collidesize)
{
	if (collidesize < 0.0 || !CollisionTestWithWorld(this, pos, collidesize, true))
	{
		m_Pos = pos;
		m_originalPos = pos;
	}
}

void AnimatingSprite::SetRotation(XYZPointf rot)
{
	m_Rot.m_x = Normalize0to360(rot.m_x);
	m_Rot.m_y = Normalize0to360(rot.m_y);
	m_Rot.m_z = Normalize0to360(rot.m_z);

	m_originalRot = m_Rot;
}
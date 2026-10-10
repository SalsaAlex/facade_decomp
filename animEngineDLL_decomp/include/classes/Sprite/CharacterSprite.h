#ifndef CHARACTERSPRITE_H
#define CHARACTERSPRITE_H

#include "AnimatingSprite.h"

class CharacterSprite : public AnimatingSprite					//proof: libnativeAnimInterface.jnilib:__const_coal:00206620
{
public:


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
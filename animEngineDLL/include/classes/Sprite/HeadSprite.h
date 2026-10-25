#ifndef HEADSPRITE_H
#define HEADSPRITE_H

class CharacterSprite;

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
	XYZPointf unknownxyz1;
	float unknown[5371];

private:
protected:

};

static_assert(sizeof(HeadSprite) == 105648, "");				//proof: animEngineDLL.dll:.text:1006C45D

#endif // HEADSPRITE_H
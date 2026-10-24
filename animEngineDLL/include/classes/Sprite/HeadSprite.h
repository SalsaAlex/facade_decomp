#ifndef HEADSPRITE_H
#define HEADSPRITE_H

class HeadSprite : public Sprite								//proof: libnativeAnimInterface.jnilib:__const_coal:00206780
{
public:

	int unknown001;
	float unknown002;
	float unknown003;
	float unknown004;
	float unknown005;
	float unknown006;
	float unknown007;
	int unknown008;
	float unknown009;
	float unknown[5374];

private:
protected:

};

static_assert(sizeof(HeadSprite) == 105648, "");				//proof: animEngineDLL.dll:.text:1006C45D

#endif // HEADSPRITE_H
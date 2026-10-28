#ifndef DRINKSPRITE_H
#define DRINKSPRITE_H

#include "Sprite.h"

class DrinkSprite : public Sprite								//proof: libnativeAnimInterface.jnilib:__const_coal:00206800
{
public:
	DrinkSprite();
	void InitDrinkSprite();


	void SetDrinkFilmstrip(ETextureFilmstrip filmstrip);
	void SetDrinkType(EDrink type);

	void PlayerSipDrink();
	void ChrSipDrink(Sprite* sipper);

	bool IsSipping();

private:
	int m_iPlayerSipAmnt;

protected:

};

static_assert(sizeof(DrinkSprite) == 84120, "");				//proof: animEngineDLL.dll:.text:10076871

#endif // DRINKSPRITE_H
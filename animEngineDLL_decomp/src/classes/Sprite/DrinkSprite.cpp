#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"
#include "classes/Sprite/DrinkSprite.h"

static void ChangeScarecrowPanelSize(int i1, float f1, float f2)
{

}

DrinkSprite::DrinkSprite()
	: Sprite()
{
	m_eObjectType = eObjectType_Drink;
}
void DrinkSprite::InitDrinkSprite()
{
	unknown = 0;
}


void DrinkSprite::SetDrinkFilmstrip(ETextureFilmstrip filmstrip)
{
	SetPanelSpriteTextureFilmstrip(filmstrip);

	SetDrawMe(GetDrawMe()); //load filmstrip into memory
}
void DrinkSprite::SetDrinkType(EDrink type)
{
	switch (type)
	{
    case eDrink_whiteWine:
        SetDrinkFilmstrip(eTextureFilmstrip_WineWhite);
        ChangeScarecrowPanelSize(a1_unknown00009, 14, 14);
        a1_unknown00007 = 14 - 12;
        break;
    case eDrink_merlot:
    case eDrink_firstFancyDrink:
    case eDrink_cabernet:
        SetDrinkFilmstrip(eTextureFilmstrip_WineRed);
        ChangeScarecrowPanelSize(a1_unknown00009, 14, 14);
        a1_unknown00007 = 14 - 12;
        break;
    case eDrink_lightBeer:
        SetDrinkFilmstrip(eTextureFilmstrip_Beerlight);
        ChangeScarecrowPanelSize(a1_unknown00009, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
    case eDrink_darkBeer:
        SetDrinkFilmstrip(eTextureFilmstrip_BeerDark);
        ChangeScarecrowPanelSize(a1_unknown00009, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
    case eDrink_orangeJuice:
    case eDrink_appleJuice:
    case eDrink_tomatoJuice:
    case eDrink_cranberryJuice:
        SetDrinkFilmstrip(eTextureFilmstrip_Juice);
        ChangeScarecrowPanelSize(a1_unknown00009, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
    case eDrink_sparklingWater:
    case eDrink_clearSoda:
    case eDrink_plainWater:
        SetDrinkFilmstrip(eTextureFilmstrip_Water);
        ChangeScarecrowPanelSize(a1_unknown00009, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
    case eDrink_colaSoda:
        SetDrinkFilmstrip(eTextureFilmstrip_Cola);
        ChangeScarecrowPanelSize(a1_unknown00009, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
    case eDrink_margarita:
    case eDrink_scotch:
    case eDrink_brownLiquor:
        SetDrinkFilmstrip(eTextureFilmstrip_LiquorScotch);
        ChangeScarecrowPanelSize(a1_unknown00009, 10, 10);
        a1_unknown00007 = 2;
        break;
    case eDrink_martiniCosmo:
        SetDrinkFilmstrip(eTextureFilmstrip_MartiniCosmo);
        ChangeScarecrowPanelSize(a1_unknown00009, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
    case eDrink_clearLiquor:
        SetDrinkFilmstrip(eTextureFilmstrip_LiquorClear);
        ChangeScarecrowPanelSize(a1_unknown00009, 10, 10);
        a1_unknown00007 = 2;
        break;
    default:
        SetDrinkFilmstrip(eTextureFilmstrip_MartiniClassic);
        ChangeScarecrowPanelSize(a1_unknown00009, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
	}
}

void DrinkSprite::PlayerSipDrink()
{
    unknown = 30;
    AddEventInfoToLog(eEventLogType_DoInteraction,
        eSpriteID_player,
        nullptr, false,
        g_spriteMgr.GetSpriteIDBySpritePtr(this),
        0);
}
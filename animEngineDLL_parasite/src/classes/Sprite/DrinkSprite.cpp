#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"
#include "classes/Sprite/DrinkSprite.h"

static void ChangeScarecrowPanelSize(int panelindex, float x, float y)
{
    XYZPointf mdlpoints[5];
    mdlpoints[0] = XYZPointf(0, 0, 0);
    mdlpoints[1] = XYZPointf(-x / 2, -y / 2, 0);
    mdlpoints[2] = XYZPointf(mdlpoints[1].m_x, y / 2, 0);
    mdlpoints[3] = XYZPointf(x / 2, mdlpoints[2].m_y, 0);
    mdlpoints[4] = XYZPointf(mdlpoints[3].m_x, mdlpoints[1].m_y, 0);

    for (int i = 0; i < 5; ++i)
        g_panelModelFrames[panelindex].frames[i] = mdlpoints[i];
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
        ChangeScarecrowPanelSize(m_iPanelIndex, 14, 14);
        a1_unknown00007 = 14 - 12;
        break;
    case eDrink_merlot:
    case eDrink_firstFancyDrink:
    case eDrink_cabernet:
        SetDrinkFilmstrip(eTextureFilmstrip_WineRed);
        ChangeScarecrowPanelSize(m_iPanelIndex, 14, 14);
        a1_unknown00007 = 14 - 12;
        break;
    case eDrink_lightBeer:
        SetDrinkFilmstrip(eTextureFilmstrip_Beerlight);
        ChangeScarecrowPanelSize(m_iPanelIndex, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
    case eDrink_darkBeer:
        SetDrinkFilmstrip(eTextureFilmstrip_BeerDark);
        ChangeScarecrowPanelSize(m_iPanelIndex, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
    case eDrink_orangeJuice:
    case eDrink_appleJuice:
    case eDrink_tomatoJuice:
    case eDrink_cranberryJuice:
        SetDrinkFilmstrip(eTextureFilmstrip_Juice);
        ChangeScarecrowPanelSize(m_iPanelIndex, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
    case eDrink_sparklingWater:
    case eDrink_clearSoda:
    case eDrink_plainWater:
        SetDrinkFilmstrip(eTextureFilmstrip_Water);
        ChangeScarecrowPanelSize(m_iPanelIndex, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
    case eDrink_colaSoda:
        SetDrinkFilmstrip(eTextureFilmstrip_Cola);
        ChangeScarecrowPanelSize(m_iPanelIndex, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
    case eDrink_margarita:
    case eDrink_scotch:
    case eDrink_brownLiquor:
        SetDrinkFilmstrip(eTextureFilmstrip_LiquorScotch);
        ChangeScarecrowPanelSize(m_iPanelIndex, 10, 10);
        a1_unknown00007 = 2;
        break;
    case eDrink_martiniCosmo:
        SetDrinkFilmstrip(eTextureFilmstrip_MartiniCosmo);
        ChangeScarecrowPanelSize(m_iPanelIndex, 12, 12);
        a1_unknown00007 = 12 - 12;
        break;
    case eDrink_clearLiquor:
        SetDrinkFilmstrip(eTextureFilmstrip_LiquorClear);
        ChangeScarecrowPanelSize(m_iPanelIndex, 10, 10);
        a1_unknown00007 = 2;
        break;
    default:
        SetDrinkFilmstrip(eTextureFilmstrip_MartiniClassic);
        ChangeScarecrowPanelSize(m_iPanelIndex, 12, 12);
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
        g_spriteMgr->GetSpriteIDBySpritePtr(this),
        0);
}

bool DrinkSprite::IsSipping()
{
    return unknown > 0;
}
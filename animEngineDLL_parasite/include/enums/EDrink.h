#ifndef ENUM_DRINK_H
#define ENUM_DRINK_H

enum EDrink
{
	eDrink_UNDECIDED = 4998,
	eDrink_NOTADRINK,
	eDrink_whiteWine,
	eDrink_merlot,
	eDrink_lightBeer,
	eDrink_darkBeer,
	eDrink_orangeJuice,
	eDrink_appleJuice,
	eDrink_tomatoJuice,
	eDrink_cranberryJuice,
	eDrink_sparklingWater,
	eDrink_clearSoda,
	eDrink_colaSoda,
	eDrink_plainWater,

	eDrink_firstFancyDrink,
	eDrink_sangria = eDrink_firstFancyDrink,
	eDrink_cabernet,
	eDrink_margarita,

	eDrink_firstMartini,
	eDrink_martiniClassic = eDrink_firstMartini,
	eDrink_martiniCosmo,
	eDrink_lastMartini = eDrink_martiniCosmo,

	eDrink_scotch,
	eDrink_brownLiquor,
	eDrink_clearLiquor,
	eDrink_lastNonTripInvention = eDrink_clearLiquor,

	eDrink_firstTripInvention,
	eDrink_tripInvention1 = eDrink_firstTripInvention,
	eDrink_tripInvention2,
	eDrink_lastTripInvention = eDrink_tripInvention2,

	eDrink_lastFancyDrink = eDrink_lastTripInvention,
	eDrink_lastDrink = eDrink_lastFancyDrink,
	eDrink_lastDrinkPlus1 = eDrink_lastDrink + 1,
};

#endif // ENUM_DRINK_H
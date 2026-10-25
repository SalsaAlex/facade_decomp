#ifndef ENUM_COLLIDABLETYPE_H
#define ENUM_COLLIDABLETYPE_H

enum ECollidableType
{
	eCollidableType_NonSolid = 0,
	eCollidableType_SimpleBox, //grace and trip
	eCollidableType_ComplexModel, // couch, bar, side table, etc
	eCollidableType_Panel //entities created with CreatePanel() have this if collide is true
};

#endif // ENUM_COLLIDABLETYPE_H
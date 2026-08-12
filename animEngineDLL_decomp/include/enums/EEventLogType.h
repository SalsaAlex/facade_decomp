#ifndef ENUM_EVENTLOGTYPE_H
#define ENUM_EVENTLOGTYPE_H

enum EEventLogType
{
	eEventLogType_unknown00 = 0,
	eEventLogType_Speech = 1,
	eEventLogType_EnterInteraction = 2,
	eEventLogType_StopInteraction = 3,
	eEventLogType_DoInteraction = 4,
	eEventLogType_Hug = 7,
	eEventLogType_Comfort = 8,
	eEventLogType_Kiss = 9,
	eEventLogType_ThrowDrink = 10, //interesting!
	eEventLogType_PlayerExit = 11,
	eEventLogType_Miscellaneous = 12
};

#endif // ENUM_EVENTLOGTYPE_H
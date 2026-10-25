#ifndef FACADE_ZONE_H
#define FACADE_ZONE_H

#define NUMRECTS_PERZONE 5
struct zone_t
{
	XZRectf rects[NUMRECTS_PERZONE];
	int numrects;
	Sprite* unknown001;
	bool unknown002;
};

#define MAXZONES 250

DEFINE_HOOKEDARRAYVAR(zone_t, g_zones, 0x1022A960)
DEFINE_HOOKEDVAR(int, g_numZones, 0x1023033C)
DEFINE_HOOKEDARRAYVAR(int, g_roomZoneIDs, 0x106988CC)

#define g_zones			_g_zones()
#define g_numZones		_g_numZones()
#define g_roomZoneIDs	_g_roomZoneIDs()


int CreateNewZone(Sprite* sprite, bool option);
void AddRectToZone(int zoneindex, XZRectf rect);
bool IsRectWithinZone(XZRectf rect, int zoneindex);
void ModifyZoneRect(int zoneindex, int rectindex, XZRectf rect);
bool IsPointWithinZone(XYZPointf point, int zoneindex);
int GetZoneIDForRoomZone(int roomzoneid);


#endif //FACADE_ZONE_H
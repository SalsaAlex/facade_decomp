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
extern zone_t g_zones[MAXZONES];
extern int g_numZones;

extern int g_roomZoneIDs[1];


int CreateNewZone(Sprite* sprite, bool option);
void AddRectToZone(int zoneindex, XZRectf rect);
bool IsRectWithinZone(XZRectf rect, int zoneindex);
void ModifyZoneRect(int zoneindex, int rectindex, XZRectf rect);
bool IsPointWithinZone(XYZPointf point, int zoneindex);
int GetZoneIDForRoomZone(int roomzoneid);


#endif //FACADE_ZONE_H
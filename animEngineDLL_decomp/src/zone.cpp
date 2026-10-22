#include "facadecommon.h"
#include "zone.h"

#define MAXZONES 250
zone_t g_zones[MAXZONES];
int g_numZones;

int g_roomZoneIDs[1];

int CreateNewZone(Sprite* sprite, bool option) {
	animengine_assert(g_numZones < MAXZONES);
	zone_t* zone = &g_zones[g_numZones];

	zone->numrects = 0;
	zone->unknown001 = sprite;
	zone->unknown002 = option;
	if (sprite)
		sprite->a1_unknown00011c = true;

	return g_numZones++;
}

void AddRectToZone(int zoneindex, XZRectf rect) {
	animengine_assert(zoneindex < g_numZones);

	zone_t* zone = &g_zones[zoneindex];
	animengine_assert(zone->numrects <= NUMRECTS_PERZONE - 1);

	zone->rects[zone->numrects] = rect;
	++zone->numrects;
}

bool IsRectWithinZone(XZRectf rect, int zoneindex) {
	animengine_assert(zoneindex < g_numZones);

	zone_t* zone = &g_zones[zoneindex];

	for (int i = 0; i < zone->numrects; i++)
	{
		if (DoXZRectsIntersect(rect, zone->rects[i]))
			return true;
	}
	return false;
}

void ModifyZoneRect(int zoneindex, int rectindex, XZRectf rect) {
	animengine_assert(zoneindex < g_numZones);

	zone_t* zone = &g_zones[zoneindex];
	animengine_assert(rectindex < zone->numrects);

	zone->rects[rectindex] = rect;
}

bool IsPointWithinZone(XYZPointf point, int zoneindex) {
	XZRectf rect;
	rect.m_xmin = rect.m_xmax = point.m_x;
	rect.m_zmin = rect.m_zmax = point.m_z;

	return IsRectWithinZone(rect, zoneindex);
}

int GetZoneIDForRoomZone(int roomzoneid) {
	animengine_assert(roomzoneid == 0);
	return g_roomZoneIDs[roomzoneid];
}
#include "facadecommon.h"
#include "facade_mdldata.h"
#include "glfuncs.h"
#include "zone.h"

#include "classes/Sprite/AnsweringMachineSprite.h"
#include "classes/Sprite/DoorSprite.h"
#include "classes/Sprite/DrinkSprite.h"
#include "classes/Sprite/EightballMessageSprite.h"
#include "classes/Sprite/PhoneSprite.h"

#include "classes/XCursor.h"

static void InitEnvironment()
{
	_set_g_numZones(0);
	for (int i = 0; i < MAXZONES; ++i)
		g_zones[i].numrects = -1;

	//for (j = 0; j < 90; ++j)
	//{
	//	for (k = 0; k < 90; ++k)
	//		g_fixedPathPlanGrid[90 * j + k] = 0;
	//}
	//g_printMapCtr = -1;
}
static void InitCinematography()
{
	_set_g_bCinematographyOn(false);
	_set_g_frameWhenLastCinematographyMove(0);
	_set_g_framePlayerLastMoved(0);
	_set_g_bRequestAutoFraming(false);
	g_autoTurnController->InitController(_rawaddr_g_autoTurnAmount(), -1000, 1000, true);
}
static void InitPlayerNavigationVars()
{
	_set_g_playerTransAmt(0);
	_set_g_playerRotAmt(0);

	_set_g_bPlayerRotateLeft(false);
	_set_g_bPlayerRotateRight(false);
	_set_g_bPlayerMoveForwards(false);
	_set_g_bPlayerMoveBackwards(false);
	_set_g_bPlayerMoveUpwards(false);
	_set_g_bPlayerMoveDownwards(false);

	_set_g_AutoMovePlayerStatus(0);
	_set_g_bSeatedOnCouch(false);
	_set_g_bTripCarry(false);
}
static void InitPlayerNavigation()
{
	InitPlayerNavigationVars();
	g_PlayerXController->InitController(&g_autoMovePlayerPt->m_x, -10000, 10000, false);
	g_PlayerYController->InitController(&g_autoMovePlayerPt->m_x, -10000, 10000, false);
	g_PlayerZController->InitController(&g_autoMovePlayerPt->m_x, -10000, 10000, false);
	g_PlayerRotController->InitController(_rawaddr_g_autoMovePlayerRot(), -1000, 1000, false);
}

static Sprite* CreateCursorSprite()
{
	Sprite* spr = new Sprite();
	spr->InitSprite(eSpriteID_cursor, "cursor", false, true);
	return spr;
}

#define START_SPRITECREATE(name, mdlframe, mdlinfo, mdlquadpoly, mdlquadlines) \
	static void Create##name##Sprite(ESpriteID spriteid, const char* spritename){ \
		Sprite* sprite = new Sprite(); \
		sprite->m_modelframe = mdlframe; \
		sprite->m_modelinfo = mdlinfo; \
		sprite->m_modelquadpoly = mdlquadpoly; \
		sprite->m_modelquadlines = mdlquadlines; \
		sprite->InitSprite(spriteid, spritename, true, true); \
		sprite->SetVolume(XYZPointf(1, 1, 1)); 

#define END_SPRITECREATE() \
		sprite->SetCollidableType(eCollidableType_ComplexModel); \
	}

START_SPRITECREATE(Couch,
	g_Couch_modelFrame,
	g_Couch_modelInfo,
	g_Couch_modelQuadPolys,
	nullptr/*g_Couch_modelQuadLines*/)

	sprite->AddSurface(14, 20, 23, 19, eSurfacePickupAngle_90);
	sprite->AddSurface(4, 10, 25, 5, eSurfacePickupAngle_90);
	sprite->AddSurface(8, 27, 12, 7, eSurfacePickupAngle_90);

END_SPRITECREATE()
START_SPRITECREATE(Cabinet,
	g_Cabinet_modelFrame,
	g_Cabinet_modelInfo,
	g_Cabinet_modelQuadPolys,
	nullptr)

	sprite->AddSurface(14, 15, 16, 17, eSurfacePickupAngle_270);
	sprite->AddSurface(18, 19, 20, 21, eSurfacePickupAngle_270);
	sprite->AddSurface(6, 7, 8, 9, eSurfacePickupAngle_270);
	sprite->AddSurface(30, 31, 32, 33, eSurfacePickupAngle_270);
	sprite->AddSurface(34, 35, 36, 37, eSurfacePickupAngle_270);
	sprite->AddSurface(38, 39, 40, 41, eSurfacePickupAngle_270);
	sprite->AddSurface(42, 43, 44, 45, eSurfacePickupAngle_270);

END_SPRITECREATE()
START_SPRITECREATE(SideTable,
	g_SideTable_modelFrame,
	g_SideTable_modelInfo,
	g_SideTable_modelQuadPolys,
	nullptr)

	sprite->AddSurface(33, 36, 38, 34, eSurfacePickupAngle_90);

END_SPRITECREATE()
START_SPRITECREATE(WorkTable,
	g_WorkTable_modelFrame,
	g_WorkTable_modelInfo,
	g_WorkTable_modelQuadPolys,
	nullptr)

	sprite->AddSurface(33, 36, 38, 34, eSurfacePickupAngle_90);

END_SPRITECREATE()
START_SPRITECREATE(Bar,
	g_Bar_modelFrame,
	g_Bar_modelInfo,
	g_Bar_modelQuadPolys,
	nullptr)

	sprite->AddSurface(4, 10, 9, 6, eSurfacePickupAngle_270);
	sprite->AddSurface(2, 4, 10, 11, eSurfacePickupAngle_270);
	sprite->AddSurface(1, 2, 11, 12, eSurfacePickupAngle_0);

END_SPRITECREATE()
START_SPRITECREATE(LuxTable,
	g_SideTable_modelFrame,
	g_SideTable_modelInfo,
	g_SideTable_modelQuadPolys,
	nullptr)

	sprite->AddSurface(33, 36, 38, 34, eSurfacePickupAngle_180);

END_SPRITECREATE()

static Sprite* CreatePanel(ESpriteID sprid, const char* sprname, bool drawable, EObjectType objtype,
	float x, float y, float rotx, float roty, float rotz,
	bool b1, bool norotation,
	EColor color1,
	ETextureFilmstrip texturefilmstrip,
	EColor color2,
	XYZPointf pos,
	bool collide, bool b3)
{
	static int s_panelID = 54;
	if (g_numPanels >= MAX_PANELS)
	{
		ShouldntBeHere("need to allocate more memory for panels");
		return nullptr;
	}

	bool unknownbool001 = false;

	if (sprid == eSpriteID_NOTASPRITE)
	{
		sprid = (ESpriteID)++s_panelID;
		unknownbool001 = x < 20;
	}

	XYZPointf mdlpoints[5];
	mdlpoints[0] = XYZPointf(0, 0, 0);
	mdlpoints[1] = XYZPointf(-x / 2, -y / 2, 0);
	mdlpoints[2] = XYZPointf(mdlpoints[1].m_x, y / 2, 0);
	mdlpoints[3] = XYZPointf(x / 2, mdlpoints[2].m_y, 0);
	mdlpoints[4] = XYZPointf(mdlpoints[3].m_x, mdlpoints[1].m_y, 0);

	if (!norotation)
		RotateArrayOfPoints(mdlpoints, 5, XYZPointf(rotx, roty, rotz), XYZPointf(0, 0, 0));


	panelmdlframes_t* panelmdlframe = &g_panelModelFrames[g_numPanels];
	panelmdlinfos_t* panelmdlinfos = &g_panelModelInfo[g_numPanels];
	panelmdlquadpolys_t* panelmdlquadpoly = &g_panelModelQuadPolys[g_numPanels];

	for (int i = 0; i < 5; ++i)
		panelmdlframe->frames[i] = mdlpoints[i];
	panelmdlframe->frames[5] = XYZPointf(DATA_NULLVAL, DATA_NULLVAL, DATA_NULLVAL);

	if (texturefilmstrip == eTextureFilmstrip_NOTATEXTURE)
	{
		animengine_assert(color1 <= eColor_greyshade_45);
	}
	else if (color1 < eColor_black)
	{
		animengine_assert(texturefilmstrip <= eTextureFilmstrip_TipJar);
		color1 = color1 == eColor_unknown ? eColor_white : eColor_black;
	}
	for (int i = 0; i < 5; ++i)
	{
		panelmdlinfos->infos[i].unknown001 = 1.0;
		panelmdlinfos->infos[i].color = color1;
		panelmdlinfos->infos[i].unknown003 = -1.0;
		panelmdlinfos->infos[i].unknown004 = -1.0;
		panelmdlinfos->infos[i].unknown005 = -1.0;
		panelmdlinfos->infos[i].unknown006 = 1.0;
		panelmdlinfos->infos[i].unknown007 = 1.0;
	}
	panelmdlinfos->infos[5].unknown001 = DATA_NULLVAL;
	panelmdlinfos->infos[5].color = DATA_NULLVAL;
	panelmdlinfos->infos[5].unknown003 = DATA_NULLVAL;
	panelmdlinfos->infos[5].unknown004 = DATA_NULLVAL;
	panelmdlinfos->infos[5].unknown005 = DATA_NULLVAL;
	panelmdlinfos->infos[5].unknown006 = DATA_NULLVAL;
	panelmdlinfos->infos[5].unknown007 = DATA_NULLVAL;

	panelmdlquadpoly->polys[0].mdlinfoindex[0] = 4;
	panelmdlquadpoly->polys[0].mdlinfoindex[1] = 1;
	panelmdlquadpoly->polys[0].mdlinfoindex[2] = 2;
	panelmdlquadpoly->polys[0].mdlinfoindex[3] = 3;
	panelmdlquadpoly->polys[0].drawtype = eQuadPolyDraw_quad;
	panelmdlquadpoly->polys[0].facecolor = color1;
	panelmdlquadpoly->polys[0].linecolor1 = color2;
	panelmdlquadpoly->polys[0].linecolor2 = color2;
	panelmdlquadpoly->polys[0].linecolor3 = color2;
	panelmdlquadpoly->polys[0].linecolor4 = color2;
	panelmdlquadpoly->polys[0].linewidth1 = 12;
	panelmdlquadpoly->polys[0].linewidth2 = 12;
	panelmdlquadpoly->polys[0].linewidth3 = 12;
	panelmdlquadpoly->polys[0].linewidth4 = 12;
	panelmdlquadpoly->polys[0].m_eFilmstrip = texturefilmstrip;

	panelmdlquadpoly->polys[1].mdlinfoindex[0] = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].mdlinfoindex[1] = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].mdlinfoindex[2] = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].mdlinfoindex[3] = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].drawtype = (EQuadPolyDraw)DATA_NULLVAL;
	panelmdlquadpoly->polys[1].facecolor = (EColor)DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linecolor1 = (EColor)DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linecolor2 = (EColor)DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linecolor3 = (EColor)DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linecolor4 = (EColor)DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linewidth1 = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linewidth2 = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linewidth3 = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].linewidth4 = DATA_NULLVAL;
	panelmdlquadpoly->polys[1].m_eFilmstrip = (ETextureFilmstrip)DATA_NULLVAL;

	Sprite* spr;
	switch (objtype)
	{
	case eObjectType_Door:				spr = new DoorSprite();				break;
	case eObjectType_Drink:				spr = new DrinkSprite();			break;
	case eObjectType_EightballMessage:	spr = new EightballMessageSprite();	break;
	case eObjectType_Phone:				spr = new PhoneSprite();			break;
	case eObjectType_AnsweringMachine:	spr = new AnsweringMachineSprite();	break;
	default:							spr = new Sprite();					break;
	}

	spr->m_eObjectType = objtype;
	spr->m_modelframe = panelmdlframe->frames;
	spr->m_modelinfo = panelmdlinfos->infos;
	spr->m_modelquadpoly = panelmdlquadpoly->polys;
	spr->a1_unknown00008 = true;
	spr->a1_unknown00010 = b1 && !norotation;
	spr->m_bSpriteFollowsCamera = norotation;
	spr->m_iPanelIndex = g_numPanels;
	spr->a1_unknown00010c = unknownbool001;

	spr->InitSprite(sprid, sprname, true, drawable);
	spr->SetVolume(XYZPointf(1, 1, 1));
	spr->SetPosition(pos);
	if (collide)
		spr->SetCollidableType(eCollidableType_Panel);
	if (b3)
		spr->a1_unknown00011a = true;

	_set_g_numPanels(g_numPanels + 1);

	return spr;
}

static int GetFullDrinkLevel(bool isplayer)
{
	if (isplayer)
		return 8;
	return rand2(-2, 2) + 10;
}
static void CreateRoom()
{
	Sprite* spr;
	CreateCouchSprite(eSpriteID_couch, "couch");
	CreateCabinetSprite(eSpriteID_cabinet, "cabinet");
	CreateSideTableSprite(eSpriteID_sideTable, "side table");
	CreateWorkTableSprite(eSpriteID_workTable, "work table");
	CreateBarSprite(eSpriteID_bar, "bar");
	CreateLuxTableSprite(eSpriteID_luxTable, "side table 2");

	CreatePanel(eSpriteID_plant,	"plant",											true,	eObjectType_Misc,		120,	120,	0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Plant,			eColor_none,	XYZPointf(160, 60, 65),						false,	false);
	spr = CreatePanel(eSpriteID_ashtray,	"ashtray",										false,	eObjectType_Misc,		10,		5,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Ashtray,			eColor_none,	XYZPointf(165, 60, 10),						false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 5, 10));
	spr = CreatePanel(eSpriteID_phone,			"phone",								true,	eObjectType_Phone,		16,		16,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Phone,			eColor_none,	XYZPointf(145, 60, -60),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 5, 10));
	spr = CreatePanel(eSpriteID_answeringMachine,	"answering machine",					true,	eObjectType_AnsweringMachine,	16,	16,	0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_AnsweringMachine,	eColor_none,	XYZPointf(170, 60, -70),					false,	false);
		spr->SetClickable(true);
		spr->SetVolume(XYZPointf(10, 5, 10));
	spr = CreatePanel(eSpriteID_FIRSTDRINKITEM,	"red wine bottle",						true,	eObjectType_Misc,		25,		25,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_RedWineBottle,	eColor_none,	XYZPointf(-185, 80, -108),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 25, 10));
	spr = CreatePanel(eSpriteID_whitewinebottle,	"white wine bottle",					true,	eObjectType_Misc,		25,		25,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_WhiteWineBottle,	eColor_none,	XYZPointf(-193, 80, -112),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 25, 10));
	spr = CreatePanel(eSpriteID_martiniDisplay1,	"martini glass 1",						true,	eObjectType_Misc,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniEmpty,		eColor_none,	XYZPointf(-184, 41, -274),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = 0;
	spr = CreatePanel(eSpriteID_martiniDisplay2,	"martini glass 2",						true,	eObjectType_Misc,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniEmpty,		eColor_none,	XYZPointf(-183, 41, -286),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = 0;
	spr = CreatePanel(eSpriteID_martiniDisplay3,	"martini glass 3",						true,	eObjectType_Misc,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniEmpty,		eColor_none,	XYZPointf(-183.5, 41, -297),				false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = 0;
	spr = CreatePanel(eSpriteID_martiniDisplay4,	"martini glass 4",						true,	eObjectType_Misc,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniEmpty,		eColor_none,	XYZPointf(-183, 41, -308),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = 0;
	spr = CreatePanel(eSpriteID_bar_redwinebottle,	"bar_redwinebottle",				false,	eObjectType_Misc,		25,		25,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_RedWineBottle,	eColor_none,	XYZPointf(-100, -100, -100),				false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 25, 10));
	spr = CreatePanel(eSpriteID_bar_whitewinebottle,	"bar_whitewinebottle",				false,	eObjectType_Misc,		25,		25,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_WhiteWineBottle,	eColor_none,	XYZPointf(-100, -100, -100),				false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 25, 10));
	spr = CreatePanel(eSpriteID_playerDrink,		"player's drink",						false,	eObjectType_Drink,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniCosmo,		eColor_none,	XYZPointf(-170, 73, -110),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = GetFullDrinkLevel(true);
	spr = CreatePanel(eSpriteID_tripDrink,		"Trip's drink",							false,	eObjectType_Drink,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniCosmo,		eColor_none,	XYZPointf(-100, -100, -100),				false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = GetFullDrinkLevel(false);
	spr = CreatePanel(eSpriteID_graceDrink,		"Grace's drink",						false,	eObjectType_Drink,		12,		12,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_MartiniClassic,	eColor_none,	XYZPointf(-100, -100, -100),				false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(12, 12, 12));
		spr->a1_unknown00007 = 2;
		spr->c1_unknown00001 = GetFullDrinkLevel(false);
	spr = CreatePanel(eSpriteID_bigAbstractPainting,	"big abstract painting",			true,	eObjectType_Misc,		52,		70,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_Painting,			eColor_black,	XYZPointf(190, 100, -380),					false,	false);
		spr->SetVolume(XYZPointf(50, 50, 70));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_littleTreePainting,	"little painting",					true,	eObjectType_Misc,		20,		32,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_LittlePainting,	eColor_black,	XYZPointf(190, 118, -335),					false,	false);
		spr->SetVolume(XYZPointf(20, 20, 30));
		spr->SetClickable(true);
	CreatePanel(eSpriteID_maoPainting,			"maoPainting",							false,	eObjectType_Misc,		40,		50,		0,		270,	0,		true,	false,	eColor_white,			eTextureFilmstrip_LittlePainting,	eColor_black,	XYZPointf(215, 100, 400),					false,	false);
	CreatePanel(eSpriteID_rug,					"rug",									true,	eObjectType_Misc,		50,		100,	90,		0,		0,		false,	false,	eColor_white,			eTextureFilmstrip_Rug,	eColor_black,	XYZPointf(85, 3, -350),									false,	false);
	spr = CreatePanel(eSpriteID_weddingPicture,	"wedding photo",						true,	eObjectType_Misc,		60,		70,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WeddingPicture,	eColor_black,	XYZPointf(190, 90, -145),					false,	false);
		spr->SetVolume(XYZPointf(60, 60, 90));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_italyPicture,		"italy picture",						true,	eObjectType_Misc,		32,		32,		0,		270,	0,		true,	false,	eColor_white,			eTextureFilmstrip_Tuscany_Romance,	eColor_none,	XYZPointf(-199, 90, -60),					false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_gownsketch1,		"wedding gown sketch",					true,	eObjectType_Misc,		32,		32,		0,		90,		0,		true,	false,	eColor_unknown,			eTextureFilmstrip_GownSketch1,		eColor_none,	XYZPointf(190, 90, 0),						false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_gownsketch2,		"wedding gown post its",				true,	eObjectType_Misc,		36,		36,		0,		90,		0,		true,	false,	eColor_unknown,			eTextureFilmstrip_PostIts,			eColor_none,	XYZPointf(190, 50, -28),					false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_gownsketch3,		"wedding gown sketch 3",				false,	eObjectType_Misc,		25,		25,		90,		0,		0,		true,	false,	eColor_unknown,			eTextureFilmstrip_GownSketch1,		eColor_none,	XYZPointf(160, 58, -60),					false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_gownsketch4,		"wedding gown sketch 4",				true,	eObjectType_Misc,		30,		30,		0,		90,		0,		true,	false,	eColor_unknown,			eTextureFilmstrip_GownSketch4,		eColor_none,	XYZPointf(190, 83, -35),					false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_tuxedosketch1,	"tuxedosketch1",						false,	eObjectType_Misc,		32,		32,		0,		90,		0,		true,	false,	eColor_unknown,			eTextureFilmstrip_GownSketch1,		eColor_none,	XYZPointf(190, 95, 25),						false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_tuxedosketch2,	"tuxedosketch2",						false,	eObjectType_Misc,		25,		25,		90,		0,		0,		true,	false,	eColor_unknown,			eTextureFilmstrip_GownSketch1,		eColor_none,	XYZPointf(160, 58, -15),					false,	false);
		spr->SetVolume(XYZPointf(30, 30, 30));
		spr->SetClickable(true);
	spr = CreatePanel(eSpriteID_eightball,		"advice ball",							true,	eObjectType_Misc,		8,		8,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_EightBall,		eColor_none,	XYZPointf(-149, 71, -170),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(8, 8, 8));
		spr->a1_unknown00006 = 1;
		spr->a1_unknown00007 = 6;
	spr = CreatePanel(	(ESpriteID)((int)eSpriteID_LAST_OBJECT|(int)eSpriteID_cabinet),	"advice ball message",	false,	eObjectType_EightballMessage,	5,	5,	0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_EightBallAnims,	eColor_none,	XYZPointf(0, 0, 0),							false,	false);
		spr->SetVolume(XYZPointf(4, 4, 1));
	spr = CreatePanel(eSpriteID_brassbull,		"brass bull",							true,	eObjectType_Misc,		16,		16,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Brassbull,		eColor_none,	XYZPointf(-185, 75, -300),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(16, 16, 10));
	spr = CreatePanel(eSpriteID_trinket1,			"trinket 1",							true,	eObjectType_Misc,		28,		28,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket1,			eColor_none,	XYZPointf(-185, 48, -450),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 28, 10));
	spr = CreatePanel(eSpriteID_trinket2,			"trinket 2",							true,	eObjectType_Misc,		28,		28,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket2,			eColor_none,	XYZPointf(-186, 48, -425),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 28, 10));
	spr = CreatePanel(eSpriteID_trinket3,			"trinket 3",							true,	eObjectType_Misc,		20,		20,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket3,			eColor_none,	XYZPointf(-184.5, 46, -400),				false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 20, 10));
	spr = CreatePanel(eSpriteID_trinket4,			"trinket 4",							true,	eObjectType_Misc,		16,		16,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket4,			eColor_none,	XYZPointf(-185, 75, -443),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 16, 10));
	spr = CreatePanel(eSpriteID_trinket5,			"trinket 5",							true,	eObjectType_Misc,		16,		16,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket5,			eColor_none,	XYZPointf(-186, 75, -405),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 16, 10));
	spr = CreatePanel(eSpriteID_trinket6,			"trinket 6",							true,	eObjectType_Misc,		20,		20,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket6,			eColor_none,	XYZPointf(-186, 98, -399),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 20, 10));
	spr = CreatePanel(eSpriteID_trinket7,			"trinket 7",							true,	eObjectType_Misc,		16,		16,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket7,			eColor_none,	XYZPointf(-186, 96, -424),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 16, 10));
	spr = CreatePanel(eSpriteID_trinket8,			"trinket 8",							true,	eObjectType_Misc,		16,		16,		0,		0,		0,		true,	true,	eColor_unknown,			eTextureFilmstrip_Trinket8,			eColor_none,	XYZPointf(-186, 96, -450),					false,	false);
		spr->SetGrabable(true);
		spr->SetVolume(XYZPointf(10, 16, 10));
		
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwall1",							true,	eObjectType_Misc,		70,		160,	0,		0,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-165, 80, -500),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwallsliver1",					true,	eObjectType_Misc,		6.5,	160,	0,		0,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-66, 80, -500),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwallsliver2",					true,	eObjectType_Misc,		6.5,	160,	0,		0,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(6, 80, -500),					true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwall3",							true,	eObjectType_Misc,		30,		160,	0,		0,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(85, 80, -500),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwall4",							true,	eObjectType_Misc,		100,	160,	0,		0,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(150, 80, -500),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwalltoppanel",					true,	eObjectType_Misc,		200,	10,		0,		0,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-30, 155, -500),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_leftwall0",							true,	eObjectType_Misc,		70,		160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-200, 80, -5),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_leftwall1",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-200, 80, -90),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_leftwall2",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-200, 80, -190),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_leftwall3",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-200, 80, -290),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_leftwall4",							true,	eObjectType_Misc,		60,		160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-200, 80, -370),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_leftwall5",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-200, 80, -450),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_rightwall0",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(200, 80, 50),					true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_rightwall1",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(200, 80, -50),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_rightwall2",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(200, 80, -150),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_rightwall3",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(200, 80, -250),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_rightwall4",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(200, 80, -350),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_rightwall5",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_black,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(200, 80, -450),				true,	false);
	
	spr = CreatePanel(eSpriteID_NOTASPRITE,		"LR_backwindow1",						true,	eObjectType_Misc,		66,		150,	0,		0,		0,		true,	false,	eColor_R2_G2_B4_A2,		eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-101, 77, -498),				true,	false);
		spr->m_bTransparent = true;
	spr = CreatePanel(	eSpriteID_viewWindow,			"LR_backwindow2",						true,	eObjectType_Misc,		66,		150,	0,		0,		0,		true,	false,	eColor_R2_G2_B4_A2,		eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-30, 77, -498),				true,	false);
		spr->m_bTransparent = true;
	spr = CreatePanel(eSpriteID_NOTASPRITE,			"LR_backwindow3",						true,	eObjectType_Misc,		66,		150,	0,		0,		0,		true,	false,	eColor_R2_G2_B4_A2,		eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(41, 77, -498),				true,	false);
		spr->m_bTransparent = true;
		
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_frontwallL",						true,	eObjectType_Misc,		60,		160,	0,		0,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(170, 80, 98),					true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_frontwallR1",						true,	eObjectType_Misc,		66,		160,	0,		0,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(17, 80, 98),					false,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_frontwallR1_real",					true,	eObjectType_Misc,		50,		160,	0,		0,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(2, 80, 108),					true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"LR_frontwallR2",						true,	eObjectType_Misc,		80,		160,	0,		0,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,	XYZPointf(-46, 80, 98),					true,	false);
	
	int zoneid = CreateNewZone(nullptr, false);
	XZRectf rect;
	rect.m_xmin = -400;
	rect.m_zmin = 0;

	rect.m_xmax = 195;
	rect.m_zmax = 0;

	AddRectToZone(zoneid, rect);
	g_roomZoneIDs[0] = zoneid;
	
	CreatePanel(eSpriteID_NOTASPRITE,			"FY_leftwall1",							true,	eObjectType_Misc,		100,	160,	0,		60,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-225, 80, 72),			true,	false);
	
	CreatePanel(eSpriteID_NOTASPRITE,			"FY_rightwall1",						true,	eObjectType_Misc,		100,	160,	0,		60,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-110, 80, 140),			true,	false);
	spr = CreatePanel(	eSpriteID_frontDoor,		"front door",							true,	eObjectType_Door,		60,		130,	0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_FrontDoor,			eColor_black,		XYZPointf(-190, 65, 130),			true,	false);
		spr->a1_unknown00002 = 1;
		spr->SetClickable(true);
		spr->SetVolume(XYZPointf(60, 130, 5));
		spr->SetRotation(XYZPointf(0, 30, 0));
		
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_abovefrontdoor",					true,	eObjectType_Misc,		60,		30,		0,		-30,	0,		true,	false,	eColor_greyshade_20,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-195, 145, 145),			false,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_leftpanel",							true,	eObjectType_Misc,		40,		160,	0,		-30,	0,		true,	false,	eColor_greyshade_20,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-235, 80, 122),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_rightpanel",						true,	eObjectType_Misc,		44,		160,	0,		-30,	0,		true,	false,	eColor_greyshade_20,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-150, 80, 171),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_leftwall1",							true,	eObjectType_Misc,		124,	160,	0,		0,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-311, 80, 115),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_leftwall2",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-370, 80, 150),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_leftwall3",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-370, 80, 250),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_leftwall4",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-370, 80, 350),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_leftwall5",							true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-370, 80, 450),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_rightwall1",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-135, 80, 232),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_rightwall2",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-135, 80, 332),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_rightwall3",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-135, 80, 432),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_rightwall4",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-135, 80, 532),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_backwall1",							true,	eObjectType_Misc,		80,		160,	0,		0,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-340, 80, 480),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_backwall2",							true,	eObjectType_Misc,		60,		160,	0,		0,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-165, 80, 480),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_backwall_top",						true,	eObjectType_Misc,		110,	 30,	0,		0,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-245, 145, 480),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_elevator1",							true,	eObjectType_Misc,		55,		130,	0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_Elevator,				eColor_black,		XYZPointf(-221, 65, 480.5),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"FD_elevator2",							true,	eObjectType_Misc,		55,		130,	0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_Elevator,				eColor_black,		XYZPointf(-274, 65, 480.5),			true,	false);
	
	spr = CreatePanel(	eSpriteID_elevatorButton,	"elevator button",						true,	eObjectType_Misc,		7,		15,		0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_ElevatorButton,		eColor_black,		XYZPointf(-315, 65, 479.5),			true,	false);
		spr->SetVolume(XYZPointf(10, 10, 20));
		spr->SetClickable(true);
		
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwall1",						true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(50, 80, 450),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwall2",						true,	eObjectType_Misc,		80,		160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(50, 80, 360),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwall2_sliver",				true,	eObjectType_Misc,		20,		160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(50, 80, 310),				false,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwallabovekitchendoor",		true,	eObjectType_Misc,		60,		40,		0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(50, 140, 270),			false,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwall3",						true,	eObjectType_Misc,		40,		160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(50, 80, 220),				false,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwall4",						true,	eObjectType_Misc,		102,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(50, 80, 149),				false,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_lefthallwall_3_4_real",				true,	eObjectType_Misc,		105,	160,	0,		90,		0,		true,	false,	eColor_greyshade_10,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(40, 80, 175),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_righthallwall1",					true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(140, 80, 450),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_righthallwall2",					true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(140, 80, 350),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_righthallwall3",					true,	eObjectType_Misc,		100,	160,	0,		90,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(140, 80, 250),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_righthallwall4",					true,	eObjectType_Misc,		102,	160,	0,		90,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(140, 80, 149),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"bedroom door",							true,	eObjectType_Misc,		60,		130,	0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_BedroomDoor,			eColor_black,		XYZPointf(95, 65, 450),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_backwall1",							true,	eObjectType_Misc,		20,		160,	0,		0,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(135, 80, 450),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_backwall2",							true,	eObjectType_Misc,		20,		160,	0,		0,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(55, 80, 450),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"HW_aboveBedroomDoor",					true,	eObjectType_Misc,		60,		30,		0,		0,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(95, 145, 450),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_rightwall1",						true,	eObjectType_Misc,		86,		160,	0,		0,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(7, 80, 200),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_rightwall2",						true,	eObjectType_Misc,		100,	160,	0,		0,		0,		true,	false,	eColor_greyshade_15,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-85, 80, 200),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_backwall1",							true,	eObjectType_Misc,		100,	160,	0,		0,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(0, 80, 450),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_backwall2",							true,	eObjectType_Misc,		90,		160,	0,		0,		0,		true,	false,	eColor_greyshade_05,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-85, 80, 450),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabfront1",					true,	eObjectType_Misc,		60,		50,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WhiteCabinetDoor1,	eColor_black,		XYZPointf(-100, 25, 230),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabfront2",					true,	eObjectType_Misc,		60,		50,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WhiteCabinetDoor1,	eColor_black,		XYZPointf(-100, 25, 290),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabfront3",					true,	eObjectType_Misc,		60,		50,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WhiteCabinetDoor1,	eColor_black,		XYZPointf(-100, 25, 350),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabfront4",					true,	eObjectType_Misc,		20,		50,		0,		90,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WhiteCabinetDoor1,	eColor_black,		XYZPointf(-100, 25, 390),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabtop1",					true,	eObjectType_Misc,		20,		60,		90,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-110, 50, 230),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabtop2",					true,	eObjectType_Misc,		20,		60,		90,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-110, 50, 290),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabtop3",					true,	eObjectType_Misc,		20,		60,		90,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-110, 50, 350),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_longwallcabtop4",					true,	eObjectType_Misc,		20,		60,		90,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-110, 50, 410),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_backwallcabfront1",					true,	eObjectType_Misc,		20,		50,		0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WhiteCabinetDoor1,	eColor_black,		XYZPointf(-90, 25, 400),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_backwallcabfront2",					true,	eObjectType_Misc,		60,		50,		0,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_WhiteCabinetDoor1,	eColor_black,		XYZPointf(-50, 25, 400),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_backwallcabtop2",					true,	eObjectType_Misc,		80,		40,		90,		0,		0,		true,	false,	eColor_white,			eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-60, 50, 420),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_fridgeside1",						true,	eObjectType_Misc,		60,		120,	0,		90,		0,		true,	false,	eColor_R9_G9_B95_A3,		eTextureFilmstrip_NOTATEXTURE,			eColor_black,		XYZPointf(-20, 60, 430),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_fridgeside2",						true,	eObjectType_Misc,		60,		120,	0,		90,		0,		true,	false,	eColor_R9_G9_B95_A3,		eTextureFilmstrip_NOTATEXTURE,			eColor_black,		XYZPointf(40, 60, 430),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_fridgefront1",						true,	eObjectType_Misc,		60,		40,		0,		0,		0,		true,	false,	eColor_R9_G9_B95_A3,		eTextureFilmstrip_NOTATEXTURE,			eColor_black,		XYZPointf(10, 100, 400),			true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_fridgefront2",						true,	eObjectType_Misc,		60,		72,		0,		0,		0,		true,	false,	eColor_R9_G9_B95_A3,		eTextureFilmstrip_NOTATEXTURE,			eColor_black,		XYZPointf(10, 44, 400),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"KI_fridgefrontbottom",					true,	eObjectType_Misc,		60,		8,		0,		0,		0,		true,	false,	eColor_greyshade_09,		eTextureFilmstrip_NOTATEXTURE,			eColor_black,		XYZPointf(10, 4, 401),				true,	false);
	CreatePanel(eSpriteID_NOTASPRITE,			"ceiling1",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_30,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-250, 160, 300),			false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"ceiling2",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_30,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(250, 160, 300),			false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"ceiling3",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_30,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-250, 160, -300),			false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"ceiling4",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_30,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(250, 160, -300),			false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"floor1",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_45,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-250, 0, 300),			false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"floor2",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_45,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(250, 0, 300),				false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"floor3",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_45,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(-250, 0, -300),			false,	true);
	CreatePanel(eSpriteID_NOTASPRITE,			"floor4",								true,	eObjectType_Misc,		500,	600,	90,		0,		0,		false,	false,	eColor_greyshade_45,	eTextureFilmstrip_NOTATEXTURE,			eColor_none,		XYZPointf(250, 0, -300),			false,	true);
}
static CharacterSprite* CreateGraceSprite()
{
	CharacterSprite* character = new CharacterSprite();
	//character->m_modelinfo = &g_Grace_modelInfo;
	//character->m_modelquadlines = &g_Grace_modelQuadLines;
	//character->m_modelquadpoly = &g_Grace_modelQuadPolys;
	//character->m_characterarms = &g_Grace_modelArms;
	//character->m_outlineconnectors = &g_Grace_modelOutlineConnectors;
	character->a1_unknown00010d = true;
	character->a1_unknown00001 = true;
	character->InitAnimatingSprite(eSpriteID_grace, "grace", 76, 4533, -5, 5, 35);
	character->SetVolume(XYZPointf(30, 100, 50));
	character->SetCollidableType(eCollidableType_SimpleBox);
	return character;
}
static CharacterSprite* CreateTripSprite()
{
	CharacterSprite* character = new CharacterSprite();
	//character->m_modelinfo = &g_Trip_modelInfo;
	//character->m_modelquadlines = &g_Trip_modelQuadLines;
	//character->m_modelquadpoly = &g_Trip_modelQuadPolys;
	//character->m_characterarms = &g_Trip_modelArms;
	//character->m_outlineconnectors = &g_Trip_modelOutlineConnectors;
	character->a1_unknown00010d = true;
	character->a1_unknown00001 = true;
	character->InitAnimatingSprite(eSpriteID_trip, "trip", 73, 7616, -5, 5, 64);
	character->SetVolume(XYZPointf(30, 100, 50));
	character->SetCollidableType(eCollidableType_SimpleBox);
	return character;
}
static HeadSprite* CreateGraceHeadSprite(CharacterSprite* grace)
{
	HeadSprite* head = new HeadSprite();
	head->m_modelframe = g_GraceHeadModel_modelFrame;
	head->m_modelinfo = g_GraceHeadModel_modelInfo;
	//head->m_modelquadlines = &g_GraceHeadModel_modelQuadLines;
	//head->m_characterarms = &g_GraceHeadModel_modelArms;
	head->m_modelquadpoly = g_GraceHeadModel_modelQuadPolys;
	head->a1_unknown00010d = true;
	head->a1_unknown00001 = false;
	head->InitSprite((ESpriteID)50, "GraceHead", true, true);
	head->SetVolume(XYZPointf(1, 1, 1));

	return head;
}
static HeadSprite* CreateTripHeadSprite(CharacterSprite* trip)
{
	HeadSprite* head = new HeadSprite();
	head->m_modelframe = g_TripHeadModel_modelFrame;
	head->m_modelinfo = g_TripHeadModel_modelInfo;
	//head->m_modelquadlines = &g_TripHeadModel_modelQuadLines;
	//head->m_characterarms = &g_TripHeadModel_modelArms;
	head->m_modelquadpoly = g_TripHeadModel_modelQuadPolys;
	head->a1_unknown00010d = true;
	head->a1_unknown00001 = false;
	head->InitSprite((ESpriteID)52, "TripHead", true, true);
	head->SetVolume(XYZPointf(1, 1, 1));

	return head;
}
static void InitGraceSprite()
{
	_set_g_GraceSprite(CreateGraceSprite());
	g_GraceSprite->m_pHeadSprite = CreateGraceHeadSprite(g_GraceSprite);
	g_GraceSprite->a1_unknown00002 = 75;
	g_GraceSprite->animsprite_unknown001 = 75;
	g_GraceSprite->unknown005 = 30;
	g_GraceSprite->unknown006 = -3;
	g_GraceSprite->unknown007 = 8;
	g_GraceSprite->unknown008 = 9;
	g_GraceSprite->InitCharacterSprite(
		-65, 65,
		-20, 35,
		-30, 30,
		-35, 35,
		0.25, 0.75,
		0.25, 0.75,
		0, 1,
		(ESpriteID)51);
	g_GraceSprite->SetEyelidHeight(0.85, true, true, true);
	//g_GraceSprite->unknown039.InitLayerController(eAnimLayer_breathe, g_GraceSprite);  
	//g_GraceSprite->unknown039.SetLayerAlt(0, "breathe",			-1, -1, 2, -1, -1, -1, true,	1, -1);
	//g_GraceSprite->unknown039.SetLayerAlt(1, "breatheMed",		-1, -1, 3, -1, -1, -1, false,	8,  0);
	//g_GraceSprite->unknown039.SetLayerAlt(2, "breatheMedHi",	-1, -1, 4, -1, -1, -1, false,	8,  1);
	//g_GraceSprite->unknown039.SetLayerAlt(3, "breatheHeavy",	-1, -1, 5, -1, -1, -1, false,	8,  2);

	//undone
}
static void InitTripSprite()
{
	_set_g_TripSprite(CreateTripSprite());
	g_TripSprite->m_pHeadSprite = CreateTripHeadSprite(g_TripSprite);
	g_TripSprite->a1_unknown00002 = 44;
	g_TripSprite->animsprite_unknown001 = 44;
	g_TripSprite->unknown005 = 30;
	g_TripSprite->unknown006 = -3;
	g_TripSprite->unknown007 = 8;
	g_TripSprite->unknown008 = 9;
	g_TripSprite->InitCharacterSprite(
		-65, 65,
		-20, 35,
		-30, 30,
		-35, 35,
		0.25, 0.75,
		0.25, 0.75,
		0, 1,
		(ESpriteID)53);
	g_TripSprite->SetEyelidHeight(0.85, true, true, true);
	//g_TripSprite->unknown039.InitLayerController(eAnimLayer_breathe, g_TripSprite);
	//g_TripSprite->unknown039.SetLayerAlt(0, "breathe",		-1, -1, 1, -1, -1, -1, true,	1, -1);
	//g_TripSprite->unknown039.SetLayerAlt(1, "breatheMed",	-1, -1, 2, -1, -1, -1, false,	8,  0);
	//g_TripSprite->unknown039.SetLayerAlt(2, "breatheMedHi", -1, -1, 3, -1, -1, -1, false,	8,  1);
	//g_TripSprite->unknown039.SetLayerAlt(3, "breatheHeavy", -1, -1, 4, -1, -1, -1, false,	8,  2);

	//undone
}
static void InitCollisionZonesAndPathPlanGrid()
{
	for (int i = 0; i < g_spriteMgr->GetNumSprites(); ++i)
	{
		Sprite* spr = g_spriteMgr->GetSpritePtr(i);
		if (!spr->m_modelframe && !spr->GetIsCharacterSprite()
			|| spr->GetCollidableType() == eCollidableType_NonSolid)
			continue;

		XZRectf rect = ComputeSingleCollisionRectForSprite(spr);

		if (spr->m_eID == eSpriteID_bar)	rect.m_xmax += 15;
		if (spr->m_eID == eSpriteID_workTable)	rect.m_xmax -= 15;

		AddRectToZone(CreateNewZone(spr, true), rect);
		if (!spr->GetIsCharacterSprite()
			&& spr->m_eID == eSpriteID_frontDoor)
		{
			//AddRectToPathPlanGrid(rect);
		}
	}
}
static void SetupSqrtTable() {
	//undone
}
static void SetupDirOffsets() {
	//undone
}
static void SetupHeuristicTable() {
	//undone
}
static void SetupTurnTable() {
	//undone
}
static void InitPathPlanning()
{
	SetupSqrtTable();
	SetupDirOffsets();
	SetupHeuristicTable();
	SetupTurnTable();
}
static void InitCircleDrawing()
{
	int itertri = 0;
	for (float i = 0; i < MATH_PIDOUBLE; i = MATH_PI / 10.f + i)
	{
		animengine_assert(itertri < TRICIRCLE_ITERATIONS);
		if (itertri == 20)	g_triangleFanSineCache[itertri] = sin(0);
		else				g_triangleFanSineCache[itertri] = sin(i);

		if (itertri == 20)	g_triangleFanSineCache[itertri] = cos(0);
		else				g_triangleFanSineCache[itertri] = cos(i);

		itertri++;
	}
}
static void InitQueuedUpRequestedPerformance()
{
	//undone
}


void FinishRoomInit()
{
	Sprite* worktable = g_spriteMgr->GetSpritePtrByName("work table");
	Sprite* ashtray = g_spriteMgr->GetSpritePtrByName("ashtray");

	//called but return value not used ?
	g_spriteMgr->GetSpritePtrByName("phone");
	g_spriteMgr->GetSpritePtrByName("answering machine");

	worktable->AddSpriteToSurface(0, ashtray);

	Sprite* bar = g_spriteMgr->GetSpritePtrByName("bar");
	bar->AddSpriteToSurface(2, g_spriteMgr->GetSpritePtrByName("white wine bottle"));
	bar->AddSpriteToSurface(2, g_spriteMgr->GetSpritePtrByName("red wine bottle"));
	bar->AddSpriteToSurface(0, g_spriteMgr->GetSpritePtrByName("advice ball"));
}

void InitTransRotScale(CharacterSprite* spr, bool b1)
{
	static int g_playerMotionNoise;
	if (!spr)
	{
		g_PlayerSprite->SetPosition(XYZPointf(-32, 97, -111));
		XYZPointf playerrot = g_PlayerSprite->GetRotation();
		playerrot.m_y = 24;
		g_PlayerSprite->SetRotation(playerrot);
	}
	g_playerMotionNoise = 0;
	if (!spr && g_numFramesRun > 1 && !b1)
		return;

	if (!spr)
	{
		g_GraceSprite->SetPosition(XYZPointf(-2, -5, -140));
		XYZPointf gracerot = g_GraceSprite->GetRotation();
		gracerot.m_y = 226;
		g_GraceSprite->SetRotation(gracerot);
	}
	g_GraceSprite->SetTorsoRotation(XYZPointf(0, 0, 0));
	g_GraceSprite->m_pHeadSprite->SetPosition(XYZPointf(0, -5, 160));
	g_GraceSprite->SetHeadRotation(XYZPointf(0, 50, 0));

	//g_GraceSprite->m_GestureFxnMgr1.StartGestureFxn(0, 1, 0);
	//g_GraceSprite->m_GestureFxnMgr6.StartGestureFxn(0, 43, 0);
	//g_GraceSprite->m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
	//g_GraceSprite->m_GestureFxnMgr8.StartGestureFxn(0, 1, 0);
	//g_GraceSprite->m_GestureFxnMgr9.StartGestureFxn(0, 1, 0);
	//g_GraceSprite->m_GestureFxnMgr10.StartGestureFxn(0, 1, 0);

	if (spr != g_GraceSprite)
		g_GraceSprite->SetGazeTrackingSprite(g_PlayerSprite, eGazeSpeed_med, eGazeType_normal, eAlterHeadCock_subtle);



	if (!spr)
	{
		g_TripSprite->SetPosition(XYZPointf(-30, -5, -155));
		XYZPointf triprot = g_PlayerSprite->GetRotation();
		triprot.m_y = 178;
		g_TripSprite->SetRotation(triprot);
	}

	g_TripSprite->SetTorsoRotation(XYZPointf(0, 0, 0));
	g_TripSprite->m_pHeadSprite->SetPosition(XYZPointf(0, -5, 170));
	g_TripSprite->SetHeadRotation(XYZPointf(0, 0, 0));

	//g_TripSprite->m_GestureFxnMgr1.StartGestureFxn(0, 1, 0);
	//g_TripSprite->m_GestureFxnMgr6.StartGestureFxn(0, 44, 0);
	//g_TripSprite->m_GestureFxnMgr7.StartGestureFxn(0, 1, 0);
	//g_TripSprite->m_GestureFxnMgr8.StartGestureFxn(0, 1, 0);
	//g_TripSprite->m_GestureFxnMgr9.StartGestureFxn(0, 1, 0);
	//g_TripSprite->m_GestureFxnMgr10.StartGestureFxn(0, 1, 0);

	if (spr != g_TripSprite)
		g_TripSprite->SetGazeTrackingSprite(g_PlayerSprite, eGazeSpeed_med, eGazeType_normal, eAlterHeadCock_subtle);



	Sprite* roomspr = g_spriteMgr->GetSpritePtrByName("couch");
		roomspr->SetPosition(XYZPointf(150, 0, -350));
		roomspr->SetRotation(XYZPointf(0, 0, 0));
	roomspr = g_spriteMgr->GetSpritePtrByName("cabinet");
		roomspr->SetPosition(XYZPointf(-177, 0, -350));
		roomspr->SetRotation(XYZPointf(0, 0, 0));
	roomspr = g_spriteMgr->GetSpritePtrByName("side table");
		roomspr->SetPosition(XYZPointf(170, 0, -210));
		roomspr->SetRotation(XYZPointf(0, 0, 0));
	roomspr = g_spriteMgr->GetSpritePtrByName("work table");
		roomspr->SetPosition(XYZPointf(160, 0, -30));
		roomspr->SetRotation(XYZPointf(0, 0, 0));
	roomspr = g_spriteMgr->GetSpritePtrByName("bar");
		roomspr->SetPosition(XYZPointf(-160, 0, -120));
		roomspr->SetRotation(XYZPointf(0, 0, 0));
	roomspr = g_spriteMgr->GetSpritePtrByName("side table 2");
		roomspr->SetPosition(XYZPointf(-180.0, 0.0, -60.0));
		roomspr->SetRotation(XYZPointf(0, 0, 0));

}

void InitGlobals()
{
	dprintf("\n---------------------------\nInitting globals");
	srand(time(0));

	static int g_startTime = GetTickCount(); //starttime is unused
	_set_g_numFramesRun(0);
	_set_g_bShouldRenderThisFrame(true);
	_set_g_SkipFramesCounter(0);
	_set_g_bAssertionThrown(false);
	g_launchErrorString[0] = '\0';
	_set_g_bAllQuadLineOutline(false);
	_set_g_bAllCircleOutline(false);

	for (int i = eTextureFilmstrip_YellowFont1; i <= eTextureFilmstrip_TipJar; ++i)
		LoadTextureFilmstrip((ETextureFilmstrip)i);

	InitEnvironment();
	InitCinematography();
	InitPlayerNavigation();

	_set_g_PlayerSprite(new Sprite());
	g_PlayerSprite->InitSprite(eSpriteID_player, "player", false, false, -1);
	g_PlayerSprite->SetUpdateMe(false);

	_set_g_CursorSprite(CreateCursorSprite());
	g_CursorSprite->SetUpdateMe(false);

	CreateRoom();

	InitGraceSprite();
	InitTripSprite();
	InitTransRotScale(nullptr, false);
	InitCollisionZonesAndPathPlanGrid();
	InitPathPlanning();
	InitCircleDrawing();
	InitQueuedUpRequestedPerformance();

	g_soundSource->InitSoundSource("sounds\\global\\globalsnd.txt");
	_set_g_soundSourceID(g_soundMgr->AddSoundSource(g_soundSource));

	BuildBitmapFont();

	XCursor::_set_theirCursor(new XCursor());
	XCursor::theirCursor->InitCursor();
	XCursor::theirCursor->JumpCutToCursor(eCursorType_Idle, true);

	LoadIcon(g_hAppInstance, (LPCSTR)IDI_FACADEWINDOWICON);

	for (int i = 0; i < 32; i++)
		g_curTextInput[i] = 0;
	_set_g_curTextCharInput(0);
}
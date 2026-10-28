#ifndef SPRITE_H
#define SPRITE_H

#include "classes/Controller.h"

class Sprite;

#define MDL_MAXPOINTS 235

struct spritesurface_t
{
	int unknown001[4];
	XYZPointf points1[4];
	XYZPointf points2[4];
	int numsprites;
	Sprite* sprites[5];
	ESurfacePickupAngle pickupangle;
};

struct spr_unknownstruct1_t
{
	float unknown001;
	XYZPointf unknown002;
	XYZPointf unknown003;
	float unknown004[3];
};
struct spr_unknownstruct2_t
{
	XYZPointf unknown000[20];
};

enum EQuadPolyDraw
{
	eQuadPolyDraw_never = 0,
	eQuadPolyDraw_flatdepth,
	eQuadPolyDraw_quad,
};

struct mdlquadpoly_t
{
	int mdlinfoindex[4];
	EQuadPolyDraw drawtype;
	EColor facecolor;
	EColor linecolor1;
	EColor linecolor2;
	EColor linecolor3;
	EColor linecolor4;
	int linewidth1;
	int linewidth2;
	int linewidth3;
	int linewidth4;
	ETextureFilmstrip m_eFilmstrip;
};
struct mdlinfo_t
{
	float unknown001;
	float color; //is this a float? is this a int? who knows!!!!!!!!!!!!!!!!!
	float unknown003;
	float unknown004;
	float unknown005;
	float unknown006;
	float unknown007;
};

struct mdloutlineconnector_t
{
	int unknown001;
	int unknown002;
	int unknown003;
	int unknown004;
	int unknown005;
	int unknown006;
	int unknown007;
	int unknown008;
};

struct mdlline_t
{
	int mdlinfoindex[2];
	int optional_pointindex;
	EColor color1;
	EColor color2;
	int linewidth;
	int unknown008;
};

//struct mdlframe_t
//{
//    XYZPointf point;
//};
typedef XYZPointf mdlframe_t;

class Sprite
{
public:

	Sprite();

	virtual ~Sprite() = default;
	virtual void InitSprite(ESpriteID spriteid, const char* name, bool hasmodel, bool drawable, int nummodelpoints = -1);
	virtual void UpdateSprite();
	virtual bool GetIsAnimatingSprite()							{ return false; };
	virtual bool GetIsCharacterSprite()							{ return false; };
	virtual bool GetIsHeadSprite()								{ return false; };
	virtual const char* GetName()								{ return m_name; };
	virtual bool GetUpdateMe()									{ return m_bUpdateMe; };
	virtual void SetUpdateMe(bool option)						{ m_bUpdateMe = option; };
	virtual bool GetDrawMe()									{ return m_bDrawMe; };
	virtual void SetDrawMe(bool option);
	virtual XYZPointf GetPosition()								{ return m_Pos; };
	virtual XYZPointf GetRotation()								{ return m_Rot; };
	virtual float GetScale()									{ return m_fScale; };
	virtual XYZPointf GetVolume()								{ return m_Volume; };
	virtual void SetPosition(XYZPointf pos, float collidesize = -1);
	virtual void SetRotation(XYZPointf rot);
	virtual void SetScale(float scale)							{ m_fScale = scale; }
	virtual void SetVolume(XYZPointf vol)						{ m_Volume = vol; }
	virtual XYZPointf GetAttachPosition()						{ return m_attachPos; }
	virtual void SetAttachPosition(XYZPointf attachpos)			{ m_attachPos = attachpos; }
	virtual void GetPickupPositionAndRotation(XZPointf& xz, float& rot);
	virtual void GetSurfaceDropPositionAndRotation(int surfindex, XZPointf& xz, float& rot);
	virtual void AddSurfaceObjectPositionAndRotationOffset(int surfindex, XZPointf& pos, float& rot);
	virtual XYZPointf GetModelPointPosition(int mdlpointindex);
	virtual ETextureFilmstrip GetPanelSpriteTextureFilmstrip()	{ return m_modelquadpoly[0].m_eFilmstrip; }
	virtual void SetPanelSpriteTextureFilmstrip(ETextureFilmstrip filmstrip);
	virtual void SetTextureFrameAnimation(int, int, int, bool);
	virtual void SetScreenText(char* text, bool option);
	virtual bool GetGrabable()									{ return m_bGrabable; }
	virtual void SetGrabable(bool option)						{ m_bGrabable = option; }
	virtual bool GetClickable()									{ return m_bClickable; }
	virtual void SetClickable(bool option)						{ m_bClickable = option; }
	virtual bool GetClickedThisFrame()							{ return m_bClickedThisFrame; }
	virtual void SetClickedThisFrame(bool option)				{ m_bClickedThisFrame = option; }
	virtual ECollidableType GetCollidableType()					{ return m_eCollidableType; }
	virtual void SetCollidableType(ECollidableType collidetype) { m_eCollidableType = collidetype; }
	virtual bool GetAmIHoldingSprite(Sprite* spr);
	virtual Sprite** GetHeldSpritesList()						{ return m_pSpritesBeingHeld; };
	virtual int GetNumHeldSprites();
	virtual Sprite* GetFirstSpriteAttachedToModelPoint(int);
	virtual int GetSpriteListAttachedToModelPoint(int mdlpointindex, Sprite** spritelist, int* spriteinfolist);
	virtual int GetNthHeldSpriteAttachModelPoint(int heldsprindex);
	virtual void SetSpriteToHold(Sprite* sprite, int, int);
	virtual void DropSprite(Sprite* sprite);
	virtual void DropAllSprites();
	virtual Sprite* GetSpriteHoldingMe()						{ return m_pSpriteHoldingMe; }
	virtual void SetSpriteHoldingMe(Sprite* sprite);
	virtual bool GetAmIStillGrabDropAnimating()					{ return m_bGettingGrabbed || m_bGettingDropped; }
	virtual XYZPointf* Get2DBoundingBox()						{ return m_2dbboxpoints; }
	virtual void Set2DBoundingBox(float xmin, float ymin, float xmax, float ymax, float z);
	virtual void AddSurface(int i1, int i2, int i3, int i4, ESurfacePickupAngle surfpickangle);
	virtual spritesurface_t* GetSurfaceListPtr() { return m_surfaces; }
	virtual int GetNumSurfaces() { return m_iNumSurfaces; }
	virtual void AddSpriteToSurface(int surfindex, Sprite* spr);
	virtual void RemoveSpriteFromSurface(int surfindex, Sprite* spr);
	virtual void UpdateSurfaces(XYZPointf pos, XYZPointf* points1, XYZPointf* points2);
	virtual void FindSurfaceIAmOn(Sprite*&, int&);
	virtual void RemoveFromAnySurfaceIMayBeOn();
	virtual int GetModelPointCursorIsOver();
	virtual void GetModelPointXRect(int mdlpointindex, XTRect<int, long>& rectout);

	XYZPointf GetSurfaceObjectPlacementPoint(int surfindex, Sprite* spr);

	ESpriteID m_eID;
	char m_name[64];
	mdlframe_t* m_modelframe = nullptr;
	mdlinfo_t* m_modelinfo = nullptr;
	mdlline_t* m_modelquadlines = nullptr;
	void* m_characterarms = nullptr;
	mdloutlineconnector_t* m_outlineconnectors = nullptr;
	mdlquadpoly_t* m_modelquadpoly = nullptr;
	bool a1_unknown00001 = false;
	bool boolean1 = false;
	// padding byte
	// padding byte
	int a1_unknown00002 = 0;
	float a1_unknown00003 = 1;
	float a1_unknown00004 = 0;
	bool a1_unknown00005 = 0;
	// padding byte
	// padding byte
	// padding byte
	float a1_unknown00006 = 0;
	float m_fYGrabOffset = 0;
	bool m_bDrawOption_1 = false;
	// padding byte
	// padding byte
	// padding byte
	int m_iPanelIndex;
	bool a1_unknown00010 = false;
	bool m_bSpriteFollowsCamera = false;
	bool a1_unknown00010c = false;
	bool a1_unknown00010d = false;
	bool a1_unknown00011a = false;
	bool m_bTransparent = false;
	bool a1_unknown00011c = false;
	int a1_unknown00012 = -1;
	int m_iNumModelPoints = 0;
	int m_iNumQuadLines = 0;
	int m_iNumQuadPolys = 0;
	int m_iNumArms;
	int b1_unknown00004;
	int b1_unknown00005;
	int b1_unknown00006;
	int b1_unknown00007;
	int b1_unknown00008;
	int b1_unknown00009;
	int b1_unknown00010;
	int b1_unknown00011;
	int b1_unknown00012;
	int b1_unknown00013;
	int b1_unknown00014;
	int b1_unknown00015;
	int b1_unknown00016;
	int b1_unknown00017;
	int b1_unknown00018;
	int m_iNumOutlineConnectors = 0;
	XYZPointf b1_unknown00020[MDL_MAXPOINTS];
	XYZPointf m_renderPoints1[MDL_MAXPOINTS];
	float pointRadius[MDL_MAXPOINTS];
	spr_unknownstruct2_t outlineconnector_data[MDL_MAXPOINTS];
	int b1_unknown15765[4428];
	spr_unknownstruct1_t b1_unknown20193[9];
	int b1_unknown20283;
	int b1_unknown20284;
	int b1_unknown20285;
	int b1_unknown20286;
	int b1_unknown20287;
	int b1_unknown20288;
	int b1_unknown20289;
	float b1_unknown20290;
	float b1_unknown20291;
	bool b1_unknown20292a;
	int m_iSpriteFrame = 0;
	int m_iFrameAnimInfo_1;
	int m_iFrameAnimInfo_2 = 0;
	int m_iFrameAnimInfo_3;
	int m_iFrameAnimInfo_4;
	bool m_iFrameAnimInfo_5 = false;
	Controller m_yawController;
	float b1_unknown20315;
	float m_GrabDropAnimFactor = 0.0;
	XYZPointf m_targetPos;
	bool m_bGettingGrabbed = false;
	bool m_bGettingDropped = false;
	Sprite* b1_unknown20321;
	int b1_unknown20322;
	char m_ScreenText[256] = "\0";
	char m_playerTextInput[32] = "\0";
	int b1_unknown20395;
	int b1_unknown20396;
	int b1_unknown20397;
	int b1_unknown20398;
	int b1_unknown20399;
	int b1_unknown20400;
	int b1_unknown20401;
	int b1_unknown20402;
	int b1_unknown20403;
	int b1_unknown20404;
	int b1_unknown20405;
	int b1_unknown20406;
	int b1_unknown20407;
	int b1_unknown20408;
	int b1_unknown20409;
	int b1_unknown20410;
	int b1_unknown20411;
	int b1_unknown20412;
	int b1_unknown20413;
	int b1_unknown20414;
	int b1_unknown20415;
	int b1_unknown20416;
	int b1_unknown20417;
	int b1_unknown20418;
	int b1_unknown20419;
	int b1_unknown20420;
	int b1_unknown20421;
	int b1_unknown20422;
	int b1_unknown20423;
	int b1_unknown20424;
	int b1_unknown20425;
	int b1_unknown20426;
	int b1_unknown20427;
	int b1_unknown20428;
	int b1_unknown20429;
	int b1_unknown20430;
	int b1_unknown20431;
	int b1_unknown20432;
	int b1_unknown20433;
	int b1_unknown20434;
	int b1_unknown20435;
	int b1_unknown20436;
	int b1_unknown20437;
	int b1_unknown20438;
	int b1_unknown20439;
	int b1_unknown20440;
	int b1_unknown20441;
	int b1_unknown20442;
	int b1_unknown20443;
	int b1_unknown20444;
	int b1_unknown20445;
	int b1_unknown20446;
	int b1_unknown20447;
	int b1_unknown20448;
	int b1_unknown20449;
	int b1_unknown20450;
	char m_playerTextOnScreen[256] = "\0";
	int m_screenTextInfo_1 = 0;
	int m_screenTextInfo_2 = 0;
	int m_screenTextInfo_3 = 0;
	char b1_unknown20518[256];
	int b1_unknown20582 = 0;
	bool b1_unknown20583 = false;
	int b1_unknown20584 = -1;
	Sprite* m_pInteractor;
	EObjectType m_eObjectType = eObjectType_Misc;
	int m_iDrinkLevel = 0;
	bool m_bUpdateMe = true;
	bool m_bDrawMe = false;
	bool m_bGrabable = false;
	bool m_bClickable = false;
	bool m_bClickedThisFrame;
	bool boolean3;
	bool boolean4;
	bool boolean5;
	ECollidableType m_eCollidableType = eCollidableType_NonSolid;
	XYZPointf m_Pos;
	XYZPointf m_Rot;
	float m_fScale = 1.0;
	XYZPointf m_Volume;
	XYZPointf m_attachPos;
	XYZPointf m_2dbboxpoints[4];
	spritesurface_t m_surfaces[10];
	int m_iNumSurfaces = 0;
	Sprite* m_pSpritesBeingHeld[5];
	int m_iSpriteHeldAttachMDLPoint[5];
	int m_iSpriteHeldInfo2[5];
	Sprite* m_pSpriteHoldingMe = nullptr;
	int c1_unknown00397;
	int c1_unknown00398;
	int c1_unknown00399;
	int c1_unknown00400;
	int c1_unknown00401;
	int c1_unknown00402;
	int c1_unknown00403;
	int c1_unknown00404;
	int c1_unknown00405;
	int c1_unknown00406;

private:

protected:

};

static_assert(sizeof(Sprite) == 84116, "");						//proof: animEngineDLL.dll:.text:1007739D

#endif // SPRITE_H
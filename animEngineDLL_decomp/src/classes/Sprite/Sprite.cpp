#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"

#include "classes/Sprite/DoorSprite.h"
#include "classes/Sprite/DrinkSprite.h"
#include "classes/Sprite/EightballMessageSprite.h"
#include "classes/Sprite/PhoneSprite.h"
#include "classes/Sprite/AnsweringMachineSprite.h"

static bool g_bElevatorButtonClicked = false;

Sprite::Sprite()
{
    for (int i = 0; i < 5; ++i)
    {
        m_pSpritesBeingHeld[i] = nullptr;
        m_iSpriteHeldAttachMDLPoint[i] = -1;
        m_iSpriteHeldInfo2[i] = -1;
    }
}

void Sprite::InitSprite(ESpriteID spriteid,
	const char* name,
	bool hasmodel, bool drawable, int nummodelpoints)
{
	m_eID = spriteid;
	animengine_assert(strlen(name) < sizeof(m_name));
	strcpy(m_name, name);
	g_spriteMgr.AddSprite(this);

	if (!hasmodel)
	{
		SetDrawMe(false);
		m_iNumModelPoints = 1;
		return;
	}

	SetDrawMe(drawable);
	if (nummodelpoints == -1)
	{
		for (int i = 0; ; ++i)
		{
			if (m_modelframe[i].m_x == DATA_NULLVAL)
			{
				m_iNumModelPoints = i;
				goto LABEL_11;
			}
			if (i >= MDL_MAXPOINTS)
				break;
		}
	LABEL_41:
		animengine_assert(0);
	}
	else
	{
        m_iNumModelPoints = nummodelpoints;
    LABEL_11:
        if (m_iNumModelPoints > MDL_MAXPOINTS)
            ShouldntBeHere("ERROR: Too many model points in sprite!");
        int j, k, m, n, ii, jj;
        for (j = 0; m_modelquadlines && m_modelquadlines[j].unknown001[0] != DATA_NULLVAL; ++j)
        {
            if (j >= 1175)
                goto LABEL_41;
        }
        m_iNumQuadLines = j;
        for (k = 0; m_modelquadpoly && m_modelquadpoly[k].mdlinfoindex[0] != DATA_NULLVAL; ++k)
        {
            if (k >= 1175)
                goto LABEL_41;
        }
        m_iNumQuadPolys = k;
        //for (m = 0; m_characterarms && m_characterarms[m] != DATA_NULLVAL; ++m)
        //{
        //    if (m >= 15)
        //        goto LABEL_41;
        //}
        //m_iNumArms = m;
        //for (n = 0; n < m_iNumArms; ++n)
        //{
        //    *(&b1_unknown00004 + n) = 0;
        //    for (ii = 0; ii < 9; ++ii)
        //    {
        //        if (m_characterarms[63 * n + 7 * ii] != -1)
        //            ++*(&b1_unknown00004 + n);
        //    }
        //    -- * (&b1_unknown00004 + n);
        //}
        //for (jj = 0; m_outlineconnectors && m_outlineconnectors[8 * jj] != DATA_NULLVAL; ++jj)
        //{
        //    if (jj >= 1175)
        //        goto LABEL_41;
        //}
        //m_iNumOutlineConnectors = jj;
        b1_unknown20299.InitController(&m_Rot.m_y, -1000, 1000, false);
        switch (m_eObjectType)
        {
        case eObjectType_Door:
            ((DoorSprite*)this)->InitDoorSprite();
            break;
        case eObjectType_Drink:
            ((DrinkSprite*)this)->InitDrinkSprite();
            break;
        case eObjectType_EightballMessage:
            ((EightballMessageSprite*)this)->InitEightballMessageSprite();
            break;
        case eObjectType_Phone:
            ((PhoneSprite*)this)->InitPhoneSprite();
            break;
        case eObjectType_AnsweringMachine:
            ((AnsweringMachineSprite*)this)->InitAnsweringMachineSprite();
            break;
        default:
            return;
        }
	}
}

void Sprite::UpdateSprite()
{
    if (g_numFramesRun > 1) b1_unknown20299.UpdateController();

    if ((m_iSpriteFrame != b1_unknown20295 || b1_unknown20298)
        && --b1_unknown20297 <= 0)
    {
        if (m_iSpriteFrame == b1_unknown20295)
        {
            animengine_assert(b1_unknown20298 != 0);
            m_iSpriteFrame = b1_unknown20294;
        }
        else
        {
            if (m_iSpriteFrame >= b1_unknown20295)
                m_iSpriteFrame--;
            else
                m_iSpriteFrame++;
        }
        b1_unknown20297 = b1_unknown20296;
    }

    if (GetClickedThisFrame())
    {
        if (a1_unknown00010 != false)
        {
            b1_unknown20584 = 4;
            m_pInteractor = g_PlayerSprite;
            AddEventInfoToLog(eEventLogType_DoInteraction, eSpriteID_player, nullptr, false, m_eID, 0);
        }
        if (m_eID == eSpriteID_elevatorButton && !g_bElevatorButtonClicked)
        {
            g_bElevatorButtonClicked = true;
            PlayGlobalSound(eGlobalSound_elevatording);
            b1_unknown20584 = 4;
            m_pInteractor = g_PlayerSprite;
            AddEventInfoToLog(eEventLogType_DoInteraction, eSpriteID_player, nullptr, false, eSpriteID_elevatorButton, 0);
        }
    }
}

extern void LoadTextureFilmstrip(ETextureFilmstrip filmstrip);
void Sprite::SetDrawMe(bool option)
{
    m_bDrawMe = option;
    if (m_bDrawMe &&  a1_unknown00008)
    {
        ETextureFilmstrip strip = GetPanelSpriteTextureFilmstrip();
        if (strip != eTextureFilmstrip_NOTATEXTURE)
            LoadTextureFilmstrip(strip);
    }
}

void Sprite::SetPosition(XYZPointf pos, float collidesize)
{
    if (collidesize < 0.0 || !CollisionTestWithWorld(this, pos, collidesize, true))
        m_Pos = pos;
}
void Sprite::SetRotation(XYZPointf rot)
{
    m_Rot.m_x = Normalize0to360(rot.m_x);
    m_Rot.m_y = Normalize0to360(rot.m_y);
    m_Rot.m_z = Normalize0to360(rot.m_z);
}

void Sprite::GetPickupPositionAndRotation(XZPointf& xz, float& rot)
{
    Sprite* surfsprite;
    int surfindex;

    xz.m_x = GetPosition().m_x;
    xz.m_z = GetPosition().m_z;
    rot = 0.0;
    FindSurfaceIAmOn(surfsprite, surfindex);
    if (surfsprite != nullptr)
        surfsprite->AddSurfaceObjectPositionAndRotationOffset(surfindex, xz, rot);
}
void Sprite::GetSurfaceDropPositionAndRotation(int surfindex, XZPointf& xz, float& rot)
{
    animengine_assert(m_iNumSurfaces > 0 && surfindex < m_iNumSurfaces);
    XYZPointf point = GetSurfaceObjectPlacementPoint(surfindex, 0);

    xz.m_x = point.m_x;
    xz.m_z = point.m_z;
    rot = 0.0;
    AddSurfaceObjectPositionAndRotationOffset(surfindex, xz, rot);
}
void Sprite::AddSurfaceObjectPositionAndRotationOffset(int surfindex, XZPointf& xz, float& rot)
{
    ESurfacePickupAngle pickupangle = m_surfaces[surfindex].pickupangle;
    switch (pickupangle)
    {
    case eSurfacePickupAngle_0:
        xz.m_z += 30;
        rot = 0.0;
        break;
    case eSurfacePickupAngle_90:
        xz.m_x -= 30;
        rot = 90;
        break;
    case eSurfacePickupAngle_180:
        xz.m_z -= 30;
        rot = 180;
        break;
    case eSurfacePickupAngle_270:
        xz.m_x += 30;
        rot = 270;
        break;
    }
}
XYZPointf Sprite::GetModelPointPosition(int mdlpointindex)
{
    XYZPointf ret;
    Sprite* spr = this;
    if (m_pSpriteHoldingMe)
        spr = m_pSpriteHoldingMe;

    XYZPointf mdlpointpos = GetObjectivePosition(spr->m_renderPoints1[mdlpointindex], spr->GetScale() / 0.625);
    mdlpointpos.m_z = spr->GetPosition().m_z;

    return mdlpointpos;
}
ETextureFilmstrip Sprite::GetPanelSpriteTextureFilmstrip()
{
    return m_modelquadpoly[0].m_eFilmstrip;
}
void Sprite::SetPanelSpriteTextureFilmstrip(ETextureFilmstrip filmstrip)
{
    animengine_assert(filmstrip < eTextureFilmstrip_Max);
    m_modelquadpoly[0].m_eFilmstrip = filmstrip;
}
void Sprite::SetTextureFrameAnimation(int a1, int a2, int a3, bool a4)
{
    m_iSpriteFrame = a1;
    b1_unknown20294 = a1;
    b1_unknown20295 = a2;
    b1_unknown20296 = a3;
    b1_unknown20297 = a3;
    b1_unknown20298 = a4;
}
void Sprite::SetScreenText(char* text, bool option)
{
    if (strlen(text) >= 256)
    {
        ShouldntBeHere("truncating requested screen text");
        text[255] = 0;
    }
    if (option)
    {
        strcpy(m_ScreenText, text);
        b1_unknown20515 = 0;
        b1_unknown20516 = 0;
    }
    else
    {
        strcat(m_ScreenText, " ");
        strcat(m_ScreenText, text);
    }
    b1_unknown20517 = 0;
}

bool Sprite::GetAmIHoldingSprite(Sprite* spr)
{
    for (int i = 0; i < 5; i++)
    {
        if (m_pSpritesBeingHeld[i] == spr)
            return true;
    }
    return false;
}
Sprite** Sprite::GetHeldSpritesList()
{
    return m_pSpritesBeingHeld;
}
int Sprite::GetNumHeldSprites()
{
    int count;
    for (count = 0; count < 5 && m_pSpritesBeingHeld[count] != nullptr; ++count)
        ;
    return count;
}
Sprite* Sprite::GetFirstSpriteAttachedToModelPoint(int mdlpointindex)
{
    if (mdlpointindex < 0 || mdlpointindex >= m_iNumModelPoints)
    {
        animengine_assert(false);
        return nullptr;
    }

    for (int i = 0; i < 5; ++i)
    {
        if (m_iSpriteHeldAttachMDLPoint[i] == mdlpointindex)
            return m_pSpritesBeingHeld[i];
    }

    return nullptr;
}
int Sprite::GetSpriteListAttachedToModelPoint(int mdlpointindex,
    Sprite** spritelist, int* spriteinfolist)
{
    if (mdlpointindex < 0 || mdlpointindex >= m_iNumModelPoints)
    {
        animengine_assert(false);
        return 0;
    }

    int count = 0;
    for (int i = 0; i < 5; ++i)
    {
        if (m_iSpriteHeldAttachMDLPoint[i] == mdlpointindex)
        {
            spritelist[count] = m_pSpritesBeingHeld[i];
            spriteinfolist[count] = m_iSpriteHeldInfo2[i];
            count++;
        }
    }
    return count;
}
int Sprite::GetNthHeldSpriteAttachModelPoint(int heldsprindex)
{
    animengine_assert(heldsprindex < 5);
    return m_iSpriteHeldAttachMDLPoint[heldsprindex];
}
void Sprite::SetSpriteToHold(Sprite* sprite, int mdlpointindex1, int mdlpointindex2)
{
    if (!sprite || sprite == this)
        return;

    int v1 = 0;
    if (GetIsAnimatingSprite())
        v1 = ((AnimatingSprite*)this)->m_animMgr.unknown[80934];

    animengine_assert(mdlpointindex1 >= -v1 
        && mdlpointindex1 < m_iNumModelPoints);
    animengine_assert(mdlpointindex2 == -DATA_NULLVAL || mdlpointindex2 >= -v1 
        && mdlpointindex2 < m_iNumModelPoints);

    if (GetSpriteHoldingMe() == sprite
        || sprite->GetAmIHoldingSprite(this))
    {
        ShouldntBeHere("Cannot hold the sprite that is holding me!");
        return;
    }

    if (sprite->GetSpriteHoldingMe() && sprite->GetSpriteHoldingMe() != this)
        return;

    int i;
    for (i = 0; i < 5 && m_pSpritesBeingHeld[i] && m_pSpritesBeingHeld[i] != sprite; i++);

    if (i == 5)
    {
        ShouldntBeHere("Holding max number of sprites already");
        return;
    }

    animengine_assert(i >= 0);

    m_pSpritesBeingHeld[i] = sprite;
    m_iSpriteHeldAttachMDLPoint[i] = mdlpointindex1;
    m_iSpriteHeldInfo2[i] = mdlpointindex2;

    sprite->RemoveFromAnySurfaceIMayBeOn();
    sprite->SetSpriteHoldingMe(this);
    sprite->b1_unknown20584 = 0;
    sprite->m_pInteractor = this;
}
void Sprite::DropSprite(Sprite* sprite)
{
    if (!sprite)
        return;

    int i;
    for (i = 0; i < 5 && m_pSpritesBeingHeld[i] != sprite; ++i);

    if (i == 5)
        return;

    XYZPointf pos = GetModelPointPosition(i);
    sprite->SetPosition(pos);
    for (int j = i; j < 4; ++j)
    {
        m_pSpritesBeingHeld[j] = m_pSpritesBeingHeld[j + 1];
        m_iSpriteHeldAttachMDLPoint[j] = m_iSpriteHeldAttachMDLPoint[j + 1];
        m_iSpriteHeldInfo2[j] = m_iSpriteHeldInfo2[j + 1];
    }

    m_pSpritesBeingHeld[4] = 0;
    m_iSpriteHeldAttachMDLPoint[4] = -1;
    m_iSpriteHeldInfo2[4] = -DATA_NULLVAL;

    Sprite* holder = sprite->GetSpriteHoldingMe();
    sprite->SetSpriteHoldingMe(nullptr);
    sprite->b1_unknown20584 = 0;


    if (holder == g_CursorSprite)
        sprite->m_pInteractor = g_PlayerSprite;
    else
        sprite->m_pInteractor = this;
}
void Sprite::DropAllSprites()
{
    for (int i = 0; i < 5; ++i)
    {
        if (!m_pSpritesBeingHeld[i])
            continue;
        m_pSpritesBeingHeld[i]->SetPosition(GetModelPointPosition(i));
        m_pSpritesBeingHeld[i]->SetSpriteHoldingMe(nullptr);
        m_pSpritesBeingHeld[i] = nullptr;
    }
}
Sprite* Sprite::GetSpriteHoldingMe()
{
    return m_pSpriteHoldingMe;
}
void Sprite::SetSpriteHoldingMe(Sprite* sprite)
{
    if (sprite == this)
        return;

    if (sprite && 
        (sprite->GetSpriteHoldingMe() == this || GetAmIHoldingSprite(sprite)))
    {
        ShouldntBeHere("Cannot be held by the sprite that I am holding!");
        return;
    }

    m_pSpriteHoldingMe = sprite;
}
bool Sprite::GetAmIStillGrabDropAnimating()
{
    return m_bGettingGrabbed || m_bGettingDropped;
}

XYZPointf* Sprite::Get2DBoundingBox()
{
    return m_2dbboxpoints;
}
void Sprite::Set2DBoundingBox(float xmin, float ymin, float xmax, float ymax, float z)
{
    m_2dbboxpoints[0] = XYZPointf(xmin, ymin, z);
    m_2dbboxpoints[1] = XYZPointf(xmax, ymin, z);
    m_2dbboxpoints[2] = XYZPointf(xmax, ymax, z);
    m_2dbboxpoints[3] = XYZPointf(xmin, ymax, z);
}

void Sprite::AddSurface(int i1, int i2, int i3, int i4, ESurfacePickupAngle surfpickangle)
{
    if (m_iNumSurfaces >= 10)
    {
        animengine_assert(false);
        return;
    }

    m_surfaces[m_iNumSurfaces].unknown001[0] = i1;
    m_surfaces[m_iNumSurfaces].unknown001[1] = i2;
    m_surfaces[m_iNumSurfaces].unknown001[2] = i3;
    m_surfaces[m_iNumSurfaces].unknown001[3] = i4;
    m_surfaces[m_iNumSurfaces].pickupangle = surfpickangle;
    m_surfaces[m_iNumSurfaces].numsprites = 0;
    for (int i = 0; i < 5; ++i)
        m_surfaces[m_iNumSurfaces].sprites[i] = 0;
    ++m_iNumSurfaces;
}

spritesurface_t* Sprite::GetSurfaceListPtr()
{
    return m_surfaces;
}
int Sprite::GetNumSurfaces()
{
    return m_iNumSurfaces;
}
void Sprite::AddSpriteToSurface(int surfindex, Sprite* spr)
{
    animengine_assert(spr != 0);

    if (surfindex < 0 || surfindex >= m_iNumSurfaces)
    {
        ShouldntBeHere("Illegal surfaceID");
        return;
    }

    if (m_surfaces[surfindex].numsprites >= 5)
    {
        ShouldntBeHere("Surface is full, cannot add sprite to surface");
        return;
    }

    for (int i = 0; i < m_surfaces[surfindex].numsprites; ++i)
    {
        if (m_surfaces[surfindex].sprites[i] == spr)
        {
            ShouldntBeHere("sprite is already on surface");
            return;
        }
    }

    XZRectf surfrect;
    surfrect.m_xmin = m_surfaces[surfindex].points1[0].m_x;
    surfrect.m_zmin = m_surfaces[surfindex].points1[0].m_z;
    surfrect.m_xmax = m_surfaces[surfindex].points1[2].m_x;
    surfrect.m_zmax = m_surfaces[surfindex].points1[2].m_z;

    XZRectf sprrect;
    sprrect.m_xmax = spr->GetPosition().m_x;
    sprrect.m_xmin = surfrect.m_xmax;
    sprrect.m_zmax = spr->GetPosition().m_z;
    sprrect.m_zmin = surfrect.m_zmax;

    float v1 = m_surfaces[surfindex].points1[0].m_y - spr->GetPosition().m_y;
    bool rectsintersect = DoXZRectsIntersect(surfrect, sprrect);

    if (!rectsintersect || fabs(v1) > 50)
        dprintf("WARNING: %s to surface is not near surface", spr->GetName());

    m_surfaces[surfindex].sprites[m_surfaces[surfindex].numsprites++] = spr;
}
void Sprite::RemoveSpriteFromSurface(int surfindex, Sprite* spr)
{
    int spr_surf_index = -1;
    animengine_assert(spr != 0);

    if (surfindex < 0 || surfindex >= m_iNumSurfaces)
    {
        ShouldntBeHere("Illegal surfaceID");
        return;
    }

    for (int i = 0; i < m_surfaces[surfindex].numsprites; ++i)
    {
        if (m_surfaces[surfindex].sprites[i] == spr)
            spr_surf_index = i;
    }
    if (spr_surf_index == -1)
        ShouldntBeHere("sprite is not on surface in RemoveSpriteFromSurface()");

    int j;
    for (j = spr_surf_index; j < m_surfaces[surfindex].numsprites - 1; ++j)
        m_surfaces[surfindex].sprites[j] = m_surfaces[surfindex].sprites[j + 1];

    animengine_assert(j <= 4);
    m_surfaces[surfindex].sprites[j] = 0;
    --m_surfaces[surfindex].numsprites;
}
void Sprite::UpdateSurfaces(XYZPointf pos, XYZPointf* points1, XYZPointf* points2)
{
    XYZPointf bboxpoints1[4];
    XYZPointf bboxpoints2[4];

    for (int i = 0; i < m_iNumSurfaces; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            int pointindex = m_surfaces[i].unknown001[j];

            bboxpoints1[j] = points2[pointindex];
            m_surfaces[i].points1[j] = points1[pointindex] + pos;
        }

        GetXYBoundingBoxOf4Points(bboxpoints1, bboxpoints2);

        for (int j = 0; j < 4; ++j)
            m_surfaces[i].points2[j] = bboxpoints2[j];
    }
}
void Sprite::FindSurfaceIAmOn(Sprite*& surfsprite, int& surfindex)
{
    for (int i = 0; i < g_spriteMgr.GetNumSprites(); ++i)
    {
        Sprite* sprite = (Sprite*)g_spriteMgr.GetSpritePtr(i);
        for (int j = 0; j < sprite->m_iNumSurfaces; ++j)
        {
            for (int k = 0; k < sprite->m_surfaces[j].numsprites; ++k)
            {
                if (sprite->m_surfaces[j].sprites[k] == this)
                {
                    surfsprite = sprite;
                    surfindex = j;
                    return;
                }
            }
        }
    }
    surfsprite = nullptr;
    surfindex = -1;
}
void Sprite::RemoveFromAnySurfaceIMayBeOn()
{
    Sprite* surfsprite;
    int surfindex;
    FindSurfaceIAmOn(surfsprite, surfindex);
    if (surfsprite != 0)
        surfsprite->RemoveSpriteFromSurface(surfindex, this);
}
int Sprite::GetModelPointCursorIsOver()
{
    XYZPointf cursorpos = g_CursorSprite->GetPosition();
    int pointindex = -1;
    for (int i = 0; i < m_iNumModelPoints; ++i)
    {
        XYZPointf mdlpoint = m_renderPoints1[i];

        float sizeonscreen = GetScale() * m_modelinfo[i].unknown001;
        if (sizeonscreen < 0.1)
            sizeonscreen = 0.1;

        if (mdlpoint.m_x - sizeonscreen <= cursorpos.m_x
            && mdlpoint.m_x + sizeonscreen >= cursorpos.m_x
            && mdlpoint.m_y - sizeonscreen <= cursorpos.m_y
            && mdlpoint.m_y + sizeonscreen >= cursorpos.m_y
            && (pointindex == -1 || m_renderPoints1[i].m_z > m_renderPoints1[pointindex].m_z))
        {
            pointindex = i;
        }
    }
    return pointindex;
}

void Sprite::GetModelPointXRect(int mdlpointindex, XTRect<int, long>& rectout)
{
    //undone
}

XYZPointf Sprite::GetSurfaceObjectPlacementPoint(int surfindex, Sprite* spr)
{
    XYZPointf placepoint;

    animengine_assert(m_iNumSurfaces > 0 && surfindex < m_iNumSurfaces);

    spritesurface_t* surf = &m_surfaces[surfindex];

    placepoint.m_x = (surf->points1[0].m_x + surf->points1[2].m_x) / 2.0;
    placepoint.m_y = surf->points1[0].m_y;
    placepoint.m_z = (surf->points1[0].m_z + surf->points1[2].m_z) / 2.0;
    if (spr != nullptr)
        placepoint.m_y += spr->GetVolume().m_y / 2.0;

    return placepoint;
}
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
        m_iSpriteHeldInfo1[i] = -1;
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
        for (j = 0; m_modelquadlines && m_modelquadlines[j].unknown001 != DATA_NULLVAL; ++j)
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

void Sprite::GetPickupPositionAndRotation(XZPointf& pos, float& rot)
{
    //undone
}
void Sprite::GetSurfaceDropPositionAndRotation(int, XZPointf& pos, float& rot)
{
    //undone
}
void Sprite::AddSurfaceObjectPositionAndRotationOffset(int, XZPointf& pos, float& rot)
{
    //undone
}
XYZPointf Sprite::GetModelPointPosition(int)
{
    //undone
    return XYZPointf(0, 0, 0);
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
    //LOBYTE(this->data.b1_unknown20298) = a4;
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

bool Sprite::GetAmIHoldingSprite()
{
    //undone
    return false;
}
Sprite** Sprite::GetHeldSpritesList()
{
    //undone
    return nullptr;

}
int Sprite::GetNumHeldSprites()
{
    //undone
    return 0;
}
Sprite* Sprite::GetFirstSpriteAttachedToModelPoint(int)
{
    //undone
    return nullptr;
}
Sprite* Sprite::GetNthHeldSpriteAttachModelPoint(int)
{
    //undone
    return nullptr;
}
void Sprite::SetSpriteToHold(Sprite* sprite, int, int)
{
    //undone
}
void Sprite::DropSprite(Sprite* sprite)
{
    //undone
}
void Sprite::DropAllSprites()
{
    //undone
}
Sprite* Sprite::GetSpriteHoldingMe()
{
    //undone
    return nullptr;
}
void Sprite::SetSpriteHoldingMe(Sprite* sprite)
{
    if (sprite == this)
        return;

    if (sprite && (sprite->GetSpriteHoldingMe() == this || GetAmIHoldingSprite()))
    {
        ShouldntBeHere("Cannot be held by the sprite that I am holding!");
    }
    else
    {
        m_pSpriteHoldingMe = sprite;
    }
}
bool Sprite::GetAmIStillGrabDropAnimating()
{
    //undone
    return false;
}

void Sprite::AddSurface(int i1, int i2, int i3, int i4, ESurfacePickupAngle surfpickangle)
{
    //undone
}

void Sprite::GetModelPointXRect(int mdlpointindex, XTRect<int, long>& rectout)
{
    //undone
}
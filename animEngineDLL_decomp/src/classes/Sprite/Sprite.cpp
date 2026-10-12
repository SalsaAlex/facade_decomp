#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"

#include "classes/Sprite/DoorSprite.h"
#include "classes/Sprite/DrinkSprite.h"
#include "classes/Sprite/EightballMessageSprite.h"
#include "classes/Sprite/PhoneSprite.h"
#include "classes/Sprite/AnsweringMachineSprite.h"

static bool g_bElevatorButtonClicked = false;


void Sprite::InitSprite(ESpriteID spriteid,
	const char* name,
	bool hasmodel, bool drawable, int nummodelpoints = -1)
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
			if (*((float*)m_modelframe + 3 * i) == 666.0)
			{
				m_iNumModelPoints = i;
				goto LABEL_11;
			}
			if (i >= 235)
				break;
		}
	LABEL_41:
		animengine_assert(0);
	}
	else
	{
        m_iNumModelPoints = nummodelpoints;
    LABEL_11:
        if (m_iNumModelPoints > 235)
            ShouldntBeHere("ERROR: Too many model points in sprite!");
        int j, k, m, n, ii, jj;
        for (j = 0; m_modelquadlines && m_modelquadlines[j].unknown001 != 666; ++j)
        {
            if (j >= 1175)
                goto LABEL_41;
        }
        m_iNumQuadLines = j;
        for (k = 0; m_modelquadpoly && m_modelquadpoly[k].mdlinfoindex_1 != 666; ++k)
        {
            if (k >= 1175)
                goto LABEL_41;
        }
        m_iNumQuadPolys = k;
        for (m = 0; m_characterarms && m_characterarms[m] != 666; ++m)
        {
            if (m >= 15)
                goto LABEL_41;
        }
        m_iNumArms = m;
        for (n = 0; n < m_iNumArms; ++n)
        {
            *(&b1_unknown00004 + n) = 0;
            for (ii = 0; ii < 9; ++ii)
            {
                if (m_characterarms[63 * n + 7 * ii] != -1)
                    ++*(&b1_unknown00004 + n);
            }
            -- * (&b1_unknown00004 + n);
        }
        for (jj = 0; m_outlineconnectors && m_outlineconnectors[8 * jj] != 666; ++jj)
        {
            if (jj >= 1175)
                goto LABEL_41;
        }
        m_iNumOutlineConnectors = jj;
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

    if ((m_iSpriteFrame != b1_unknown20295 || LOBYTE(b1_unknown20298))
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
        if (a1_unknown00010 != 0)
        {
            b1_unknown20584 = 4;
            b1_unknown20585 = g_PlayerSprite;
            AddEventInfoToLog(eEventLogType_DoInteraction, eSpriteID_player, nullptr, false, m_eID, 0);
        }
        if (m_eID == eSpriteID_elevatorButton && !g_bElevatorButtonClicked)
        {
            g_bElevatorButtonClicked = true;
            PlayGlobalSound(eGlobalSound_elevatording);
            b1_unknown20584 = 4;
            b1_unknown20585 = g_PlayerSprite;
            AddEventInfoToLog(eEventLogType_DoInteraction, eSpriteID_player, nullptr, false, eSpriteID_elevatorButton, 0);
        }
    }
}

void Sprite::SetPosition(XYZPointf pos, float unknown)
{
    if (unknown < 0.0 || !CollisionTestWithWorld(this, pos, unknown, true))
        m_Pos = pos;
}
void Sprite::SetRotation(XYZPointf rot)
{
    m_Rot.m_x = Normalize0to360(rot.m_x);
    m_Rot.m_y = Normalize0to360(rot.m_y);
    m_Rot.m_z = Normalize0to360(rot.m_z);
}
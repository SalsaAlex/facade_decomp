#include "facadecommon.h"
#include "classes/Sprite/Sprite.h"

#include "classes/Sprite/DoorSprite.h"
#include "classes/Sprite/DrinkSprite.h"
#include "classes/Sprite/EightballMessageSprite.h"
#include "classes/Sprite/PhoneSprite.h"
#include "classes/Sprite/AnsweringMachineSprite.h"

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
        for (j = 0; m_modelquadlines && m_modelquadlines[8 * j] != 666; ++j)
        {
            if (j >= 1175)
                goto LABEL_41;
        }
        b1_unknown00001 = j;
        for (k = 0; m_modelquadpoly && m_modelquadpoly[15 * k] != 666; ++k)
        {
            if (k >= 1175)
                goto LABEL_41;
        }
        b1_unknown00002 = k;
        for (m = 0; m_characterarms && m_characterarms[63 * m] != 666; ++m)
        {
            if (m >= 15)
                goto LABEL_41;
        }
        b1_unknown00003 = m;
        for (n = 0; n < b1_unknown00003; ++n)
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
        b1_unknown00019 = jj;
        b1_unknown20299.InitController(&c1_unknown00008.m_y, -1000, 1000, false);
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
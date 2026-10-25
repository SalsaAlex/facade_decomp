#include "facadecommon.h"
#include "classes/Sprite/DoorSprite.h"
#include "gl/GLU.h"

#include "glfuncs.h"

DEFINE_HOOKEDVAR(float, g_fadeIncr, 0x106988C0)
DEFINE_HOOKEDVAR(float, g_fadeAmount, 0x106988C4)
DEFINE_HOOKEDARRAYVAR(GLYPHMETRICSFLOAT, g_FontGlyphs, 0x10698940)
DEFINE_HOOKEDVAR(GLuint, g_BitmapFontBase, 0x1069A160)

#define g_fadeIncr			_g_fadeIncr()
#define g_fadeAmount		_g_fadeAmount()
#define g_FontGlyphs		_g_FontGlyphs()
#define g_BitmapFontBase	_g_BitmapFontBase()

enum E2DPosterRotateType
{
	e2DPosterRotateType_normal = 0,
	e2DPosterRotateType_nolowerbody_nopelvis,
	e2DPosterRotateType_pelvis,
	e2DPosterRotateType_lowerbody,
};

#define DRAW_MSG(text, verticalcursor, amnt_to_move_cursor_down) \
	DrawArbitraryScreenText(text, -58, verticalcursor, 350, 5, 0, eColor_white, eColor_black); \
	verticalcursor -= amnt_to_move_cursor_down

#define DRAWTEXTUREDBOX(pointx, pointy, pointz, sizex, sizey, filmstrip) \
			glLoadIdentity(); \
			glTranslatef(pointx, pointy, pointz); \
			glBindTexture(GL_TEXTURE_2D, g_TextureFilmstrips[filmstrip].entries[0].texid); \
				glBegin(GL_QUADS); \
					glTexCoord2f(0, 0); \
					glVertex3f(0, 0, 0); \
					glTexCoord2f(1, 0); \
					glVertex3f(sizex, 0, 0); \
					glTexCoord2f(1, 1); \
					glVertex3f(sizex, sizey, 0); \
					glTexCoord2f(0, 1); \
				glVertex3f(0, sizey, 0); \
			glEnd(); \
			glLoadIdentity()

#define ISCURSORINBOUNDS(xmin, xmax, ymin, ymax) ( (g_CursorSprite->GetPosition().m_x < xmax) && \
												 (g_CursorSprite->GetPosition().m_x > xmin) && \
												 (g_CursorSprite->GetPosition().m_y < ymax) && \
												 (g_CursorSprite->GetPosition().m_y > ymin) )


struct font_charinfo_t
{
	char character;
	int unknown001;
	int unknown002;
	int unknown003;
	int unknown004;
};

font_charinfo_t g_fontInfo[] = {
	{'a',		  1,	 -1,	55, 85},
	{'b',		 64,	 -1,	55, 85},
	{'c',		131,	 -1,	49, 85},
	{'d',		192,	 -1,	56, 85},
	{'e',		260,	 -1,	49, 85},
	{'f',		322,	 -1,	42, 85},
	{'g',		367,	 -1,	53, 85},
	{'h',		430,	 -1,	59, 85},
	{'i',		  1,	 81,	32, 85},
	{'j',		 36,	 81,	38, 85},
	{'k',		 88,	 81,	58, 85},
	{'l',		157,	 81,	32, 85},
	{'m',		203,	 81,	78, 85},
	{'n',		293,	 81,	58, 85},
	{'o',		362,	 81,	52, 85},
	{'p',		425,	 81,	56, 85},
	{'q',		  0,	166,	55, 85},
	{'r',		 66,	166,	44, 85},
	{'s',		120,	166,	47, 85},
	{'t',		181,	166,	38, 85},
	{'u',		228,	166,	61, 85},
	{'v',		301,	166,	53, 85},
	{'w',		361,	166,	71, 85},
	{'x',		441,	166,	53, 85},
	{'y',		  0,	252,	56, 85},
	{'z',		 61,	252,	48, 85},
	{'0',		119,	252,	55, 85},
	{'1',		196,	252,	39, 85},
	{'2',		256,	252,	52, 85},
	{'3',		324,	252,	53, 85},
	{'4',		390,	252,	54, 85},
	{'5',		459,	252,	51, 85},
	{'6',		  2,	338,	54, 85},
	{'7',		 69,	338,	53, 85},
	{'8',		135,	338,	54, 85},
	{'9',		202,	338,	56, 85},
	{'!',		274,	338,	26, 85},
	{'?',		316,	338,	50, 85},
	{'.',		381,	338,	26, 85},
	{',',		427,	338,	27, 85},
	{'\"',		470,	338,	34, 85},
	{'\'',		  1,	425,	24, 85},
	{';',		 43,    425,	27, 85},
	{':',		 89,    425,	25, 85},
	{'(',		134,    425,	29, 85},
	{')',		176,    425,	29, 85},
	{'-',		222,    425,	36, 85},
	{'A',		  6,	 15,	68, 85},
	{'B',		 81,     15,	68, 85},
	{'C',		157,     15,	67, 85},
	{'D',		235,     15,	71, 85},
	{'E',		315,     15,	66, 85},
	{'F',		390,     15,	62, 85},
	{'G',		  9,	101,	72, 85},
	{'H',		 92,    101,	69, 85},
	{'I',		170,    101,	38, 85},
	{'J',		214,    101,	61, 85},
	{'K',		285,    101,	68, 85},
	{'L',		360,    101,	60, 85},
	{'M',		426,    101,	78, 85},
	{'N',		 10,    186,	67, 85},
	{'O',		 85,    186,	72, 85},
	{'P',		168,    186,	61, 85},
	{'Q',		234,    186,	72, 85},
	{'R',		315,    186,	67, 85},
	{'S',		390,    186,	61, 85},
	{'T',		  6,	272,	65, 85},
	{'U',		 77,    272,	70, 85},
	{'V',		153,    272,	70, 85},
	{'W',		227,    272,	86, 85},
	{'X',		318,    272,	71, 85},
	{'Y',		  6,	358,	66, 85},
	{'Z',		 77,    358,	63, 85},
};

static float get_gl_linewidth(float val) {
	return val * 0.125 + 0.5;
}

void RotateArrayOfPoints(XYZPointf* in_n_out, int numpoints, XYZPointf rotation, XYZPointf pos)
{
	glPushMatrix();
	for (int i = 0; i < numpoints; ++i)
	{
		glLoadIdentity();
		if (rotation.m_x != 0.0)	glRotatef(rotation.m_x, 1.0, 0.0, 0.0);
		if (rotation.m_y != 0.0)	glRotatef(rotation.m_y, 0.0, 1.0, 0.0);
		if (rotation.m_z != 0.0)	glRotatef(rotation.m_z, 0.0, 0.0, 1.0);

		GLdouble mdlmatrix[16];
		GLdouble mdlviewmatrix[16];

		memset(mdlmatrix, 0, sizeof(mdlmatrix));
		mdlmatrix[0] = in_n_out[i].m_x - pos.m_x;
		mdlmatrix[1] = in_n_out[i].m_y - pos.m_y;
		mdlmatrix[2] = in_n_out[i].m_z - pos.m_z;
		mdlmatrix[5] = 1.0;
		mdlmatrix[10] = 1.0;
		mdlmatrix[15] = 1.0;

		glMultMatrixd(mdlmatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, mdlviewmatrix);

		in_n_out[i].m_x = pos.m_x + mdlviewmatrix[0];
		in_n_out[i].m_y = pos.m_y + mdlviewmatrix[1];
		in_n_out[i].m_z = pos.m_z + mdlviewmatrix[2];
	}
	glPopMatrix();
}

void RotateModel(Sprite* spr, XYZPointf rotation, XYZPointf* pointsout)
{
	bool no_rotation_to_do = rotation.m_x == 0.0 
		&& rotation.m_y == 0.0 
		&& rotation.m_z == 0.0;

	int pointindex = spr->a1_unknown00002;
	HeadSprite* headsprite = nullptr;

	if (spr->GetIsHeadSprite())
	{
		headsprite = (HeadSprite*)spr;
		//undone
	}
	else
		animengine_assert(!spr->a1_unknown00010d);


	XYZPointf basepoint = spr->m_modelframe[pointindex];

	if (!no_rotation_to_do)
		glPushMatrix();

	for (int i = 0; i < spr->m_iNumModelPoints; ++i)
	{
		XYZPointf curpoint = spr->m_modelframe[i];

		bool headspritebool =  headsprite
			//&& (i == LODWORD(headspriteptr[1].data.b1_unknown00020[19].m_y)
			//	|| i == LODWORD(headspriteptr[1].data.b1_unknown00020[17].m_y) + 2
			//	|| i == LODWORD(headspriteptr[1].data.b1_unknown00020[17].m_y) + 5)
			;

		if ((no_rotation_to_do || headspritebool) && spr->a1_unknown00003 == 1.0)
		{
			pointsout[i] = curpoint;
			continue;
		}

		glLoadIdentity();
		if (rotation.m_x != 0.0)	glRotatef(rotation.m_x, 1.0, 0.0, 0.0);
		if (rotation.m_y != 0.0)	glRotatef(rotation.m_y, 0.0, 1.0, 0.0);
		if (rotation.m_z != 0.0)	glRotatef(rotation.m_z, 0.0, 0.0, 1.0);

		GLdouble mdlmatrix[16];
		GLdouble mdlviewmatrix[16];

		memset(mdlmatrix, 0, sizeof(mdlmatrix));

		mdlmatrix[0] = curpoint.m_x - basepoint.m_x;
		mdlmatrix[1] = curpoint.m_y - basepoint.m_y;
		mdlmatrix[2] = curpoint.m_z - basepoint.m_z;

		if (spr->a1_unknown00003 != 1.0)
		{
			mdlmatrix[0] *= spr->a1_unknown00003;
			mdlmatrix[1] *= spr->a1_unknown00003;
			mdlmatrix[2] *= spr->a1_unknown00003;
		}

		mdlmatrix[5] = 1.0;
		mdlmatrix[10] = 1.0;
		mdlmatrix[15] = 1.0;

		glMultMatrixd(mdlmatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, mdlviewmatrix);

		pointsout[i].m_x = basepoint.m_x + mdlviewmatrix[0];
		pointsout[i].m_y = basepoint.m_y + mdlviewmatrix[1];
		pointsout[i].m_z = basepoint.m_z + mdlviewmatrix[2];
	}

	if (!no_rotation_to_do)
		glPopMatrix();
}


void Do2DPosterRotateAndProject(
	Sprite* spr,
	XYZPointf* list,
	XYZPointf a3,
	XYZPointf rotation,
	int index,
	E2DPosterRotateType rotatetype)
{
	GLdouble winz;
	GLdouble winy;
	GLdouble winx;
	GLdouble projMatrix[16];
	GLdouble modelMatrix[16];
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	glPushMatrix();

	if (index >= 0)									glTranslatef(list[index].m_x, list[index].m_y, list[index].m_z);

	XYZPointf diff = rotation - g_PlayerSprite->GetRotation();

	if (diff.m_x != 0)								glRotatef(diff.m_x, 1, 0, 0);
	if (!spr->GetIsHeadSprite() && diff.m_y != 0)	glRotatef(diff.m_y, 0, 1, 0);
	if (diff.m_z != 0)								glRotatef(diff.m_z, 0, 0, 1);

	if (index >= 0)									glTranslatef(-list[index].m_x, -list[index].m_y, -list[index].m_z);

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

	for (int i = 0; i < spr->m_iNumModelPoints; ++i)
	{
		if (rotatetype != e2DPosterRotateType_normal)
		{
			bool IsLowerBodyModelPointIndex = ((CharacterSprite*)spr)->IsLowerBodyModelPointIndex(i);
			bool IsPelvisModelPointIndex = ((CharacterSprite*)spr)->IsPelvisModelPointIndex(i);

			if (rotatetype == e2DPosterRotateType_nolowerbody_nopelvis && (IsLowerBodyModelPointIndex || IsPelvisModelPointIndex))
				continue;

			if (rotatetype == e2DPosterRotateType_lowerbody && !IsLowerBodyModelPointIndex)
				continue;

			if (rotatetype == e2DPosterRotateType_pelvis && !IsPelvisModelPointIndex)
				continue;
		}
		gluProject(list[i].m_x, list[i].m_y, list[i].m_z, modelMatrix, projMatrix, viewport, &winx, &winy, &winz);
		spr->m_renderPoints1[i].m_x = spr->GetScale() * (winx - (double)(g_windowWidth / 2)) + a3.m_x;
		spr->m_renderPoints1[i].m_y = spr->GetScale() * (winy - (double)(g_windowHeight / 2)) + a3.m_y;
		spr->m_renderPoints1[i].m_z = winz;
	}
	glPopMatrix();
}


static float glPrintPolygonalFont(const char* text)
{
	char buffer[256];
	float spacetaken = 0.0;

	if (!text)
		return 0.0;

	strcpy(buffer, text);
	for (int i = 0; i < strlen(buffer); i++)
		spacetaken += g_FontGlyphs[buffer[i]].gmfCellIncX;

	glPushAttrib(GL_LIST_BIT);
	glListBase(g_BitmapFontBase);
	glCallLists(strlen(buffer), GL_UNSIGNED_BYTE, buffer);
	glPopAttrib();
	return spacetaken;
}


void DrawArbitraryScreenText(const char* Str,
	float arg_x, float arg_y,
	int ifactor,
	GLfloat scale,
	float floatfactor,
	EColor maincolor, EColor outlinecolor)
{
	int v9;
	char stringchar[4];
	int v18;

	int stringlen = strlen(Str);
	for (int i = 1; i < 2; ++i)
	{
		v18 = -1;
		glLoadIdentity();
		float v15 = 0;
		if (i==1) SetColorFromIndex(maincolor); else SetColorFromIndex(outlinecolor);
		for (int j = 0; j < stringlen; ++j)
		{
			bool changecursor = 0;
			if (v18 == -1)
			{
				changecursor = true;
			}
			else if (j > 0 && Str[j - 1] == 32)
			{
				changecursor = (float)ifactor / 10.f <= v15;
			}

			if (changecursor)
			{
				++v18;
				GLfloat x = arg_x - 25.0;
				GLfloat y = arg_y - (float)v18 * scale * 0.7;
				GLfloat z = floatfactor + -2.0;
				if (!i)
				{
					x += 0.3;
					y -= 0.3;
					z -= 2.0;
				}
				glLoadIdentity();
				glTranslatef(x, y, z);
				v15 = 0;
				glScalef(scale, scale, scale);
			}
			stringchar[0] = Str[j];
			stringchar[1] = '\0';
			float spacetaken = glPrintPolygonalFont(stringchar);
			if (stringchar[0] != -25)
				v15 = spacetaken + v15;
		}
	}
	glLoadIdentity();
}


static void* LoadDIBitmap(const char* filename, BITMAPINFOHEADER** imginfo)
{
	BITMAPFILEHEADER bmpheader;

	void* imagedata = nullptr;
	uint32_t dataoffset;
	uint32_t bytestoread;
	FILE* hfile = fopen(filename, "rb");

	if (!hfile)
		return nullptr;

	if (!fread(&bmpheader, sizeof(BITMAPFILEHEADER), 1, hfile)
		|| bmpheader.bfType != ('M' << 8 | 'B')
		|| (*imginfo = (BITMAPINFOHEADER*)malloc(bmpheader.bfOffBits - sizeof(BITMAPFILEHEADER))) == nullptr)
	{
		goto fail_closefile;
	}
	dataoffset = bmpheader.bfOffBits - sizeof(BITMAPFILEHEADER);
	if (fread(*imginfo, 1, dataoffset, hfile) < dataoffset)
		goto fail_deallocate_block;


	bytestoread = (*imginfo)->biSizeImage;
	if (!(*imginfo)->biSizeImage)
	{
		bytestoread = ((*imginfo)->biWidth *
			(*imginfo)->biBitCount + 7) / 8 *
			abs((*imginfo)->biHeight);
	}

	if ((imagedata = malloc(bytestoread)) == nullptr)
		goto fail_deallocate_block;

	if (fread(imagedata, 1, bytestoread, hfile) < bytestoread)
		goto fail_deallocate_imagedata;



	fclose(hfile);
	return imagedata;



fail_deallocate_imagedata:
	free(imagedata);
fail_deallocate_block:
	free(*imginfo);
fail_closefile:
	fclose(hfile);

	return nullptr;
}

void BuildBitmapFont()
{
	int tickstart = GetTickCount();
	dprintf("Building bitmap font...");
	_set_g_BitmapFontBase(glGenLists(256));
	HFONT h = CreateFontA(-12, 0, 0, 0, 700, 0, 0, 0, 0, 4, 0, 4, 0, "Comic Sans MS");
	SelectObject(g_hDeviceContext, h);
	wglUseFontOutlinesA(g_hDeviceContext, 0, 255, g_BitmapFontBase, 0, 0, 1, g_FontGlyphs);
	dprintf("Done building bitmap font (%.3f seconds)", (GetTickCount() - tickstart) / 1000.0);
}
void KillPolygonalFont()
{
	glDeleteLists(g_BitmapFontBase, 256);
}

void SetupRC()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	if (g_bAntiAliasing)
		glEnable(GL_LINE_SMOOTH);
}

GLuint LoadTexture(const char* filename, bool alpha, uint32_t wraptype, uint32_t magnify_type, uint32_t minify_type)
{
	BITMAPINFOHEADER* imginfo;
	void* imagedata = LoadDIBitmap(filename, &imginfo);
	uint8_t* imagehead = (uint8_t*)imagedata;
	uint32_t numpixels = imginfo->biWidth * imginfo->biHeight;

	//transpose red&green (?)
	for (int i = 0; i < numpixels; i++)
	{
		VAL_SWAP(uint8_t, imagehead[0], imagehead[2]);
		imagehead += 3;
	}

	GLuint textarget;
	GLuint texid = -1;
	if (imginfo->biHeight == 1)
		textarget = GL_TEXTURE_1D;
	else
		textarget = GL_TEXTURE_2D;

	glGenTextures(1, &texid);
	glBindTexture(textarget, texid);
	glTexParameterf(textarget, GL_TEXTURE_WRAP_S, wraptype);
	glTexParameterf(textarget, GL_TEXTURE_WRAP_T, wraptype);
	glTexParameterf(textarget, GL_TEXTURE_MAG_FILTER, magnify_type);
	glTexParameterf(textarget, GL_TEXTURE_MIN_FILTER, minify_type);

	//upload the image
	if (alpha)
	{
		void* alphaimagedata = malloc(4 * imginfo->biHeight * imginfo->biWidth);
		uint8_t* alphaimagehead = (uint8_t*)alphaimagedata;
		imagehead = (uint8_t*)imagedata;
		for (int i = 0; i < numpixels; i++)
		{
			//alpha channel is based on pixel brightess, like goldsrc decals
			alphaimagehead[0] = imagehead[0];
			alphaimagehead[1] = imagehead[1];
			alphaimagehead[2] = imagehead[2];
			alphaimagehead[3] = (imagehead[0] + imagehead[1] + imagehead[2]) / 3;
			alphaimagehead += 4;
			imagehead += 3;
		}
		if (minify_type == GL_LINEAR || minify_type == GL_NEAREST)
			glTexImage2D(textarget, 0, 4/*what?*/, imginfo->biWidth, imginfo->biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, alphaimagedata);
		else if (textarget == GL_TEXTURE_1D)
			gluBuild1DMipmaps(GL_TEXTURE_1D, 4, imginfo->biWidth, GL_RGBA, GL_UNSIGNED_BYTE, alphaimagedata);
		else
			gluBuild2DMipmaps(textarget, 4, imginfo->biWidth, imginfo->biHeight, GL_RGBA, GL_UNSIGNED_BYTE, alphaimagedata);

		free(alphaimagedata);
	}
	else
	{
		if (minify_type == GL_LINEAR || minify_type == GL_NEAREST)
			glTexImage2D(textarget, 0, 3/*what?x2*/, imginfo->biWidth, imginfo->biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata);
		else if (textarget == GL_TEXTURE_1D)
			gluBuild1DMipmaps(GL_TEXTURE_1D, 3, imginfo->biWidth, GL_RGB, GL_UNSIGNED_BYTE, imagedata);
		else
			gluBuild2DMipmaps(textarget, 3, imginfo->biWidth, imginfo->biHeight, GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	}


	free(imginfo);
	free(imagedata);
	return texid;
}


void ChangeSize()
{
	glViewport(0, 0, 1016, 1016);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100, 100, -100, 100, 0, 100 * 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	_set_g_windowWidth(1016);
	_set_g_windowHeight(741);
}

void SetColorFromIndex(EColor color)
{
    switch (color)
    {
    case eColor_R96_G87_B81:    glColor4f(0.96, 0.87, 0.81, g_opacity);     break;
    case eColor_R628_G734_B921: glColor4f(0.628, 0.734, 0.921, g_opacity);  break;
    case eColor_R52_G515_B52:   glColor4f(0.52, 0.515, 0.52, g_opacity);    break;
    case eColor_R2_G2_B3:		glColor4f(0.2, 0.2, 0.3, g_opacity);        break;
    case eColor_R42_G415_B42:   glColor4f(0.42, 0.415, 0.42, g_opacity);    break;
    case eColor_R428_G534_B721: glColor4f(0.428, 0.534, 0.721, g_opacity);  break;
    case eColor_white:          glColor4f(1.0, 1.0, 1.0, g_opacity);        break;
    case eColor_R25_G15_B01:    glColor4f(0.25, 0.15, 0.1, g_opacity);      break;
    case eColor_R1_G75_B7:		glColor4f(1.0, 0.75, 0.7, g_opacity);       break;
    case eColor_R1_G8_B0:		glColor4f(1.0, 0.8, 0.0, g_opacity);        break;
    case eColor_R1_G6_B0:		glColor4f(1.0, 0.6, 0.0, g_opacity);        break;
    case eColor_R1_G1_B95:		glColor4f(1.0, 1.0, 0.95, g_opacity);       break;
    case eColor_R83_G83_B83:    glColor4f(0.83, 0.83, 0.83, g_opacity);     break;
    case eColor_R75_G1_B75:     glColor4f(0.75, 1.0, 0.75, g_opacity);      break;
    case eColor_R9_G1_B1:		glColor4f(0.9, 1.0, 1.0, g_opacity);        break;
    case eColor_R1_G1_B9:		glColor4f(1.0, 1.0, 0.9, g_opacity);        break;
    case eColor_greyshade_09:	glColor4f(0.9, 0.9, 0.9, g_opacity);        break;
    case eColor_R1_G1_B2:		glColor4f(1.0, 1.0, 0.2, g_opacity);        break;
    case eColor_greyshade_06:   glColor4f(0.6, 0.6, 0.6, g_opacity);        break;
    case eColor_grey:           glColor4f(0.5, 0.5, 0.5, g_opacity);        break;
    case eColor_R6_G7_B1:		glColor4f(0.6, 0.7, 1.0, g_opacity);        break;
    case eColor_R62_G53_B57:    glColor4f(0.62, 0.53, 0.57, g_opacity);     break;
    case eColor_R42_G33_B37:    glColor4f(0.42, 0.33, 0.37, g_opacity);     break;
    case eColor_R273_G222_B191: glColor4f(0.273, 0.222, 0.191, g_opacity);  break;
    case eColor_R83_G77_B7:     glColor4f(0.83, 0.77, 0.7, g_opacity);      break;
    case eColor_R4_G45_B4:		glColor4f(0.4, 0.45, 0.4, g_opacity);       break;
    case eColor_R35_G4_B35:     glColor4f(0.35, 0.4, 0.35, g_opacity);      break;
    case eColor_R6_G5_B3:		glColor4f(0.6, 0.5, 0.3, g_opacity);        break;
    case eColor_R5_G4_B25:		glColor4f(0.5, 0.4, 0.25, g_opacity);       break;
    case eColor_R95_G8_B75:     glColor4f(0.95, 0.8, 0.75, g_opacity);      break;
    case eColor_R5_G9_B55:		glColor4f(0.5, 0.9, 0.55, g_opacity);       break;
    case eColor_R77_G73_B7:     glColor4f(0.77, 0.73, 0.7, g_opacity);      break;
    case eColor_R2_G2_B4_A2:    glColor4f(0.2, 0.2, 0.4, 0.2);              break;
    case eColor_R1_G1_B8:		glColor4f(1.0, 1.0, 0.8, g_opacity);        break;
    case eColor_R95_G85_B78:    glColor4f(0.95, 0.85, 0.78, g_opacity);     break;
    case eColor_R2_G2_B2_A3:    glColor4f(0.2, 0.2, 0.2, 0.3);              break;
    case eColor_R9_G9_B95_A3:	glColor4f(0.9, 0.9, 0.95, 0.3);             break;
    case eColor_clear:
        ShouldntBeHere("called SetColorFromIndex() with eColor_clear");
        break;
    case eColor_red:            glColor4f(1.0, 0.0, 0.0, g_opacity);        break;
    case eColor_blue:           glColor4f(0.0, 0.0, 1.0, g_opacity);        break;
    case eColor_green:          glColor4f(0.0, 1.0, 0.0, g_opacity);        break;
    case eColor_pink:           glColor4f(1.0, 0.0, 1.0, g_opacity);        break;
    case eColor_R06_G08_B10:    glColor4f(0.6, 0.8, 1.0, g_opacity);        break;
    case eColor_R33_G16_B02:    glColor4f(0.33, 0.16, 0.02, g_opacity);     break;
    case eColor_R43_G26_B12:    glColor4f(0.43, 0.26, 0.12, g_opacity);     break;
    case eColor_R53_G36_B22:    glColor4f(0.53, 0.36, 0.22, g_opacity);     break;
    case eColor_R63_G46_B32:    glColor4f(0.63, 0.46, 0.32, g_opacity);     break;
    case eColor_R23_G16_B02:    glColor4f(0.23, 0.16, 0.02, g_opacity);     break;
    case eColor_R33_G26_B12:	glColor4f(0.33, 0.26, 0.12, g_opacity);     break;
    case eColor_R43_G36_B22:    glColor4f(0.43, 0.36, 0.22, g_opacity);     break;
    case eColor_R48_G41_B27:    glColor4f(0.48, 0.41, 0.27, g_opacity);     break;
    case eColor_greyshade_05:   glColor4f(0.05, 0.05, 0.05, g_opacity);     break;
    case eColor_greyshade_10:   glColor4f(0.10, 0.10, 0.10, g_opacity);     break;
    case eColor_greyshade_15:   glColor4f(0.15, 0.15, 0.15, g_opacity);     break;
    case eColor_greyshade_20:   glColor4f(0.2, 0.2, 0.2, g_opacity);        break;
    case eColor_greyshade_25:   glColor4f(0.25, 0.25, 0.25, g_opacity);     break;
    case eColor_greyshade_30:   glColor4f(0.3, 0.3, 0.3, g_opacity);        break;
    case eColor_greyshade_35:   glColor4f(0.35, 0.35, 0.35, g_opacity);     break;
    case eColor_greyshade_40:   glColor4f(0.4, 0.4, 0.4, g_opacity);        break;
    case eColor_greyshade_45:   glColor4f(0.45, 0.45, 0.45, g_opacity);     break;
    default:                    glColor4f(0.0, 0.0, 0.0, g_opacity);        break;
    }
}

void DrawRedCurtain()
{
	SetColorFromIndex(eColor_white);

	glEnable(GL_TEXTURE_2D);
	DRAWTEXTUREDBOX(-100, -100, -8, 200, 150, eTextureFilmstrip_RedCurtain);
	glDisable(GL_TEXTURE_2D);
}

void DrawExtraEnvironmentStuff()
{
	if (!g_bShouldRenderThisFrame || g_numFramesRun <= 1)
		return;

	glLoadIdentity();
	float playerz1 = (-g_PlayerSprite->GetPosition().m_z / 20) + 100;

	SetColorFromIndex(eColor_white);
	textureentry_t* skytexture = &g_TextureFilmstrips[eTextureFilmstrip_NYC_Sky];
	if (skytexture->isalpha)
	{
		glDisable(GL_BLEND);
		glAlphaFunc(GL_NOTEQUAL, 1.0);
		glEnable(GL_ALPHA_TEST);
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, skytexture->entries[0].texid);
	glBegin(GL_QUADS);
	for (int i = 0; i < 2; ++i)
	{
		float v9, v8, v7, v14;
		float v16 = NormalizeNeg180to180(g_PlayerSprite->GetRotation().m_y);
		float v15 = (g_PlayerSprite->GetPosition().m_z + 500.0) / 50.0;

		v9 = v15 >= 4 ? 4 : v15;
		v8 = v9 <= 2 ? 2 : v9;


		v16 *= v8;
		v7 = i != 0 ? 1 : -1;
		v14 = -playerz1 * (v16 / 180.f + v7);

		glTexCoord2f(0, 0);
		glVertex3f(playerz1 + v14, -playerz1, -1300);

		glTexCoord2f(1, 0);
		glVertex3f(v14 - playerz1, -playerz1, -1300);

		glTexCoord2f(1, 1);
		glVertex3f(v14 - playerz1, playerz1, -1300);

		glTexCoord2f(0, 1);
		glVertex3f(playerz1 + v14, playerz1, -1300);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	if (skytexture->isalpha)
	{
		glDisable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
	}
}
static void InstructionsScreenMessage() {
	if (g_mouseLButtonClicked)
		_set_g_baseScreenMessage(eBaseScreenMessage_none);
	_set_g_bPlayerInputActive(false);

	DrawArbitraryScreenText("Type to speak to the characters at any time",	-32, -15, 350, 7, 0, eColor_white, eColor_black);
	DrawArbitraryScreenText("Move around with arrow keys",					-60, -67, 350, 7, 0, eColor_white, eColor_black);
	DrawArbitraryScreenText("Pick up and use objects with the mouse",		 15, -85, 350, 7, 0, eColor_white, eColor_black);

	glEnable(GL_TEXTURE_2D);
	SetColorFromIndex(eColor_white);

	DRAWTEXTUREDBOX(-35, -8, -4,	68, 35, eTextureFilmstrip_Instruct_Keyboard);
	DRAWTEXTUREDBOX(-71, -60, -4,	36, 35, eTextureFilmstrip_Instruct_ArrowKeys);
	DRAWTEXTUREDBOX(12, -77, -4,	54, 38, eTextureFilmstrip_Instruct_Mouse);

	glDisable(GL_TEXTURE_2D);
}
static void HintsScreenMessage() {

	if (g_bQuittingApp)
		return;
	float verticaltextcursor = 35;

	DRAW_MSG("Grace and Trip are in a desperate situation - they need you.", verticaltextcursor, 6);
	DRAW_MSG("You can influence them in several ways - exactly how is up to you.", verticaltextcursor, 6 * 2);
	DRAW_MSG("One approach is to try to help them, to get them to work through their problems.", verticaltextcursor, 6);
	DRAW_MSG("Or you could try to provoke and upset them, even be cruel if you want.", verticaltextcursor, 6);
	DRAW_MSG("You can try to dig under the surface, to expose the secrets they're hiding.", verticaltextcursor, 6 * 2);
	DRAW_MSG("Sometimes it's necessary to talk fast and interrupt Grace and Trip, to get a word in.", verticaltextcursor, 6);
	DRAW_MSG("They tend to respond to agreement and disagreement, praise, criticism,", verticaltextcursor, 6);
	DRAW_MSG("flirtation, references to relationship topics such as divorce and sex,", verticaltextcursor, 6);
	DRAW_MSG("questions about their life, explanations and advice, and so on.", verticaltextcursor, 6 * 2);
	DRAW_MSG("As the tension builds, they may try to force you to take sides.", verticaltextcursor, 6);
	DRAW_MSG("Eventually they may seek your advice, to get you to tell them what's wrong with them.", verticaltextcursor, 6);
	DRAW_MSG("Depending on what you say or do will determine if they stay together or not!", verticaltextcursor, 6 * 2);
	DRAW_MSG("Façade needs to be replayed several times, to experience the variation in the drama.", verticaltextcursor, 6 * 2);
	DRAW_MSG("For more detailed hints, and a complete description of what's ", verticaltextcursor, 6 * 2);
	DRAW_MSG("really going on behind the Façade, visit our help page at", verticaltextcursor, 0);

	bool cursoronlink = ISCURSORINBOUNDS(-25, 31, verticaltextcursor-8, verticaltextcursor);
	if (cursoronlink && g_mouseLButtonClicked)
	{
		ShellExecute(g_hMainAppWindow, "open", "http://www.interactivestory.net/help", 0, 0, 1);
		_set_g_quitAppCountdown(1);
	}
	DrawArbitraryScreenText(
		"interactivestory.net/help.",
		45,
		verticaltextcursor,
		200,
		5,
		0,
		cursoronlink ? eColor_R1_G1_B9 : eColor_R06_G08_B10, eColor_black);
	verticaltextcursor -= 12;

	bool cursoronbacktomenu = ISCURSORINBOUNDS(-20, 26, verticaltextcursor-8, verticaltextcursor);

	if (cursoronbacktomenu && g_mouseLButtonClicked)	_set_g_baseScreenMessage(eBaseScreenMessage_none);
	DrawArbitraryScreenText("Back to Menu", 
		8.0, verticaltextcursor, 
		200, 
		7, 
		0, 
		cursoronbacktomenu ? eColor_R1_G1_B9 : eColor_R06_G08_B10, eColor_black);
}
static void CreditsScreenMessage() {

	static int whenClickedOnLogo = 0;

	bool cursoronlink = ISCURSORINBOUNDS(72, 93, 13, 31);

	if (g_mouseLButtonClicked)
	{
		if (g_bAtEndOfDrama && cursoronlink && (!whenClickedOnLogo || g_numFramesRun - whenClickedOnLogo > 150))
		{
			whenClickedOnLogo = g_numFramesRun;
			ShellExecute(g_hMainAppWindow, "open", "http://www.auto-mata.org", NULL, NULL, TRUE);
		}
		_set_g_baseScreenMessage(g_bAtEndOfDrama ? eBaseScreenMessage_About : eBaseScreenMessage_none);
	}
	if (!g_bAtEndOfDrama)
		CutToBlack();

	_set_g_bPlayerInputActive(false);
	float verticaltextcursor = 30;
	DRAW_MSG("Façade", verticaltextcursor, 6);
	DRAW_MSG("a one-act interactive drama", verticaltextcursor, 6);
	DRAW_MSG("(c)opyright 2005-2006 Procedural Arts, www.interactivestory.net", verticaltextcursor, 6);
	DRAW_MSG(" ", verticaltextcursor, 6);
	DRAW_MSG("Design, engineering, writing, animation and production:  Michael Mateas and Andrew Stern", verticaltextcursor, 6);
	DRAW_MSG(" ", verticaltextcursor, 6);
	DRAW_MSG("Voice acting:  Chloe Johnston, Andy Bayiates", verticaltextcursor, 6);
	DRAW_MSG("Music:  Billy Gomberg, Matt Ganucheau, Jim Doran, Aaron Acosta, Leo Caruso", verticaltextcursor, 6);
	DRAW_MSG("Additional programming:  Mehmet Fidanboylu", verticaltextcursor, 6);
	DRAW_MSG("Additional character animation and art:  John Rines, Andy Webster", verticaltextcursor, 6);
	DRAW_MSG("Additional voice:  Mary McCann, Joel Bresler", verticaltextcursor, 6);
	DRAW_MSG("Story consultant:  John Lavin", verticaltextcursor, 6);
	DRAW_MSG(" ", verticaltextcursor, 6);
	DRAW_MSG("Very special thanks:  Tania Vu and Anne Siegel", verticaltextcursor, 6);
	DRAW_MSG(" ", verticaltextcursor, 6);
	DRAW_MSG("Special thanks:  Paul Fowler, Joshua Jones, Bethany Benzur, Robin Hunicke, Rob Zubek,", verticaltextcursor, 6);
	DRAW_MSG("  Andre Pluess, Adam Goldstein, Diana Slickman, Adam Chapman, Christine Egan, Mike Mare", verticaltextcursor, 6);
	DRAW_MSG(" ", verticaltextcursor, 6);
	DRAW_MSG("Additional Thanks:  Brenda Harger, Mary Poole, Kathy Buys, the Neo-Futurists, NeHe Productions, Marco Pinter,", verticaltextcursor, 5);
	DRAW_MSG("  Greg Stern, Guy Whitmore, William Huber, Noah Wardrip-Fruin, Nick Montfort, Scott Rettberg,", verticaltextcursor, 5);
	DRAW_MSG("  Jaime Carbonell, Jessica Hodgins, Ian Horswill, Marie-Laure Ryan", verticaltextcursor, 5);
	
#define CREDITS_DRAW_MSG(text) \
	DrawArbitraryScreenText(text, -58, verticaltextcursor, 400, 4, 0, eColor_white, eColor_black); \
	verticaltextcursor -= 5
	
	CREDITS_DRAW_MSG("Additional Thanks:  Brenda Harger, Mary Poole, Kathy Buys, the Neo-Futurists, NeHe Productions, Marco Pinter,");
	CREDITS_DRAW_MSG("  Greg Stern, Guy Whitmore, William Huber, Noah Wardrip-Fruin, Nick Montfort, Scott Rettberg,");
	CREDITS_DRAW_MSG("  Jaime Carbonell, Jessica Hodgins, Ian Horswill, Marie-Laure Ryan");
	DrawArbitraryScreenText("v1.1", 105.0, verticaltextcursor + 5, 350, 4.0, 0.0, eColor_grey, eColor_black);
	CREDITS_DRAW_MSG("Inspiration:  Brenda Laurel, Joseph Bates, Bryan Loyall, Peter Weyrauch, Scott Reilly, Chris Crawford, Janet Murray");

#undef CREDITS_DRAW_MSG
}
static void EndGameMenuScreenMessage() {
	//undone
}
static void ReplayScreenMessage() {
	_set_g_bPlayerInputActive(false);

	float verticaltextcursor = 10;
	DrawArbitraryScreenText("Façade is designed to be played many times,", 
		-22.0, verticaltextcursor, 
		200, 
		6.0, 
		0.0, 
		eColor_white, eColor_black);
	verticaltextcursor -= 6;
	DrawArbitraryScreenText("in order to see the full variation in the drama.", 
		-23.5, verticaltextcursor, 
		200, 
		6.0, 
		0.0, 
		eColor_white, eColor_black);
	verticaltextcursor -= 6*2;
	DrawArbitraryScreenText("To replay Façade, please exit and re-launch Façade.", 
		-30.0, verticaltextcursor, 
		200, 
		6.0, 
		0.0, 
		eColor_white, eColor_black);


	bool cursoronexit = ISCURSORINBOUNDS(-10, 16, -38, -30);
	if (cursoronexit && g_mouseLButtonClicked)
		_set_g_baseScreenMessage(eBaseScreenMessage_TipJar);
	DrawArbitraryScreenText("Exit", 19, -30, 200, 7, 0, cursoronexit ? eColor_R1_G1_B9 : eColor_R06_G08_B10, eColor_black);

	bool cursoronbacktomenu = ISCURSORINBOUNDS(-20, 26, -53, -45);
	if (cursoronbacktomenu && g_mouseLButtonClicked)
		_set_g_baseScreenMessage(eBaseScreenMessage_EndGameMenu);
	DrawArbitraryScreenText("Back to Menu", 8.0, -45, 200, 7, 0, cursoronbacktomenu ? eColor_R1_G1_B9 : eColor_R06_G08_B10, eColor_black);
}
static void TipJarScreenMessage() {
	_set_g_bPlayerInputActive(false);
	int verticaltextcursor = 30;
	DrawArbitraryScreenText("Façade is freeware - please give copies to your friends!",
		-34, verticaltextcursor,
		300,
		6,
		0,
		eColor_white, eColor_black);
	verticaltextcursor -= 6*3;
	DrawArbitraryScreenText("If you enjoyed Façade,", 
		1, verticaltextcursor,
		300, 
		6, 
		0, 
		eColor_white, eColor_black);
	verticaltextcursor -= 6;
	DrawArbitraryScreenText("how about dropping a dollar in our Electronic Tip Jar?", 
		-30, verticaltextcursor,
		300, 
		6, 
		0, 
		eColor_white, eColor_black);


	//unfinished
}
static void LaunchingTipJarScreenMessage() {
	if (g_bQuittingApp)
		return;

	static int ctr = 0;

	_set_g_bPlayerInputActive(false);
	DrawArbitraryScreenText("Opening the Façade electronic tip jar in your browser...", -33, 7, 300, 6, 0, eColor_white, eColor_black);
	DrawArbitraryScreenText("Thanks for your support!", -1, -47, 300, 6, 0, eColor_white, eColor_black);

	if (ctr == 1)
		ShellExecute(g_hMainAppWindow, "open", "http://www.interactivestory.net/tipjar", NULL, NULL, TRUE);
	if (++ctr >= 90)
		_set_g_quitAppCountdown(1);

	SetColorFromIndex(eColor_white);

	glEnable(GL_TEXTURE_2D);
	DRAWTEXTUREDBOX(-20, -35, -4,	40, 35, eTextureFilmstrip_TipJar);
	glDisable(GL_TEXTURE_2D);
}
static void AboutScreenMessage() {
	//undone
}
static void PlayerChoosesName() {
	static int choice = 0;
	static int blinkCountdown = 40;
	static int delayTillNextScroll = 0;

	_set_g_bPlayerInputActive(false);

	if (g_curTextCharInput == '\r')
	{
		_set_g_playerNameID(choice);
		_set_g_playerGender(strcmp("m", g_PlayerNameStrings[choice].gender) != 0);
		strcpy(g_PlayerSprite->m_name, g_PlayerNameStrings[choice].name);
		_set_g_bPlayerChoosingName(false);
		return;
	}
	DrawArbitraryScreenText("Please Choose a Name",			-15,  10,  75, 10, 0, eColor_white, eColor_black);
	DrawArbitraryScreenText("type the first letter",		 -2,  -2, 200,  7, 0, eColor_white, eColor_black);
	DrawArbitraryScreenText("scroll with the arrow keys",	 -8,  -8, 200,  7, 0, eColor_white, eColor_black);
	DrawArbitraryScreenText("press enter to begin",			 -1, -14, 200,  7, 0, eColor_white, eColor_black);

	bool cursorininstructionsbutton = ISCURSORINBOUNDS(-12, 18, -68, -60);
	if (cursorininstructionsbutton && g_mouseLButtonClicked) 
		_set_g_baseScreenMessage(eBaseScreenMessage_Instructions);
	DrawArbitraryScreenText("Instructions", 10.5, -60, 200, 7, 0, cursorininstructionsbutton ? eColor_R1_G1_B9 : eColor_R06_G08_B10, eColor_black);
	
	bool cursorinhintsbutton = ISCURSORINBOUNDS(-12, 18, -78, -70);
	if (cursorinhintsbutton && g_mouseLButtonClicked) 
		_set_g_baseScreenMessage(eBaseScreenMessage_Hints);
	DrawArbitraryScreenText("Hints", 18.5, -70, 200, 7, 0, cursorinhintsbutton ? eColor_R1_G1_B9 : eColor_R06_G08_B10, eColor_black);

	bool cursorinsubtitlesbutton = ISCURSORINBOUNDS(-12, 18, -88, -80);
	if (cursorinsubtitlesbutton && g_mouseLButtonClicked)
		_set_g_bDisplayDialogText(!g_bDisplayDialogText);

	const char* subtitlesbutton = g_bDisplayDialogText ? "Subtitles: On" : "Subtitles: Off";
	float subtitlesbuttonx = g_bDisplayDialogText ? 9 : 8;
	DrawArbitraryScreenText(subtitlesbutton, subtitlesbuttonx, -80, 200, 7, 0, cursorinsubtitlesbutton ? eColor_R1_G1_B9 : eColor_R06_G08_B10, eColor_black);


	bool cursorinaboutbutton = ISCURSORINBOUNDS(-12, 18, -98, -90);
	if (cursorinaboutbutton && g_mouseLButtonClicked)
		_set_g_baseScreenMessage(eBaseScreenMessage_Credits);
	DrawArbitraryScreenText("About", 17.5, -90, 200, 7, 0, cursorinhintsbutton ? eColor_R1_G1_B9 : eColor_R06_G08_B10, eColor_black);

	if (g_curTextCharInput >= 'a' && g_curTextCharInput <= 'z')
		_set_g_curTextCharInput(g_curTextCharInput - ' '); //convert to lowercase
	if (g_curTextCharInput >= 'A' && g_curTextCharInput <= 'Z')
	{
		choice = 0;
		for (int i = 0; i < NUM_PLAYERNAMES; i++)
		{
			if (g_curTextCharInput > *g_PlayerNameStrings[choice].name)
				break;
			choice++;
		}

		if (choice >= NUM_PLAYERNAMES)	choice = NUM_PLAYERNAMES-1;

		blinkCountdown = 60;
	}

	bool choosegoleft = g_bPlayerMoveForwards || g_bPlayerRotateLeft;
	bool choosegoright = g_bPlayerMoveBackwards || g_bPlayerRotateRight;

	_set_g_bPlayerMoveForwards(false);
	_set_g_bPlayerMoveBackwards(false);
	_set_g_bPlayerRotateLeft(false);
	_set_g_bPlayerRotateRight(false);

	if (delayTillNextScroll > 0)
	{
		--delayTillNextScroll;
	}
	else
	{
		if (choosegoleft && choice > 0)						--choice;
		if (choosegoright && choice < NUM_PLAYERNAMES-1)	++choice;

		if (choosegoleft || choosegoright)
		{
			blinkCountdown = 60;
			delayTillNextScroll = 4;
		}
	}

	if (blinkCountdown > 10)
	{
		bool v37 = true;

		char textbuffer[100];
		animengine_assert(choice <= NUM_PLAYERNAMES - 1);
		strcpy(textbuffer, g_PlayerNameStrings[choice].name);
		if (choice < NUM_PLAYERNAMES - 1
			&& !strcmp(
				g_PlayerNameStrings[choice].name,
				g_PlayerNameStrings[choice + 1].name))
		{
			strcat(textbuffer, " (female)");
		}
		else if (choice > 0
			&& !strcmp(
				g_PlayerNameStrings[choice].name,
				g_PlayerNameStrings[choice - 1].name))
		{
			strcat(textbuffer, " (male)");
		}
		float textx = 20.0 - strlen(textbuffer) * 1.4;
		DrawArbitraryScreenText(textbuffer, textx, -32, 200, 10, 0, v37 ? eColor_R06_G08_B10 : eColor_grey, eColor_black);
	}

	if (--blinkCountdown <= 0)
		blinkCountdown = 40;
	glLoadIdentity();
	glTranslatef(-50.0, -50.0, -4.0);
	SetColorFromIndex(eColor_greyshade_20);
		glBegin(GL_QUADS);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(100, 0.0, 0.0);
			glVertex3f(100, 75, 0.0);
			glVertex3f(0.0, 75, 0.0);
		glEnd();
	glLoadIdentity();

}

void DrawScreenMessage()
{
	static int fadeCurtainInCtr = 0;

	if (!g_bAtEndOfDrama)
		goto LABEL_6;

	DrawRedCurtain();
	if (!fadeCurtainInCtr)
	{
		CutToBlack();
		StartFade(0.015);
	}

	if (fadeCurtainInCtr < 60)
	{
		++fadeCurtainInCtr;
	}
	else
	{
	LABEL_6:
		switch (g_baseScreenMessage)
		{
		case eBaseScreenMessage_Instructions:	InstructionsScreenMessage();	break;
		case eBaseScreenMessage_Hints:			HintsScreenMessage();			break;
		case eBaseScreenMessage_Credits:		CreditsScreenMessage();			break;
		case eBaseScreenMessage_EndGameMenu:	EndGameMenuScreenMessage();		break;
		case eBaseScreenMessage_Replay:			ReplayScreenMessage();			break;
		case eBaseScreenMessage_TipJar:			TipJarScreenMessage();			break;
		case eBaseScreenMessage_LaunchingTipJar:LaunchingTipJarScreenMessage();	break;
		case eBaseScreenMessage_About:			AboutScreenMessage();			break;
		default:
			if (g_bPlayerChoosingName)
			{
				CutToBlack();
				PlayerChoosesName();
			}
			break;
		}
	}
}

static XYZPointf GetCursorGrabPosition(XYZPointf pos, Sprite* spr, bool option) {
	if (option && !spr->m_bGettingGrabbed && !spr->m_bGettingDropped)
	{
		pos.m_y += spr->a1_unknown00007;
	}
	else
	{
		animengine_assert(spr->m_GrabDropAnimFactor >= 0.0 && spr->m_GrabDropAnimFactor <= MATH_PI);

		//salsatobias : it doesnt pass even if grabdropanimfactor is equal to mathpi :( damn floating points
		// (or very likely im doing something wrong)
		if (option && spr->m_bGettingGrabbed && spr->m_GrabDropAnimFactor >= MATH_PI)
		{
			spr->m_bGettingGrabbed = false;
			pos.m_y += spr->a1_unknown00007;
		}
		else if (option && (!spr->m_bGettingDropped || spr->m_GrabDropAnimFactor > 0.0))
		{
			if (spr->m_bGettingGrabbed)
			{
				spr->m_GrabDropAnimFactor += MATH_PI / 15;
				if (spr->m_GrabDropAnimFactor > MATH_PI)
					spr->m_GrabDropAnimFactor = MATH_PI + 0.1; //salsatobias: 0.1 was not originally here, but i kinda needed to put it here. look at comment above
			}
			else if (spr->m_bGettingDropped)
			{
				spr->m_GrabDropAnimFactor -= MATH_PI / 15;
				if (spr->m_GrabDropAnimFactor < 0)
					spr->m_GrabDropAnimFactor = 0;
			}
			float v5 = 1.0 - (cos(spr->m_GrabDropAnimFactor) + 1.0) / 2.0;
			animengine_assert(v5 >= 0.0 && v5 <= 1.0);

			float v6 = sin(spr->m_GrabDropAnimFactor);
			animengine_assert(v6 >= 0.0 && v6 <= 1.0);

			pos.m_y += v5 * spr->a1_unknown00007;
			XYZPointf target;
			target.m_x = (pos.m_x - spr->m_targetPos.m_x) * v5 + spr->m_targetPos.m_x;
			target.m_y = (pos.m_y - spr->m_targetPos.m_y) * v5 + spr->m_targetPos.m_y + v6 * 8.0;
			target.m_z = (pos.m_z - spr->m_targetPos.m_z) * v5 + spr->m_targetPos.m_z;
			
			if (spr->a1_unknown00006 > 0.0)
				spr->a1_unknown00003 = v5 * spr->a1_unknown00006 + 1.0;

			if (spr->m_eID == eSpriteID_eightball || spr->m_eID == eSpriteID_phone)
			{
				if (v5 < 0.5 && spr->m_iSpriteFrame != 0)
				{
					spr->SetTextureFrameAnimation(0, 0, 1, false);
					if (spr->m_eID == eSpriteID_phone)
						spr->a1_unknown00007 = 0.0;
				}
				else if (v5 > 0.5 && spr->m_iSpriteFrame != 1)
				{
					spr->SetTextureFrameAnimation(1, 1, 1, false);
					if (spr->m_eID == eSpriteID_phone)
						spr->a1_unknown00007 = 5.0;
				}
			}
			pos = target;
		}
		else
		{
			spr->m_bGettingDropped = false;
			g_CursorSprite->DropSprite(spr);
			spr->SetPosition(spr->m_targetPos);
			spr->b1_unknown20321->AddSpriteToSurface(spr->b1_unknown20322, spr);
			pos = spr->m_targetPos;
		}
	}

	return pos;
}


static bool PrepareRenderPoints(Sprite* spr, CharacterSprite* character, HeadSprite* head,
	bool option,
	bool& optionout, XYZPointf& spritesubjpos, float& f1out,
	int* renderdata)
{
	XYZPointf spritepos = spr->GetPosition();


	if (!spr->a1_unknown00010d || head || (character && character->AreWeInAnEditor(true)))
	{
		spr->b1_unknown20315 = NormalizeNeg180to180(0) * 0.5;
	}
	else
	{
		XYZPointf playerrot = g_PlayerSprite->GetRotation();
		XYZPointf playerpos = g_PlayerSprite->GetPosition();
		XYZPointf spritepos = spr->GetPosition();

		float factor = playerrot.m_y - GetXZAngleBetweenPoints(playerpos, spritepos);
		spr->b1_unknown20315 = NormalizeNeg180to180(factor) * 0.5;
	}

	if (character /*&& !LOBYTE(character->unknown039[10423])*/)
	{
		//undone
	}

	if (head)
	{
		//undone
	}

	if (spr->m_bSpriteFollowsCamera)
	{
		XYZPointf spriterot = spr->GetRotation();
		spriterot.m_y = g_PlayerSprite->GetRotation().m_y + 180.0;
		if (option)
		{
			spriterot.m_x = spriterot.m_z = 0.0;
			spriterot.m_y = 180;
		}
		spr->SetRotation(spriterot);
	}

	animengine_assert(!spr->a1_unknown00010d || !option);

	bool cursor_holding_us = spr->GetSpriteHoldingMe() == g_CursorSprite;
	if (cursor_holding_us || spr->GetAmIStillGrabDropAnimating())
	{
		XYZPointf objectivepos = GetObjectivePosition(g_CursorSprite->GetPosition(), spr->GetScale());
		
		spritepos = GetCursorGrabPosition(objectivepos, spr, cursor_holding_us);

		spr->SetPosition(spritepos);

		XYZPointf playerrot = g_PlayerSprite->GetRotation();
		playerrot.m_y += 180.0;
		spr->SetRotation(playerrot);
	}

	float f1;
	GetSubjectivePosition(spr->GetPosition(), spritesubjpos, f1, true);
	f1out = spritesubjpos.m_z;

	if (head)
	{
		//undone
	}

	if (spr->a1_unknown00010d)
	{
		spr->SetScale(f1 * 0.625);
	}
	else
	{
		spr->SetScale(f1);
	}


	if (spr->GetScale() <= 0 && !spr->a1_unknown00011a)
	{
		spr->boolean1 = false;
		return false;
	}

	spr->boolean1 = true;

	XYZPointf sprvolume = spr->GetVolume();
	float v1 = f1 * 1.2 / 2.0;
	XYZPointf attachpos = spritesubjpos;
	if (character)
		attachpos.m_y = 70.0 * f1 + attachpos.m_y;
	if (option)
		attachpos = spr->GetAttachPosition();
	if (spr->m_eObjectType == eObjectType_Door && ((DoorSprite*)spr)->IsDoorOpen())
	{
		XYZPointf point = spritepos;
		point.m_x -= 30;
		point.m_z -= 30;
		GetSubjectivePosition(point, attachpos, f1, true);
	}
	float ymax = sprvolume.m_y * v1 + attachpos.m_y;
	float xmax = sprvolume.m_x * v1 + attachpos.m_x;
	float ymin = attachpos.m_y - sprvolume.m_y * v1;
	float xmin = attachpos.m_x - sprvolume.m_x * v1;
	spr->Set2DBoundingBox(xmin, ymin, xmax, ymax, attachpos.m_z);

	if (head)
	{
		//undone
	}
	else if (spr->a1_unknown00010d)
	{
		for (int i = 0; i < spr->m_iNumModelPoints; ++i)
		{
			XYZPointf *modelpoint = &spr->m_modelframe[i];
			//for (int j = 0; j < 2; ++j)
			//{
			//	if (character && *(&character->unknown009 + 4 * j) == i)
			//	{
			//		v124->p.m_x = v124->p.m_x + *((float*)&character->data.unknown010 + 4 * j);
			//		v124->p.m_y = v124->p.m_y + *((float*)&character->data.unknown011 + 4 * j);
			//		v124->p.m_z = v124->p.m_z + *((float*)&character->data.unknown012 + 4 * j);
			//	}
			//}

			spr->b1_unknown00020[i] = *modelpoint;
		}
	}
	else
	{
		RotateModel(spr, -spr->GetRotation(), spr->b1_unknown00020);
	}

	animengine_assert(spr->m_iNumModelPoints < MDL_MAXPOINTS);

	if (spr->a1_unknown00010d)
	{
		XYZPointf rot = spr->GetRotation();
		rot.m_y = Normalize0to360(rot.m_y + spr->b1_unknown20315);

		if (character)
		{
			//undone
		}
		else
		{
			Do2DPosterRotateAndProject(spr, 
				spr->b1_unknown00020,
				spritesubjpos,
				rot,
				spr->a1_unknown00002,
				e2DPosterRotateType_normal);
		}
	}
	else
	{
		for (int i = 0; i < spr->m_iNumModelPoints; ++i)
		{
			if (option)
			{
				spr->m_renderPoints1[i] = spr->GetAttachPosition() + (spr->b1_unknown00020[i] * spr->GetScale());

				spr->m_renderPoints1[i].m_z = spr->GetAttachPosition().m_z;
				f1out = spr->GetAttachPosition().m_z;
			}
			else
			{
				XYZPointf pos = spr->b1_unknown00020[i] + spritepos;
				GetSubjectivePosition(pos, spr->m_renderPoints1[i], f1, !(spr->a1_unknown00010 && !option));
				if (cursor_holding_us && !spr->GetAmIStillGrabDropAnimating())
					spr->m_renderPoints1[i].m_z = -8.0;
				if (spr->a1_unknown00004 != 0.0)
					spr->m_renderPoints1[i].m_z = spr->a1_unknown00004;
			}
		}

		if (spr->a1_unknown00010 && !option)
		{
			float offset = 0.0;
			XYZPointf panelpoints[4];
			animengine_assert(spr->m_iNumModelPoints == 5);
			for (int i = 1; i < spr->m_iNumModelPoints; ++i)
			{
				panelpoints[i - 1] = spr->m_renderPoints1[i];
			}
			for (int i = 0; ; ++i)
			{
				if (i >= 4)
				{
					for (int j = 0; j < spr->m_iNumModelPoints; ++j)
					{
						float f1 = -spr->m_renderPoints1[j].m_z;
						if (f1 < 0.1) f1 = 0.1;
						spr->m_renderPoints1[j].m_x *= (100.0 / f1) * 1.25;
						spr->m_renderPoints1[j].m_y *= (100.0 / f1) * 1.25;
					}
					break;
				}
				if (panelpoints[i].m_z > offset)
				{
					XYZPointf point2;
					XYZPointf point1 = panelpoints[i];

					int i1 = i - 1;
					int i2 = i + 1;

					if (i - 1 < 0)	i1 = 3;
					if (i + 1 > 3)	i2 = 0;

					if (panelpoints[i1].m_z >= offset)
					{
						if (panelpoints[i2].m_z >= offset)
							continue;
						point2 = panelpoints[i2];
					}
					else
					{
						point2 = panelpoints[i1];
					}
					animengine_assert(point1.m_y == point2.m_y);

					float v1;

					if (point1.m_x - point2.m_x < 0.0)
						v1 = -(point1.m_x - point2.m_x);
					else
						v1 = point1.m_x - point2.m_x;

					if (v1 > 0.1)
					{
						float v2 = (point1.m_z - point2.m_z) / (point1.m_x - point2.m_x);
						float v3 = point1.m_z - v2 * point1.m_x;
						point1.m_x = (offset - v3) / v2;
					}
					point1.m_z = offset;
					spr->m_renderPoints1[i + 1] = point1;
				}
			}
		}

	}

	spr->UpdateSurfaces(spr->GetPosition(), spr->b1_unknown00020, spr->m_renderPoints1);

	if (spr->a1_unknown00001)
	{
		//sub_10001226((int)sprite->data.m_renderPoints1, renderdata, sprite->data.m_iNumModelPoints);
	}
	else
	{
		for (int i = 0; i < spr->m_iNumModelPoints; ++i)
			renderdata[i] = i;
	}


	return true;
}

static int GetFrameTexCoords(ETextureFilmstrip filmstrip, int spriteframe,
	float& u_min, float& v_min,
	float& u_max, float& v_max)
{
	textureentry_t* texture = &g_TextureFilmstrips[filmstrip];

	float v12 = 0;
	float v11 = 0;
	float v10 = texture->width2;
	float v9 = texture->height2;
	int textureframeindex = 0;
	for (int i = 0; i != spriteframe; ++i)
	{
		v12 += texture->width2;
		v10 += texture->width2;
		if (v10 > texture->width1)
		{
			v12 = 0;
			v10 = texture->width2;
			v11 += texture->height2;
			v9 += texture->height2;
			if (v9 > texture->height1)
			{
				++textureframeindex;
				v11 = 0;
				v9 = texture->height2;
			}
		}
	}
	u_min = v12 / texture->width1;
	v_min = 1.0 - v9 / texture->height1;
	u_max = v10 / texture->width1;
	v_max = 1.0 - v11 / texture->height1;

	if (textureframeindex >= 20)
	{
		dprintf("illegal texture bitmap %d for frame %d of filmstrip %d", textureframeindex, spriteframe, filmstrip);
		animengine_assert(0);
		textureframeindex = 0;
	}
	animengine_assert(textureframeindex < texture->numentries);
	return textureframeindex;
}

static EColor GetModelCircleColor(Sprite* a1, int a2)
{
	return (EColor)FloatToInt(a1->m_modelinfo[a2].color);
}

static void DrawLinesFromModelPoint(Sprite* spr, int pointindex, int* pointindices, float z, float f2)
{
	if (g_numFramesRun <= 1)
		return;

	if (!spr->m_modelquadlines)
		return;

	int i = 0;
	bool option1 = false;

	XYZPointf points[4];
	XYZPointf* renderpoints = spr->m_renderPoints1;

	//while (true)
	//{
	//	while (true)
	//	{
	//		if (spr->m_modelquadlines[i].unknown001 == 666)
	//			return;
	//
	//		if (spr->m_modelquadlines[i].unknown001 == pointindex 
	//			|| spr->m_modelquadlines[i].unknown002 == pointindex)
	//			break;
	//
	//		int index1 = pointindex;
	//		int index2 = spr->m_modelquadlines[i].unknown003;
	//		int index3 = -1;
	//
	//	LABEL_29:
	//		if (i != 0)
	//		{
	//			animengine_assert(v61 >= 0);
	//			if (index1 > index3)
	//			{
	//				VAL_SWAP(int, index3, index1);
	//			}
	//
	//			float zval = option1 ? 0.05 : -0.05;
	//			zval += z;
	//
	//			if (unknown004 > 1 &&
	//				unknown004 == 2 || unknown004 == 3 &&
	//				spr->m_modelquadlines[i].color2 != eColor_clear)
	//			{
	//				SetColorFromIndex(spr->m_modelquadlines[i].color2);
	//				float width = spr->m_modelquadlines[i].linewidth;
	//				if (width > 0)
	//					width = get_gl_linewidth(width);
	//
	//				glLineWidth(width);
	//				if (index2 < 0)
	//				{
	//					glBegin(GL_LINES);
	//					glVertex3f(renderpoints[index1].m_x, renderpoints[index1].m_y, zval);
	//					glVertex3f(renderpoints[index3].m_x, renderpoints[index3].m_y, zval);
	//					glEnd();
	//				}
	//				else
	//				{
	//					points[0] = renderpoints[index1];
	//					points[1] = renderpoints[index2];
	//					points[2] = renderpoints[index3];
	//
	//					points[0].m_z = 
	//						points[1].m_z =
	//						points[2].m_z = zval;
	//
	//					glMap1f(GL_MAP1_VERTEX_3, 0.0, 10.0, 3, 3, &points[0].m_x);
	//					glEnable(GL_MAP1_VERTEX_3);
	//					glBegin(GL_LINE_STRIP);
	//					for (float j = 0; j <= 10.0; ++j)
	//					{
	//						glEvalCoord1f(j);
	//					}
	//					glEnd();
	//				}
	//			}
	//		}
	//
	//		i++;
	//
	//	}
	//	i++;
	//}
}


static void DrawPolysFromModelPoint(Sprite* spr, int pointindex, int* pointindices, float f1, float f2)
{
	if (g_numFramesRun <= 1)
		return;

	if (!spr->m_modelquadpoly)
		return;

	XYZPointf facepoints[4];

	int i = 0;
	EQuadPolyDraw drawtype;

	int lastinfoindex1 = 0;
	int lastinfoindex2 = 0;
	int lastinfoindex3 = 0;
	int lastinfoindex4 = 0;

	bool drawpoly = false;

	mdlquadpoly_t* poly;

	bool skipchecks = false;

	while (true)
	{
		do
		{
			if (spr->m_modelquadpoly[i].mdlinfoindex[0] == DATA_NULLVAL)
				return;

			poly = &spr->m_modelquadpoly[i];

			if(!skipchecks)
			{
				drawpoly = false;

				lastinfoindex1 = poly->mdlinfoindex[0];
				lastinfoindex2 = poly->mdlinfoindex[1];
				lastinfoindex3 = poly->mdlinfoindex[2];
				lastinfoindex4 = poly->mdlinfoindex[3];


				if (poly->mdlinfoindex[0] == pointindex
					|| poly->mdlinfoindex[1] == pointindex
					|| poly->mdlinfoindex[2] == pointindex
					|| poly->mdlinfoindex[3] == pointindex)
				{
					break;
				}
			}

			skipchecks = false;

			if (!drawpoly)
			{
				i++;
				continue;
			}
			bool shoulddraw = false;
			for (int j = 0; j < 4; j++)
			{
				XYZPointf point = spr->m_renderPoints1[spr->m_modelquadpoly[i].mdlinfoindex[j]];

				XYZPointf* facepoint = &facepoints[j];
				*facepoint = point;

				if (drawtype <= eQuadPolyDraw_flatdepth)	facepoint->m_z = f1;

				if (facepoint->m_z <= 0.0)	shoulddraw = true;
			}

			if (!shoulddraw)
			{
				i++;
				continue;
			}

			EColor facecolor = poly->facecolor;
			if (facecolor == -1)
				facecolor = GetModelCircleColor(spr, lastinfoindex1);
			if (facecolor == eColor_clear)
			{
				i++;
				continue;
			}

			SetColorFromIndex(facecolor);
			ETextureFilmstrip texture = poly->m_eFilmstrip;
			if (texture <= eTextureFilmstrip_NOTATEXTURE)
			{
				glBegin(GL_QUADS);
					glVertex3f(facepoints[0].m_x, facepoints[0].m_y, facepoints[0].m_z);
					glVertex3f(facepoints[1].m_x, facepoints[1].m_y, facepoints[1].m_z);
					glVertex3f(facepoints[2].m_x, facepoints[2].m_y, facepoints[2].m_z);
					glVertex3f(facepoints[3].m_x, facepoints[3].m_y, facepoints[3].m_z);
				glEnd();
			}
			else
			{
				textureentry_t* textureentry = &g_TextureFilmstrips[texture];
				if (textureentry->isalpha)
				{
					glDisable(GL_BLEND);
					glAlphaFunc(GL_NOTEQUAL, 1.0);
					glEnable(GL_ALPHA_TEST);
				}

				float u_min, u_max;
				float v_min, v_max;


				int filmstrip_frame = GetFrameTexCoords(texture, spr->m_iSpriteFrame, u_min, v_min, u_max, v_max);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureentry->entries[filmstrip_frame].texid);
					glBegin(GL_QUADS);
						glTexCoord2f(u_min, v_min);
						glVertex3f(facepoints[0].m_x, facepoints[0].m_y, facepoints[0].m_z);
						glTexCoord2f(u_max, v_min);
						glVertex3f(facepoints[1].m_x, facepoints[1].m_y, facepoints[1].m_z);
						glTexCoord2f(u_max, v_max);
						glVertex3f(facepoints[2].m_x, facepoints[2].m_y, facepoints[2].m_z);
						glTexCoord2f(u_min, v_max);
						glVertex3f(facepoints[3].m_x, facepoints[3].m_y, facepoints[3].m_z);
					glEnd();
				glDisable(GL_TEXTURE_2D);
				if (textureentry->isalpha)
				{
					glDisable(GL_ALPHA_TEST);
					glEnable(GL_BLEND);
				}
			}
			
			//undone
			
			i++;
		} while (true);

		if (spr->m_modelinfo[lastinfoindex1].unknown007 != 2
			&& spr->m_modelinfo[lastinfoindex2].unknown007 != 2
			&& spr->m_modelinfo[lastinfoindex3].unknown007 != 2
			&& spr->m_modelinfo[lastinfoindex4].unknown007 != 2)
		{
			skipchecks = true;
			drawtype = spr->m_modelquadpoly[i].drawtype;
			if (drawtype == eQuadPolyDraw_quad)
			{
				drawpoly = pointindex == lastinfoindex1;
			}
			else
			{
				int v33 = 0;
				for (int j = 0; j < spr->m_iNumModelPoints && pointindices[j] != pointindex; ++j)
				{
					if (pointindices[j] == lastinfoindex1
						|| pointindices[j] == lastinfoindex2
						|| pointindices[j] == lastinfoindex3
						|| pointindices[j] == lastinfoindex4)
					{
						++v33;
					}
				}
				if (drawtype == eQuadPolyDraw_never && !v33 
					|| drawtype == eQuadPolyDraw_flatdepth && v33 == 3)
					drawpoly = true;
			}
		}
		else
			i++;
	}
}

void DrawSprite(Sprite* spr, bool option)
{
	if (!g_bShouldRenderThisFrame)
		return;

	int pointsindices[MDL_MAXPOINTS];

	glLoadIdentity();
	CharacterSprite* character = nullptr;
	HeadSprite* head = nullptr;

	if (spr->GetIsCharacterSprite())
		character = (CharacterSprite*)spr;
	if (spr->GetIsHeadSprite())
		head = (HeadSprite*)spr;

	bool b1;
	XYZPointf xyz;
	float f1;


	if (!PrepareRenderPoints(spr, character, head, option, b1, xyz, f1, pointsindices))
		return;

	if (g_numFramesRun > 1 && spr->GetIsCharacterSprite())
	{
		//undone
	}

	float v82 = 0;

	for (int i = 0; i < spr->m_iNumModelPoints; i++)
	{
		int indice = pointsindices[i];
		if (spr->a1_unknown00010d)
			v82 += 0.2;
		if (head)
		{

		}
		//Sprite* sprlist[5];
		//int* unknown[5];
		//int numspritesattached = spr->GetSpriteListAttachedToModelPoint(option, sprlist, unknown);
		//for (int j = 0; j < numspritesattached; j++)
		//{
		//
		//}

		float unknown001 = spr->m_modelinfo[indice].unknown001;

		if (spr->a1_unknown00010d)
		{
			float v69 = 0.0;
			if (spr->GetIsAnimatingSprite())
			{
				//undone
			}
			if (character && indice == character->unknown005)
			{
				//undone
			}
		}

		spr->b1_unknown01430[indice] = unknown001;

		unknown001 *= spr->GetScale();

		bool v75 = false;

		XYZPointf spriterot = spr->GetRotation();
		spriterot.m_y += spr->b1_unknown20315;
		for (int j = 0; j < spr->m_iNumOutlineConnectors; ++j)
		{
			//if (spr->m_outlineconnectors[j].unknown001 == a2)
			//{
				//undone
			//}
		}
		if (spr->m_modelinfo[indice].unknown007 == 1 /*&& numspritesattached > 0*/)
			v75 = true;
		if (unknown001 > 0.0 && (!spr->m_modelinfo[indice].unknown007 || v75 || g_bAllCircleOutline))
		{
			//undone
		}

		XYZPointf playerrot = g_PlayerSprite->GetRotation();
		
		DrawLinesFromModelPoint(spr, indice, pointsindices, v82+f1, spriterot.m_y - playerrot.m_y);
		DrawPolysFromModelPoint(spr, indice, pointsindices, v82+f1, spriterot.m_y - playerrot.m_y);
		//v22 = (XYZPointf*)((int(__thiscall*)(Sprite*, _BYTE*))g_PlayerSprite->vftable->GetRotation)(
		//	g_PlayerSprite,
		//	v53);
		//zh = v76.y - v22->y;
		//*(float*)&ye = v82 + *(float*)&float1;
		//SaveLimbInfoFromModelPoint((int)sprite, a2, ye, zh);
	}

	//DrawArms(spr);

	//undone
}

void StartFade(float amnt) //amnt is 0.0-1.0
{
	_set_g_fadeIncr(-amnt);
	if (g_fadeIncr < 0)
	{
		_set_g_fadeAmount(1);
	}
	else if (g_fadeIncr == 0)
	{
		_set_g_fadeAmount(0);
	}
}
void CutToBlack()
{
	_set_g_fadeAmount(1);
	_set_g_fadeIncr(0);
}
void UpdateFade()
{
	if (g_fadeIncr != 0.0 || g_fadeAmount != 0.0)
	{
		_set_g_fadeAmount(g_fadeAmount + g_fadeIncr);
		if (g_fadeAmount <= 0)
		{
			_set_g_fadeAmount(0);
			_set_g_fadeIncr(0);
		}
		if (g_fadeAmount >= 1)
		{
			_set_g_fadeAmount(1);
			_set_g_fadeIncr(0);
		}
		glLoadIdentity();
		glTranslatef(-150, -150, -7);
		glColor4f(0, 0, 0, g_fadeAmount);
		glBegin(GL_QUADS);
			glVertex3f(0, 0, 0);
			glVertex3f(400, 0, 0);
			glVertex3f(400, 250, 0);
			glVertex3f(0, 250, 0);
		glEnd();
		glLoadIdentity();
	}
}

static char g_origTextCharInput = '\0';
extern void PlayerEnteredText();

static void ProcessRealtimeTypedPlayerScreenText()
{
	if (!g_bPlayerInputActive)
		return;

	_set_g_bNewPlayerTextEntered(false);
	g_origTextCharInput = g_curTextCharInput;
	int screentext_len = strlen(g_PlayerSprite->m_ScreenText);
	if (g_curTextCharInput == '\r')
	{
		PlayerEnteredText();
	}
	else if (g_curTextCharInput == '<' && screentext_len > 0)
	{
		g_PlayerSprite->m_ScreenText[screentext_len - 1] = '\0';
		_set_g_bTextCharWasPressed(true);
	}
	else if (screentext_len < sizeof(g_PlayerSprite->m_ScreenText) && g_curTextCharInput)
	{
		bool shouldenterchar = false;
		for (int i = 0; i < 73; ++i)
		{
			if (g_fontInfo[i].character == g_curTextCharInput || g_curTextCharInput == ' ')
				shouldenterchar = 1;
		}
		if (shouldenterchar)
		{
			g_PlayerSprite->m_ScreenText[screentext_len] = g_curTextCharInput;
			g_PlayerSprite->m_ScreenText[screentext_len + 1] = '\0';
		}
		_set_g_bTextCharWasPressed(true);
	}
	_set_g_curTextCharInput('\0');
}

static bool textcheck(const char* txt) {
	return !strcmp("q", txt) || !strcmp("g", txt) || txt[0] == ':';
}

static bool UpdateScreenText(Sprite* spr)
{
	int screentxtlen = strlen(spr->m_ScreenText);
	if (screentxtlen <= 0)
		return false;

	int diff = spr->b1_unknown20516 - spr->b1_unknown20515;
	float v7 = 0;
	if (spr->b1_unknown20516 != screentxtlen || textcheck(spr->m_ScreenText))
		v7 = 1.0 * 0.75;
	else
		v7 = 60.0 / diff;
	if (spr->b1_unknown20517 + diff * v7 < g_numFramesRun
		|| spr->b1_unknown20516 == 0)
	{
		if (spr->b1_unknown20515 == screentxtlen)
		{
			spr->m_ScreenText[0] = '\0';
			spr->m_playerTextOnScreen[0] = '\0';
			return false;
		}
		spr->b1_unknown20515 = spr->b1_unknown20516;
		for (int i = spr->b1_unknown20516; i < screentxtlen && !spr->m_ScreenText[spr->b1_unknown20516]; ++i)
			++spr->b1_unknown20516;
		strcpy(spr->m_playerTextOnScreen, spr->m_ScreenText);
		spr->m_playerTextOnScreen[spr->b1_unknown20516] = 0;
		spr->b1_unknown20517 = g_numFramesRun;
	}
	return true;
}

void DrawPlayerScreenText()
{
	if (g_bAtEndOfDrama)
		return;

	static int g_textCursorBlinkCounter = 0;

	if (g_textDisplayState != 0)
	{
		if(g_textDisplayState == 1)
		{
			_set_g_textDisplayState(2);
			return;
		}
		if (!UpdateScreenText(g_PlayerSprite))
			_set_g_textDisplayState(0);
		_set_g_curTextCharInput('\0');
	}
	else
	{
		ProcessRealtimeTypedPlayerScreenText();
		strcpy(g_PlayerSprite->m_playerTextOnScreen, g_PlayerSprite->m_playerTextInput);
	}

	//tidy this up...

	const float v35 = 512.0;
	const float v30 = 95.0;
	const float v29 = 0.125;

	float v31;
	float v32;
	float v33;
	float v34;

	for (int i = 0; i < 2; ++i)
	{
		v31 = -v30;
		v32 = -80.0;
		v33 = -5.0;
		v34 = 0;
		for (int j = 0; j < strlen(g_PlayerSprite->m_playerTextOnScreen); ++j)
		{
			if (v30 - 5.0 < v31)
			{
				g_PlayerSprite->m_playerTextOnScreen[j] = '\0';
				g_PlayerSprite->m_ScreenText[j] = '\0';
				PlayGlobalSound(eGlobalSound_keystop);
				break;
			}
			if (g_PlayerSprite->m_playerTextOnScreen[j] == ' ')
			{
				v31 = 30 * v29 + v31;
			}
			else
			{
				int fontinfo_index = -1;
				for (int k = 0; k < sizeof(g_fontInfo)/sizeof(font_charinfo_t) && fontinfo_index == -1; ++k)
				{
					if (g_PlayerSprite->m_playerTextOnScreen[j] == g_fontInfo[k].character)
						fontinfo_index = k;
				}
				if (fontinfo_index != -1)
				{
					font_charinfo_t* fontinfo = &g_fontInfo[fontinfo_index];
					ETextureFilmstrip fontstrip;
					if (g_PlayerSprite->m_playerTextOnScreen[j] < 'A' 
						|| g_PlayerSprite->m_playerTextOnScreen[j] > 'Z')
						fontstrip = eTextureFilmstrip_YellowFont1;
					else
						fontstrip = eTextureFilmstrip_YellowFont2;
					float unknown001 = fontinfo->unknown001;
					float unknown002 = fontinfo->unknown002;
					float v21 = fontinfo->unknown003 + unknown001;
					float v20 = fontinfo->unknown004 + unknown002;

					float x = v31;
					float y = v32;
					float v17 = (v21 - unknown001) * v29 + v31;
					float v16 = (v20 - unknown002) * v29 + v32;
					float s = unknown001 / v35;
					float t = 1.0 - v20 / v35;
					float v12 = v21 / v35;
					float v11 = 1.0 - unknown002 / v35;
					if (i == 1)
					{
						_set_g_opacity(1.0);
						SetColorFromIndex(eColor_white);
						glDisable(GL_BLEND);
						glAlphaFunc(GL_LESS, g_opacity);
						glEnable(GL_ALPHA_TEST);
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, g_TextureFilmstrips[fontstrip].entries[0].texid);
							glBegin(GL_QUADS);
								glTexCoord2f(s, t);
								glVertex3f(x, y, -5);
								glTexCoord2f(s, v11);
								glVertex3f(x, v16, -5);
								glTexCoord2f(v12, v11);
								glVertex3f(v17, v16, -5);
								glTexCoord2f(v12, t);
								glVertex3f(v17, y, -5);
							glEnd();
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_ALPHA_TEST);
						glEnable(GL_BLEND);
					}
					v31 = v17 - x - 0.40000001 + v31;
				}
			}

		}
		//if(i == 0 && 0)
	}
	glLoadIdentity();
	animengine_assert(true); //some macro assertion that we'll never know..
	if (g_origTextCharInput > 0 && strlen(g_PlayerSprite->m_playerTextOnScreen) > 1)
		g_textCursorBlinkCounter = 0;
	if (!strlen(g_PlayerSprite->m_playerTextOnScreen) || g_textDisplayState != 0)
		g_textCursorBlinkCounter = 25;

	if (++g_textCursorBlinkCounter < 25)
	{
		XYZPointf p1(v31 - 0.6875 + 1.25, v32 - 4.6875 + 6.75, v33);
		XYZPointf p2(v31 + 0.6875 + 1.25, v32 + 4.6875 + 6.75, v33);

		SetColorFromIndex(eColor_R1_G1_B8);
		glBegin(GL_QUADS);
			glVertex3f(p1.m_x, p1.m_y, p1.m_z - 1);
			glVertex3f(p2.m_x, p1.m_y, p1.m_z - 1);
			glVertex3f(p2.m_x, p2.m_y, p1.m_z - 1);
			glVertex3f(p1.m_x, p2.m_y, p1.m_z - 1);
		glEnd();

		glLineWidth(2);
		SetColorFromIndex(eColor_black);
		glBegin(GL_LINE_LOOP);
			glVertex3f(p1.m_x, p1.m_y, p1.m_z);
			glVertex3f(p2.m_x, p1.m_y, p1.m_z);
			glVertex3f(p2.m_x, p2.m_y, p1.m_z);
			glVertex3f(p1.m_x, p2.m_y, p1.m_z);
		glEnd();
	}
	if (g_textCursorBlinkCounter >= 35)
		g_textCursorBlinkCounter = 0;
}
void DrawNPCsScreenText()
{
	for (int i = 0; i < 4; ++i)
	{
		//undone
	}

	glLoadIdentity();
}
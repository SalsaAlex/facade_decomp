#include "facadecommon.h"
#include "gl/GLU.h"


int g_windowWidth;
int g_windowHeight;

float g_opacity;

float g_fadeIncr;
float g_fadeAmount;

GLYPHMETRICSFLOAT g_FontGlyphs[255];
GLuint g_BitmapFontBase;



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
	if (fread(imginfo, 1, dataoffset, hfile) < dataoffset)
		goto fail_deallocate_block;


	bytestoread = (*imginfo)->biSizeImage;
	if (!(*imginfo)->biSizeImage)
		bytestoread = abs((*imginfo)->biHeight) * ((*imginfo)->biBitCount * (*imginfo)->biWidth + 7) / 8;

	if ((imagedata = malloc(bytestoread)) == nullptr)
		goto fail_deallocate_block;

	if (fread(imagedata, 1, bytestoread, hfile) < bytestoread)
		goto fail_deallocate_imagedata;



	fclose(hfile);
	return imagedata;



fail_deallocate_imagedata:
	free(imagedata);
fail_deallocate_block:
	free(imginfo);
fail_closefile:
	fclose(hfile);

	return nullptr;
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



void BuildBitmapFont()
{
	int tickstart = GetTickCount();
	dprintf("Building bitmap font...");
	g_BitmapFontBase = glGenLists(256);
	HFONT h = CreateFontA(-12, 0, 0, 0, 700, 0, 0, 0, 0, 4u, 0, 4u, 0, "Comic Sans MS");
	SelectObject(g_hDeviceContext, h);
	wglUseFontOutlinesA(g_hDeviceContext, 0, 255, g_BitmapFontBase, 0.0, 0.0, 1, g_FontGlyphs);
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
	uint32_t numpixels = imginfo->biHeight * imginfo->biHeight;

	//transpose red&green (?)
	for (int i = 0; i < numpixels; i++)
	{
		uint8_t temp = imagehead[0];
		imagehead[0] = imagehead[2];
		imagehead[2] = temp;
		imagehead += 3;
	}

	GLuint textarget;
	GLuint texid;
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
	glOrtho(-100.0, 100.0, -100.0, 100.0, 0.0, 100.0 * 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	g_windowWidth = 1016;
	g_windowHeight = 741;
}

void SetColorFromIndex(EColor color)
{
    switch (color)
    {
    case eColor_unnamed000:     glColor4f(0.96, 0.87, 0.81, g_opacity);     break;
    case eColor_unnamed001:     glColor4f(0.628, 0.734, 0.921, g_opacity);  break;
    case eColor_unnamed002:     glColor4f(0.52, 0.515, 0.52, g_opacity);    break;
    case eColor_unnamed003:     glColor4f(0.2, 0.2, 0.3, g_opacity);        break;
    case eColor_unnamed004:     glColor4f(0.42, 0.415, 0.42, g_opacity);    break;
    case eColor_unnamed005:     glColor4f(0.428, 0.534, 0.721, g_opacity);  break;
    case eColor_white:          glColor4f(1.0, 1.0, 1.0, g_opacity);        break;
    case eColor_unnamed006:     glColor4f(0.25, 0.15, 0.1, g_opacity);      break;
    case eColor_unnamed007:     glColor4f(1.0, 0.75, 0.7, g_opacity);       break;
    case eColor_unnamed008:     glColor4f(1.0, 0.8, 0.0, g_opacity);        break;
    case eColor_unnamed009:     glColor4f(1.0, 0.6, 0.0, g_opacity);        break;
    case eColor_unnamed010:     glColor4f(1.0, 1.0, 0.95, g_opacity);       break;
    case eColor_unnamed011:     glColor4f(0.83, 0.83, 0.83, g_opacity);     break;
    case eColor_unnamed012:     glColor4f(0.75, 1.0, 0.75, g_opacity);      break;
    case eColor_unnamed013:     glColor4f(0.9, 1.0, 1.0, g_opacity);        break;
    case eColor_unnamed014:     glColor4f(1.0, 1.0, 0.9, g_opacity);        break;
    case eColor_unnamed015:		glColor4f(0.9, 0.9, 0.9, g_opacity);        break;
    case eColor_unnamed016:     glColor4f(1.0, 1.0, 0.2, g_opacity);        break;
    case eColor_greyshade_06:   glColor4f(0.6, 0.6, 0.6, g_opacity);        break;
    case eColor_grey:           glColor4f(0.5, 0.5, 0.5, g_opacity);        break;
    case eColor_unnamed018:     glColor4f(0.6, 0.7, 1.0, g_opacity);        break;
    case eColor_unnamed019:     glColor4f(0.62, 0.53, 0.57, g_opacity);     break;
    case eColor_unnamed020:     glColor4f(0.42, 0.33, 0.37, g_opacity);     break;
    case eColor_unnamed021:     glColor4f(0.273, 0.222, 0.191, g_opacity);  break;
    case eColor_unnamed022:     glColor4f(0.83, 0.77, 0.7, g_opacity);      break;
    case eColor_unnamed023:     glColor4f(0.4, 0.45, 0.4, g_opacity);       break;
    case eColor_unnamed024:     glColor4f(0.35, 0.4, 0.35, g_opacity);      break;
    case eColor_unnamed025:     glColor4f(0.6, 0.5, 0.3, g_opacity);        break;
    case eColor_unnamed026:     glColor4f(0.5, 0.4, 0.25, g_opacity);       break;
    case eColor_unnamed027:     glColor4f(0.95, 0.8, 0.75, g_opacity);      break;
    case eColor_unnamed028:     glColor4f(0.5, 0.9, 0.55, g_opacity);       break;
    case eColor_unnamed029:     glColor4f(0.77, 0.73, 0.7, g_opacity);      break;
    case eColor_unnamed030:     glColor4f(0.2, 0.2, 0.4, 0.2);              break;
    case eColor_unnamed031:     glColor4f(1.0, 1.0, 0.8, g_opacity);        break;
    case eColor_unnamed032:     glColor4f(0.95, 0.85, 0.78, g_opacity);     break;
    case eColor_unnamed033:     glColor4f(0.2, 0.2, 0.2, 0.3);              break;
    case eColor_unnamed034:		glColor4f(0.9, 0.9, 0.95, 0.3);             break;
    case eColor_clear:
        ShouldntBeHere("called SetColorFromIndex() with eColor_clear");
        break;
    case eColor_red:            glColor4f(1.0, 0.0, 0.0, g_opacity);        break;
    case eColor_blue:           glColor4f(0.0, 0.0, 1.0, g_opacity);        break;
    case eColor_green:          glColor4f(0.0, 1.0, 0.0, g_opacity);        break;
    case eColor_pink:           glColor4f(1.0, 0.0, 1.0, g_opacity);        break;
    case eColor_unnamed035:     glColor4f(0.6, 0.8, 1.0, g_opacity);        break;
    case eColor_unnamed036:     glColor4f(0.33, 0.16, 0.02, g_opacity);     break;
    case eColor_unnamed037:     glColor4f(0.43, 0.26, 0.12, g_opacity);     break;
    case eColor_unnamed038:     glColor4f(0.53, 0.36, 0.22, g_opacity);     break;
    case eColor_unnamed039:     glColor4f(0.63, 0.46, 0.32, g_opacity);     break;
    case eColor_unnamed040:     glColor4f(0.23, 0.16, 0.02, g_opacity);     break;
    case eColor_unnamed041:     glColor4f(0.33, 0.26, 0.12, g_opacity);     break;
    case eColor_unnamed042:     glColor4f(0.43, 0.36, 0.22, g_opacity);     break;
    case eColor_unnamed043:     glColor4f(0.48, 0.41, 0.27, g_opacity);     break;
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
	glLoadIdentity();
	glTranslatef(-100.0, -100.0, -8.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_TextureFilmstrips[eTextureFilmstrip_RedCurtain].entries[0].texid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(200.0, 0.0, 0.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(200.0, 150.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.0, 150.0, 0.0);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
}

void DrawExtraEnvironmentStuff()
{
	if (!g_bShouldRenderThisFrame || g_numFramesRun <= 1)
		return;

	glLoadIdentity();
	float playerz1 = (g_PlayerSprite->GetPosition().z / 20) + 100;

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
		float v16 = NormalizeNeg180to180(g_PlayerSprite->GetRotation().z);
		float v15 = (g_PlayerSprite->GetPosition().z + 500.0) / 50.0;

		v9 = v15 >= 4.0 ? 4.0 : v15;

		v8 = v9 <= 2.0 ? 2.0 : v9;


		v16 *= v8;
		v7 = i ? 1.0 : -1.0;
		v14 = -playerz1 * (v16 / 180.0 + v7);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(playerz1 + v14, -playerz1, -1300);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(v14 - playerz1, -playerz1, -1300);

		glTexCoord2f(1.0, 1.0);
		glVertex3f(v14 - playerz1, playerz1, -1300);

		glTexCoord2f(0.0, 1.0);
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

void StartFade(float amnt) //amnt is 0.0-1.0
{
	g_fadeIncr = -amnt;
	if (g_fadeIncr < 0.0)
	{
		g_fadeAmount = 1.0;
	}
	else if (g_fadeIncr == 0.0)
	{
		g_fadeAmount = 0.0;
	}
}
void CutToBlack()
{
	g_fadeAmount = 1.0;
	g_fadeIncr = 0.0;
}
void UpdateFade()
{
	if (g_fadeIncr != 0.0 || g_fadeAmount != 0.0)
	{
		g_fadeAmount = g_fadeAmount + g_fadeIncr;
		if (g_fadeAmount <= 0.0)
		{
			g_fadeAmount = 0.0;
			g_fadeIncr = 0.0;
		}
		if (g_fadeAmount >= 1.0)
		{
			g_fadeAmount = 1.0;
			g_fadeIncr = 0.0;
		}
		glLoadIdentity();
		glTranslatef(-150.0, -150.0, -7.0);
		glColor4f(0.0, 0.0, 0.0, g_fadeAmount);
		glBegin(GL_QUADS);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(400.0, 0.0, 0.0);
			glVertex3f(400.0, 250.0, 0.0);
			glVertex3f(0.0, 250.0, 0.0);
		glEnd();
		glLoadIdentity();
	}
}
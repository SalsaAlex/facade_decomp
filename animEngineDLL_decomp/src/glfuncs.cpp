#include "facadecommon.h"
#include "gl/GLU.h"


int g_windowWidth;
int g_windowHeight;

float g_opacity;

float g_fadeIncr;
float g_fadeAmount;

GLYPHMETRICSFLOAT g_FontGlyphs[255];
GLuint g_BitmapFontBase;

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
	glOrtho(-100, 100, -100, 100, 0, 100 * 20);
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

	glEnable(GL_TEXTURE_2D);
	DRAWTEXTUREDBOX(-100, -100, -8, 200, 150, eTextureFilmstrip_RedCurtain);
	glDisable(GL_TEXTURE_2D);
}

void DrawExtraEnvironmentStuff()
{
	if (!g_bShouldRenderThisFrame || g_numFramesRun <= 1)
		return;

	glLoadIdentity();
	float playerz1 = (g_PlayerSprite->GetPosition().m_z / 20) + 100;

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
		float v16 = NormalizeNeg180to180(g_PlayerSprite->GetRotation().m_z);
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
		g_baseScreenMessage = eBaseScreenMessage_none;
	g_bPlayerInputActive = false;

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
		ShellExecuteA(g_hMainAppWindow, "open", "http://www.interactivestory.net/help", 0, 0, 1);
		g_quitAppCountdown = 1;
	}
	DrawArbitraryScreenText(
		"interactivestory.net/help.",
		45,
		verticaltextcursor,
		200,
		5,
		0,
		cursoronlink ? eColor_unnamed014 : eColor_unnamed035, eColor_black);
	verticaltextcursor -= 12;

	bool cursoronbacktomenu = ISCURSORINBOUNDS(-20, 26, verticaltextcursor-8, verticaltextcursor);

	if (cursoronbacktomenu && g_mouseLButtonClicked)	g_baseScreenMessage = eBaseScreenMessage_none;
	DrawArbitraryScreenText("Back to Menu", 
		8.0, verticaltextcursor, 
		200, 
		7, 
		0, 
		cursoronbacktomenu ? eColor_unnamed014 : eColor_unnamed035, eColor_black);
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
		g_baseScreenMessage = g_bAtEndOfDrama ? eBaseScreenMessage_About : eBaseScreenMessage_none;
	}
	if (!g_bAtEndOfDrama)
		CutToBlack();

	g_bPlayerInputActive = false;
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

}
static void ReplayScreenMessage() {
	g_bPlayerInputActive = false;

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
		g_baseScreenMessage = eBaseScreenMessage_TipJar;
	DrawArbitraryScreenText("Exit", 19, -30, 200, 7, 0, cursoronexit ? eColor_unnamed014 : eColor_unnamed035, eColor_black);

	bool cursoronbacktomenu = ISCURSORINBOUNDS(-20, 26, -53, -45);
	if (cursoronbacktomenu && g_mouseLButtonClicked)
		g_baseScreenMessage = eBaseScreenMessage_EndGameMenu;
	DrawArbitraryScreenText("Back to Menu", 8.0, -45, 200, 7, 0, cursoronbacktomenu ? eColor_unnamed014 : eColor_unnamed035, eColor_black);
}
static void TipJarScreenMessage() {
	g_bPlayerInputActive = false;
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

	g_bPlayerInputActive = false;
	DrawArbitraryScreenText("Opening the Façade electronic tip jar in your browser...", -33, 7, 300, 6, 0, eColor_white, eColor_black);
	DrawArbitraryScreenText("Thanks for your support!", -1, -47, 300, 6, 0, eColor_white, eColor_black);

	if (ctr == 1)
		ShellExecute(g_hMainAppWindow, "open", "http://www.interactivestory.net/tipjar", NULL, NULL, TRUE);
	if (++ctr >= 90)
		g_quitAppCountdown = 1;

	SetColorFromIndex(eColor_white);

	glEnable(GL_TEXTURE_2D);
	DRAWTEXTUREDBOX(-20, -35, -4,	40, 35, eTextureFilmstrip_TipJar);
	glDisable(GL_TEXTURE_2D);
}
static void AboutScreenMessage() {

}
static void PlayerChoosesName() {
	static int choice = 0;
	if (g_curTextCharInput == '\r')
	{
		g_playerNameID = choice;
		g_playerGender = strcmp("m", g_PlayerNameStrings[choice].gender) != 0;
		strcpy(g_PlayerSprite->m_name, g_PlayerNameStrings[choice].name);
		g_bPlayerChoosingName = false;
		return;
	}
	DrawArbitraryScreenText("Please Choose a Name",			-15,  10,  75, 10, 0, eColor_white, eColor_black);
	DrawArbitraryScreenText("type the first letter",		 -2,  -2, 200,  7, 0, eColor_white, eColor_black);
	DrawArbitraryScreenText("scroll with the arrow keys",	 -8,  -8, 200,  7, 0, eColor_white, eColor_black);
	DrawArbitraryScreenText("press enter to begin",			 -1, -14, 200,  7, 0, eColor_white, eColor_black);

	bool cursorininstructionsbutton = ISCURSORINBOUNDS(-12, 18, -68, -60);
	if (cursorininstructionsbutton) g_baseScreenMessage = eBaseScreenMessage_Instructions;
	
	//unfinished

}

void DrawScreenMessage()
{
	static int fadeCurtainInCtr = 0;

	if (g_bAtEndOfDrama)
	{
		DrawRedCurtain();
		if (!fadeCurtainInCtr)
		{
			CutToBlack();
			StartFade(0.015);
		}

		if (fadeCurtainInCtr <= 59)
			++fadeCurtainInCtr;
	}
	if (fadeCurtainInCtr <= 59)
		return;
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

void StartFade(float amnt) //amnt is 0.0-1.0
{
	g_fadeIncr = -amnt;
	if (g_fadeIncr < 0)
	{
		g_fadeAmount = 1;
	}
	else if (g_fadeIncr == 0)
	{
		g_fadeAmount = 0;
	}
}
void CutToBlack()
{
	g_fadeAmount = 1;
	g_fadeIncr = 0;
}
void UpdateFade()
{
	if (g_fadeIncr != 0.0 || g_fadeAmount != 0.0)
	{
		g_fadeAmount = g_fadeAmount + g_fadeIncr;
		if (g_fadeAmount <= 0)
		{
			g_fadeAmount = g_fadeIncr = 0;
		}
		if (g_fadeAmount >= 1)
		{
			g_fadeAmount = 1;
			g_fadeIncr = 0;
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
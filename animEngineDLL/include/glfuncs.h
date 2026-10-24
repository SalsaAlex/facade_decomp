#ifndef GLFUNCS_H
#define GLFUNCS_H

#include "facadecommon.h"
#include "gl/GL.h"

#include "enums/EColor.h"

extern float g_opacity;
extern int g_windowWidth;
extern int g_windowHeight;

void SetupRC();

void CutToBlack();
void StartFade(float amnt); //amnt is 0.0-1.0

void BuildBitmapFont();
void KillPolygonalFont();

GLuint LoadTexture(const char* filename, bool alpha, uint32_t wraptype, uint32_t magnify_type, uint32_t minify_type);

void ChangeSize();

void SetColorFromIndex(EColor color);

void DrawSprite(Sprite* spr, bool option);

void UpdateFade();
void DrawPlayerScreenText();
void DrawNPCsScreenText();
void DrawScreenMessage();
void DrawExtraEnvironmentStuff();
void DrawRedCurtain();

void DrawArbitraryScreenText(const char* Str,
	float arg_x, float arg_y,
	int ifactor,
	GLfloat scale,
	float floatfactor,
	EColor maincolor, EColor outlinecolor);

void RotateArrayOfPoints(XYZPointf* in_n_out, int numpoints, XYZPointf rotation, XYZPointf pos);
void RotateModel(Sprite* spr, XYZPointf rotation, XYZPointf* pointsout);

#endif // GLFUNCS_H
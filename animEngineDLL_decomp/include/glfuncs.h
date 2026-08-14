#ifndef GLFUNCS_H
#define GLFUNCS_H

#include "facadecommon.h"
#include "gl/GL.h"

#include "enums/EColor.h"

void SetupRC();

GLuint LoadTexture(const char* filename, bool alpha, uint32_t wraptype, uint32_t magnify_type, uint32_t minify_type);

void ChangeSize();

void SetColorFromIndex(EColor color);

#endif // GLFUNCS_H
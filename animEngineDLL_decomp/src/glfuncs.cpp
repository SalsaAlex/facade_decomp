#include "facadecommon.h"
#include "gl/GLU.h"


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

	if((imagedata = malloc(bytestoread)) == nullptr)
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
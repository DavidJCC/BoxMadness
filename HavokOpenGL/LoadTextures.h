#ifndef LOADTEXTURES_H
#define LOADTEXTURES_H
/*
	Version 1.0	Basic 24 bit bitmap loader. SM.
	Version 2.0	Added extra ctor, LoadBMPandAlpha and LoadGLTexturesAndAlpha functions so that a transparent bgrd colour can be specified.
	To use, BLENDING must be switched on. Also, textureImage made public for easy access. SM.
*/
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <stdio.h>			// Header File For File I/O
//
// Class to load a bitmap file to a texture
// Note: Use getTexture to return texture number
//
class LoadTextures{
	bool status;
	unsigned int *texture;
	static int num;
public:
	BITMAP textureImage;  //data structure for bitmaps
	BYTE* textureImageData;  //bitmap data
	LoadTextures(char *Filename);
	LoadTextures(char *Filename, byte bgcR, byte bgcG, byte bgcB);
	LoadTextures(bool noGenTexture);
	void initialise();
	~LoadTextures();
	BITMAP LoadBMP(char *Filename);
	bool LoadGLTextures(char *s);
	bool getStatus(){ return status; }
	static int getNum(){ return num; }
	unsigned int getTexture(){ return *texture; }
	BYTE* getTextureImage(){ return textureImageData; }

	BITMAP LoadBMPandAlpha(char *Filename, byte bgcR, byte bgcG, byte bgcB);	//added for alpha bit
	bool LoadGLTexturesAndAlpha(char *s, byte bgcR, byte bgcG, byte bgcB);
};

#endif
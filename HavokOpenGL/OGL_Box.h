#pragma once
#include "hvkoglobj.h"
#include "Box.h"

class OGL_Box :	public HvkOGLObj
{
private:
	Vector verts[8];
public:
	OGL_Box(Box* box);
	OGL_Box(Box* box, char* img);
	~OGL_Box(void);

	void render();
	void setSize(float sx, float sy, float sz);
	void drawFace(int v0, int v1, int v2, int v3);
};


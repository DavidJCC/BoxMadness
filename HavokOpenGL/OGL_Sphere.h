#pragma once
#include "hvkoglobj.h"
#include "Sphere.h"

class OGL_Sphere :	public HvkOGLObj
{
private:
	GLUquadricObj* oglQuad;
public:
	OGL_Sphere(Sphere* sphere);
	OGL_Sphere(Sphere* sphere, char* img);
	~OGL_Sphere(void);

	void render();
	void setSize(float sx, float sy, float sz);
	void drawFace(int v0, int v1, int v2, int v3);
};


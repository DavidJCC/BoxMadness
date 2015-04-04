#pragma once

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "HavokObj.h"
#include "LoadTextures.h"
#include "Vector.h"	

/*
 Abstract Base Class for rendering a Havok Rigid body object in OpenGL
*/

class HvkOGLObj{
protected:
	HavokObj *hObj;
	float angle;	//rotation angle
	float r, g, b;
	float radius;
	unsigned int texture;
public:
	HvkOGLObj(HavokObj* havokObj);
	HvkOGLObj(HavokObj* havokObj, char* img);
	virtual ~HvkOGLObj(void);

	virtual void update();
	virtual void render() = 0;	//pure virtual
	float getAngle(){ return angle; }
	void setRGB(float r = 0.5f, float g = 0.5f, float b = 0.5f);	//set colours. Default 0.5f for each
	HavokObj* getHavokObj(){ return hObj; }
};



#pragma once
#include "havokobj.h"
class Box :	public HavokObj
{
private:
	float sx, sy, sz;

public:
	Box(float sx=0.5f,float sy=0.5f,float sz=0.5f); //default 0.2 cube
	~Box(void);
	
	float getSx() { return sx; }
	float getSy() { return sy; }
	float getSz() { return sz; }

	void init(hkpWorld* world);

	void setVel(float x, float y, float z);
	void update(float time);
	void move(float time);

};


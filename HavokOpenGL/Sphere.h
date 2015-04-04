#pragma once
#include "havokobj.h"
class Sphere : public HavokObj
{
private:
	float mass;
public:
	Sphere(void);
	Sphere(float x, float y, float z);
	~Sphere(void);

	void init(hkpWorld* world);
	void setPos(float x, float y, float z);
	void setRad(float radius) { this->setRadius(radius); }
};


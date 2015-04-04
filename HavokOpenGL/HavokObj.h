#pragma once

#include "HavokInit.h"	//for Havok
#include "Vector.h"	

/*
	Abstract Base Class for storing a Havok Rigid body object
*/
class HavokObj{
protected:
	hkpRigidBody* rb;	
	hkpRigidBodyCinfo rigidBodyInfo;
	Vector pos, dir, vel;
	float density;
	float radius;	
	float friction;
public:
	HavokObj();
	~HavokObj();

	virtual void init(hkpWorld *world) = 0;	//initialise specific Hvk body rb and attach to world

	Vector getPos(){ return pos; }	//create inline functions
	void setPos(Vector p){ pos = p; }
	void setDir(Vector d){ dir = d; }
	Vector getDir(){ return dir; }
	hkpRigidBody* getRigidBody() { return rb; }
	void setDensity(float d){ density = d; }
	void setRadius(float r){ radius = r; }
	float getRadius(){ return radius; }
	void setRigidBodyInfo(hkpWorld* world, hkpShape* hks, float mass);
	void setFriction(float friction) { this->friction = friction; }
};
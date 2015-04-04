#include "Box.h"

Box::Box(float x, float y, float z): HavokObj()
{
	sx = x;
	sy = y;
	sz = z;
	pos = dir = Vector(0,0,0);
}

void Box::init(hkpWorld *world)
{
	float calcMass = density * sx * sy * sz;	//mass = density*vol
	hkpBoxShape* sBox = new hkpBoxShape(hkVector4(sx, sy, sz));
	sBox->setRadius(0.001f); // adjust the convex radius as req’d
	setRigidBodyInfo(world, sBox, calcMass);
}

void Box::setVel(float x, float y, float z)
{
	vel.x = x;
	vel.y = y;
	vel.z = z;

	this->getRigidBody()->setLinearVelocity(hkVector4(x, y, z));
}

void Box::move(float time)
{
	pos.x += vel.x * time;
	pos.y += vel.y * time;
	pos.z += vel.z * time;
}


void Box::update(float time)
{
	move(time);
}


Box::~Box(void)
{
	
}

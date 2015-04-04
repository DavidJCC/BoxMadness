#include "Sphere.h"


Sphere::Sphere(void) : HavokObj()
{
}

Sphere::Sphere(float x, float y, float z)
{
	pos = Vector(x, y, z);
}

Sphere::~Sphere(void)
{
}

void Sphere::init(hkpWorld* m_world)
{
	hkpSphereShape* sphere = new hkpSphereShape(0.2);		// convex radius for spheres is exactly the sphere radius
	radius = 0.2f;
	this->mass = (4/3)*HK_REAL_PI*(radius*radius*radius);
	setRigidBodyInfo(m_world, sphere, this->mass);
}

void Sphere::setPos(float x, float y, float z)
{
	pos = Vector(x, y, x);
}

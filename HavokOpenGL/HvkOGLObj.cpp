#include "HvkOGLObj.h"

HvkOGLObj::HvkOGLObj(HavokObj* havokObj){
	hObj = havokObj;
	angle = 0;
	r = g = b = 0.5f;
}

HvkOGLObj::HvkOGLObj(HavokObj* havokObj, char* img){
	hObj = havokObj;
	angle = 0;
	r = g = b = 0.5f;

	LoadTextures *lt = new LoadTextures(img);  //for texture
	texture = lt->getTexture();
	delete lt;
}

void HvkOGLObj::setRGB(float r, float g, float b){
	this->r = r;
	this->g = g;
	this->b = b;
}

void HvkOGLObj::update(){
	if(hObj->getRigidBody()){
		hObj->setPos(Vector(hObj->getRigidBody()->getPosition().getComponent(0), //get physics obj pos
			hObj->getRigidBody()->getPosition().getComponent(1),
			hObj->getRigidBody()->getPosition().getComponent(2)));
		hkQuaternion quaternion = hObj->getRigidBody()->getRotation();	//get orientation as a quaternion
		if(quaternion.hasValidAxis()){
			angle = quaternion.getAngle() * 180.0f / HK_REAL_PI;	//convert to degrees for OpenGL rotation
			hkVector4 axis;
			quaternion.getAxis(axis);
			hObj->setDir(Vector(axis.getSimdAt(0), axis.getSimdAt(1), axis.getSimdAt(2)));
		}
	}
}

HvkOGLObj::~HvkOGLObj(void){
}

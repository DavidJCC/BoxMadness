#include "OGL_Sphere.h"

OGL_Sphere::OGL_Sphere(Sphere* sphere) : HvkOGLObj(sphere)
{
	oglQuad = gluNewQuadric();
	gluQuadricDrawStyle(oglQuad, GLU_FILL);
	gluQuadricNormals(oglQuad, GLU_NONE);
}

OGL_Sphere::OGL_Sphere(Sphere* sphere, char* img) : HvkOGLObj(sphere, img)
{
	oglQuad = gluNewQuadric();
	gluQuadricDrawStyle(oglQuad, GLU_FILL);
	gluQuadricNormals(oglQuad, GLU_NONE);
}

void OGL_Sphere::render()
{
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(r, g, b);
		// Perform transformations here in TRS order
		glTranslatef(hObj->getPos().x, hObj->getPos().y, hObj->getPos().z);
		glRotatef(angle,hObj->getDir().x,hObj->getDir().y,hObj->getDir().z);		
		gluQuadricTexture(oglQuad, true);
		gluSphere(oglQuad, hObj->getRadius(), 30, 35);	
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

OGL_Sphere::~OGL_Sphere(void)
{
}


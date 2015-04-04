#include "OGL_Box.h"

OGL_Box::OGL_Box(Box* hvkBox) : HvkOGLObj(hvkBox)
{
	setSize(hvkBox->getSx(), hvkBox->getSy(), hvkBox->getSz());
	setRGB();
}

OGL_Box::OGL_Box(Box* hvkBox, char* img) : HvkOGLObj(hvkBox, img)
{
	setSize(hvkBox->getSx(), hvkBox->getSy(), hvkBox->getSz());
	setRGB();
}

void OGL_Box::render()
{
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);  
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(r, g, b);
		// Perform transformations here in TRS order
		glTranslatef(hObj->getPos().x, hObj->getPos().y, hObj->getPos().z);
		glRotatef(angle,hObj->getDir().x,hObj->getDir().y,hObj->getDir().z);
		drawFace(0, 4, 5, 1); // Draw each face
		drawFace(3, 7, 4, 0);
		drawFace(2, 6, 7, 3);
		drawFace(1, 5, 6, 2);
		drawFace(3, 0, 1, 2);
		drawFace(4, 7, 6, 5);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

OGL_Box::~OGL_Box(void){
}

void OGL_Box::drawFace(int v0, int v1, int v2, int v3)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(verts[v0].x, verts[v0].y, verts[v0].z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(verts[v1].x, verts[v1].y, verts[v1].z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(verts[v2].x, verts[v2].y, verts[v2].z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(verts[v3].x, verts[v3].y, verts[v3].z);
	glEnd();
}

void OGL_Box::setSize(float sx,float sy, float sz)
{
	verts[0].set(-sx, sy, sz);
	verts[1].set( sx, sy, sz);
	verts[2].set( sx, sy,-sz);
	verts[3].set(-sx, sy,-sz);
	verts[4].set(-sx,-sy, sz);
	verts[5].set( sx,-sy, sz);
	verts[6].set( sx,-sy,-sz);
	verts[7].set(-sx,-sy,-sz);
}
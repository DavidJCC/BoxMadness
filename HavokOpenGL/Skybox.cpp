#include "Skybox.h"


Skybox::Skybox(void)
{
}

Skybox::~Skybox(void)
{
}

void Skybox::init()
{
}

void Skybox::render(float size, float x, float y, float z)
{
	glPushMatrix();
		glLoadIdentity();
		gluLookAt(0,0,0,x, y, z, 0, 1, 0);
		bool b1=glIsEnabled(GL_TEXTURE_2D);	//new, we left the textures turned on, if it was turned on
		bool b2=glIsEnabled(GL_LIGHTING);
		glDisable(GL_LIGHTING);	//turn off lighting, when making the skybox
		glDisable(GL_DEPTH_TEST);	//turn off depth texting
		glEnable(GL_TEXTURE_2D);	//and turn on texturing
		glColor3f(1,1,1);

		glBindTexture(GL_TEXTURE_2D,texture[SKY_BACK]);	//use the texture we want
		glBegin(GL_QUADS);	//and draw a face
			//back face
			glTexCoord2f(0,0);	//use the correct texture vector3d
			glVertex3f(size/2,size/2,size/2);	//and a vertex
			glTexCoord2f(1,0);	//and repeat it...
			glVertex3f(-size/2,size/2,size/2);
			glTexCoord2f(1,1);
			glVertex3f(-size/2,-size/2,size/2);
			glTexCoord2f(0,1);
			glVertex3f(size/2,-size/2,size/2);
		glEnd();
		glBindTexture(GL_TEXTURE_2D,texture[SKY_LEFT]);
		glBegin(GL_QUADS);	
			//left face
			glTexCoord2f(0,0);
			glVertex3f(-size/2,size/2,size/2);
			glTexCoord2f(1,0);
			glVertex3f(-size/2,size/2,-size/2);
			glTexCoord2f(1,1);
			glVertex3f(-size/2,-size/2,-size/2);
			glTexCoord2f(0,1);
			glVertex3f(-size/2,-size/2,size/2);
		glEnd();
		glBindTexture(GL_TEXTURE_2D,texture[SKY_FRONT]);
		glBegin(GL_QUADS);	
			//front face
			glTexCoord2f(1,0);
			glVertex3f(size/2,size/2,-size/2);
			glTexCoord2f(0,0);
			glVertex3f(-size/2,size/2,-size/2);
			glTexCoord2f(0,1);
			glVertex3f(-size/2,-size/2,-size/2);
			glTexCoord2f(1,1);
			glVertex3f(size/2,-size/2,-size/2);
		glEnd();
		glBindTexture(GL_TEXTURE_2D,texture[SKY_RIGHT]);	
		glBegin(GL_QUADS);	
			//right face
			glTexCoord2f(0,0);
			glVertex3f(size/2,size/2,-size/2);
			glTexCoord2f(1,0);
			glVertex3f(size/2,size/2,size/2);
			glTexCoord2f(1,1);
			glVertex3f(size/2,-size/2,size/2);
			glTexCoord2f(0,1);
			glVertex3f(size/2,-size/2,-size/2);
		glEnd();
		glBindTexture(GL_TEXTURE_2D,texture[SKY_TOP]);
			//top face
		glBegin(GL_QUADS);			
			glTexCoord2f(1,0);
			glVertex3f(size/2,size/2,size/2);
			glTexCoord2f(0,0);
			glVertex3f(-size/2,size/2,size/2);
			glTexCoord2f(0,1);
			glVertex3f(-size/2,size/2,-size/2);
			glTexCoord2f(1,1);
			glVertex3f(size/2,size/2,-size/2);
		glEnd();
		glBindTexture(GL_TEXTURE_2D,texture[SKY_BOTTOM]);		
		glBegin(GL_QUADS);	
			//bottom face
			glTexCoord2f(1,1);
			glVertex3f(size/2,-size/2,size/2);
			glTexCoord2f(0,1);
			glVertex3f(-size/2,-size/2,size/2);
			glTexCoord2f(0,0);
			glVertex3f(-size/2,-size/2,-size/2);
			glTexCoord2f(1,0);
			glVertex3f(size/2,-size/2,-size/2);
		glEnd();
		glEnable(GL_DEPTH_TEST);
		if(!b1)
			glDisable(GL_TEXTURE_2D);
		if(b2)
			glEnable(GL_LIGHTING);	//turn everything back, which we turned on, and turn everything off, which we have turned on.
	glPopMatrix();
}

void Skybox::loadTextures()
{
	char s[60];

	for(int i = 0; i < 6; i++)
	{
		sprintf(s, "Images/skybox.bmp", i);
		LoadTextures* lt = new LoadTextures(s);
		if(!lt) return;
		texture[i] = lt->getTexture();
		delete lt;
	}
}

void Skybox::kill()
{
	glDeleteTextures(6, texture);
}

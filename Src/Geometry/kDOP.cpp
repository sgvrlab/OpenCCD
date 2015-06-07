#include <math.h>
#include <assert.h>
#include "kDOP.h"
#include <GL/glut.h>

void
kDOP18::visulization()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	Vec3f org = center();
	glTranslatef(org[0], org[1], org[2]);

	float w = length(0);
	float h = length(1);
	float d = length(2);

	glScalef(w, h, d);
	glutWireCube(1.f);
	glPopMatrix();
}

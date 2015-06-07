#include <math.h>
#include <assert.h>
#include "aabb.h"
#include <GL/glut.h>
#include <iostream>

using namespace std ;

void
aabb::visulization()
{
	glColor3f(1.f, 1.f, 1.f);
	glPushMatrix();
	Vec3f org = center();
	glTranslatef(org[0], org[1], org[2]);
	glScalef(width(), height(), depth());
//	cout << " [ " << width() << " , " << height() << " , " << depth() << " ] " << endl ;
	glutWireCube(1.f);
	glPopMatrix();
}

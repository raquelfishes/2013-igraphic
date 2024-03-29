#include "Square.h"


Square::Square(void)
{
	this->ver1 = new Point;
	this->ver2 = new Point;
	this->ver3 = new Point;
	this->ver4 = new Point;
	selec = false;
	red = 1.0;
	blue = 0.0;
	green = 0.0;
}

Square::Square(Point *p1, Point *p2, Point *p3, Point *p4, GLdouble d)
{
	this->ver1 = p1;
	this->ver2 = p2;
	this->ver3 = p3;
	this->ver4 = p4;
	this->side = d;
	selec = false;
	red = 1.0;
	blue = 0.0;
	green = 0.0;
}

Point* Square::getPoint1()
{
	return this->ver1;
}

Point* Square::getPoint2()
{
	return this->ver2;
}
Point* Square::getPoint3()
{
	return this->ver3;
}
Point* Square::getPoint4()
{
	return this->ver4;
}

GLdouble Square::getSide()
{
	return side;
}

void Square::setSelec(bool b)
{
	this->selec = b;
}

void Square::render()
{
	if (selec) glColor3f(0.0,0.0,1.0);
	else glColor3f(red,blue,green);
	glBegin(GL_QUADS);
		glVertex2d( ver1->GetX(), ver1->GetY() );
		glVertex2d( ver2->GetX(), ver2->GetY() );
		glVertex2d( ver3->GetX(), ver3->GetY() );
		glVertex2d( ver4->GetX(), ver4->GetY() );
    glEnd();
}

Square::~Square(void)
{
}

Point* Square::getClosestCorner(Point* p){
	Point* aux_p = getPoint1();
	GLdouble aux_dist = p->distance(aux_p);
	if(p->distance(getPoint2())<aux_dist) {
		aux_p = getPoint2();
		aux_dist = p->distance(aux_p);
	}
	if(p->distance(getPoint3())<aux_dist) {
		aux_p = getPoint3();
		aux_dist = p->distance(aux_p);
	}
	if(p->distance(getPoint4())<aux_dist) {
		aux_p = getPoint4();
		aux_dist = p->distance(aux_p);
	}

	return aux_p;
}


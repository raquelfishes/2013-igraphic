#include "Car.h"


Car::Car(GLdouble x,GLdouble y,GLdouble z)
{
	this->aproximacion=40;
    this->x=x;
    this->y=y;
    this->z=z;
    this->radioRueda=0.1*x;
    this->anchoRueda=0.1*x;
        
    build();
}


Car::~Car(void)
{
}

void Car::build()
{
	cubo1=new Cube(0.6*x,0.6*y,0.8*z);   
	cubo1->build();
	cubo2=new Cube(0.4*x,0.2*y,0.6*z);   
	cubo2->build();

	for (int i=0; i<4; i++){
		this->wheels->at(i) = gluNewQuadric();
		this->covers->at(i) = gluNewQuadric();
	}
}

void Car::draw(GLdouble acumCoche)
{
	glMatrixMode(GL_MODELVIEW);
       
		glPushMatrix();
			glTranslated(0.2*x,radioRueda/2,radioRueda);
			//PV3D* color1= new PV3D(1,0,0);
			cubo1->draw(true,false);//parte de abajo
			//delete color1;
		glPopMatrix();

		glPushMatrix();
			//PV3D* color2= new PV3D(1,1,0);
			glTranslated(0.3*x,radioRueda/2+0.6*x,radioRueda+0.1*z);
			cubo2->draw(true,false); //parte de arriba
			//delete color2;
		glPopMatrix();

	   for (int i=0; i<4; i++){
		   //Wheel
			glPushMatrix();
				glColor3f(0,0,0);//negro
				gluQuadricDrawStyle(wheels->at(i), GLU_FILL);
				gluQuadricNormals(wheels->at(i), GLU_FLAT);
				glRotated(90,0,1,0);
				glTranslated(-radioRueda,0,0.1*x);
				glRotated(acumCoche,0,0,1);
				gluCylinder(wheels->at(i), radioRueda*2, radioRueda*2, anchoRueda, this->aproximacion,this->aproximacion);
			glPopMatrix();

			//Cover
			glPushMatrix();
				glColor3f(0,0,0);//negro
				gluQuadricDrawStyle(covers->at(i), GLU_FILL);
				gluQuadricNormals(covers->at(i), GLU_FLAT);
				glRotated(90,0,1,0);
				glTranslated(-radioRueda,0,0.1*x);
				gluDisk(covers->at(i),0, radioRueda*2, this->aproximacion,this->aproximacion);
			glPopMatrix();
	   }

	glPopMatrix();
}
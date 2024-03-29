#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <GL/freeglut.h>
//#include <GL/glut.h>

#include <Scene.h>
#include <Lectora/TextureLoader.h>
#include <Pixmap.h>

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// Freeglut parameters
// Flag telling us to keep processing events
// bool continue_in_main_loop= true; //(**)

// Viewport size
int WIDTH= 500, HEIGHT= 500;


Scene* escena;
Pixmap *pixMap1,*pixMap2;
boolean seleccionado1 = true;
int pmID;  // PixMap Identifier
boolean makeRotate = false; //Flag to mouse input for rotate
GLdouble angleRotate;

void intitGL(){

	glClearColor(1.0,1.0,1.0,1.0);
	glColor3f(escena->red,escena->green,escena->blue); 

	glPointSize(4.0);
	glLineWidth(2.0);

	// Viewport
    glViewport(0, 0, WIDTH, HEIGHT);
    
	// Model transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Scene Visible Area
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(escena->xLeft, escena->xRight, escena->yBot, escena->yTop); 
 }


void display(void){
  glClear( GL_COLOR_BUFFER_BIT );
 
  switch(seleccionado1){
  case true:
	  if(pixMap1!=NULL)pixMap1->drawMatrix(0,0);  // esto es con prop�sitos de debug unicamente
	  break;
  case false:
	  if(pixMap2!=NULL)pixMap2->drawMatrix(0,0);  // esto es con prop�sitos de debug unicamente
	  break;
  }
  // Scene rendering
  escena->render();
  cout<<"se renderiza"<<endl;

  glFlush();
  glutSwapBuffers();
}


void resize(int newWidth, int newHeight){
  //Resize Viewport
  WIDTH= newWidth;
  HEIGHT= newHeight;
 /* GLdouble RatioViewPort= (float)WIDTH/(float)HEIGHT;
  glViewport ( 0, 0, WIDTH, HEIGHT ) ;
 
  //Resize Scene Visible Area 
  //Se actualiza el �rea visible de la escena
  //para que su ratio coincida con ratioViewPort
  GLdouble SVAWidth= escena->xRight-escena->xLeft;
  GLdouble SVAHeight= escena->yTop-escena->yBot;
  GLdouble SVARatio= SVAWidth/SVAHeight;
  if (SVARatio >= RatioViewPort) {
	 // Increase SVAHeight
     GLdouble newHeight= SVAWidth/RatioViewPort;
	 GLdouble yMiddle= ( escena->yBot+escena->yTop )/2.0;
     escena->yTop= yMiddle + newHeight/2.0;
     escena->yBot= yMiddle - newHeight/2.0;
     }
  else {
     //Increase SVAWidth
     GLdouble newWidth= SVAHeight*RatioViewPort;
     GLdouble xMiddle= ( escena->xLeft+escena->xRight )/2.0;
	 escena->xRight= xMiddle + newWidth/2.0;
     escena->xLeft=  xMiddle - newWidth/2.0;
  }
  

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(escena->xLeft, escena->xRight, escena->yBot, escena->yTop); */
}

bool buferToPixMap(Pixmap *&pm){  // borra el "pixmap" entero cuando toca "sobreescribir"
	if (pm!=NULL) {delete(pm);pm=NULL;}
	pm = new Pixmap();
	glutSwapBuffers();
	bool ret = pm->loadFromBufer(WIDTH,HEIGHT,0,0); // se supone que (0,0) es la esquina inferior izquierda de la ventana
	glutSwapBuffers();
	return ret;
}

bool fileToPixMap(Pixmap *&pm,const char* imagepath){
	if (pm!=NULL) {delete(pm);pm=NULL;}
	pm = new Pixmap();
	return pm->loadFromFile(imagepath);
}

bool desenfoqueGauss(Pixmap *&pm){
	if(pm==NULL) return false;
	else return pm->gaussianBlur();
}

void key(unsigned char key, int x, int y){
 
  bool need_redisplay = true;

  switch (key) {
  case 27:  /* Escape key */
    //continue_in_main_loop = false; // (**)
	glutLeaveMainLoop (); //Freeglut's sentence for stopping glut's main loop (*)
    break;

// Movimientos de c�mara : RIGHT / LEFT / UP / DOWN
  case 'd' :
  case 'D' :
    escena->cam_right();
    break ;

  case 'a' :
  case 'A' :
    escena->cam_left();
    break ;

  case 'w' :
  case 'W' :
    escena->cam_up();
    break ;

  case 's' :
  case 'S' :
    escena->cam_down();
    break ;

// Zoom de c�mara : IN / OUT

  case 'e' :
  case 'E' :
	escena->cam_in();
	break ;

  case 'q' :
  case 'Q' :
    escena->cam_out();
    break ;

// Resetear el programa

  case 'r' :
  case 'R' :
	if (escena->reset()) cout << "La escena ha sido reseteada" << endl;
	else cout << "No hab�a nada que resetear" << endl;
	delete pixMap1; pixMap1= NULL;
	delete pixMap2; pixMap2=NULL;
	cout << "Pixmaps vaciados" << endl;
	break;

// Borrar solo el �rbol
  case 't' :
  case 'T' :
	if (escena->reset()) cout << "La escena ha sido reseteada" << endl;
	else cout << "No hab�a nada que resetear" << endl;
	break;

// Angle setting

  case 'k':
  case 'K':
	if (escena->read_angle()) cout << "El angulo ha sido impuesto" << endl;
	else cout << "No hay arbol" << endl;
	break;

//  Load BMP

  case 'l':
  case 'L':{
		//fileToPixMap(pixMap1,"./images/huevo.bmp");
		//fileToPixMap(pixMap2,"./images/paisaje.bmp");
		string name = "";
		cout << "Escriba el nombre del fichero .BMP: ";
		cin >> name;
		std::string fullName = std::string("./images/").append(name);
		bool succesful=false;
		switch (seleccionado1) {
			case true: succesful=fileToPixMap(pixMap1,fullName.c_str()); break;
			case false: succesful=fileToPixMap(pixMap2,fullName.c_str()); break;
			default: break;
			}
		if(!succesful) cout << "Repita la operacion correctamente.";
		}
	  break;

//	Asignar lo que se muestra a un pixMap
	
	case 'm':
	case 'M':
		// TODO  Coger el bufer y asignarlo a una de nuestras estructuras pixMap
		
		cout << "Elija el pixMap que recibir� el contenido del Buffer: ";
		cin >> pmID;
		switch (pmID) {
		case 1: buferToPixMap(pixMap1); break;
		case 2: buferToPixMap(pixMap2); break;
		default: break;
		}
	break;
	  
// Rotaci�n de un angulo
	case 'v':
	case 'V':
		cout << "Elija el �ngulo para la rotacion: ";
		cin >> angleRotate;
		angleRotate = (angleRotate*M_PI)/180;
		cout << "Click en el centro de rotaci�n.";
		makeRotate=true;
	break;

// Media Ponderada de BitMaps
	case 'z':
	case 'Z':
		if(pixMap1==NULL||pixMap2==NULL){
		cout << "Debe tener los dos pixMaps cargados" << endl;}
		else{
			double factor;
			cout << "Introduzca el valor del factor que corresponder� al pixMap seleccionado (entre 0 y 1): ";
			cin >> factor;
			if(factor>0&&factor<1){
				switch (seleccionado1) {
				case true: pixMap1->weightedAverage(factor,pixMap2);break;
				case false: pixMap2->weightedAverage(factor,pixMap1); break;
				default: break;
				}
			}else{
				cout << "Repita la operaci�n siguiendo las instrucciones." << endl;
			}
		}
		break;

// Diferencia de BitMaps

	case 'x':
	case 'X':
		if(pixMap1==NULL||pixMap2==NULL){
		cout << "Debe tener los dos pixMaps cargados" << endl;}
		else{
			switch (seleccionado1) {
			case true: pixMap1->difference(pixMap2);break;
			case false: pixMap2->difference(pixMap1); break;
			default: break;
			}
		}
		break;

// Gaussian Blur

	case 'b':
	case 'B':
		
		/*double factor;
		cout << "Introduzca el valor del factor con el que realizar la operaci�n: ";
		cin >> factor;*/
		switch (seleccionado1) {
		case true: desenfoqueGauss(pixMap1); break;
		case false: desenfoqueGauss(pixMap2); break;
		default: break;
		}
		break;

// Selecci�n del pixMap

	case 49:
		seleccionado1 = true;
		break;

	case 50:
		seleccionado1 = false;
		break;

// Crecimiento/Decrecimiento del �rbol

  case '+' :
	if (escena->tree_grow()) cout << "El �rbol existe y crece" << endl;
	else cout << "El �rbol no existe o no crece" << endl;
	break;

  case '-' :
	if (escena->tree_drecrease()) cout << "El arbol existe y decrece" << endl;
	else cout << "El arbol no existe o no decrece" << endl;
	break;

  default:
    need_redisplay = false;
	cout << "la tecla es: " << (int) key << endl;
    break;
  }//switch

  if (need_redisplay)
    glutPostRedisplay();
}

void mouse(int button, int state,int x, int y){
	switch (button) {
	case GLUT_LEFT_BUTTON:
		switch (state){
			case GLUT_DOWN:{
					y = HEIGHT - y;
					cout << x << " " << y << " " << endl;

					GLdouble aux_x = (((escena->xRight - escena->xLeft)*x)/WIDTH) + escena->xLeft;
					GLdouble aux_y = (((escena->yTop - escena->yBot)*y)/HEIGHT)+escena->yBot;
					
					if (makeRotate){	  
						
						switch (seleccionado1) {
							case true: 
								if(x<pixMap1->nCols&&y<pixMap1->nRows){
									pixMap1->rotate(angleRotate,y,x); 
									makeRotate = false;
								}
								break;
							case false: 
								if(x<pixMap2->nCols&&y<pixMap2->nRows){
									pixMap2->rotate(angleRotate,y,x); 
									makeRotate = false;
								}
								break;
							default: break;
						}
						
					}
					else{
						escena->mouse_input(aux_x,aux_y);
					}
					}
				break;
			case GLUT_UP:
				break; 
		}// switch state
		break;
	}//switch
}



int main(int argc, char *argv[]){
  cout<< "Starting console..." << endl;

  int my_window; //my window's identifier

  //Initialization
  glutInitWindowSize(500, 500);
  glutInitWindowPosition (0,0);//140, 140);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE );
  glutInit(&argc, argv);

  //Window construction
  my_window = glutCreateWindow( "Freeglut 2D-project" );
    
  //Callback registration
  glutReshapeFunc(resize);
  glutKeyboardFunc(key);
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
    
  escena = new Scene();
  pixMap1 = pixMap2 = NULL;

  //OpenGL basic setting
  intitGL(); 

  // Freeglut's main loop can be stopped executing (**)
  //while ( continue_in_main_loop )
  //  glutMainLoopEvent();

  // Classic glut's main loop can be stopped after X-closing the window,
  // using the following freeglut's setting (*)
  glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION ) ;
    //GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  // Classic glut's main loop can be stopped in freeglut using (*)
  glutMainLoop(); 
  
  cout << "se sale del programa" << endl;
  delete escena;
  escena = NULL;

  // We would never reach this point using classic glut
  system("PAUSE"); 
   
  return 0;
}

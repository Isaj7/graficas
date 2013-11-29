/***************************************
| Maria Isabel Jilo. Carnet: 07-41063   |
| Octavio Manzano.   Carnet: 08-10672	|
| Proyecto 2 OpenGL						|
| Brick Breaker							|
*****************************************/

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0

int activarBono = 0;
float X1= -1.5;
float X2= 1.5;
float PI = 3.141592;
float X = 0.0;
float Y = -7.8;
float movY = -0.1;
float movX = 0.1;
int ranNum;
float radio2;
float bonoX[5];
float bonoY[5];
int numeroBono = 0;

struct ladrillos {
   int posX;
   int posY;
   int doble;
   int bonus;
   int esta;
} matriz[35];

int generarRandom = 1;
int aleatorio = 0;

/* Funcion que genera 7 ladrillos aleatorios especiales
   y 5 ladrillos aleatorios con bono */
void ladrillosAleatorios() {

	for (int inicio = 0; inicio < 5; inicio ++) {
		bonoX[inicio] = -17;
		bonoY[inicio] = -17;
	}

	for(ranNum = 0; ranNum < 7; ranNum++) {
		int aleatorio = rand() %34;
		if (matriz[aleatorio].doble == 1) {
			ranNum--;
		}
		matriz[aleatorio].doble = 1;
	}

	for(ranNum = 0; ranNum < 5; ranNum++) {
		aleatorio = rand() %34;
		if (matriz[aleatorio].bonus == 1) {
			ranNum--;
		}
		matriz[aleatorio].bonus = 1;
		
	}
	generarRandom = 0;
}

void ejesCoordenada() {
	
	glLineWidth(2.5);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(1.5);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}
		
	glEnd();

	glLineWidth(1.0);
}

/* Funcion que dibuja el tablero */
void tablero () {
	/* NOTA: 
	   Max Altura Y = 9.5
	   Min Altura Y = -9.0
	   MARGEN IZQUIERDO: X = -9.1 hasta X = -9.5
	   MARGEN DERECHO: X = 9.1 hasta X = 9.5
	*/
	glLineWidth(2.0);
	glBegin(GL_LINES);
		glColor3f(0.0,1.0,0.0);
		glVertex2f(-9.1,9.5);
		glVertex2f(-9.1,-9.0);
		glVertex2f(-9.5,9.5);
		glVertex2f(-9.5,-9.0);
		glVertex2f(-9.1,-9.0);
		glVertex2f(-9.5,-9.0);
		glVertex2f(-9.1,9.5);
		glVertex2f(-9.5,9.5);

		glVertex2f(9.1,9.5);
		glVertex2f(9.1,-9.0);
		glVertex2f(9.5,9.5);
		glVertex2f(9.5,-9.0);
		glVertex2f(9.1,-9.0);
		glVertex2f(9.5,-9.0);
		glVertex2f(9.1,9.5);
		glVertex2f(9.5,9.5);
		glVertex2f(9.1,9.5);
		glVertex2f(-9.1,9.5);
		glVertex2f(9.1,9.1);
		glVertex2f(-9.1,9.1);
	glEnd();

	/* Ciclo que genera los ladrillos */
	glBegin(GL_LINES);
	int aux = 0;
		for (float i = -8.25; i < 8.0; i = i + 2.5) {
			for (float j = 7; 0.0 < j; j = j - 1.5) {
				glColor3f(1.0,0.0,0.0);
				/* Se verifica si es un ladrillo especial para cambiar el color*/
				if (matriz[aux].doble == 1) {
					glColor3f(1.0,1.0,0.0);
				}
				if (matriz[aux].doble == 2) {
					glColor3f(0.0,0.0,1.0);
				}
				if (matriz[aux].bonus == 1) {
					glColor3f(1.0,1.0,1.0);
				}
				if (matriz[aux].esta == 0) {
					glVertex2f(i,j);
					glVertex2f(i + 1.5,j);
					glVertex2f(i,j - 0.5);
					glVertex2f(i + 1.5,j - 0.5);
					glVertex2f(i,j);
					glVertex2f(i,j - 0.5);
					glVertex2f(i + 1.5,j);
					glVertex2f(i + 1.5,j - 0.5);
					matriz[aux].posX = i;
					matriz[aux].posY = j;
				}
				aux++;
			}
		}

		/* Base azul*/
		glColor3f(0.0,0.0,1.0);
		glVertex2f(X1,-8.0);
		glVertex2f(X2,-8.0);
		glVertex2f(X1,-8.3);
		glVertex2f(X2,-8.3);
		glVertex2f(X1,-8.0);
		glVertex2f(X1,-8.3);
		glVertex2f(X2,-8.0);
		glVertex2f(X2,-8.3);
	glEnd();

	/* Pelota */
	float radio = 0.2;
	glColor3f(1.0,1.0,1.0);
	glBegin (GL_LINE_LOOP);
		for (float angle = 0; angle <= PI * 2; angle = angle + 0.01) {
			glVertex2f(X + sin(angle) * radio, Y + cos(angle) * radio);
		}
	glEnd();

	/* Bono especial */
	if (activarBono == 1) {
		for (int auxbono = 0; auxbono <= numeroBono; auxbono++) {
			radio2 = 0.2;
			glColor3f(1.0,0.5,0.0);
			glBegin (GL_LINE_LOOP);
			for (float angle = 0; angle <= PI * 2; angle = angle + 0.01) {
				glVertex2f(bonoX[auxbono] + sin(angle) * radio2, bonoY[auxbono] + cos(angle) * radio2);
			}
			glEnd();
			radio2 = 0.15;
			glColor3f(1.0,1.0,0.0);
			glBegin (GL_LINE_LOOP);
			for (float angle = 0; angle <= PI * 2; angle = angle + 0.01) {
				glVertex2f(bonoX[auxbono] + sin(angle) * radio2, bonoY[auxbono] + cos(angle) * radio2);
			}
			glEnd();
			radio2 = 0.1;
			glColor3f(0.0,1.0,0.0);
			glBegin (GL_LINE_LOOP);
			for (float angle = 0; angle <= PI * 2; angle = angle + 0.01) {
				glVertex2f(bonoX[auxbono] + sin(angle) * radio2, bonoY[auxbono] + cos(angle) * radio2);
			}
			glEnd();
		}
	}
}




/* Funcion encargada de actualizar el movimiento de la pelota*/

/* NOTA: Maxima Altura que puede alcanzar la bola Y = 8.8 
		 Minima Altura: Y = -7.8 */

int lost = 0;
int nextX;
int nextY;
void actualizar(int value) {
	int estado = 0;
	nextX = movX;
	nextY = movY;
	for (int auxBono = 0; auxBono <= numeroBono; auxBono++) {
		bonoY[auxBono] -= 0.1;
	}
	/* Verifica si esta chocando a partir de Y mayor que cero */
	if (movY + Y >= 0.0) {
		for (float i = -8.25; i < 8.0; i = i + 2.5) {
			for (float j = 7; 0.0 < j; j = j - 1.5) {
				/* Vericia si se esta estrellando contra lineas horizontales */
				if ((nextX + X <= (i + 1.5 + 0.3)) && (nextX + X >= i - 0.3)) {
					if (((nextY + Y) <= j + 0.3) && ((nextY + Y) >= (j - 0.5 - 0.3))) {
						if (matriz[estado].esta == 0) {
							/* Verifica si es un bloque con bono */
							if (matriz[estado].bonus == 1)  {
								matriz[estado].bonus = 0;
								activarBono = 1;
								bonoX[numeroBono] = i + 0.75;
								bonoY[numeroBono] = j - 0.5;
								numeroBono++;
							}
							/* Verifica si es un bloque especial */
							if (matriz[estado].doble == 1)  {
								matriz[estado].doble = 2;
							} else {
								matriz[estado].doble = 0;
								matriz[estado].esta = 1;
							}
							movY = movY*(-1);
							Y += movY;
						}
					}
				/* Vericia si se esta estrellando contra lineas verticales */
				} else if ((nextY + Y <= j + 0.3) && (nextY + Y >= (j - 0.3- 0.5))) {
					if (((nextX + X) >= i - 0.3) && ((nextX + X) <= (i+ 0.3 + 1.5))) {
						if (matriz[estado].esta == 0) {
							/* Verifica si es un bloque con bono */
							if (matriz[estado].bonus == 1)  {
								matriz[estado].bonus = 0;
								activarBono = 1;
								bonoX[numeroBono] = i + 0.75;
								bonoY[numeroBono] = j - 0.5;
								numeroBono++;
							}
							/* Verifica si es un bloque especial */
							if (matriz[estado].doble == 1)  {
								matriz[estado].doble = 2;
							} else {
								matriz[estado].doble = 0;
								matriz[estado].esta = 1;
							}
							movX = movX*(-1);
							X += movX;
						}
					} 
				}
				estado++;
			}
		}
	}
	/* REBOTE CONTRA LOS MARGENES */
	/* Movimiento del eje Y [De arriba hacia abajo] 
	   Si no esta tocando un margen, se sigue moviendo */
	if (((Y + movY) < 8.8) && ((Y + movY) > -7.8)) {
		Y += movY;
	} else {
		/* Cambia de direccion si:
			- Esta tocando el margen superior
			- Al llegar al margen inferior, esta tocando la barra */
		if (((((Y + movY) <= -7.8) && (X1 <= (X + movX) && (X + movX) <= X2)) || ((Y + movY) >= 8.8)) && lost == 0) {
			movY = movY*(-1);
			Y += movY;
		/* Si no esta tocando la barra, se termina el juego */
		} else if (((Y + movY) <= -7.8) && ((Y + movY) > -9.0)) {
			Y += movY;
			lost = 1;
		} else if ((Y + movY) <= -9.0) {
			movY = 0;
			movX = 0;
			X1= -1.5;
			X2= 1.5;
			X = 0.0;
			Y = -7.8;
			lost = 0;
			generarRandom = 1;
			glutPostRedisplay();
			glutTimerFunc(25, actualizar, 0);
		}
	}
	/* Movimiento del eje X [De derecha a izquierda] */
	if (((X + movX) < 8.9) && ((X + movX) > -8.9)) {
		X += movX;
			glutPostRedisplay();
			glutTimerFunc(25, actualizar, 0);
	} else {
			movX = movX*(-1);
			X += movX;
			glutPostRedisplay();
			glutTimerFunc(25, actualizar, 0);
	}
}


/* Funcion encargada de detectar los movimientos del jugador en el teclado */
void detectarTeclado (unsigned char key, int x, int y) {
	switch (key) {
		case'a':
			if (movX != 0) {
				if (X1 > -8.9) {
					X1 -= 0.5;
					X2 -= 0.5;
				}
			}
		break;
		case's':
			if (movX != 0) {
				if (X2 < 8.9) {
					X1 += 0.5;
					X2 += 0.5;
				}
			}
			break;
		case'1':
			if (movX == 0) {
				movX = 0.1;
				movY = -0.1;
			}
			break;
	}
	glutPostRedisplay();
	tablero();
}




void changeViewport(int w, int h) {
	
	float aspectratio;

	if (h==0)
		h=1;

	
	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	
	aspectratio = (float) w / (float) h;

	if (w<=h) {
		
		glOrtho(-10.0,10.0,-10.0/aspectratio,10.0/aspectratio,1.0,50.0);

	}else {

		glOrtho(-10.0*aspectratio,10.0*aspectratio,-10.0,10.0,1.0,50.0);

	}

	glMatrixMode (GL_MODELVIEW);
}



void render(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
/*	
	// Render Grid 
	
	glLineWidth(1.0);
	glPushMatrix();
    glColor3f( 0.0, 0.7, 0.7 );
    glBegin( GL_LINES );
    zExtent = DEF_floorGridScale * DEF_floorGridZSteps;
    for(loopX = -DEF_floorGridXSteps; loopX <= DEF_floorGridXSteps; loopX++ )
	{
	xLocal = DEF_floorGridScale * loopX;
	glVertex3f( xLocal, -zExtent, 0.0 );
	glVertex3f( xLocal, zExtent,  0.0 );
	}
    xExtent = DEF_floorGridScale * DEF_floorGridXSteps;
    for(loopZ = -DEF_floorGridZSteps; loopZ <= DEF_floorGridZSteps; loopZ++ )
	{
	zLocal = DEF_floorGridScale * loopZ;
	glVertex3f( -xExtent, zLocal, 0.0 );
	glVertex3f(  xExtent, zLocal, 0.0 );
	}
    glEnd();
    glPopMatrix();
	
	// Fin Grid

*/
	//Suaviza las lineas
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );

	
	//ejesCoordenada();
	if (generarRandom == 1){
		/* Limpiando los ladrillos de sus estados*/
		for (int limpiar = 0; limpiar < 35; limpiar++) {
			matriz[limpiar].doble = 0;
			matriz[limpiar].bonus = 0;
			matriz[limpiar].esta = 0;
		}
		ladrillosAleatorios();
	}
	tablero();

	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);

	glutSwapBuffers();
}

int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(800,800);

	glutCreateWindow("Test Opengl");

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc(detectarTeclado);
	glutTimerFunc(25, actualizar, 0);
	
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	

	glutMainLoop();
	return 0;

}
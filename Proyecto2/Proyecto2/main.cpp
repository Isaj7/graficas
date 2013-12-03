/***************************************
| Maria Isabel Jilo. Carnet: 07-41063   |
| Octavio Manzano.   Carnet: 08-10672	|
| Proyecto 2 OpenGL						|
| Brick Breaker							|
*****************************************/

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <time.h>
using namespace std;

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0

/* Declaracion de variables/estructuras globales */
int ranNum;
int activarBono = 0;
int numeroBono = 0;
int generarRandom = 1;
int aleatorio = 0;
int lost = 0;
int explosionVar;

float X1 = -1.5;
float X2 = 1.5;
float PI = 3.141592;
float X = 0.0;
float Y = -7.8;
float velX = 0.1;
float movY = -0.1;
float movX = 0.1;
float radio2;
float bonoX[5];
float bonoY[5];
float R=0.0,G=1.0,B=0.0;
int estar = 35;

struct ladrillos {
   float posX;
   float posY;
   int doble;
   int bonus;
   int esta;
   int explota;
} matriz[35];

/*________________________________________________________________________
					FUNCIONES AUXILIARES
________________________________________________________________________*/

/* Funcion que genera: - 7 ladrillos aleatorios especiales
					   - 5 ladrillos aleatorios con bono */
void ladrillosAleatorios() {
	
	for (int inicio = 0; inicio < 5; inicio ++) {
		bonoX[inicio] = -17;
		bonoY[inicio] = -17;
	}
	/* Creacion de 7 ladrillos especiales */
	for(ranNum = 0; ranNum < 7; ranNum++) {
		int aleatorio = rand() %35;
		if (matriz[aleatorio].doble == 1) {
			ranNum--;
		}
		matriz[aleatorio].doble = 1;
	}
	/* Creacion de 5 ladrillos con bono */
	for(ranNum = 0; ranNum < 5; ranNum++) {
		aleatorio = rand() %35;
		if (matriz[aleatorio].bonus == 1) {
			ranNum--;
		}
		matriz[aleatorio].bonus = 1;
		
	}
	generarRandom = 0;
}

/*Funcion que genera la explosion de los bloques dobles*/
int explosion(float cX, float cY, int estado, float type){
	float radio = estado*0.1;
	glColor3f(R,G,B);
	
	for(int i = 0; i < 10; i++){ 
			float theta = 2.0f * 3.1415926f * float(i) / float(10);//get the current angle 
			float x = radio * cosf(theta);//calculate the x component 
			float y = radio * sinf(theta);//calculate the y component 
		glBegin (GL_LINES);
		for (float angle = 0; angle <= PI * 2; angle = angle + 0.01) {
			glVertex2f(x + cX + sinf(angle) * (type-radio*0.1), y+ cY +cosf(angle) * (type-radio*0.1));
		}
		glEnd();
	}
	
	return estado+1;
}

/*________________________________________________________________________
					DIBUJADO DEL TABLERO
________________________________________________________________________*/

/* Funcion que dibuja el tablero */
void tablero () {
	/* Dibujo de los margenes */
	glLineWidth(2.0);
	glBegin(GL_LINES);
		glColor3f(R,G,B);
		glVertex2f(-9.1,9.5);
		glVertex2f(-9.1,-9.0);
		glVertex2f(-9.5,9.5);
		glVertex2f(-9.5,-9.0);
		glVertex2f(-9.1,-9.0);
		glVertex2f(-9.5,-9.0);
		glVertex2f(-9.1,9.5);
		glVertex2f(-9.5,9.5);

		/* Si se ha perdido el juego se completa el tablero con un margen inferior */
		if (lost == 1){
			glVertex2f(-9.1,-9.0);
			glVertex2f(9.1,-9.0);
			glVertex2f(-9.1,-8.6);
			glVertex2f(9.1,-8.6);
		}

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

	/* Dibujo de los ladrillos */
	int aux = 0;
		for (float i = -8.25; i < 8.0; i = i + 2.5) {
			for (float j = 7; 0.0 < j; j = j - 1.5) {
				glColor3f(R,G,B);
				/* Se verifica si es un ladrillo especial para cambiar el color*/
				if (matriz[aux].doble == 1) {
					glColor3f(1.0,1.0,0.0);
				}
				else if (matriz[aux].doble == 2) {
					glColor3f(R,G,B);
					glBegin(GL_TRIANGLES);
						glVertex2f(i + 0.78-0.15,j-0.25-0.05);
						glVertex2f(i + 0.75,j - 0.25 - 0.07);
						glVertex2f(i + 0.70 + 0.38,j - 0.25 + 0.2);
					glEnd();
					glBegin(GL_LINES);
						glVertex2f(i + 0.75,j - 0.25);
						glVertex2f(i,j - 0.25 - 0.07);
						glVertex2f(i + 0.75,j - 0.25 - 0.07);
						glVertex2f(i + 0.9,j - 0.5);
						glVertex2f(i + 0.75 + 0.2,j - 0.25 + 0.1);
						glVertex2f(i + 1.5,j - 0.25 + 0.2);
					glEnd();
				}
				if (matriz[aux].esta == 0) {
					glBegin(GL_LINES);
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
					glEnd();
				}

				/*Se verifica si el ladrillo debe explotar y explota si le corresponde*/
				if (matriz[aux].explota > 0 && matriz[aux].explota<=15)
					if (matriz[aux].bonus == 2){
						matriz[aux].explota = explosion(matriz[aux].posX+0.75,matriz[aux].posY-0.25,matriz[aux].explota,2.0);
						if (matriz[aux].explota==15)
							matriz[aux].bonus = 0;
					}
					else if (matriz[aux].bonus == 3)
						matriz[aux].explota = explosion(matriz[aux].posX+0.75,matriz[aux].posY-0.25,matriz[aux].explota,0.5);
					else
						matriz[aux].explota = explosion(matriz[aux].posX+0.75,matriz[aux].posY-0.25,matriz[aux].explota,0.2);
				aux++;
			}
		}


		/* dibujo de la Base azul:
				- Si no se ha perdido el juego se dibuja la barra, en caso contrario:
				- Se muestra la vista de juego finalizado o juego ganado */
		if (lost == 0) {
			/* Juego ganado */
			if (estar == 0) {
				if (explosionVar < 1) {
					for (int i = 0; i < 35; i++){
						matriz[i].bonus = 3;
						matriz[i].explota = 1;
						matriz[i].esta = 1;
						matriz[i].doble = 0;
					}
					explosionVar++;
				} else {
					movY = 0;
					movX = 0;
					X1= -1.5;
					X2= 1.5;
					X = 0.0;
					Y = -7.8;
					glColor3f(1.0,1.0,1.0);

					/* Ojo izquierdo */
					glBegin (GL_LINES);
						for (float angle = 0; angle <= 2*PI; angle = angle + 0.01)
							glVertex2f(-4+sinf(angle),cosf(angle)*0.7);
					glEnd();

					/* Ojo derecho */
					glBegin (GL_LINES);
						for (float angle = 0; angle <= 2*PI; angle = angle + 0.01)
							glVertex2f(4+sinf(angle),cosf(angle)*0.7);
					glEnd();
					glPushMatrix();
					glRotatef(90,0,0,1);

					/* Boca */
					glBegin (GL_LINES);
						for (float angle = 0; angle <= PI; angle = angle + 0.01) {
						glVertex2f( - 4.5 - sinf(angle) * 2,cosf(angle) * 3);
						}
					glEnd();
					glPopMatrix();

			
					/* Iris izquierdo */
					glColor3f(R,G,B);
					glBegin (GL_LINES);
						for (float angle = 0; angle <= 2*PI; angle = angle + 0.01)
							glVertex2f(-4+sinf(angle)*0.1,-0.2+cosf(angle)*0.1);
					glEnd();

					/* Iris izquierdo */
					glBegin (GL_LINES);
						for (float angle = 0; angle <= 2*PI; angle = angle + 0.01)
							glVertex2f(4+sinf(angle)*0.1,-0.2+cosf(angle)*0.1);
					glEnd();

					/* Texto (YOU WIN!) */
					glPushMatrix();
					glColor3f(R,G,B);
					glTranslatef(-5.5, 4.0, 0.0);
					glScalef(0.02,0.02,0.02);
					for (char* i="You Win!"; *i; i++)
						glutStrokeCharacter(GLUT_STROKE_ROMAN, *i);
					glPopMatrix();
				}
			} 
			/* Condicionales para que la barra no pise los margenes al crecer */
			if (X1 <= -9.1){
				X1 = -9.0;
				X2 = X1 + 3;
			} else if  (X2 >= 9.1){
				X2 = 9.0;
				X1 = X2 - 3;
			}
			glColor3f(0.0,0.0,1.0);
			glBegin(GL_LINES);
				glVertex2f(X1,-8.0);
				glVertex2f(X2,-8.0);
				glVertex2f(X1,-8.3);
				glVertex2f(X2,-8.3);
				glVertex2f(X1,-8.0);
				glVertex2f(X1,-8.3);
				glVertex2f(X2,-8.0);
				glVertex2f(X2,-8.3);
			glEnd();

		/* Si se pierde el juego */
		}  else {
			glColor3f(1.0,1.0,1.0);

			/* Ojo izquierdo */
			glBegin (GL_LINES);
				for (float angle = 0; angle <= 2*PI; angle = angle + 0.01)
					glVertex2f(-4+sinf(angle),cosf(angle)*0.7);
			glEnd();

			/* Ojo derecho */
			glBegin (GL_LINES);
				for (float angle = 0; angle <= 2*PI; angle = angle + 0.01)
					glVertex2f(4+sinf(angle),cosf(angle)*0.7);
			glEnd();
			glPushMatrix();
			glRotatef(90,0,0,1);

			/* Boca */
			glBegin (GL_LINES);
				for (float angle = 0; angle <= PI; angle = angle + 0.01) {
				glVertex2f( -4.5 + sinf(angle) * 2,cosf(angle) * 3);
				}
			glEnd();
			glPopMatrix();

			/* Iris izquierdo */
			glColor3f(R,G,B);
			glBegin (GL_LINES);
				for (float angle = 0; angle <= 2*PI; angle = angle + 0.01)
					glVertex2f(-4+sinf(angle)*0.1,-0.2+cosf(angle)*0.1);
			glEnd();

			/* Iris izquierdo */
			glBegin (GL_LINES);
				for (float angle = 0; angle <= 2*PI; angle = angle + 0.01)
					glVertex2f(4+sinf(angle)*0.1,-0.2+cosf(angle)*0.1);
			glEnd();

			/* Lagrima */
			glColor3f(0,0,1);
			glPushMatrix();
			glTranslatef(-4,-1.1,0);
			glRotatef(-90,0,0,1);
			glBegin (GL_LINES);
				for (float angle = 0; angle <= PI; angle = angle + 0.01)
					glVertex2f(sinf(angle)*0.3,cosf(angle)*0.3);
			glEnd();
			glPopMatrix();
			glBegin (GL_LINES);
					glVertex2f(-4-0.3,-1.1);
					glVertex2f(-4,-1.1+0.4);
					glVertex2f(-4+0.3,-1.1);
					glVertex2f(-4,-1.1+0.4);
			glEnd();

			/* Texto (GAME OVER!) */
			glPushMatrix();
			glColor3f(R,G,B);
            glTranslatef(-7.5, 4, 0);
			glScalef(0.02,0.02,0.02);
            for (char* i="Game Over!"; *i; i++)
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *i);
			glPopMatrix();
		}

	/* Dibujo de la Pelota */
	float radio = 0.2;
	glColor3f(1.0,1.0,1.0);
	glBegin (GL_LINE_LOOP);
		for (float angle = 0; angle <= PI * 2; angle = angle + 0.01) {
			glVertex2f(X + sin(angle) * radio, Y + cos(angle) * radio);
		}
	glEnd();

	/* Dibujo del Bono especial */
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

/*________________________________________________________________________
					REBOTE Y ACTUALIZACION
________________________________________________________________________*/

/*Se realiza un barrido de izquierda a derecha y de abajo hacia arriba 
por un area del tablero relativa a la posicion de la bola y se verifica
si la bola va a chocar con un ladrillo */

void barrerTablero(float minX,float maxX,float minY,float maxY){
	int entro = 0;
	for (float i = minX; i <= maxX; i = i + 2.5) {
		for (float j = minY; j <= maxY; j = j + 1.5) {
			/* Verifica si se esta estrellando contra lineas horizontales */
			if ((movX + X <= i + 1.5 + 0.2) && (movX + X >= i - 0.2)) {
				if (((movY + Y <= j + 0.2) && (movY + Y >= j - 0.2)) || ((movY + Y <= j - 0.5 + 0.2) && (movY + Y >= j - 0.5 - 0.2))) {
					entro = 1;
					for (int k = 0; k < 35;k++){
						if (matriz[k].posX == i && matriz[k].posY == j){
							if (matriz[k].esta == 0) {
								/* Verifica si es un bloque con bono */
								if (matriz[k].bonus == 1)  {
									matriz[k].bonus = 2;
									matriz[k].explota = 1;
									activarBono = 1;
									bonoX[numeroBono] = i + 0.75;
									bonoY[numeroBono] = j - 0.25;
									numeroBono++;
								}
								/* Verifica si es un bloque especial */
								if (matriz[k].doble == 1)  {
									matriz[k].doble = 2;
								} else {
									matriz[k].esta = 1;
									estar--;
									if (matriz[k].doble == 2){
										matriz[k].explota = 1;
										matriz[k].doble = 0;
									}
								}
								movY = movY*(-1);
								Y += movY;
							}
						}
					}
					
				}
			/* Verifica si se esta estrellando contra lineas verticales */
			} 
			if (((movY + Y <= j + 0.2) && (movY + Y >= (j - 0.2- 0.5))) && (entro == 0)) {
				  if (((movX + X) >= i - 0.2) && ((movX + X) <= (i+ 0.2 + 1.5))) {
					for (int k = 0; k < 35;k++){
						if (matriz[k].posX==i && matriz[k].posY==j){
							if (matriz[k].esta == 0) {
								/* Verifica si es un bloque con bono */
								if (matriz[k].bonus == 1)  {
									matriz[k].bonus = 2;
									activarBono = 1;
									bonoX[numeroBono] = i + 0.75;
									bonoY[numeroBono] = j - 0.5;
									numeroBono++;
								}
								/* Verifica si es un bloque especial */
								if (matriz[k].doble == 1)  {
									matriz[k].doble = 2;
								} else {
									matriz[k].doble = 0;
									matriz[k].esta = 1;
									estar--;
									matriz[k].explota = 1;
								}
								movX = movX*(-1);
								velX = velX*(-1);
								X += movX;
							}
						}
					} 
				}
			}
		}
	}
}

/* Funcion encargada de actualizar el movimiento de la pelota*/
void actualizar(int value) {
	/* Actualizacion de la caida de los bonos */
	for (int auxBono = 0; auxBono <= numeroBono; auxBono++) {
		if (((bonoY[auxBono] -= 0.1) <= -7.8) && (X1 <= (bonoX[auxBono]) && (bonoX[auxBono]) <= X2)) {
			int ranBono = rand() %100;
			bonoX[auxBono] = -17;
			bonoY[auxBono] = -17;
			if (ranBono > 50) {
				X1 -= 0.075;
				X2 += 0.075;
			} else {
				movX = movX + movX*0.1;
				velX = velX + velX*0.1;
				movY = movY + movY*0.1;
			}
		} else {
			bonoY[auxBono] -= 0.1;
		}
	}

	/* REBOTE CONTRA LOS LADRILLOS */
	/* Si corresponde, se llama la funcion de barrido para 
	verificar los posibles choques */
	if (movY + Y >= 0.0) {
		if (X + movX <= 0.75 && movY + Y <= 4)
			barrerTablero(-8.25,-0.75,1.0,4.0);
		else if (X + movX <= 0.75 && movY + Y > 4)
			barrerTablero(-8.25,-0.75,5.5,7.0);
		else if (X + movX > 0.75 && movY + Y <= 4)
			barrerTablero(1.75,8.0,1.0,4.0);
		else if (X + movX > 0.75 && movY + Y > 4)
			barrerTablero(1.75,8.0,5.5,7.0);
	}

	/* REBOTE CONTRA LOS MARGENES */

	/* Movimiento del eje Y */
	if (((Y + movY) < 8.8) && ((Y + movY) > -7.8)) {
		Y += movY;
	} 
	else{
		/* Cambia de direccion si:
				- Esta tocando el margen superior
				- Al llegar al margen inferior, esta tocando la barra */
		if (lost == 0 && ((((Y + movY) <= -7.8) && (X1 <= (X + movX) && (X + movX) <= X2)) || ((Y + movY) >= 8.8))) {
			movY = movY*(-1);
			Y += movY;
			if (Y+movY <= -7.8){
				if ((((X + movX) <= X1 + 0.6) && (movX > 0)) || ((((X + movX) >= X2 - 0.6)) && (movX < 0))) {
					movX = movX*(-1);
					velX = velX*(-1);
				}
				int perturbacion = rand() %2;
				if (perturbacion == 1) {
						movX = velX + 0.02;
				} else if (perturbacion == 0) {
						movX = velX - 0.02;
				}
				X += velX;
			}
		/* Se genera un color aleatorio */
		R = float(rand()%10)*0.1;
		G = float(rand()%10)*0.1;
		B = float(rand()%10)*0.1;

		/* Si no esta tocando la barra, se termina el juego */
		} else if (lost == 0 & ((Y + movY) <= -7.8) && ((Y + movY) > -9.0)) {
			Y += movY;
			lost = 1;
			for (int i=0;i<35;i++){
				if (matriz[i].esta == 0){
					matriz[i].bonus = 3;
					matriz[i].explota = 1;
					matriz[i].esta = 1;
					matriz[i].doble = 0;
				}
			}
			glutPostRedisplay();
			glutTimerFunc(25, actualizar, 0);
			tablero();
		} 
	}

	/* Movimiento del eje X */
	if (((X + movX) < 8.9) && ((X + movX) > -8.9)) {
		X += movX;
			glutPostRedisplay();
			glutTimerFunc(25, actualizar, 0);
	} else {
			R = float(rand()%10)*0.1;
			G = float(rand()%10)*0.1;
			B = float(rand()%10)*0.1;
			movX = movX*(-1);
			velX = velX*(-1);
			X += movX;
			glutPostRedisplay();
			glutTimerFunc(25, actualizar, 0);
	}
}

/*________________________________________________________________________
					FUNCIONES AUXILIARES
________________________________________________________________________*/
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

void SetVar(){
	if (generarRandom == 1){
		/* Limpiando los ladrillos de sus estados*/
		for (int limpiar = 0; limpiar < 35; limpiar++) {
			matriz[limpiar].doble = 0;
			matriz[limpiar].bonus = 0;
			matriz[limpiar].esta = 0;
			matriz[limpiar].explota = 0;
		}
		velX = 0.1;
		ladrillosAleatorios();
	}
	X1 = -1.5;
	X2 = 1.5;
	X = 0.0;
	Y = -7.8;
	movY = -0.1;
	movX = 0.1;
	tablero();
}

/* Funcion encargada de detectar los movimientos del jugador en el teclado */
void detectarTeclado (unsigned char key, int x, int y) {
	switch (key) {
		case'a':
			if (lost != 1) {
				if (X1 > -8.9) {
					X1 -= 0.5;
					X2 -= 0.5;
				}
			}
		break;
		case'A':
		if (lost != 1) {
			if (X1 > -8.9) {
				X1 -= 0.5;
				X2 -= 0.5;
			}
		}
		break;
		case's':
			if (lost != 1) {
				if (X2 < 8.9) {
					X1 += 0.5;
					X2 += 0.5;
				}
			}
		break;
		case'S':
			if (lost != 1) {
				if (X2 < 8.9) {
					X1 += 0.5;
					X2 += 0.5;
				}
			}
		break;
		case'1':
			if (lost == 1) {
				movX = 0.1;
				movY = -0.1;
				SetVar();
			}
			break;
	}
	glutPostRedisplay();
	tablero();
}

void render(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	//Suaviza las lineas
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );

	if (generarRandom == 1){
        /* Limpiando los ladrillos de sus estados*/
        for (int limpiar = 0; limpiar < 35; limpiar++) {
                matriz[limpiar].doble = 0;
                matriz[limpiar].bonus = 0;
                matriz[limpiar].esta = 0;
                matriz[limpiar].explota = 0;
        }
        ladrillosAleatorios();
	}
	tablero();

	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);

	glutSwapBuffers();
}

int main (int argc, char** argv) {
	srand(time(NULL));
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

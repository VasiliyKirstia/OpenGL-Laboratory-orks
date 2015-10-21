#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

//инициализируем параметры материалов и источника света
void init (void){
	/* включаем освещение и источник света */
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float pos[4] = {1000, 1000, 1000, 1},
		  dir[3] = {-1, -1, -1};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
}

void display_axes(GLfloat length, GLfloat radius){
	GLUquadricObj *pQuad = gluNewQuadric();
	gluQuadricDrawStyle(pQuad, GLU_FILL);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glTranslatef(.0f, .0f, .0f);

	//x
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glColor3f(1, 0, 0);
	gluCylinder(pQuad, 2*radius, 2*radius, length, 100, 100);
	glTranslatef(.0f, .0f, length);
	gluCylinder(pQuad, 2*radius, .0f, length*0.05, 10, 10);
	glPopMatrix();

	//y
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glColor3f(0, 1, 0);
	gluCylinder(pQuad, 2*radius, 2*radius, length, 100, 100);
	glTranslatef(.0f, .0f, length);
	gluCylinder(pQuad, 2*radius, .0f, length*0.05, 10, 10);
	glPopMatrix();

	//z
	glPushMatrix();
	glColor3f(0, 0, 1);
	gluCylinder(pQuad, 2*radius, 2*radius, length, 100, 100);
	glTranslatef(.0f, .0f, length);
	gluCylinder(pQuad, 2*radius, .0f, length*0.05, 10, 10);
	glPopMatrix();

	glPopMatrix();
}

void display_snowman(GLfloat x, GLfloat y, GLfloat z, GLfloat angle, GLfloat size){
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glColor3f(1.f,1.f,1.f);

	glTranslatef(x, y+50*size, z);
	glRotatef(angle, 0, 1, 0);
	glutSolidSphere(50*size, 100, 100);

	glTranslatef(0, (40 + 35)*size, 0);
	glutSolidSphere(35*size, 100, 100);

	glPushMatrix();
	glTranslatef((30 + 15)*size, 0, 0);
	glutSolidSphere(15*size, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-(30 + 15)*size, 0, 0);
	glutSolidSphere(15*size, 100, 100);
	glPopMatrix();

	glTranslatef(0, (30 + 20)*size, 0);
	glutSolidSphere(20*size, 100, 100);

	glColor3f(.0f, .0f, .0f);
	glPushMatrix();
	glTranslatef(size*20*cosf(3.14/4.0), size*20*sinf(3.14/4.0), size*20*sinf(3.14/4.0));
	glutSolidSphere(5*size, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(size*20*cosf(3*3.14/4.0), size*20*sinf(3.14/4.0), size*20*sinf(3*3.14/4.0));
	glutSolidSphere(5*size, 100, 100);
	glPopMatrix();

	glColor3f(1.f, .2f, .2f);
	glPushMatrix();
	glTranslatef(size*20*cosf(3.14/2.0), 0, size*20*sinf(3.14/2.0));
	glutSolidCone(5*size, 20*size, 100, 100);
	glPopMatrix();

	glColor3f(.2f, .2f, 1.f);
	glTranslatef(0, 18 * size, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(30 * size, 10*size, 100, 100);

	glPopMatrix();
}

// функция вызывается при необходимости перерисовки изображения
// в ней осуществляется весь вывод геометрии
void display(void){
	// очищаем буфер кадра и буфер глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	display_axes(200.0f, 1.5f);
	display_snowman(100, 0, 100, 45, 1);

	/* выводим сцену на экран */
	glFlush();
}

// Вызывается при изменении пользователем размеров окна
void reshape(int w, int h){
	//устанавливаем размер области вывода равным размеру окна
	glViewport(0, 0, w, h);

	//задаем матрицу проекций с учетом размера окна
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w/2.0, w/2.0, -h/2.0, h/2.0, -500, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		100.0f, 10.0f, 100.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	);
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 27:
			exit(0);
		break;
	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(
		GLUT_SINGLE |
		GLUT_RGB |
		GLUT_DEPTH
	);
	glutInitWindowSize(500, 500);
	glutCreateWindow(argv[0]);

	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

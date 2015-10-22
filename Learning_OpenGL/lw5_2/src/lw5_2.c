#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

const float d_theta = 0.1;
const float d_phi = 0.1;
const float pi = 3.14;

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

void display_chunk(GLfloat hight, GLuint fragmentation){
	auto float R = hight/2.0;
	glColor3f(1 , 1, 1);
	glBegin(GL_TRIANGLE_STRIP);
	for(float phi = 3.14/2.0 + d_phi/2.0; phi <= 3*pi/2.0 - d_phi/2.0; phi+=d_phi){
		int s = 1;
		for(float theta = 0; theta <= 2*pi/(fragmentation*1.0); theta+= 0.01){
			glVertex3f(R*cosf(phi - s*d_phi/2.0)*cosf(theta), R*sinf(phi - s*d_phi/2.0), R*cosf(phi - s*d_phi/2.0)*sinf(theta));
			s*=-1;
		}
	}
	glEnd();

}

// функция вызывается при необходимости перерисовки изображения
// в ней осуществляется весь вывод геометрии
void display(void){
	// очищаем буфер кадра и буфер глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	display_axes(200.0f, 1.5f);
	display_chunk(200,4);

	/* выводим сцену на экран */
	glutSwapBuffers();
	glutPostRedisplay();
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
		100.0f, 50.0f, 100.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	);
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 27:
			exit(0);
		break;
		case 97: //a
			glRotatef(10, 0, 1, 0);
			break;
		case 100: //d
			glRotatef(-10, 0, 1, 0);
			break;
		case 119://w
			glRotatef(-10, 1, 0, 0);
			break;
		case 115: //s
			glRotatef(10, 1, 0, 0);
			break;
		default:
			printf("%d",key);
		break;

	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(
		GLUT_DOUBLE |
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

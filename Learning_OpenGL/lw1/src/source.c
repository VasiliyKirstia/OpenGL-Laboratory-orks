#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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

	float pos[4] = {3, 3, 3, 1},
		  dir[3] = {-1, -1, -1};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
}

void display_axes(void){}

// функция вызывается при необходимости перерисовки изображения
// в ней осуществляется весь вывод геометрии
void display(void){
	// очищаем буфер кадра и буфер глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotatef(20.0, 1.0, 0.0, 0.0);

	//отображаем тор
	glColor3ub(80, 70, 200);

	glPushMatrix();
	glTranslatef( -0.75, 0.5, 0.0);
	glRotatef( 90.0, 1.0, 0.0, 0.0 );
	glutSolidTorus(0.275, 0.85, 15, 15);
	glPopMatrix();

	//отображаем конус
	glColor3ub(100, 30, 0);

	glPushMatrix();
	glTranslatef( -0.75, -0.5, 0.0);
	glRotatef( 270.0, 1.0, 0.0, 0.0 );
	glutSolidCone(1.0, 2.0, 15, 15);
	glPopMatrix();

	//отображаем шар
	glColor3ub(0, 100, 100);

	glPushMatrix();
	glTranslatef( 0.75, 0.0, -1.0);
	glutSolidSphere(1.0, 15, 15);
	glPopMatrix();

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
	gluPerspective(
		40.0,
		(GLfloat)w/h,
		1, 100.0
	);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0.0f, 0.0f, 8.0f,
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

#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define NODES_COUNT 11

typedef struct{
	GLfloat Length;
	GLfloat Side;
	GLfloat Angle;
}Node;

typedef struct {
	Node Shoulder;
	Node Forearm;
	Node Wrist;
	//fingers
	Node f0_node0;
	Node f0_node1;

	Node f1_node0;
	Node f1_node1;

	Node f2_node0;
	Node f2_node1;

	Node f3_node0;
	Node f3_node1;

	Node *nodes[NODES_COUNT];
	size_t current_node_index;
}Arm;

static Arm arm;
static GLfloat activeNodeColor[3] = {1.0f, 0.0f, 0.0f};
static GLfloat NodeColor[3] = {1.0f, 1.0f, 1.0f};

void init_arm(void){
	arm.Forearm.Length = 50.0f;
	arm.Forearm.Side = 10.0f;
	arm.Forearm.Angle = 0.0f;

	arm.Shoulder.Length = 50.0f;
	arm.Shoulder.Side = 10.0f;
	arm.Shoulder.Angle = 0.0f;

	arm.Wrist.Length = 2.0f;
	arm.Wrist.Side = 15.0f;
	arm.Wrist.Angle = 0.0f;

	arm.f0_node0.Length = 7.0f;
	arm.f0_node0.Side = 2.0f;
	arm.f0_node0.Angle = 0.0f;

	arm.f0_node1.Length = 5.0f;
	arm.f0_node1.Side = 2.0f;
	arm.f0_node1.Angle = 0.0f;

	arm.f1_node0.Length = 7.0f;
	arm.f1_node0.Side = 2.0f;
	arm.f1_node0.Angle = 0.0f;

	arm.f1_node1.Length = 5.0f;
	arm.f1_node1.Side = 2.0f;
	arm.f1_node1.Angle = 0.0f;

	arm.f2_node0.Length = 7.0f;
	arm.f2_node0.Side = 2.0f;
	arm.f2_node0.Angle = 0.0f;

	arm.f2_node1.Length = 5.0f;
	arm.f2_node1.Side = 2.0f;
	arm.f2_node1.Angle = 0.0f;

	arm.f3_node0.Length = 7.0f;
	arm.f3_node0.Side = 2.0f;
	arm.f3_node0.Angle = 0.0f;

	arm.f3_node1.Length = 5.0f;
	arm.f3_node1.Side = 2.0f;
	arm.f3_node1.Angle = 0.0f;

	arm.nodes[0] = &arm.Shoulder;
	arm.nodes[1] = &arm.Forearm;
	arm.nodes[2] = &arm.Wrist;
	arm.nodes[3] = &arm.f0_node0;
	arm.nodes[4] = &arm.f0_node1;
	arm.nodes[5] = &arm.f1_node0;
	arm.nodes[6] = &arm.f1_node1;
	arm.nodes[7] = &arm.f2_node0;
	arm.nodes[8] = &arm.f2_node1;
	arm.nodes[9] = &arm.f3_node0;
	arm.nodes[10] = &arm.f3_node1;

	arm.current_node_index = 0;
}

void setNodeColor(size_t node_index){
	if( node_index == arm.current_node_index )
		glColor3fv(activeNodeColor);
	else
		glColor3fv(NodeColor);
}

void wireBox(GLfloat length, GLfloat side){
	glBegin(GL_LINES);

	glVertex3f(0.0f, side/2.0, side/2.0);
	glVertex3f(0.0f, side/2.0, -side/2.0);
	glVertex3f(0.0f, side/2.0, -side/2.0);
	glVertex3f(0.0f, -side/2.0, -side/2.0);
	glVertex3f(0.0f, -side/2.0, -side/2.0);
	glVertex3f(0.0f, -side/2.0, side/2.0);
	glVertex3f(0.0f, -side/2.0, side/2.0);
	glVertex3f(0.0f, side/2.0, side/2.0);
	glVertex3f(length, -side/2.0, side/2.0);
	glVertex3f(length, -side/2.0, -side/2.0);
	glVertex3f(length, -side/2.0, -side/2.0);
	glVertex3f(length, side/2.0, -side/2.0);
	glVertex3f(length, side/2.0, -side/2.0);
	glVertex3f(length, side/2.0, side/2.0);
	glVertex3f(length, side/2.0, side/2.0);
	glVertex3f(length, -side/2.0, side/2.0);
	glVertex3f(0.0f, side/2.0, side/2.0);
	glVertex3f(length, side/2.0, side/2.0);
	glVertex3f(0.0f, side/2.0, -side/2.0);
	glVertex3f(length, side/2.0, -side/2.0);
	glVertex3f(0.0f, -side/2.0, -side/2.0);
	glVertex3f(length, -side/2.0, -side/2.0);
	glVertex3f(0.0f, -side/2.0, side/2.0);
	glVertex3f(length, -side/2.0, side/2.0);

	glEnd();
}

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

void display_arm(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(100, 100, 100);
	glRotatef(arm.Shoulder.Angle, 0, 0, 1);
	setNodeColor(0);
	wireBox(arm.Shoulder.Length, arm.Shoulder.Side);

	glTranslatef(arm.Shoulder.Length, 0, 0);
	glRotatef(arm.Forearm.Angle, 0, 0, 1);
	setNodeColor(1);
	wireBox(arm.Forearm.Length, arm.Forearm.Side);

	glTranslatef(arm.Forearm.Length, 0, 0);
	glRotatef(arm.Wrist.Angle, 1, 0, 0);
	setNodeColor(2);
	wireBox(arm.Wrist.Length, arm.Wrist.Side);

	glPushMatrix();
	glTranslatef(arm.Wrist.Length, -arm.Wrist.Side/2.0 + arm.f0_node0.Side/2.0, 0);
	glRotatef(arm.f0_node0.Angle, 0, 0, 1);
	setNodeColor(3);
	wireBox(arm.f0_node0.Length, arm.f0_node0.Side);

	glTranslatef(arm.f0_node0.Length, 0, 0);
	glRotatef(arm.f0_node1.Angle, 0, 0, 1);
	setNodeColor(4);
	wireBox(arm.f0_node1.Length, arm.f0_node1.Side);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(arm.Wrist.Length, arm.Wrist.Side/2.0 - arm.f1_node0.Side/2.0, -arm.Wrist.Side/2.0 + arm.f1_node0.Side/2.0);
	glRotatef(-arm.f1_node0.Angle, 0, 0, 1);
	setNodeColor(5);
	wireBox(arm.f1_node0.Length, arm.f1_node0.Side);

	glTranslatef(arm.f1_node0.Length, 0, 0);
	glRotatef(-arm.f1_node1.Angle, 0, 0, 1);
	setNodeColor(6);
	wireBox(arm.f1_node1.Length, arm.f1_node1.Side);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(arm.Wrist.Length, arm.Wrist.Side/2.0 - arm.f2_node0.Side/2.0, 0);
	glRotatef(-arm.f2_node0.Angle, 0, 0, 1);
	setNodeColor(7);
	wireBox(arm.f2_node0.Length, arm.f2_node0.Side);

	glTranslatef(arm.f2_node0.Length, 0, 0);
	glRotatef(-arm.f2_node1.Angle, 0, 0, 1);
	setNodeColor(8);
	wireBox(arm.f2_node1.Length, arm.f2_node1.Side);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(arm.Wrist.Length, arm.Wrist.Side/2.0 - arm.f3_node0.Side/2.0, arm.Wrist.Side/2.0 - arm.f3_node0.Side/2.0);
	glRotatef(-arm.f3_node0.Angle, 0, 0, 1);
	setNodeColor(9);
	wireBox(arm.f3_node0.Length, arm.f3_node0.Side);

	glTranslatef(arm.f3_node0.Length, 0, 0);
	glRotatef(-arm.f3_node1.Angle, 0, 0, 1);
	setNodeColor(10);
	wireBox(arm.f3_node1.Length, arm.f3_node1.Side);
	glPopMatrix();

	glPopMatrix();
}

// функция вызывается при необходимости перерисовки изображения
// в ней осуществляется весь вывод геометрии
void display(void){
	// очищаем буфер кадра и буфер глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	display_axes(200.0f, 1.5f);
	display_arm();

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
		case 32:
			arm.current_node_index = (arm.current_node_index + 1)%NODES_COUNT;
			break;
		case 43:
			arm.nodes[arm.current_node_index]->Angle += 5;
			break;
		case 45:
			arm.nodes[arm.current_node_index]->Angle -= 5;
			break;
		default:
			printf("%d",key);
		break;

	}
}

int main(int argc, char** argv){
	init_arm();

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

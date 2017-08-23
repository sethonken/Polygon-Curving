#include "GL/freeglut.h"
#include <algorithm>
using namespace std;

#define DEFINE_POINTS	 1
#define CUT_MODE		 2
#define CLEAR_ALL		 3
#define POINT_MAX		 100000

//structs
struct Point {
	double x = 0.0;
	double y = 0.0;
};

//global variables
int width = 600;
int height = 600;
int pointNumber = 0;
int originalNumber = 0;
bool leftClicked = false;
int clickFunction = DEFINE_POINTS;
Point points1[POINT_MAX];
Point points2[POINT_MAX];
Point original[POINT_MAX];
int pointSwitch = 1;
double q = 0.3333;

//functions
static void display();
void processMenuEvents(int option);

void createGLUTMenus() {
	int menu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Cut Mode", CUT_MODE);
	glutAddMenuEntry("Clear All", CLEAR_ALL);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void processMenuEvents(int option) {
	switch (option) {
	case CLEAR_ALL:
		clickFunction = DEFINE_POINTS;
		pointNumber = 0;
		originalNumber = 0;
		pointSwitch = 1;
		display();
		break;
	case CUT_MODE:
		clickFunction = CUT_MODE;
		break;
	}
}

static void mouse_press(int button, int state, int x, int y) {
	double widthd = width;
	double heightd = height;
	double pointx = (x / widthd) * 2 - 1;
	double pointy = ((heightd - y) / heightd) * 2 - 1;

	if (button == 0 && state == 1) { //left button released
		switch (clickFunction) {
		case DEFINE_POINTS:
			if (pointNumber > POINT_MAX - POINT_MAX / 2.0) {
				break;
			}

			points1[pointNumber].x = pointx;
			points1[pointNumber].y = pointy;
			original[pointNumber].x = pointx;
			original[pointNumber].y = pointy;

			pointNumber++;
			originalNumber++;
			break;
		case CUT_MODE:
			if (pointNumber > POINT_MAX - POINT_MAX / 2.0) {
				break;
			}
			int newMax = 0;
			if (pointSwitch == 1) {
				for (int i = 0; i < pointNumber; i++) {
					double pointSlope;
					Point OGpoint1 = points1[i];
					Point OGpoint2;
					if (i == pointNumber - 1) {
						OGpoint2 = points1[0];
					}
					else {
						OGpoint2 = points1[i + 1];
					}

					//q from current to next
					if (OGpoint1.x > OGpoint2.x) {
						points2[newMax].x = OGpoint1.x - q * abs(OGpoint1.x - OGpoint2.x);
					}
					else {
						points2[newMax].x = OGpoint1.x + q * abs(OGpoint1.x - OGpoint2.x);
					}
					if (OGpoint1.y > OGpoint2.y) {
						points2[newMax].y = OGpoint1.y - q * abs(OGpoint1.y - OGpoint2.y);
					}
					else {
						points2[newMax].y = OGpoint1.y + q * abs(OGpoint1.y - OGpoint2.y);
					}

					newMax++;

					//1-q from current to next
					if (OGpoint1.x > OGpoint2.x) {
						points2[newMax].x = OGpoint1.x - (1 - q) * abs(OGpoint1.x - OGpoint2.x);
					}
					else {
						points2[newMax].x = OGpoint1.x + (1 - q) * abs(OGpoint1.x - OGpoint2.x);
					}
					if (OGpoint1.y > OGpoint2.y) {
						points2[newMax].y = OGpoint1.y - (1 - q) * abs(OGpoint1.y - OGpoint2.y);
					}
					else {
						points2[newMax].y = OGpoint1.y + (1 - q) * abs(OGpoint1.y - OGpoint2.y);
					}

					newMax++;
				}
				pointSwitch = 2;
			}
			else {
				for (int i = 0; i < pointNumber; i++) {
					double pointSlope;
					Point OGpoint1 = points2[i];
					Point OGpoint2;
					if (i == pointNumber - 1) {
						OGpoint2 = points2[0];
					}
					else {
						OGpoint2 = points2[i + 1];
					}

					//q from current to next
					if (OGpoint1.x > OGpoint2.x) {
						points1[newMax].x = OGpoint1.x - q * abs(OGpoint1.x - OGpoint2.x);
					}
					else {
						points1[newMax].x = OGpoint1.x + q * abs(OGpoint1.x - OGpoint2.x);
					}
					if (OGpoint1.y > OGpoint2.y) {
						points1[newMax].y = OGpoint1.y - q * abs(OGpoint1.y - OGpoint2.y);
					}
					else {
						points1[newMax].y = OGpoint1.y + q * abs(OGpoint1.y - OGpoint2.y);
					}

					newMax++;

					//1-q from current to next
					if (OGpoint1.x > OGpoint2.x) {
						points1[newMax].x = OGpoint1.x - (1 - q) * abs(OGpoint1.x - OGpoint2.x);
					}
					else {
						points1[newMax].x = OGpoint1.x + (1 - q) * abs(OGpoint1.x - OGpoint2.x);
					}
					if (OGpoint1.y > OGpoint2.y) {
						points1[newMax].y = OGpoint1.y - (1 - q) * abs(OGpoint1.y - OGpoint2.y);
					}
					else {
						points1[newMax].y = OGpoint1.y + (1 - q) * abs(OGpoint1.y - OGpoint2.y);
					}

					newMax++;
				}
				pointSwitch = 1;
			}
			pointNumber = newMax;
			break;
		}
		display();
	}
}

static void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	//original shape
	//connect points
	glLineWidth(2);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < originalNumber; i++) {
		glVertex2d(original[i].x, original[i].y);
	}
	glEnd();

	//draw points
	glPointSize(2);
	for (int i = 0; i < originalNumber; i++) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POINTS);
		glVertex2d(original[i].x, original[i].y);
		glEnd();
	}

	//cut shape
	//connect points
	glLineWidth(4);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < pointNumber; i++) {
		if (pointSwitch == 1) {
			glVertex2d(points1[i].x, points1[i].y);
		}
		else {
			glVertex2d(points2[i].x, points2[i].y);
		}
	}
	glEnd();

	//SwapBuffers
	glutSwapBuffers();
}

void reshape(int w, int h) {
	width = w;
	height = h;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}


void init(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float s = width / 2.0;
	if (height < width) s = height / 2;
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	glutInit(&__argc, __argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	//g window
	glutInitWindowSize(width, height);
	glutInitWindowPosition(800, 100);
	glutCreateWindow("Point Finger Exercise");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	//mouse callbacks
	createGLUTMenus();
	glutMouseFunc(mouse_press);

	glutMainLoop();
	return 0;
}
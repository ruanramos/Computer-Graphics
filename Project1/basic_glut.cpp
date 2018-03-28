/*

Program by Ruan da Fonseca Ramos, student from Federal University of Rio de Janeiro
Persuing computer science bachelor degree
Made this project for graphic computer discipline


 USE glEnd(GL_) to start a drawing. Do not forget glEnd()

GL_POINTS			Draws points on screen.Every vertex specified is a point.
GL_LINES			Draws lines on screen.Every two vertices specified compose a line.
GL_LINE_STRIP		Draws connected lines on screen.Every vertex specified after first two are connected.
GL_LINE_LOOP		Draws connected lines on screen.The last vertex specified is connected to first vertex.
GL_TRIANGLES		Draws triangles on screen.Every three vertices specified compose a triangle.
GL_TRIANGLE_STRIP	Draws connected triangles on screen.Every vertex specified after first three vertices creates a triangle.
GL_TRIANGLE_FAN		Draws connected triangles like GL_TRIANGLE_STRIP, except draws triangles in fan shape.
GL_QUADS			Draws quadrilaterals(4 – sided shapes) on screen.Every four vertices specified compose a quadrilateral.
GL_QUAD_STRIP		Draws connected quadrilaterals on screen.Every two vertices specified after first four compose a connected quadrilateral.
GL_POLYGON			Draws a polygon on screen.Polygon can be composed of as many sides as you want.

*/

#include <GL\freeglut.h>
#include <iostream>
#include <algorithm>
#include <vector>

#define PI 3.14159265358979323846
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_X_POSITION 100
#define WINDOW_Y_POSITION 100

#define RED 1
#define GREEN 2
#define BLUE 3
#define ORANGE 4

using namespace std;

// STRUCTS DEFINITIONS

typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} tPoint;

typedef struct {
	tPoint p1;
	tPoint p2;
} tLine2D;

typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} tVector3D;

typedef struct {
	double a;
	double b;
} tLineFunction2D;

typedef struct {
	GLfloat red;
	GLfloat green;
	GLfloat blue;
} colorRGB;

typedef struct {
	vector<tLine2D> sides;
} tPolygon;

// END OF STRUCTS DEFINITIONS

void Render(void);

void Keyboard(unsigned char c, int x, int y);

void Mouse(int button, int state, int x, int y);

// START OF MENU FUNCTIONS

void CreateGLUTMenus();

void ProcessMenuEvents(int option);

// END OF MENU FUNCTIONS

// DRAWING FUNCTIONS SIGNATURES

void drawAxis();

void DrawVector3D(tVector3D vector);

void DrawLine3D(tPoint a, tPoint b);
void DrawLine3D(tLine2D line);

void DrawVector3D(tVector3D vector);

void DrawPolygon(tPolygon polygon);

// END OF DRAWING FUNCTIONS SIGNATURES

// MATH FUNCTIONS WITH VECTORS SIGNATURES

tPolygon InstantiatePolygon(int n);

GLfloat mouseToWindowCoordinateX(GLfloat mx);

GLfloat mouseToWindowCoordinateY(GLfloat yx);

tVector3D SumVectors(tVector3D vector1, tVector3D vector2);

double DotProduct3D(tVector3D vector1, tVector3D vector2);

tVector3D CrossProduct(tVector3D vector1, tVector3D vector2);

double Norm(tVector3D vector);

bool LinesIntersect2D(tLine2D AB, tLine2D CD);
tPoint LinesIntersect2D(tLineFunction2D func1, tLineFunction2D func2);
bool LinesIntersect2DBool(tLineFunction2D func1, tLineFunction2D func2);

bool IsOrthogonal3D(tVector3D vector1, tVector3D vector2);

tVector3D ScalarMultiplication(tVector3D vector, double n);

tVector3D InverseVector(tVector3D vector, const char axis);

tVector3D NormalizeVector(tVector3D vector);

tLineFunction2D GetLineEquation2D(tPoint p1, tPoint p2);
tLineFunction2D GetLineEquation2D(tLine2D line);

bool PointInsidePolygon(tPoint point, tPolygon polygon);

// END OF MATH FUNCTIONS WITH VECTORS SIGNATURES

// GLOBAL VARIABLES

tPoint origin = { 0, 0, 0 };
tVector3D up = { 0,1,0 };
tVector3D down = { 0,-1,0 };
tVector3D left = { -1,0,0 };
tVector3D right = { 1,0,0 };

vector<tPoint> mouseCoordinates;


// END OF GLOBAL VARIABLES

int main(int argc, char** argv) {
	// glut initiation and window creation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(WINDOW_X_POSITION, WINDOW_Y_POSITION);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Simple Glut Application");

	// Register Callbacks
	glutDisplayFunc(Render); // displayFunc me diz qual a função que vai ser usada para desenhar na tela
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	
	CreateGLUTMenus();

	// call the main loop
	glutMainLoop(); // isso quer dizer que terminei a inicialização e posso começar a renderizar
	return 0;
}

// KEYBOARD AND MOUSE FUNCTIONS

void Keyboard(unsigned char c, int x, int y) {
	if (c == 27) { // esc button = 27
		exit(0);
	}
	if (c == 'c') { // c button clears the screen and reset index of points vector
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawAxis();
		mouseCoordinates.clear();
		glutSwapBuffers();
	}
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		GLfloat mx = GLfloat(x), my = GLfloat(y);
		tPoint p = { mouseToWindowCoordinateX(mx), mouseToWindowCoordinateY(my) };
		mouseCoordinates.push_back(p);
	}
}


// This function creates a menu on the screen when we right click
void CreateGLUTMenus() {
	int menu;
	
	// create the menu and tell glut that "ProcessMenuEvents" will handle the events
	menu = glutCreateMenu(ProcessMenuEvents);

	// add entries to menu
	glutAddMenuEntry("Clear Screen", 1);
	glutAddMenuEntry("End Program", 2);


	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// functions that process the menu options
void ProcessMenuEvents(int option) {
	switch (option) {
		case 1:
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			drawAxis();
			mouseCoordinates.clear();
			glutSwapBuffers();
			break;
		case 2:
			exit(0);
	}
}

// END OF KEYBOARD AND MOUSE FUNCTIONS

// DRAWING FUNCTIONS

void drawAxis() {
	tPoint a, b, d, e;
	a = { GLfloat(-10), GLfloat(0), GLfloat(0) };
	b = { GLfloat(10), GLfloat(0), GLfloat(0) };
	d = { GLfloat(0), GLfloat(-10), GLfloat(0) };
	e = { GLfloat(0), GLfloat(10), GLfloat(0) };
	DrawLine3D(a, b);
	DrawLine3D(d, e);
}

void DrawLine3D(tPoint a, tPoint b) {
	glBegin(GL_LINES);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glEnd();
}

void DrawLine3D(tLine2D line) {
	glBegin(GL_LINES);
	glVertex3f(line.p1.x, line.p1.y, line.p1.z);
	glVertex3f(line.p2.x, line.p2.y, line.p2.z);
	glEnd();
}

void DrawLine3D(tLineFunction2D func) {
	glBegin(GL_LINE_STRIP);
	glVertex2f(GLfloat(-1), GLfloat(func.a * -1 + func.b));
	glVertex2f(GLfloat(1), GLfloat(func.a * 1 + func.b));
	glEnd();
}


void DrawVector3D(tVector3D vector) {
	DrawLine3D(origin, { vector.x, vector.y, vector.z });
}

// Draws the polygon given on the screen
void DrawPolygon(tPolygon polygon) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < polygon.sides.size(); i++) {
		glVertex2f(mouseCoordinates[i].x, mouseCoordinates[i].y);
	}
	glEnd();
}

// END OF DRAWING FUNCTIONS

// MATH FUNCTIONS FOR VECTORS

// Instantiate a polygon given the number of sides on a variable of type tPolygon 
tPolygon InstantiatePolygon(int n) {
	tPolygon p;
	tLine2D line;
	for (int i = 0; i < n; i++) {
		line.p1 = mouseCoordinates[i];
		line.p2 = mouseCoordinates[(i + 1) % n];
		glVertex2f(mouseCoordinates[i].x, mouseCoordinates[i].y);
		p.sides.push_back(line);
	}
	return p;
}

// transform mouse coordinates to window coordinates
GLfloat mouseToWindowCoordinateX(GLfloat mx) {
	return (mx / (WINDOW_WIDTH / 2) - 1);

}

GLfloat mouseToWindowCoordinateY(GLfloat my) {
	return (-((my / (WINDOW_HEIGHT / 2)) - 1));
}

// sum 2 vectors
tVector3D SumVectors(tVector3D vector1, tVector3D vector2) {
	tVector3D resultantVector;
	resultantVector.x = vector1.x + vector2.x;
	resultantVector.y = vector1.y + vector2.y;
	resultantVector.z = vector1.z + vector2.z;
	return resultantVector;
}

// dot product between 2 vectors
double DotProduct3D(tVector3D vector1, tVector3D vector2) {
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

// cross product between 2 vectors
tVector3D CrossProduct(tVector3D vector1, tVector3D vector2) {
	tVector3D v = {
		vector1.y * vector2.z - vector1.z * vector2.y,
		vector1.z * vector2.x - vector1.x * vector2.y,
		vector1.x * vector2.y - vector1.y * vector2.x
	};
	return v;
}

// check if 2 vectors are orthogonal
bool IsOrthogonal3D(tVector3D vector1, tVector3D vector2) {
	return DotProduct3D(vector1, vector2) == 0;
}

// calculates the Norm of a vector
double Norm(tVector3D vector) {
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
}

// calculates the distance between 2 vectors
double Distance(tVector3D vector1, tVector3D vector2) {
	return Norm(SumVectors(vector1, ( ScalarMultiplication(vector2, -1) )));
}

// calculate the angle between 2 vectors
double AngleBetweenVectors(tVector3D vector1, tVector3D vector2) {
	double cosTheta = DotProduct3D(vector1, vector2) / (Norm(vector1) * Norm(vector2));
	double val = 180.0 / PI;
	return acos(cosTheta) * val;
}

// calculates scalar multiplication between a double number and a vector
tVector3D ScalarMultiplication(tVector3D vector, double n) {
	tVector3D v = vector;
	v.x *= GLfloat(n);
	v.y *= GLfloat(n);
	v.z *= GLfloat(n);
	return v;
}

// invert the vector coordinate given an axis to invert
tVector3D InverseVector(tVector3D vector, const char axis) {
	tVector3D v;
	v = vector;
	if (axis == 'x') {
		v.x *= -1;
	}
	else if (axis == 'y') {
		v.y *= -1;
	}
	else if (axis == 'z') {
		v.y *= -1;
	}
	return v;
}

// normalizes a vector so it has Norm = 1
tVector3D NormalizeVector(tVector3D vector) {
	GLfloat norm = GLfloat(Norm(vector));
	vector.x = vector.x / norm;
	vector.y = vector.y / norm;
	vector.z = vector.z / norm;
	return vector;
}

// checks if 2 lines intersect and return true or false
bool LinesIntersect2D(tLine2D AB, tLine2D CD) {
	tVector3D ab, ac, ad, cd, ca, cb;
	
	ab = { AB.p2.x - AB.p1.x , AB.p2.y - AB.p1.y , 0 };
	ac = { CD.p1.x - AB.p1.x, CD.p1.y - AB.p1.y, 0 };
	ad = { CD.p2.x - AB.p1.x, CD.p2.y - AB.p1.y, 0 };
	cd = { CD.p2.x - CD.p1.x, CD.p2.y - CD.p1.y, 0 };
	ca = { AB.p1.x - CD.p1.x, AB.p1.y - CD.p1.y, 0 };
	cb = { AB.p2.x - CD.p1.x, AB.p2.y - CD.p1.y, 0 };

	return (DotProduct3D(CrossProduct(ab, ac), CrossProduct(ab, ad)) < 0 && DotProduct3D(CrossProduct(cd, ca), CrossProduct(cd, cb)) < 0);
}

// check if 2 lines intercept given their equations and return the point
tPoint LinesIntersect2D(tLineFunction2D func1, tLineFunction2D func2) {
	tPoint p;
	p.x = GLfloat((func2.b - func1.b)) / GLfloat(func1.a - func2.a);
	p.y = GLfloat(func1.a) * GLfloat(p.x) + GLfloat(func1.b);
	p.z = 0;
	return p;
}
// check if 2 lines intercept given their equations and return true or false
bool LinesIntersect2DBool(tLineFunction2D func1, tLineFunction2D func2) {
	tPoint p;
	p.x = GLfloat((func2.b - func1.b)) / GLfloat(func1.a - func2.a);
	p.y = GLfloat(func1.a) * GLfloat(p.x) + GLfloat(func1.b);
	p.z = 0;
	if (p.x != NULL) return true;
	else return false;
}

// gets a line equation given 2 points
tLineFunction2D GetLineEquation2D(tPoint p1, tPoint p2) {
	double a, b;
	tLineFunction2D func;
	// y - y0 = a(x - x0)
	// y = ax + b
	a = (p2.y - p1.y) / (p2.x - p1.x);
	b = p2.y - a * p2.x;
	func.a = a;
	func.b = b;
	return func;
}

// gets a line equation given the line
tLineFunction2D GetLineEquation2D(tLine2D line) {
	double a, b;
	tLineFunction2D func;
	// y - y0 = a(x - x0)
	// y = ax + b
	a = (line.p2.y - line.p1.y) / (line.p2.x - line.p1.x);
	b = line.p2.y - a * line.p2.x;
	func.a = a;
	func.b = b;
	return func;
}

// checks if a given point is inside a polygon or not
bool PointInsidePolygon(tPoint point, tPolygon polygon) {	
	int counter = 0;
	tLine2D l1 = { point, { 2, 2, 0 } };
	tLineFunction2D l1Func = GetLineEquation2D(l1);
	tLine2D l2;
	for (int i = 0; i < polygon.sides.size(); i++) {
		tPoint a, b;
		a = mouseCoordinates[i];
		b = mouseCoordinates[(i + 1) % polygon.sides.size()];
		l2 = { a, b };
		tLineFunction2D l2Func = GetLineEquation2D(l2);
		tPoint intersection = LinesIntersect2D(l1Func, l2Func);

		if (point.x < intersection.x && min(a.x,b.x) < intersection.x && intersection.x < max(a.x,b.x) && min(a.y, b.y) < intersection.y && intersection.y < max(a.y, b.y)) {
			counter++;
		}
	}
	printf("counter = %d\n", counter);
	if (counter % 2 != 0) return true;
	else return false;
}

// END OF MATH FUNCTIONS FOR VECTORS

void Render() {
	// clear what was on the screen before
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Every time I use glBegin I use also glEnd. I can give the vertexes like glVertex234idf

	glColor3f(0, 0, 0);
	drawAxis();

	int n = 6; // polygon sides you want to draw
	glColor3f(0, 0, 0);
	tPolygon polygon;

	if (mouseCoordinates.size() >= n) {
		polygon = InstantiatePolygon(n);
		for (int i = 0; i < polygon.sides.size(); i++) {
			printf("lado %d: (%f %f) --> (%f %f)\n", i, polygon.sides[i].p1.x, polygon.sides[i].p1.y, polygon.sides[i].p2.x, polygon.sides[i].p2.y);
		}
		DrawPolygon(polygon);
		if (mouseCoordinates.size() >= n+ 1) {
			if (PointInsidePolygon(mouseCoordinates[n], polygon)) {
				glColor3f(0, 1, 0);
				glPointSize(8);
				glBegin(GL_POINTS);
				glVertex3f(mouseCoordinates[n].x, mouseCoordinates[n].y, mouseCoordinates[n].z);
				glEnd();
				printf("INSIDE\n");
			}
			else {
				printf("OUTSIDE\n");
				glColor3f(1, 0, 0);
				glPointSize(8);
				glBegin(GL_POINTS);
				glVertex3f(mouseCoordinates[n].x, mouseCoordinates[n].y, mouseCoordinates[n].z);
				glEnd();
			}
		}
		glColor3f(0, 0, 0);
	}	
	glutSwapBuffers(); // envia o que desenhamos para a tela para que possa ser renderizado
}
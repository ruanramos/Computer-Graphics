#include <GL\freeglut.h>
#include <iostream>

#define PI 3.14159265358979323846

// STRUCTS DEFINITIONS

typedef struct point {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} tPoint;

typedef struct line {
	tPoint p1;
	tPoint p2;
} tLine2D;

typedef struct vector {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} tVector3D;

typedef struct lineFunction {
	double a;
	double b;
} tLineFunction2D;

typedef struct color {
	GLfloat red;
	GLfloat green;
	GLfloat blue;
} colorRGB;

// END OF STRUCTS DEFINITIONS

void Render(void);

void Keyboard(unsigned char c, int x, int y);

void Mouse(int button, int state, int x, int y);

// DRAWING FUNCTIONS SIGNATURES

void DrawVector3D(tVector3D vector);

void DrawLine3D(tPoint a, tPoint b);
void DrawLine3D(tLine2D line);

void DrawVector3D(tVector3D vector);

// END OF DRAWING FUNCTIONS SIGNATURES

// MATH FUNCTIONS WITH VECTORS SIGNATURES

tVector3D SumVectors(tVector3D vector1, tVector3D vector2);

double DotProduct3D(tVector3D vector1, tVector3D vector2);

tVector3D CrossProduct(tVector3D vector1, tVector3D vector2);

double Norm(tVector3D vector);

bool LinesIntersect2D(tLine2D AB, tLine2D CD);

bool IsOrthogonal3D(tVector3D vector1, tVector3D vector2);

tVector3D ScalarMultiplication(tVector3D vector, double n);

tVector3D InverseVector(tVector3D vector, const char axis);

tVector3D NormalizeVector(tVector3D vector);

tLineFunction2D GetLineEquation2D(tPoint p1, tPoint p2);
tLineFunction2D GetLineEquation2D(tLine2D line);

// END OF MATH FUNCTIONS WITH VECTORS SIGNATURES

// GLOBAL VARIABLES

tPoint origin = { 0, 0, 0 };
tVector3D up = { 0,1,0 };
tVector3D down = { 0,-1,0 };
tVector3D left = { -1,0,0 };
tVector3D right = { 1,0,0 };

int clickCount = 0;


// END OF GLOBAL VARIABLES

int main(int argc, char** argv) {
	// glut initiation and window creation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Simple Glut Application");

	// Register Callbacks
	glutDisplayFunc(Render); // displayFunc me diz qual a função que vai ser usada para desenhar na tela
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

	// call the main loop
	glutMainLoop(); // isso quer dizer que terminei a inicialização e posso começar a renderizar
	return 0;
}

void Keyboard(unsigned char c, int x, int y) {
	if (c == 'a') { // ou c == 27
		exit(0);
	}
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		exit(0);
	}
}

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

// END OF DRAWING FUNCTIONS

// MATH FUNCTIONS FOR VECTORS

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

// END OF MATH FUNCTIONS FOR VECTORS

void Render() {
	// limpa o que tava na tela anteriormente.
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Sempre que dou um begin dou um end. Dentro posso dar os vértices na forma glVertex234idf

	glColor3f(0, 0, 0);
	drawAxis();

	tVector3D v1 = {1,0,0};
	tVector3D v2 = { 0,1,0 };

	tLine2D l1 = { {GLfloat(0.3), GLfloat(-0.1), GLfloat(0)}, { GLfloat(0.2), GLfloat(0.2), GLfloat(0)} };
	tLine2D l2 = { { GLfloat(0.1), GLfloat(0), GLfloat(0) },{ GLfloat(-0.4), GLfloat(0), GLfloat(0) } };

	tLineFunction2D func;
	func.a = 2;
	func.b = 1;
	DrawLine3D(func);

	glutSwapBuffers(); // envia o que desenhamos para a tela para que possa ser renderizado
}
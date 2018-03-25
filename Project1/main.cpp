#include <GL\freeglut.h>

#define DIMX 700
#define DIMY 700


void display() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);
/*
	glBegin(GL_LINE_STRIP);

	glVertex2f(GLfloat(0.0), GLfloat(-0.8));
	glVertex2f(GLfloat(0.9), GLfloat(0.3));
	glVertex2f(GLfloat(-0.9), GLfloat(0.3));
	glVertex2f(GLfloat(0.0), GLfloat(-0.8));
	glVertex2f(GLfloat(-0.65), GLfloat(0.6));
	glVertex2f(GLfloat(0.0), GLfloat(0.6));
	glVertex2f(GLfloat(0.65), GLfloat(0.6));
	glVertex2f(GLfloat(0.0), GLfloat(-0.8));
	glVertex2f(GLfloat(0.65), GLfloat(0.6));
	glVertex2f(GLfloat(0.9), GLfloat(0.3));
	glVertex2f(GLfloat(-0.9), GLfloat(0.3));
	glVertex2f(GLfloat(-0.65), GLfloat(0.6));
	glVertex2f(GLfloat(-0.9), GLfloat(0.3));
	glVertex2f(GLfloat(-0.5), GLfloat(0.3));
	glVertex2f(GLfloat(0.5), GLfloat(0.3));
	glVertex2f(GLfloat(0.0), GLfloat(0.6));
	glVertex2f(GLfloat(-0.5), GLfloat(0.3));

	*/

	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(GLfloat(-0.8), GLfloat(-0.6));
	glVertex2f(GLfloat(0.8), GLfloat(-0.6));
	glVertex2f(GLfloat(0.0), GLfloat(0.8));
	glEnd();

	glFlush();
	glutSwapBuffers();
}
/*
void Window_Initiation(char name[]) {

	

}
*/

int main2(int argc, char *argv[]) {
	glutInit(&argc, argv);
	// aqui eu to mexendo nas paradas da janela
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(DIMX, DIMY); // tamanho da janela
	glutInitWindowPosition(50, 50); // posição da tela que a janela surge
									// fim de características da janela
	glutCreateWindow("janela louca");
	//gluOrtho2D(0.5, 1, -0.5, 0.5);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
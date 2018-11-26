#define _USE_MATH_DEFINES
#include <windows.h>   // use as needed for your system
#include <gl/Gl.h>
#include "glut.h"
#include <math.h>

class Point   //single point w/ floating point coordinates
{
public:
	Point() {
		x = y = 0.0f;
	} //constructor 1

	Point(double x, double y) {
		this->x = x; 
		this->y = y;
	} //constructor 2

	void draw(void)
	{
		glBegin(GL_POINTS); //draw this point
		glVertex2d((GLdouble)x, (GLdouble)y);
		glEnd();
	}

	double x, y;
};

class baseVector{
public:
	Point origin;
	double angle;
	baseVector(){};
	baseVector( Point origin, double angle);
};

double degToRad(double angle)
{
	return (angle * M_PI)/180.0;
}

baseVector::baseVector(Point origin, double angle)
{
	this->origin = origin;
	this->angle = degToRad(angle);
}

Point drawVector(baseVector vector, double length, double lineWidth)
{
	Point endPoint = Point(vector.origin.x + length*cos(vector.angle), vector.origin.y + length*sin(vector.angle));
	glEnable(GL_LINE_WIDTH);
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
		//Body
		glVertex2d(vector.origin.x,vector.origin.y);
		glVertex2d(endPoint.x, endPoint.y);
	glEnd();
	//Tip
	glBegin(GL_POLYGON);
		glVertex2d(endPoint.x + 0.4*cos(vector.angle-15), endPoint.y + 0.4*sin(vector.angle-15));
		glVertex2d(endPoint.x, endPoint.y);
		glVertex2d(endPoint.x + 0.4*cos(vector.angle+15), endPoint.y + 0.4*sin(vector.angle+15));
	glEnd();
	glDisable(GL_LINE_WIDTH);

	return endPoint;
}
//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
 void myInit(void)
 {
    glClearColor(1.0,1.0,1.0,0.0);       // set white background color
    glColor3f(0.0f, 0.0f, 0.0f);          // set the drawing color 
 	glPointSize(4.0);		       // a ‘dot’ is 4 by 4 pixels
	glEnable(GL_LINE_WIDTH);
	glLineWidth(2);
	glDisable(GL_LINE_SMOOTH);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluOrtho2D(-30.0, 30.0, -30.0, 30.0);
}


//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
	double radius = 4.0;
	baseVector points[6];
	points[0] = baseVector(Point(-6.6, 4.3), 335.0);
	points[1] = baseVector(Point(0.0, 0.0), 15.0);
	points[2] = baseVector(Point(8.2, 5.6), 110);
	points[3] = baseVector(Point(-3.3, -2.0),225);
	points[4] = baseVector(Point(1.8, -7.2),101.1);
	points[5] = baseVector(Point(5.0, -1.0), 281.1);
	
	double scalars[6];
	scalars[0] = .21;
	scalars[1] = 5.6;
	scalars[2] = 2.7;
	scalars[3] = .86;
	scalars[4] = 4.95;
	scalars[5] = 7.3;
	glClear(GL_COLOR_BUFFER_BIT);     // clear the screen 
	
	glLineWidth(2);
	for(int i = 0; i < 6; i++){
		points[i].origin.draw();
		drawVector(points[i], radius, 1.0);
		glColor3f(0.325, 0.709, 0.27);
		drawVector(points[i], scalars[i] * radius, 1.0);
		glColor3f(0.0f, 0.0f, 0.0f);
	}
	glFlush();		                 // send all output to display 
}

int width = 640, height = 640;

double getWindowAspectRatio()
{
	return width/height;
}

void setViewport(GLint left, GLint right, GLint bottom, GLint top)
{
	glViewport(left, bottom, right - left, top - bottom);
}

void myReshape(GLsizei W, GLsizei H)
{
	if(getWindowAspectRatio() > W/H) // use (global) window aspect ratio
		setViewport(0, W, 0, W/getWindowAspectRatio());
	else
		setViewport(0, H * getWindowAspectRatio(), 0, H);
}

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
void main(int argc, char** argv)
{
	glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
	glutInitWindowSize(width,height);     // set window size
	glutInitWindowPosition(100, 150); // set window position on screen
	glutCreateWindow("Problem 1 - Stage 1"); // open the screen window
	glutDisplayFunc(myDisplay);     // register redraw function
	glutReshapeFunc(myReshape);
	myInit();                   
	glutMainLoop(); 		     // go into a perpetual loop
}

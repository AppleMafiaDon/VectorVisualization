#define _USE_MATH_DEFINES
#include <windows.h>   // use as needed for your system
#include <gl/Gl.h>
#include "glut.h"
#include <math.h>
#include <iostream>

using namespace std;

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
	baseVector(double x, double y, double angle);
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

baseVector::baseVector(double x, double y, double angle)
{
	this->origin = Point(x,y);
	this->angle = degToRad(angle);
}

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
 void myInit(void)
 {
    glClearColor(1.0,1.0,1.0,0.0);       // set white background color
    glColor3f(0.0f, 0.0f, 0.0f);          // set the drawing color 
 	glPointSize(4.0);		       // a ‘dot’ is 4 by 4 pixels
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(5);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluOrtho2D(-30.0, 30.0, -30.0, 30.0);
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

void drawVector(Point origin, Point endPoint, double angle, double lineWidth)
{
	cout << "P(" << origin.x <<", " << origin.y <<")" << endl;
	cout << "P(" << endPoint.x <<", " << endPoint.y <<") " <<endl;
	cout << angle << endl << "-----" << endl;
	glEnable(GL_LINE_WIDTH);
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
		//Body
		glVertex2d(origin.x, origin.y);
		glVertex2d(endPoint.x, endPoint.y);
	glEnd();
	//Tip
	
	glBegin(GL_POLYGON);
		glVertex2d(endPoint.x - 0.4*cos(degToRad(angle+20)), endPoint.y - 0.4*sin(degToRad(angle+20)));
		glVertex2d(endPoint.x, endPoint.y);
		glVertex2d(endPoint.x - 0.4*cos(degToRad(angle-20)), endPoint.y - 0.4*sin(degToRad(angle-20)));
	glEnd();
	glDisable(GL_LINE_WIDTH);
	glColor3f(0.2,0.2,1.0);
	glPointSize(6.0);	
	endPoint.draw();
	glColor3f(0.0,0.0,0.0);
	glPointSize(4.0);	
}

double getDegFromSlope (Point curr, Point prev){
	return abs(atan((curr.y - prev.y)/(curr.x - prev.x))) * (180/M_PI);
}
//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
	double radius = 3.5, angle=0.0;
	Point endpoints[6];
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
		drawVector(points[i],radius, 1.2);
		glColor3f(0.25,0.75,0.125);
		endpoints[i] = drawVector(points[i], scalars[i] * radius, 1.5);	
		if(i > 0){
			//Quadrant I
			if((endpoints[i].y - endpoints[i-1].y) > 0 && (endpoints[i].x - endpoints[i-1].x) > 0)
				angle = getDegFromSlope(endpoints[i], endpoints[i-1]);
			//Quadrant II
			else if((endpoints[i].y - endpoints[i-1].y) > 0 && (endpoints[i].x - endpoints[i-1].x) < 0)
				angle = 180 - getDegFromSlope(endpoints[i], endpoints[i-1]);
			//Quadrant III
			else if((endpoints[i].y - endpoints[i-1].y) < 0 && (endpoints[i].x - endpoints[i-1].x) < 0)
				angle = 180 + getDegFromSlope(endpoints[i], endpoints[i-1]);
			//Quadrant IV
			else if((endpoints[i].y - endpoints[i-1].y) < 0 && (endpoints[i].x - endpoints[i-1].x) > 0)
				angle = 360 - getDegFromSlope(endpoints[i], endpoints[i-1]);
			glColor3f(0.7,0.2,0.3);
			drawVector(endpoints[i-1], endpoints[i], angle, 1.5);
		}
		
	}
	glFlush();		                 // send all output to display 
	glColor3f(0.0f, 0.0f, 0.0f);
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
	glutCreateWindow("Problem 2 - Stage 1"); // open the screen window
	glutDisplayFunc(myDisplay);     // register redraw function
	glutReshapeFunc(myReshape);
	myInit();                   
	glutMainLoop(); 		     // go into a perpetual loop
}

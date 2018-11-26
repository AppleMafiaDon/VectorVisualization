#define _USE_MATH_DEFINES
#include <windows.h>   // use as needed for your system
#include <gl/Gl.h>
#include "glut.h"
#include <math.h>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std;

class Point   //single point w/ floating point coordinates
{
public:
	double x, y;
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
		glVertex2f((GLfloat)x, (GLfloat)y);
		glEnd();
	}
};

class baseVector{
public:
	Point origin;
	double angle;
	baseVector(){};
	baseVector(double x, double y, double angle);
	baseVector(Point origin, double angle);
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
		glVertex2d(endPoint.x + 0.5*cos(vector.angle-15), endPoint.y + 0.5*sin(vector.angle-15));
		glVertex2d(endPoint.x, endPoint.y);
		glVertex2d(endPoint.x + 0.5*cos(vector.angle+15), endPoint.y + 0.5*sin(vector.angle+15));
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
		glVertex2d(endPoint.x - 0.5*cos(degToRad(angle+20)), endPoint.y - 0.5*sin(degToRad(angle+20)));
		glVertex2d(endPoint.x, endPoint.y);
		glVertex2d(endPoint.x - 0.5*cos(degToRad(angle-20)), endPoint.y - 0.5*sin(degToRad(angle-20)));
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
//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
 void myInit(void)
 {
    glClearColor(1.0,1.0,1.0,0.0);       // set white background color
    glColor3f(0.0f, 0.0f, 0.0f);          // set the drawing color 
 	glPointSize(2.0);		       // a ‘dot’ is 4 by 4 pixels
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluOrtho2D(-12.0, 12.0, -12.0, 12.0);
}
//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);     // clear the screen 
	string fileName = "baseVectorList.txt";
	int nLines = 0;
	string line = "";
	double value =0.0;
	ifstream dataFile(fileName);

	double** readValues;

	while (getline(dataFile, line))
        ++nLines;

	//Resets and returns pointer to start of ile
	dataFile.clear();
	dataFile.seekg(0, ios::beg);

	//Declares array of values
	readValues = new double*[nLines];
	for(int i = 0; i < nLines; i++)
	{
		readValues[i] = new double[3];
	}

	int row = 0, j =0;
	 if (dataFile.is_open())
	 {
			 while(getline(dataFile,line))
			{
				stringstream linestream(line);
				string svalue;
				//replace(line.begin(), line.end(), '\n',' '), line.end();
				 while(getline(linestream,svalue,','))
				{
					readValues[row][j++] = atof(svalue.c_str());
					if(j > 2)
					{
						row++;
						j = 0;
					}
				 }
			}
		 dataFile.close();
	 }
	 else 
		 cout << "Unable to open file"; 

	 baseVector* vectors = new baseVector[nLines];

	 for(int i = 0; i < nLines; i++)
	 {
		 
		 for(int j = 0; j < 3; j++)
		 {
			 cout << readValues[i][j] << " | ";

		 }
		 
		 vectors[i] = baseVector(readValues[i][0],readValues[i][1],readValues[i][2]);
		 cout<<endl;
	 }
	
	//Reading scalars file
	fileName="scalarList.txt";
	line.clear();
	ifstream scalarDataFile(fileName);

	double** scalars;
	nLines = 0;
	while (getline(scalarDataFile, line))
        ++nLines;

	//Resets and returns pointer to start of ile
	scalarDataFile.clear();
	scalarDataFile.seekg(0, ios::beg);

	//Declares array of values
	scalars = new double*[nLines];
	for(int i = 0; i < nLines; i++)
	{
		scalars[i] = new double[6];
	}

	row = 0, j =0;
	if (scalarDataFile.is_open())
	 {
			 while(getline(scalarDataFile,line))
			{
				stringstream linestream(line);
				string svalue;
				//replace(line.begin(), line.end(), '\n',' '), line.end();
				 while(getline(linestream,svalue,','))
				{
					scalars[row][j++] = atof(svalue.c_str());
					if(j > 6)
					{
						row++;
						j = 0;
					}
				 }
			}
		 scalarDataFile.close();
	 }
	 else 
		 cout << "Unable to open file"; 
	
	 
	Point *endPoints = new Point[nLines];
	double angle = 0.0, radius = 4.0;
	
	for(int i = 0; i < 6; i++)
	{
		vectors[i].origin.draw();
		drawVector(vectors[i], radius, 1);
		glColor3f(0.25,0.75,0.125);
		endPoints[i] = drawVector(vectors[i], radius * scalars[0][i], 3);
		if(i > 0)
		{
			//Quadrant I
			if((endPoints[i].y - endPoints[i-1].y) > 0 && (endPoints[i].x - endPoints[i-1].x) > 0)
				angle = getDegFromSlope(endPoints[i], endPoints[i-1]);
			//Quadrant II
			else if((endPoints[i].y - endPoints[i-1].y) > 0 && (endPoints[i].x - endPoints[i-1].x) < 0)
				angle = 180 - getDegFromSlope(endPoints[i], endPoints[i-1]);
			//Quadrant III
			else if((endPoints[i].y - endPoints[i-1].y) < 0 && (endPoints[i].x - endPoints[i-1].x) < 0)
				angle = 180 + getDegFromSlope(endPoints[i], endPoints[i-1]);
			//Quadrant IV
			else if((endPoints[i].y - endPoints[i-1].y) < 0 && (endPoints[i].x - endPoints[i-1].x) > 0)
				angle = 360 - getDegFromSlope(endPoints[i], endPoints[i-1]);

			glColor3f(0.7,0.2,0.3);
			drawVector(endPoints[i-1], endPoints[i], angle, 1.5);
		}
		glColor3f(0.0,0.0,0.0);
	}
	  glFlush();	                 // send all output to display 
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
	glutCreateWindow("Problem 2 - Stage 2"); // open the screen window
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);     // register redraw function
	
	myInit();                   
	glutMainLoop(); 		     // go into a perpetual loop
}

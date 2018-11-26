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

enum classNum{
	First = 0, Second = 1, Third = 2
};

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
	//cout << "P(" << origin.x <<", " << origin.y <<")" << endl;
	//cout << "P(" << endPoint.x <<", " << endPoint.y <<") " <<endl;
	//cout << angle << endl << "-----" << endl;
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
	glPointSize(2.0);	
	endPoint.draw();
	glPointSize(2.0);	
}

double getDegFromSlope (Point curr, Point prev){
	return abs(atan((curr.y - prev.y)/(curr.x - prev.x))) * (180/M_PI);
}

void LabelBitmap( string d, bool left, bool top, float x, float y, float z  )
{
	double w = 0.0;
	for(int i = 0; i < d.length(); i++){
		char c = d[i];
		w += 0.6;
		glRasterPos3f(x + (w), y, z );
		glBitmap(0, 0, 0, 0, left ? -w : 0, top ? -10 : 0, NULL);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c); 
	}
}

void printInfo(float x, float y)
{
	//OUTLINE
	glBegin(GL_LINE_LOOP);
		glVertex2f(x,y);
		glVertex2f(x+16,y);
		glVertex2f(x+16,y-10);
		glVertex2f(x,y-10);
	glEnd();

	//RED
	glColor3f(0.7,0.2,0.3);
	glRectf(x+1,y-2,x+2,y-3);

	//GREEN
	glColor3f(0.325, 0.709, 0.27);
	glRectf(x+1,y-4,x+2,y-5);

	//BLUE
	glColor3f(0.07, 0.47, 0.929);
	glRectf(x+1,y-6,x+2,y-7);

	//BLACK TO NORMAL
	glColor3f(0, 0, 0);

	LabelBitmap("Classes", false, true, x+3, y-0.9, 0);
	LabelBitmap("Iris-setosa", false, true, x+2.5, y-1.9, 0);
	LabelBitmap("Iris-versicolor", false, true, x+2.5, y-3.9, 0);
	LabelBitmap("Iris-virginica", false, true, x+2.5, y-5.9, 0);
}

void drawBaseVectors(baseVector *vectors, double radius){
	glColor3f(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 6; i++)
	{
		vectors[i].origin.draw();
		drawVector(vectors[i], radius, 1);
	}
}
//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
 void myInit(void)
 {
    glClearColor(1.0,1.0,1.0,0.0);       // set white background color
    glColor3f(0.0f, 0.0f, 0.0f);          // set the drawing color 
 	glPointSize(4.0);		       // a ‘dot’ is 4 by 4 pixels
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluOrtho2D(-30.0, 30.00, -30.0, 30.0);
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
			 //cout << readValues[i][j] << " | ";

		 }
		 
		 vectors[i] = baseVector(readValues[i][0],readValues[i][1],readValues[i][2]);
		 cout<<endl;
	 }
	
	//Reading scalars file
	fileName="IRIS6D.txt";
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
		scalars[i] = new double[7];
		for (int j = 0; j < 7; j++){
			scalars[i][j] = 0.0;
		}
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
					scalars[row][j] = atof(svalue.c_str());
					j++;
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
	//Draws original vectors
	for(int i = 0; i < 6; i++)
	{
		vectors[i].origin.draw();
		drawVector(vectors[i], radius, 1);
	}

	//Used to pause the program
	int previousClass = 0;
	int classNum = 0;

	for(int j = 0; j < nLines; j++)
	{

		for(int i = 0; i < 6; i++)
		{
			classNum = (int)(scalars[j][6]+0.5);
			cout<< j << "-> " << classNum << endl;
			if(classNum != previousClass)
			{
				previousClass = classNum;
				drawBaseVectors(vectors, radius);
				glFlush();
				system("pause");
				glClear(GL_COLOR_BUFFER_BIT);
			}

			switch(classNum){
			case 0:
				glColor3f(0.7,0.2,0.3);
				break;
			case 1:
				glColor3f(0.325, 0.709, 0.27);
				break;
			case 2:
				glColor3f(0.07, 0.47, 0.929);
				break;
			}
			endPoints[i] = drawVector(vectors[i], radius * scalars[j][i], 1);
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
				drawVector(endPoints[i-1], endPoints[i], angle, 1);
			}
		}
	}
	drawBaseVectors(vectors, radius);
	printInfo(12,-18);
	glFlush();
	system("pause");
	glClear(GL_COLOR_BUFFER_BIT);

	for (int j = 0; j < nLines; j++)
	{
		for (int i = 0; i < 6; i++)
		{
			classNum = (int)(scalars[j][6] + 0.5);
			switch (classNum) {
			case 0:
				glColor3f(0.7, 0.2, 0.3);
				break;
			case 1:
				glColor3f(0.325, 0.709, 0.27);
				break;
			case 2:
				glColor3f(0.07, 0.47, 0.929);
				break;
			}
			endPoints[i] = drawVector(vectors[i], radius * scalars[j][i], 1);
			if (i > 0)
			{
				//Quadrant I
				if ((endPoints[i].y - endPoints[i - 1].y) > 0 && (endPoints[i].x - endPoints[i - 1].x) > 0)
					angle = getDegFromSlope(endPoints[i], endPoints[i - 1]);
				//Quadrant II
				else if ((endPoints[i].y - endPoints[i - 1].y) > 0 && (endPoints[i].x - endPoints[i - 1].x) < 0)
					angle = 180 - getDegFromSlope(endPoints[i], endPoints[i - 1]);
				//Quadrant III
				else if ((endPoints[i].y - endPoints[i - 1].y) < 0 && (endPoints[i].x - endPoints[i - 1].x) < 0)
					angle = 180 + getDegFromSlope(endPoints[i], endPoints[i - 1]);
				//Quadrant IV
				else if ((endPoints[i].y - endPoints[i - 1].y) < 0 && (endPoints[i].x - endPoints[i - 1].x) > 0)
					angle = 360 - getDegFromSlope(endPoints[i], endPoints[i - 1]);
				drawVector(endPoints[i - 1], endPoints[i], angle, 1);
			}
		}
	}
	drawBaseVectors(vectors, radius);
	printInfo(12, -18);
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
	glutCreateWindow("Problem 2 - Stage 3"); // open the screen window
	glutDisplayFunc(myDisplay);     // register redraw function
	glutReshapeFunc(myReshape);
	myInit();                   
	glutMainLoop(); 		     // go into a perpetual loop
}

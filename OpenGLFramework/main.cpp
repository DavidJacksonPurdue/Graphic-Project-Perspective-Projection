/**
*	CS 334 - Fundamentals of Computer Graphics
*	Initial framework for assignment 1
*/

#include <iostream>
#include "GL/glut.h"

/* Constant values */
const float PI = 3.14159265359;

/* Information of the box world */
int world[8][8] = 
{
	{3, 0, 0, 0, 0, 0, 0, 5},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 3},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 2, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 4}
};

/* Window information */
float windowWidth = 800;
float windowHeight = 600;

/* #### The modelview matrix to load; remember OpenGL is row-major and not column major */
/* You do not necessarily need to use this matrix */
GLfloat myModelMat[4][4] = {
{ 1, 0, 0, 0 },
{ 0, 1, 0, 0 },
{ 0, 0, 1, 0},
{ -4, -1, -26, 1 }
};


/* #### Define variables for the camera here... */


/* Field of view, aspect and near/far planes for the perspective projection */
float fovy = 45.0;
float aspect = windowWidth / windowHeight;
float zNear = 1.0;
float zFar = 100.0;

float growth = 1;
bool up = true;
bool perspective = true;

float bot = -6;
float left = -6;
float right = 6;
float top = 6;

float rotz = 0;
float roty = 0;

float xtrans = -4;
float ztrans = -26;

/* Vertices for a cube */
/* Note: Vertices are with respect to the origin */
float cubeVertices[24] = {
     1.0,  1.0,  1.0,        // Right - Top - Front
     1.0,  1.0,  0.0,        // Right - Top - Back
     0.0,  1.0,  0.0,        // Left - Top - Back
     0.0,  1.0,  1.0,        // Left - Top - Front
     1.0,  0.0,  1.0,        // Right - Bottom - Front
     1.0,  0.0,  0.0,        // Right - Bottom - Back
     0.0,  0.0,  0.0,        // Left - Bottom - Back
     0.0,  0.0,  1.0,        // Left - Bottom - Front
};

/* The indices for drawing the triangles that generate the faces of the cube */
/* 3 points per triangle x 2 triangles per face x 6 faces of the cube */
GLubyte cubeIndices[36] = {
    0, 1, 2,   2, 3, 0,        // Triangles for the top face
    4, 5, 6,   6, 7, 4,        // Triangles for the bottom face
    0, 1, 5,   5, 4, 0,        // Triangles for the Right face
    2, 3, 7,   7, 6, 2,        // Triangles for the left face
    0, 4, 7,   7, 3, 0,        // Triangles for the front face
    1, 2, 6,   6, 5, 1         // Triangles for the back face
};

/* Vertices for the floor plane */
float floorVertices[12] = 
{
	-1.0,  0.0, -1.0,
	-1.0,  0.0,  9.0,
	 9.0,  0.0,  9.0,
	 9.0,  0.0, -1.0
};


/**
*    Function invoked for drawing using OpenGL
*/
void display()
{
    static int frameCount=0;

	/* #### frame count, might come in handy for animations */
	frameCount++;
	
    /* Clear the window */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* #### Load/set the model view matrix */
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf((GLfloat *)myModelMat);

	/* Set the perspective projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (perspective == true) {
		gluPerspective(fovy, aspect, zNear, zFar);
		glRotatef(rotz, -1, 0, 0);
		glRotatef(roty, 0, -1, 0);
	}
	if (perspective == false) {
		glOrtho(left, right, bot, top, zNear, zFar);
		glRotatef(roty, 0, -1, 0);
		glRotatef(rotz, -1, 0, 0);
	}
	
	
	
	
	//glTranslatef(xtrans, -1, ztrans);


    

    /* Enable client */
	glEnableClientState(GL_VERTEX_ARRAY);
	
	if (growth < 1.5 && up == true) {
		for (int i = 0; i < 24; i++) {
			if (cubeVertices[i] != 0) {
				cubeVertices[i] += 0.0005;
			}
		}
		growth += 0.0005;
	}
	else {
		up = false;
	}

	if (growth > 0.5 && up == false) {
		for (int i = 0; i < 24; i++) {
			if (cubeVertices[i] != 0) {
				cubeVertices[i] -= 0.0005;
			}
		}
		growth -= 0.0005;
	}
	else {
		up = true;
	}

	/* Draw the world */
	glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
	for(int i = 0; i < 8; i += 1) 
	{
		for(int j = 0; j < 8; j += 1) 
		{

			if(world[i][j] > 0) 
			{
				for(int k = 0; k < world[i][j]; k += 1) 
				{
					glPushMatrix();
					glTranslatef(i, k, j);
					glColor3f(1.0, 0.0, 0.0);
					glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cubeIndices);
					glColor3f(0.0, 0.0, 0.0);
					glDrawElements(GL_LINE_STRIP, 36, GL_UNSIGNED_BYTE, cubeIndices);
					glPopMatrix();
				}
				
			}
		}
	}

	/* Draw the floor */
	glVertexPointer(3, GL_FLOAT, 0, floorVertices);
	glColor3f(0.0, 1.0, 0.0);
	glDrawArrays(GL_POLYGON, 0, 4);
	
	/* Disable client */
	glDisableClientState(GL_VERTEX_ARRAY);

    /* Force execution of OpenGL commands */
    glFlush();

    /* Swap buffers for animation */
    glutSwapBuffers();
}


/**
*    Function invoked when window system events are not being received
*/
void idle()
{
    /* Redraw the window */
    glutPostRedisplay();
}


/**
*    #### Function invoked when an event on regular keys occur
*/
void keyboard(unsigned char k, int x, int y)
{
    /* Show which key was pressed */
    //std::cout << "Pressed \"" << k << "\" ASCII: " << (int)k << std::endl;

    if(k == 'a') 
	{
		myModelMat[3][0] += 0.5;
		//xtrans += 0.5;
	}
	else if(k == 'd') 
	{
		myModelMat[3][0] -= 0.5;
		//xtrans -= 0.5;
	}
	else if(k == 'w') 
	{
		myModelMat[3][2] += 0.5;
		zNear += 0.1;
		//ztrans += 0.5;
	}
	else if(k == 's') 
	{
		myModelMat[3][2] -= 0.5;
		zNear -= 0.1;
		//ztrans -= 0.5;
	}
	else if (k == 'p') {
		perspective = true;
	}
	else if (k == 'o') {
		perspective = false;
	}
    else if (k == 27)
    {
        /* Close application if ESC is pressed */
        exit(0);
    }
}


/**
*	#### Function invoked when an event on special keys occur
*/
void special(int key, int x, int y) 
{
	if(key == GLUT_KEY_UP) 
	{
		rotz += PI / 2;
	}
	else if(key == GLUT_KEY_DOWN) 
	{
		rotz -= PI / 2;
	}
	else if(key == GLUT_KEY_RIGHT) 
	{
		roty -= PI / 2;
	}
	else if(key == GLUT_KEY_LEFT) 
	{
		roty += PI / 2;
	}
}


/**
*	Set OpenGL initial state
*/
void init()
{
    /* Set clear color */
    glClearColor(1.0, 1.0, 1.0, 0.0);

    /* Enable the depth buffer */
    glEnable(GL_DEPTH_TEST);
}


/**
*	Main function
*/
int main(int argc, char **argv)
{
    /* Initialize the GLUT window */
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(30, 30);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("CS 334 - Assignment 1: Cook it!");

    /* Set OpenGL initial state */
    init();

    /* Callback functions */
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

    /* Start the main GLUT loop */
    /* NOTE: No code runs after this */
    glutMainLoop();
}
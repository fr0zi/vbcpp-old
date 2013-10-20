#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <string>
#include <sstream>
#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Includes.hpp"
#include "CDirector.hpp"
#include "CWarehouser.hpp"
#include "CVisioner.hpp"
#include "CCamera.hpp"

#include "shader.hpp"

CDirector* director = 0;
CNode* busNode = 0;
CNode* node1 = 0;

enum _EGameState {
	EGS_RUN,
	EGS_PAUSE,
	EGS_QUIT
} EGameState;

std::string windowTitle = "Virtual Bus Core++";



static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check if the ESC key was pressed or the window was closed
	if (key == GLFW_KEY_ESCAPE && action  == GLFW_PRESS)
	{
			EGameState = EGS_QUIT;
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS )
	{
		bool state = busNode->getIsActive();

		busNode->setIsActive(!state);
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS )
	{
		bool state = node1->getIsActive();

		node1->setIsActive(!state);
	}
}


void readInput(GLFWwindow* window, double deltaTime)
{


	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS)
	{
		float zRot = busNode->getZRotation();

		zRot += 5.0f * deltaTime;

		if ((zRot < -360.0f) || (zRot > 360.0f))
			zRot = 0.0f;

		busNode->setZRotation( zRot );

	}

	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
	{
		float zRot = busNode->getZRotation();

		zRot -= 5.0f * deltaTime;

		if ((zRot < -360.0f) || (zRot > 360.0f))
			zRot = 360.f;

		busNode->setZRotation( zRot );
	}

	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS)
	{
		float xRot = busNode->getXRotation();

		xRot += 5.0f * deltaTime;

		if ((xRot < -360.0f) || (xRot > 360.0f))
			xRot = 0.0f;

		busNode->setXRotation( xRot );
	}

	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS)
	{
		float xRot = busNode->getXRotation();

		xRot -= 5.0f * deltaTime;

		if ((xRot < -360.0f) || (xRot > 360.0f))
			xRot = 0.0f;

		busNode->setXRotation( xRot );
	}
}


static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}


//! Create a window
GLFWwindow* createWindow(GLuint width, GLuint height, GLuint xPos = 0, GLuint yPos = 0)
{
	glfwSetErrorCallback(error_callback);

	GLFWwindow* window;

    if ( !glfwInit() )
    {
        fprintf( stderr, "VIDEO: Failed to initialize GLFW!\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 4); //4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	window = glfwCreateWindow(width, height, "VBC++ Window", NULL, NULL);
	
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowPos(window, xPos, yPos);

	glewExperimental = GL_TRUE;

    // Initialize GLEW
    if( glewInit() != GLEW_OK )
    {
        fprintf( stderr, "VIDEO: Failed to initialize GLEW!\n");
        exit(EXIT_FAILURE);
    }

	if (GLEW_VERSION_3_3)
	{
		fprintf( stderr, "VIDEO: OpenGL 3.3 Core is suported. GOOD!\n");
	}
	else
	{
		fprintf( stderr, "VIDEO: OpenGL 3.3 Core is not suported! NOT GOOD!\n");
		exit(EXIT_FAILURE);
	}

	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);


	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return window;
}


int main(int argc, char* argv[])
{
    
	if(argc < 3)
	{
		printf("You must specify model name and texture path as parameter.\n");
		return 1;
	}
	

	// Creating OpenGL window
	GLFWwindow* win	= createWindow(1024, 768, 100, 100);

	glfwSetWindowTitle(win, "Virtual Bus Core++");

	glfwSetKeyCallback(win, keyboard_callback);


	// CREATING OBJECTS

	// Camera
	CCamera* cam = new CCamera;

	cam->setWindowDimensions(1024, 768);
	cam->setPosition(0, 5, -15);

	// Scene Manager
	director = new CDirector("Director");

	// Scene nodes
	CMesh* busMesh = director->loadMesh(argv[1], argv[2]);

    busNode = director->addMeshSceneNode(0, "Bus", busMesh);
	busNode->setXRotation(-90.0f);

	busNode->setPosition(vec3(-1,0,0));

	node1 = director->addMeshSceneNode(busNode, "Foo2", busMesh);
	node1->setXRotation(90.0f);
	
	node1->setPosition(vec3(3,3,0));


	// Setting game state to RUN
	EGameState = EGS_RUN;

	double lastTime, oldTime = glfwGetTime();
 	int nbFrames = 0;

	// Main loop
	while( EGameState == EGS_RUN && !glfwWindowShouldClose(win) )
	{
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;

		// Compute delta time to animation
		double deltaTime = currentTime - oldTime;
		oldTime = currentTime;

		readInput(win, deltaTime);

		if ( currentTime - lastTime >= 1.0 )
		{ // If last window title update was more than 1 sec ago
			// printf and reset timer

			float timing = 1000.0/double(nbFrames);

			nbFrames = 0;
			lastTime += 1.0;

			// Convert the fps value into a string using an output stringstream
			std::ostringstream stream;
			stream << timing;
			std::string sTiming = stream.str();

			// Append the FPS value to the window title details
			std::string theWindowTitle = windowTitle + " | Timing: " + sTiming;
			// Convert the new window title to a c_str and set it
			const char* pszConstString = theWindowTitle.c_str();
			glfwSetWindowTitle(win, pszConstString);
		}

		// Rendering
		glClearColor(0.7, 0.7, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			director->getVisioner()->renderNodes(cam);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	// Dropping Scene Manager
	director->drop();

	// Dropping camera
	cam->drop();

	return 0;
}


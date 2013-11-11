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
#include "CStaticCamera.hpp"
#include "CCameraFPS.hpp"

//#include "shader.hpp"

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const float ROT_SPEED = 20.0f;


CDirector* director = 0;
CNode* busNode = 0;
CCameraFPS* camFPS = 0;

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

	if (key == GLFW_KEY_1 && action == GLFW_PRESS )
	{
		bool state = busNode->getIsActive();

		busNode->setIsActive(!state);
	}
}


void readInput(GLFWwindow* window, double deltaTime)
{	
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS)
	{
		camFPS->moveForward(deltaTime);
	}

	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS)
	{
		camFPS->moveBackward(deltaTime);
	}

	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS)
	{
		camFPS->strafeRight(deltaTime);
	}

	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS)
	{
		camFPS->strafeLeft(deltaTime);	
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

	window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
	
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

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glDepthFunc(GL_LESS);

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
	GLFWwindow* win	= createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 100, 100);

	glfwSetWindowTitle(win, "Virtual Bus Core++");

	glfwSetKeyCallback(win, keyboard_callback);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	// CREATING OBJECTS

	// Camera
	camFPS = new CCameraFPS;
	camFPS->setMoveSpeed(15.0f);

	camFPS->setWindowDimensions(WINDOW_WIDTH, WINDOW_HEIGHT);
	camFPS->setPosition(0, 0, 15);

	// Scene Manager
	director = new CDirector("Director");

	// Scene nodes
	CMesh* busMesh = director->loadMesh(argv[1], argv[2]);

    busNode = director->addMeshSceneNode(0, "Bus", busMesh);
	busNode->setXRotation(-90.0f);

	busNode->setPosition(vec3(0,0,0));


	// Setting game state to RUN
	EGameState = EGS_RUN;

	double lastTime;
	double oldTime; 
	lastTime = oldTime = glfwGetTime();
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
		double xpos, ypos;

		glfwGetCursorPos(win, &xpos, &ypos);

		glfwSetCursorPos( win, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);		

		camFPS->update(xpos, ypos);

		if ( currentTime - lastTime >= 1.0 )
		{ // If last window title update was more than 1 sec ago
			// printf and reset timer

			float timing = double(nbFrames);

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

			director->getVisioner()->renderNodes(camFPS);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	// Dropping Scene Manager
	director->drop();

	// Dropping camera
	camFPS->drop();

	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();

	return 0;
}


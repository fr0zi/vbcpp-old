#include <glew/glew.h>
#include <glfw/glfw.h>

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

#include "shader.hpp"

CDirector* director = 0;
CWarehouser* warehouser = 0;
CVisioner* visioner = 0;

CBusNode* busNode = 0;

enum _EGameState {
	EGS_RUN,
	EGS_PAUSE,
	EGS_QUIT
} EGameState;

std::string windowTitle = "Virtual Bus Core++";



void readInput()
{
	// Check if the ESC key was pressed or the window was closed
	if( glfwGetKey( GLFW_KEY_ESC ) == GLFW_PRESS )
			EGameState = EGS_QUIT;


	if( glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS )
	{
		float zRot = busNode->getZRotation();

		zRot += 0.1f;

		if ((zRot < -360.0f) || (zRot > 360.0f))
			zRot = 0.0f;

		busNode->setZRotation( zRot );
	}

	if( glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS )
	{
		float zRot = busNode->getZRotation();

		zRot -= 0.1f;

		if ((zRot < -360.0f) || (zRot > 360.0f))
			zRot = 360.f;

		busNode->setZRotation( zRot );
	}

	if( glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS )
	{
		float xRot = busNode->getXRotation();

		xRot += 0.1f;

		if ((xRot < -360.0f) || (xRot > 360.0f))
			xRot = 0.0f;

		busNode->setXRotation( xRot );
	}

	if( glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS )
	{
		float xRot = busNode->getXRotation();

		xRot -= 0.1f;

		if ((xRot < -360.0f) || (xRot > 360.0f))
			xRot = 0.0f;

		busNode->setXRotation( xRot );
	}

}


//! Create a window
int createWindow(GLuint width, GLuint height, GLuint xPos = 0, GLuint yPos = 0)
{
    if ( !glfwInit() )
    {
        fprintf( stderr, "VIDEO: Failed to initialize GLFW!\n");
        return 0;
    }

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); //4x antialiasing
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE );

    // Open window and create its OpenGL context
    if( !glfwOpenWindow( width, height, 0, 0, 0, 0, 32, 0, GLFW_WINDOW ))
    {
        fprintf( stderr, "VIDEO: Failed to open GLFW window!\n");
        glfwTerminate();
        return 0;
    }

	glfwSetWindowPos(xPos, yPos);

	glewExperimental = GL_TRUE;

    // Initialize GLEW
    if( glewInit() != GLEW_OK )
    {
        fprintf( stderr, "VIDEO: Failed to initialize GLEW!\n");
        return 0;
    }

	if (GLEW_VERSION_3_3)
	{
		fprintf( stderr, "VIDEO: OpenGL 3.3 Core is suported. GOOD!\n");
	}
	else
	{
		fprintf( stderr, "VIDEO: OpenGL 3.3 Core is not suported! NOT GOOD!\n");
		return 0;
	}

	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);


	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 1;
}


int main(int argc, char* argv[])
{
    
	if(argc < 3)
	{
		printf("You must specify model name and texture path as parameter.\n");
		return 1;
	}
	

	// Creating OpenGL window
	if(!createWindow(1024, 768, 100, 100))
		return -1;

	glfwSetWindowTitle("Virtual Bus Core++");

	visioner = new CVisioner;

	// Creating our Scene Manager
	director = new CDirector("Director");


	warehouser = new CWarehouser;

	// Loading mesh from 3ds file, adding Bus Node to Scene Manager and setting mesh for it
	CLoader3ds* loader3ds = new CLoader3ds(warehouser);

	CMesh* busMesh = loader3ds->getMesh(argv[1], argv[2]);

	delete loader3ds;

    busNode = director->addBusMeshSceneNode(0, "Bus", busMesh);
	busNode->setXRotation(-90.0f);

	vec3 pos = vec3(-1,0,0);
	busNode->setPosition(pos);


	CBusNode* node1 = director->addBusMeshSceneNode(busNode, "Foo2", busMesh);
	node1->setXRotation(90.0f);


	pos = vec3(3,3,0);
	node1->setPosition(pos);

	// Setting game state to RUN
	EGameState = EGS_RUN;

	double lastTime = glfwGetTime();
 	int nbFrames = 0;

	// Main loop
	while( EGameState == EGS_RUN && glfwGetWindowParam( GLFW_OPENED ) )
	{

		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;

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
			glfwSetWindowTitle(pszConstString);
		}

		glClearColor(0.7, 0.7, 0.9, 1.0);
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			director->runAll();

		glfwSwapBuffers();

		readInput();
	}


	warehouser->drop();

	// Dropping Scene Manager
	director->drop();

	visioner->drop();

	return 0;
}


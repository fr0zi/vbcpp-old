#include <glew/glew.h>
#include <glfw/glfw.h>

#include <string>
#include <sstream>
#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "include/Includes.hpp"
#include "include/CSceneManager.hpp"

#include "include/shader.hpp"

CBusNode* busNode;

enum _EGameState {
	EGS_RUN,
	EGS_PAUSE,
	EGS_QUIT
} EGameState;

std::string windowTitle = "Virtual Bus Core++ FPS: ";


int calcFPS(double theTimeInterval = 1.0, std::string theWindowTitle = "NONE") 
{ 
	// Static values which only get initialised the first time the function runs 
	static double t0Value = glfwGetTime(); 
	// Set the initial time to now 
	static int fpsFrameCount = 0; 
	// Set the initial FPS frame count to 0 
	static int oldfps = 0; 
	// Set the initial FPS value to 0.0   
	
	int newfps = 0;
	
	// Get the current time in seconds since the program started (non-static, so executed every time) 
	double currentTime = glfwGetTime();   
	
	// Ensure the time interval between FPS checks is sane (low cap = 0.1s, high-cap = 10.0s) 
	// Negative numbers are invalid, 10 fps checks per second at most, 1 every 10 secs at least. 
	if (theTimeInterval < 0.1) 
	{ 
		theTimeInterval = 0.1; 
	} 
	if (theTimeInterval > 10.0) 
	{ 
		theTimeInterval = 10.0; 
	}   
	
	// Calculate and display the FPS every specified time interval 
	
	if ((currentTime - t0Value) > theTimeInterval) 
	{ 
		// Calculate the FPS as the number of frames divided by the interval in seconds 
		newfps = (double)fpsFrameCount / (currentTime - t0Value);   
		
		if( newfps != oldfps )
		{
		
			// If the user specified a window title to append the FPS value to... 
			if (theWindowTitle != "NONE") 
			{ 
				// Convert the fps value into a string using an output stringstream 
				std::ostringstream stream; 
				stream << newfps; 
				std::string fpsString = stream.str();   
			
				// Append the FPS value to the window title details 
				theWindowTitle += " | FPS: " + fpsString;   
				// Convert the new window title to a c_str and set it 
				const char* pszConstString = theWindowTitle.c_str(); 
				glfwSetWindowTitle(pszConstString); 
			} 
			else // If the user didn't specify a window to append the FPS to then output the FPS to the console 
			{ 
				std::cout << "FPS: " << newfps << std::endl; 
			}
		}
		
		oldfps = newfps;
		
		// Reset the FPS frame counter and set the initial time to be now 
		
		fpsFrameCount = 0; 
		t0Value = glfwGetTime(); 
	} 
	else // FPS calculation time interval hasn't elapsed yet? Simply increment the FPS frame counter 
	{ 
		fpsFrameCount++; 
	}   // Return the current FPS - doesn't have to be used if you don't want it! 
		
	return oldfps; 
}


void readInput()
{
	// Check if the ESC key was pressed or the window was closed
	if( glfwGetKey( GLFW_KEY_ESC ) == GLFW_PRESS )
			EGameState = EGS_QUIT;
			
	if( glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS )
	{
		float zRot = busNode->getZRotation();
		
		zRot += 0.05f;
		
		if( zRot > 360.0f )
			zRot = 0.0f;
		
		busNode->setZRotation( zRot );
	}
	
	if( glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS )
	{
		float zRot = busNode->getZRotation();	
	
		zRot -= 0.05f;
		
		if( zRot < 0.0f )
			zRot = 360.f;
		
		busNode->setZRotation( zRot );
	}

	if( glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS )
	{
		float xRot = busNode->getXRotation();	
	
		xRot += 0.05f;
		busNode->setXRotation( xRot );
	}
	
	if( glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS )
	{
		float xRot = busNode->getXRotation();
	
		xRot -= 0.05f;
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
	if(!createWindow(800, 600, 100, 100))
		return -1;

	glfwSetWindowTitle("Virtual Bus Core++");

	// Creating our Scene Manager
	CSceneManager* smgr = new CSceneManager(0, "SceneManager");

	// Loading mesh from 3ds file, adding Bus Node to Scene Manager and setting mesh for it
	CBusMesh* busMesh = new CBusMesh(argv[1], argv[2]);
	busNode = smgr->addBusMeshSceneNode(busMesh, "Bus"); 
	busNode->setXRotation(-90.0f);
	
	// Setting game state to RUN
	EGameState = EGS_RUN;

	// Main loop
    while( EGameState == EGS_RUN && glfwGetWindowParam( GLFW_OPENED ) )
    {		
		glClearColor(0.7, 0.7, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					
			smgr->renderAll();	

		glfwSwapBuffers();
		
		readInput();
		
		calcFPS(1.0f, windowTitle);
	}
	
	// Dropping Scene Manager
	smgr->drop();
	
	return 0;
}


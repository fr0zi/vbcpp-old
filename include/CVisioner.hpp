#ifndef __CVISIONER_HPP__
#define __CVISIONER_HPP__

#include <stdio.h>
#include <string>

#include <glew/glew.h>
#include <glfw/glfw.h>

#include "Includes.hpp"
#include "CReferenceCounter.hpp"
#include "CMeshBuffer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


class CVisioner : virtual public CReferenceCounter
{
    public:
        CVisioner()
        {
            fprintf(stdout, "Creating Video Server\n");
        }

        virtual ~CVisioner()
        {
            fprintf(stdout, "Destroying Video Server\n");
			
			//if (_VertexArrayID)
			//	glDeleteVertexArrays(1, &_VertexArrayID);

			glfwTerminate();
        }

    //! Create a window
    virtual int createWindow(GLuint width, GLuint height, GLuint xPos = 0, GLuint yPos = 0)
    {
        if ( !glfwInit() )
        {
            fprintf( stderr, "VISIONER: Failed to initialize GLFW!\n");
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
            fprintf( stderr, "VISIONER: Failed to open GLFW window!\n");
            glfwTerminate();
            return 0;
        }

		glfwSetWindowPos(xPos, yPos);

		glewExperimental = GL_TRUE;
		
        // Initialize GLEW
        if( glewInit() != GLEW_OK )
        {
            fprintf( stderr, "VISIONER: Failed to initialize GLEW!\n");
            return 0;
        }

		if (GLEW_VERSION_3_3)
		{
			fprintf( stderr, "VISIONER: OpenGL 3.3 Core is suported. GOOD!\n");
		}
		else
		{
			fprintf( stderr, "VISIONER: OpenGL 3.3 Core is not suported! NOT GOOD!\n");
			return 0;
		}
		
		
		glShadeModel(GL_SMOOTH);

		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);
		//glEnable(GL_NORMALIZE);

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//glGenVertexArrays(1, &_VertexArrayID);
		//glBindVertexArray(_VertexArrayID);
		
		return 1;
    }

    //! Start rendering the scene
    /*!

     */
    virtual void startRender(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha)
    {
        glClearColor(r, g, b, alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    //! Finish rendering the scene
    /*!

     */
    virtual void finishRender()
    {
        glfwSwapBuffers();
    }

	
	void setActiveShaderProgram(GLuint id)
	{
		glUseProgram(id);
	}

    //! Set window title
    virtual void setWindowTitle(std::string title)
    {
        glfwSetWindowTitle( title.c_str() );
    }

	

	private:
		GLuint _VertexArrayID;
		glm::mat4 _projectionMatrix;
};

#endif // __CVISIONER_HPP__


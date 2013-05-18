#ifndef __CRENDERER_HPP__
#define __CRENDERER_HPP__

#include <stdio.h>
#include <string>

#include <glew/glew.h>
#include <glfw/glfw.h>

#include "Includes.hpp"
#include "CReferenceCounter.hpp"
#include "CMeshBuffer.hpp"



class CRenderer : virtual public CReferenceCounter
{
    public:
        CRenderer()
        {
            fprintf(stdout, "Creating Video Server\n");
        }

        virtual ~CRenderer()
        {
            fprintf(stdout, "Destroying Video Server\n");
			
			if (_VertexArrayID)
				glDeleteVertexArrays(1, &_VertexArrayID);

			glfwTerminate();
        }

    //! Create a window
    virtual void createWindow(GLuint width, GLuint height, GLuint xPos = 0, GLuint yPos = 0)
    {
        if ( !glfwInit() )
        {
            fprintf( stderr, "Failed to initialize GLFW!\n");
            return;
        }
		
        glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); //4x antialiasing
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
        glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Open window and create its OpenGL context
        if( !glfwOpenWindow( width, height, 0, 0, 0, 0, 32, 0, GLFW_WINDOW ))
        {
            fprintf( stderr, "Failed to open GLFW window!\n");
            glfwTerminate();
            return;
        }

		glfwSetWindowPos(xPos, yPos);

		glewExperimental = GL_TRUE;
		
        // Initialize GLEW
        if( glewInit() != GLEW_OK )
        {
            fprintf( stderr, "Failed to initialize GLEW!\n");
            return;
        }

		glGenVertexArrays(1, &_VertexArrayID);
		glBindVertexArray(_VertexArrayID);
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
};

#endif // __CRENDERER_HPP__


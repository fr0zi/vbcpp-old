#include "CVisioner.hpp"

CVisioner::CVisioner(vbcString name) : m_Name(name)
{
	#ifdef DEBUG_MODE	
		fprintf(stdout, "Creating Visioner\n");
	#endif
}

CVisioner::~CVisioner()
{
	#ifdef DEBUG_MODE	
		fprintf(stdout, "Destroying Visioner\n");
	#endif

    m_RenderList.clear();
}


void CVisioner::setProjectionMatrix(glm::mat4 matrix)
{
	m_ProjectionMatrix = matrix;
}


void CVisioner::setViewMatrix(glm::mat4 matrix)
{
	m_ViewMatrix = matrix;
}


void CVisioner::setShaderID(GLuint id)
{
	m_CurrentShaderID = id;
}


void CVisioner::registerNodeForRender(CVideoNode* node)
{
    m_RenderList.push_back(node);
}


void CVisioner::renderNodes(CStaticCamera* cam)
{
    std::list<CVideoNode*>::iterator it = m_RenderList.begin();

	// Solid pass
    for (; it != m_RenderList.end(); ++it)
    {
		if ( (*it)->getIsActive() == true )
		{
			GLuint shaderId =  (*it)->getShaderID();

			glm::mat4 MVP = cam->getProjectionMatrix() * cam->getViewMatrix() * (*it)->getAbsoluteTransformation();

			GLuint MVPID = glGetUniformLocation(shaderId, "MVP");

			glUseProgram((*it)->getShaderID());

			glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
					
			(*it)->render(ERP_SOLID);
		}
    }

	// Transparent pass
    for (it = m_RenderList.begin(); it != m_RenderList.end(); ++it)
    {
		if ( (*it)->getIsActive() == true )
		{
			GLuint shaderId =  (*it)->getShaderID();

			glm::mat4 MVP = cam->getProjectionMatrix() * cam->getViewMatrix() * (*it)->getAbsoluteTransformation();

			GLuint MVPID = glGetUniformLocation(shaderId, "MVP");

			glUseProgram((*it)->getShaderID());

			glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
					
			(*it)->render(ERP_TRANSPARENT);
		}
    }
}



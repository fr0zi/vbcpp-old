#include "CVisioner.hpp"

CVisioner::CVisioner(vbcString name) : m_Name(name)
{

}

CVisioner::~CVisioner()
{
	#ifdef DEBUG_MODE	
		fprintf(stdout, "Creating Reference Counted object\n");
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


void CVisioner::registerNodeForRender(CNode* node)
{
    m_RenderList.push_back(node);
}


void CVisioner::renderNodes()
{

    std::list<CNode*>::iterator it = m_RenderList.begin();

    for (; it != m_RenderList.end(); ++it)
    {
        GLuint shaderId =  (*it)->getShaderProgramID();

        GLuint ProjectionMatrixID = glGetUniformLocation(shaderId, "ProjectionMatrix");
        GLuint ModelMatrixID = glGetUniformLocation(shaderId, "ModelMatrix");
        GLuint ViewMatrixID = glGetUniformLocation(shaderId, "ViewMatrix");

        glUseProgram(shaderId);


        glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &m_ProjectionMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &m_ViewMatrix[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(*it)->getAbsoluteTransformation()[0][0]);

        (*it)->render();
    }

}

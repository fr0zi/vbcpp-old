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


void CVisioner::registerNodeForRender(CNode* node)
{
    m_RenderList.push_back(node);
}


void CVisioner::renderNodes(CCamera* cam)
{
	// TODO: Add sorting function to render transparent object in proper order


    std::list<CNode*>::iterator it = m_RenderList.begin();

    for (; it != m_RenderList.end(); ++it)
    {
		if ( (*it)->getIsActive() == true )
		{
			std::list<IComponent*> components = (*it)->getComponents();

			std::list<IComponent*>::iterator cit = components.begin();

			for (; cit != components.end(); ++cit)
			{
				if ((*cit)->getComponentType() == VIDEO_COMPONENT)
				{
					GLuint shaderId =  dynamic_cast<CVideoComponent*>((*cit))->getShaderID();

					glm::mat4 MVP = cam->getProjectionMatrix() * cam->getViewMatrix() * (*it)->getAbsoluteTransformation();

					GLuint MVPID = glGetUniformLocation(shaderId, "MVP");

					glUseProgram(shaderId);

					glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
					
					dynamic_cast<CVideoComponent*>((*cit))->render();
				}
			}
		}
    }

}



#include "CWarehouser.hpp"


CWarehouser::CWarehouser(vbcString name) : m_Name(name)
{
	#ifdef DEBUG_MODE
		std::cout << "\t -- Creating object " << m_Name << std::endl;
	#endif
}


CWarehouser::~CWarehouser()
{
	#ifdef DEBUG_MODE
		std::cout << "\t -- Deleting object " << m_Name << std::endl;
	#endif

	ResourceList::iterator it = m_Resources.begin();

	for (; it != m_Resources.end(); ++it)
		(*it)->drop();

	m_Resources.clear();
}


GLuint CWarehouser::loadTexture(vbcString filename)
{
	ResourceList::iterator it = m_Resources.begin();

	for (; it != m_Resources.end(); ++it)
	{
		if ( (*it)->getFilename() == filename )
		{
			printf("Texture resource exists! Returning id: %d\n", (*it)->getID());

			return (*it)->getID();
		}
	}



	CTextureResource* resource = new CTextureResource(filename);
	//m_CurrentResourceID++;
	m_Resources.push_back(resource);

	printf("Texture resource doesn't exist. Creating new resource [%d].\n", resource->getID());
	
	return resource->getID();
}


GLuint CWarehouser::loadShader(vbcString filename)
{
	ResourceList::iterator it = m_Resources.begin();

	for (; it != m_Resources.end(); ++it)
	{
		if ( (*it)->getFilename() == filename )
		{
			printf("Shader resource exists! Returning id: %d\n", (*it)->getID());

			return (*it)->getID();
		}
	}

	

	CShaderResource* resource = new CShaderResource(filename);
	//m_CurrentResourceID++;
	m_Resources.push_back(resource);

	printf("Shader resource doesn't exist. Creating new resource [%d].\n", resource->getID());

	return resource->getID();
}

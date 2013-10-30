#include "CWarehouser.hpp"


CWarehouser::CWarehouser(vbcString name) : m_Name(name), m_CurrentResourceID(1)
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
			printf("Resources exists! Returning id.\n");

			return (*it)->getID();
		}
	}

	printf("Resources doesn't exist. Creating new resource.\n");

	CTextureResource* resource = new CTextureResource(m_CurrentResourceID, filename);
	m_CurrentResourceID++;
	m_Resources.push_back(resource);
	
	return resource->getID();
}


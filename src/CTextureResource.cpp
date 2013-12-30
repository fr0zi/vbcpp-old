#include "CTextureResource.hpp"

CTextureResource::CTextureResource(vbcString filename)
: CResource(filename)
{
	#ifdef DEBUG_MODE
	printf("Creating texture resource: %s\n", m_Filename.c_str());
	#endif

	loadResource();
}


CTextureResource::~CTextureResource()
{
	#ifdef DEBUG_MODE
	printf("Destroying texture resource: %s\n", m_Filename.c_str());
	#endif

	glDeleteTextures(1, &m_ID);
}


void CTextureResource::loadResource()
{
	unsigned dotPos = m_Filename.rfind(".");

	vbcString fileExtension = m_Filename.substr(dotPos+1);

	if ( fileExtension == "bmp" )
	{
		#ifdef DEBUG_MODE
		printf("Texture format is BMP\n");
		#endif

		m_ID = loadTextureBMP(m_Filename);

		return;
	}
	else if ( fileExtension == "tga" )
	{
		#ifdef DEBUG_MODE
		printf("Texture format is TGA\n");
		#endif

		m_ID = loadTextureTGA(m_Filename);

		return;
	}
	else if ( fileExtension == "dds" )
	{
		#ifdef DEBUG_MODE
		printf("Texture format is DDS\n");
		#endif

		m_ID = loadTextureDDS(m_Filename);

		return;
	}

	m_ID = 0;
}


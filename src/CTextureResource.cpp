#include "CTextureResource.hpp"

CTextureResource::CTextureResource(vbcString filename)
: CResource(filename)
{
	printf("Creating texture resource: %s\n", m_Filename.c_str());

	loadResource();
}


CTextureResource::~CTextureResource()
{
	printf("Destroying texture resource: %s\n", m_Filename.c_str());

	glDeleteTextures(1, &m_ID);
}


void CTextureResource::loadResource()
{
	unsigned dotPos = m_Filename.rfind(".");

	vbcString fileExtension = m_Filename.substr(dotPos+1);

	if ( fileExtension == "bmp" )
	{
		printf("Texture format is BMP\n");

		m_ID = loadTextureBMP(m_Filename);

		return;
	}
	else if ( fileExtension == "tga" )
	{
		printf("Texture format is TGA\n");

		m_ID = loadTextureTGA(m_Filename);

		return;
	}
	else if ( fileExtension == "dds" )
	{
		printf("Texture format is DDS\n");

		m_ID = loadTextureDDS(m_Filename);

		return;
	}

	m_ID = 0;
}


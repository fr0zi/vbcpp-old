#include "CResource.hpp"

CResource::CResource(GLuint id, vbcString filename)
: m_ID(id), m_Filename(filename)
{

}

CResource::~CResource()
{

}


vbcString CResource::getFilename()
{
	return m_Filename;
}


GLuint CResource::getID()
{
	return m_ID;
}


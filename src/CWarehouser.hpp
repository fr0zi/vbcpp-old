#ifndef __CWAREHOUSER_HPP__
#define __CWAREHOUSER_HPP__

#include <map>
#include <cstring>

//#include "CResource.hpp"
#include "Includes.hpp"
#include "CReferenceCounter.hpp"


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

//std::map<int, CResource*> ResourceList;


class CWarehouser : virtual public CReferenceCounter
{
	public:
		CWarehouser(vbcString name = "Warehouser");
		virtual ~CWarehouser();

		GLuint loadTexture(vbcString filename);

	private:
		GLuint addTextureDDS(vbcString filename);
	
		GLuint addTextureBMP(vbcString filename);

		GLuint addTextureTGA(vbcString filename);

//		ResourceList	m_Resources;
		GLuint			m_CurrentResourceID;
		vbcString		m_Name;
};

#endif // __CWAREHOUSER_HPP__

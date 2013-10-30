#ifndef __CRESOURCE_HPP__
#define __CRESOURCE_HPP__

#include "Includes.hpp"
#include "CReferenceCounter.hpp"


class CResource : virtual public CReferenceCounter
{
	public:
		CResource(GLuint id = 0, vbcString filename = "");

		virtual ~CResource();

		virtual vbcString getFilename();

		virtual GLuint getID();

	protected:
		GLuint		m_ID;
		vbcString	m_Filename;

		virtual void loadResource() = 0;
};

#endif // __CRESOURCE_HPP__

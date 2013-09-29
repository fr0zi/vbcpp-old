#ifndef __CRESOURCE_HPP__
#define __CRESOURCE_HPP__

#include "Includes.hpp"
#include "CReferenceCounter.hpp"


class CResource : virtual public CReferenceCounter
{
	public:
		CResource();
		virtual ~CResource();

		virtual GLuint loadResource(vbcString filename) = 0;
};

#endif // __CRESOURCE_HPP__

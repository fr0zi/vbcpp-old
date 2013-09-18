#ifndef __CLOADER3DS_HPP__
#define __CLOADER3DS_HPP__

#include <lib3ds/file.h>
#include <lib3ds/material.h>
#include <lib3ds/mesh.h>

#include <soil/SOIL.h>
#include <cstring>

#include "Includes.hpp"
#include "CMesh.hpp"

class CLoader3ds
{
	public:
		CLoader3ds();
		virtual ~CLoader3ds();

		CMesh* getMesh(vbcString filename, vbcString texPath);

	private:
		SMaterial loadMaterialData(Lib3dsMaterial* material, vbcString texPath);

		S3DVertex* loadGeometryByMaterial(SMaterial& material, unsigned int& quantumOfVertices, bool& isValid);

		Lib3dsFile* m_File3ds;
};

#endif // __CLOADER3DS_HPP__

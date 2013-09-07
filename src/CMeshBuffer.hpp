#ifndef __CMESHBUFFER_HPP__
#define __CMESHBUFFER_HPP__

#include <cstring>

#include <lib3ds/file.h>
#include <lib3ds/material.h>
#include <lib3ds/mesh.h>
#include <soil/SOIL.h>

#include "Includes.hpp"
#include "SMaterial.hpp"

//! \brief MeshBuffer class
/*!
	  Mesh Buffer holds vertices with a single material.
*/
class CMeshBuffer
{

public:
	CMeshBuffer(Lib3dsFile* file, Lib3dsMaterial* material, vbcString texPath);

	virtual ~CMeshBuffer();


	bool isValid();

	GLuint getVertexBufferID();

	GLuint getNormalBufferID();

	GLuint getTexCoordBufferID();

	unsigned int getQuantumOfVertices();

	Lib3dsVector* getVertices();

	Lib3dsVector* getNormals();

	Lib3dsTexel* getTexCoords();

	SMaterial& getMaterial();

private:
	void loadGeometry(Lib3dsFile* file, Lib3dsMaterial* material);


	Lib3dsVector* m_Vertices;
	Lib3dsVector* m_Normals;
	Lib3dsTexel* m_TexCoords;
	SMaterial _Material;
	bool m_Valid;
	unsigned int m_QuantumOfVertices;
	GLuint m_VertexBufferID;
	GLuint m_NormalBufferID;
	GLuint m_TexCoordBufferID;
};

#endif // __CMESHBUFFER_HPP__

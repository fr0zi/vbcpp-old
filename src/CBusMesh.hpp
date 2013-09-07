#ifndef __CBUSMESH_HPP__
#define __CBUSMESH_HPP__

#include <vector>
#include <cassert>

#include <lib3ds/file.h>
#include <lib3ds/material.h>

#include "CReferenceCounter.hpp"
#include "CMeshBuffer.hpp"

#include <glm/glm.hpp>

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat4 mat4;


typedef std::vector<CMeshBuffer*> MeshBufferList;


class CBusMesh : virtual public CReferenceCounter
{
	public:

		CBusMesh();

		CBusMesh(vbcString filename, vbcString texturePath);

		virtual ~CBusMesh();

		CMeshBuffer* getMeshBuffer(unsigned int i);
  
		unsigned int getQuantumOfMeshBuffers();


	private:
		MeshBufferList m_MeshBuffers;
};

#endif // __CBUSMESH_HPP__

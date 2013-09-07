#include "CBusMesh.hpp"

CBusMesh::CBusMesh()
{

}


CBusMesh::CBusMesh(vbcString filename, vbcString texturePath)
{
	Lib3dsFile* file;

	file = lib3ds_file_load(filename.c_str());

	Lib3dsMaterial* material;

	assert( file != NULL );
			
	for (material = file->materials; material != NULL; material = material->next)
	{
		CMeshBuffer* mb = new CMeshBuffer(file, material, texturePath);

		m_MeshBuffers.push_back(mb);
	}

	lib3ds_file_free(file);

}


CBusMesh::~CBusMesh()
{
	for (unsigned int i = 0; i < m_MeshBuffers.size(); i++)
	{
		if (m_MeshBuffers[i])
			delete m_MeshBuffers[i];
	}

	m_MeshBuffers.clear();
}


CMeshBuffer* CBusMesh::getMeshBuffer(unsigned int i)
{
	return m_MeshBuffers[i];
}

  
unsigned int CBusMesh::getQuantumOfMeshBuffers()
{
	return m_MeshBuffers.size();
}


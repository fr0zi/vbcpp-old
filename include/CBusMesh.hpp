#ifndef __CBUSMESH_HPP__
#define __CBUSMESH_HPP__

#include <vector>

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

		CBusMesh()
		{
	
		}

		CBusMesh(vbcString filename, vbcString texturePath)
		{
			Lib3dsFile* file;

			file = lib3ds_file_load(filename.c_str());

			Lib3dsMaterial* material;

			for(material = file->materials; material != NULL; material = material->next)
			{
				CMeshBuffer* mb = new CMeshBuffer(file, material, texturePath);

				_MeshBuffers.push_back(mb);
			}

			lib3ds_file_free(file);

		}

		virtual ~CBusMesh()
		{
			for(unsigned int i = 0; i < _MeshBuffers.size(); i++)
			{
				if(_MeshBuffers[i])
				delete _MeshBuffers[i];
			}

			_MeshBuffers.clear();
		}

	
		bool loadModel(vbcString filename, vbcString texturePath)
		{
			Lib3dsFile* file;

			file = lib3ds_file_load(filename.c_str());

			if(file == NULL)
				return false;

			Lib3dsMaterial* material;

			for(material = file->materials; material != NULL; material = material->next)
			{
				CMeshBuffer* mb = new CMeshBuffer(file, material, texturePath);

				_MeshBuffers.push_back(mb);
			}

			lib3ds_file_free(file);
		}


		CMeshBuffer* getMeshBuffer(unsigned int i)
		{
			return _MeshBuffers[i];
		}

  
		unsigned int getQuantumOfMeshBuffers()
		{
			return _MeshBuffers.size();
		}


	private:
		MeshBufferList _MeshBuffers;
};

#endif // __CBUSMESH_HPP__

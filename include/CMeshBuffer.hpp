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
	CMeshBuffer(Lib3dsFile* file, Lib3dsMaterial* material, vbcString texPath)
	{
		_Material.name = material->name;
		_Material.textureName = material->texture1_map.name;
		_Material.transparency = material->transparency;

		_Material.ambient[0] = material->ambient[0];
		_Material.ambient[1] = material->ambient[1];
		_Material.ambient[2] = material->ambient[2];
		_Material.ambient[3] = material->ambient[3];

		_Material.diffuse[0] = material->diffuse[0];
		_Material.diffuse[1] = material->diffuse[1];
		_Material.diffuse[2] = material->diffuse[2];
		_Material.diffuse[3] = material->diffuse[3];

		_Material.specular[0] = material->specular[0];
		_Material.specular[1] = material->specular[1];
		_Material.specular[2] = material->specular[2];
		_Material.specular[3] = material->specular[3];

		_Material.shininess = material->shininess;

		_Material.offset[0] = material->texture1_map.offset[0];
		_Material.offset[1] = material->texture1_map.offset[1];
		_Material.scale[0] = material->texture1_map.scale[0];
		_Material.scale[1] = material->texture1_map.scale[1];

		// make Texture Name lowercase
		vbcString texStr = vbcString(_Material.textureName);

				for( int i = 0; i < texStr.size(); i++ )
					texStr[i] = tolower(texStr[i]);

		_Material.textureName = texStr;
		// ===

		vbcString texturePath = texPath + _Material.textureName;

		
		if( _Material.textureName != "" )
		{	
			printf("Loading texture: %s\n", texturePath.c_str() );
		
			GLuint texId = SOIL_load_OGL_texture(texturePath.c_str(),
											SOIL_LOAD_AUTO,
											SOIL_CREATE_NEW_ID,
											SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);

			if( texId == 0 )
			{
				printf( "SOIL error! %s: %s\n", SOIL_last_result(), texturePath.c_str() );
			}

			printf("Texture ID: %d\n", texId);
			
			_Material.textureId = texId;		
		}
		
		
		loadGeometry(file, material);

		valid = false;
	}


	virtual ~CMeshBuffer()
	{
		if(valid)
		{
			delete[] _TexCoords;
			delete[] _Normals;
			delete[] _Vertices;

			glDeleteBuffers(1, &_VertexBufferID);
			glDeleteBuffers(1, &_NormalBufferID);
			glDeleteBuffers(1, &_TexCoordBufferID);
		}
	}


	bool isValid()
	{
		return valid;
	}

	GLuint getDisplayList()
	{
		return _DisplayList;
	}

	GLuint getVertexBufferID()
	{
		return _VertexBufferID;
	}

	GLuint getNormalBufferID()
	{
		return _NormalBufferID;
	}

	GLuint getTexCoordBufferID()
	{
		return _TexCoordBufferID;
	}

	unsigned int getQuantumOfVertices()
	{
		return _QuantumOfVertices;
	}

	Lib3dsVector* getVertices()
	{
		return _Vertices;
	}

	Lib3dsVector* getNormals()
	{
		return _Normals;
	}

	Lib3dsTexel* getTexCoords()
	{
		return _TexCoords;
	}

	SMaterial& getMaterial()
	{
		return _Material;
	}

private:
	void loadGeometry(Lib3dsFile* file, Lib3dsMaterial* material)
	{
		Lib3dsMesh* mesh;
		Lib3dsFace* face;

		unsigned int validFaces = 0;

		for(mesh = file->meshes; mesh != NULL; mesh = mesh->next)
		{
      		for(unsigned int currentFace = 0; currentFace < mesh->faces; currentFace++)
			{
				face = &mesh->faceL[currentFace];

				vbcString faceMaterial = vbcString(face->material);

				if(faceMaterial == _Material.name)
	  				validFaces++;
      		}
    	}

		valid = validFaces;
		_QuantumOfVertices = validFaces * 3;

		if(valid)
		{
			_Vertices = new Lib3dsVector[validFaces * 3];
			_Normals = new Lib3dsVector[validFaces * 3];
    		_TexCoords = new Lib3dsTexel[validFaces * 3];

			GLfloat u, v;

			unsigned int faceWithCurrentMaterial = 0;

    		for(mesh = file->meshes; mesh != NULL; mesh = mesh->next)
			{
				unsigned int totalFacesInMesh = mesh->faces;

				Lib3dsVector* tmpNormals = new Lib3dsVector[totalFacesInMesh * 3];

				lib3ds_mesh_calculate_normals(mesh, &tmpNormals[0]);

				for(unsigned int currentFace = 0; currentFace < mesh->faces; currentFace++)
				{
					face = &mesh->faceL[currentFace];

					vbcString faceMaterial = vbcString(face->material);

					if(faceMaterial == _Material.name)
					{
						for(int currentVertex = 0; currentVertex < 3; currentVertex++)
						{
							memcpy(&_Vertices[faceWithCurrentMaterial * 3 + currentVertex], mesh->pointL[face->points[currentVertex]].pos, sizeof(Lib3dsVector));

							memcpy(&_Normals[faceWithCurrentMaterial * 3 + currentVertex], tmpNormals[currentFace * 3 + currentVertex] , sizeof(Lib3dsVector));

							if(mesh->texels)
							{
								u = mesh->texelL[face->points[currentVertex]][0];
								v = mesh->texelL[face->points[currentVertex]][1];

								u += _Material.offset[0];
								v += _Material.offset[1];

								u *= _Material.scale[0];
								v *= _Material.scale[1];

								_TexCoords[faceWithCurrentMaterial * 3 + currentVertex][0] = u;
								_TexCoords[faceWithCurrentMaterial * 3 + currentVertex][1] = v;
							} // if (texels)
						} // for

						faceWithCurrentMaterial++;
					} // if

			    } // for

				delete[] tmpNormals;


		glGenBuffers(1, &_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * _QuantumOfVertices, _Vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &_NormalBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _NormalBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * _QuantumOfVertices, _Normals, GL_STATIC_DRAW);

		glGenBuffers(1, &_TexCoordBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _TexCoordBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsTexel) * _QuantumOfVertices, _TexCoords, GL_STATIC_DRAW);

			}
		}




		// Generate Display List

		//GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };

		/*
		_DisplayList = glGenLists(1);

		glNewList( _DisplayList, GL_COMPILE );

			//glMaterialfv(GL_FRONT, GL_AMBIENT, _Material.ambient);
			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, _Material.diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, _Material.specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, &_Material.shininess);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, sizeof(Lib3dsVector), _Vertices);

			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, sizeof(Lib3dsVector), _Normals);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, sizeof(Lib3dsTexel), _TexCoords);

			glBindTexture(GL_TEXTURE_2D, _Material.textureId);

			glEnable(GL_TEXTURE_2D);

			glDrawArrays(GL_TRIANGLES, 0, _QuantumOfVertices );

			glDisable(GL_TEXTURE_2D);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glEndList();
		*/
	}


	Lib3dsVector* _Vertices;
	Lib3dsVector* _Normals;
	Lib3dsTexel* _TexCoords;
	SMaterial _Material;
	bool valid;
	unsigned int _QuantumOfVertices;
	GLuint _DisplayList;
	GLuint _VertexBufferID;
	GLuint _NormalBufferID;
	GLuint _TexCoordBufferID;
};

#endif // __CMESHBUFFER_HPP__

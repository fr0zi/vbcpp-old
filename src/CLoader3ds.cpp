#include "CLoader3ds.hpp"

CLoader3ds::CLoader3ds()
{

}

CLoader3ds::~CLoader3ds()
{
	if ( m_File3ds != 0 )
	{
		lib3ds_file_free(m_File3ds);

		m_File3ds = 0;
	}	
}


CMesh* CLoader3ds::getMesh(vbcString filename, vbcString texPath)
{
	m_File3ds = lib3ds_file_load(filename.c_str());

	assert( m_File3ds != NULL );

	CMesh* mesh = new CMesh;

	Lib3dsMaterial* material;
			
	for (material = m_File3ds->materials; material != NULL; material = material->next)
	{
		// Create new mesh buffer
		CMeshBuffer* mb = new CMeshBuffer;

		// Loading material data and textures into memory
		SMaterial mat = loadMaterialData(material, texPath);

		// Load material data and send to mesh buffer
		mb->setMaterial( mat );

		// Variables fo storing vertex data info
		// quantity of vertices
		unsigned int verticesCount = 0;

		// is mesh buffer valid - it is set to TRUE if any vertex data has been loaded from file into memory
		bool isValid = false;

		// Loading vertex data from file
		S3DVertex* vertices = loadGeometryByMaterial(mat, verticesCount, isValid );

		// Send pointer of vertex data to mesh buffer
		mb->setVerticesData( vertices, verticesCount, isValid );

		// Creating Vertex Buffer Object
		mb->createVBO();

		// Add mesh buffer to mesh
		mesh->addMeshBuffer(mb);
	}

	// Return loaded mesh
	return mesh;
}



SMaterial CLoader3ds::loadMaterialData(Lib3dsMaterial* material, vbcString texPath)
{
	SMaterial sMaterial;

	sMaterial.name = material->name;
	sMaterial.textureName = material->texture1_map.name;
	sMaterial.transparency = material->transparency;

	sMaterial.ambient[0] = material->ambient[0];
	sMaterial.ambient[1] = material->ambient[1];
	sMaterial.ambient[2] = material->ambient[2];
	sMaterial.ambient[3] = material->ambient[3];

	sMaterial.diffuse[0] = material->diffuse[0];
	sMaterial.diffuse[1] = material->diffuse[1];
	sMaterial.diffuse[2] = material->diffuse[2];
	sMaterial.diffuse[3] = material->diffuse[3];

	sMaterial.specular[0] = material->specular[0];
	sMaterial.specular[1] = material->specular[1];
	sMaterial.specular[2] = material->specular[2];
	sMaterial.specular[3] = material->specular[3];

	sMaterial.shininess = material->shininess;

	sMaterial.offset[0] = material->texture1_map.offset[0];
	sMaterial.offset[1] = material->texture1_map.offset[1];
	sMaterial.scale[0] = material->texture1_map.scale[0];
	sMaterial.scale[1] = material->texture1_map.scale[1];

	// make Texture Name lowercase
	vbcString texStr = vbcString(sMaterial.textureName);

	for( int i = 0; i < texStr.size(); i++ )
		texStr[i] = tolower(texStr[i]);

	sMaterial.textureName = texStr;

	
	vbcString texturePath = texPath + sMaterial.textureName;

	GLuint texId = 0;
	
		
	if( sMaterial.textureName != "" )
	{	
		printf("Loading texture: %s\n", texturePath.c_str() );
		
		texId = SOIL_load_OGL_texture(texturePath.c_str(),
											SOIL_LOAD_AUTO,
											SOIL_CREATE_NEW_ID,
											SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);

		if( texId == 0 )
		{
			printf( "SOIL error! %s: %s\n", SOIL_last_result(), texturePath.c_str() );
		}

		printf("Texture ID: %d\n", texId);
			
		sMaterial.textureId = texId;		
	}
	

	return sMaterial;
}


S3DVertex* CLoader3ds::loadGeometryByMaterial(SMaterial& material, unsigned int& quantumOfVertices, bool& isValid)
{
	Lib3dsMesh* mesh;
	Lib3dsFace* face;

	//VertexList vertices;

	unsigned int validFaces = 0;

	for(mesh = m_File3ds->meshes; mesh != NULL; mesh = mesh->next)
	{
		for(unsigned int currentFace = 0; currentFace < mesh->faces; currentFace++)
		{
			face = &mesh->faceL[currentFace];

			vbcString faceMaterial = vbcString(face->material);

			if(faceMaterial == material.name)
 				validFaces++;
			}
		}

	// If mesh contains any vertices with given material - we will set Mesh Buffer as valid (it contains data)
	isValid = (bool)(validFaces);

	// Each face (triangle) has 3 vertices, so total quantity of vertices is number of faces * 3
	quantumOfVertices = validFaces * 3;

	S3DVertex* vertices;

	// If mesh with has any vertices with given material, we load them
	if(isValid)
	{
		vertices = new S3DVertex[quantumOfVertices];

		GLfloat u, v;

		unsigned int faceWithCurrentMaterial = 0;

		// Loop through all meshes in the file
		for(mesh = m_File3ds->meshes; mesh != NULL; mesh = mesh->next)
		{
			unsigned int totalFacesInMesh = mesh->faces;


			Lib3dsVector* tmpNormals = new Lib3dsVector[totalFacesInMesh * 3];

			// For each mesh we calculate normals with Lib3ds function
			lib3ds_mesh_calculate_normals(mesh, &tmpNormals[0]);

			// In each mesh we loop through all faces to find these with given material
			for(unsigned int currentFace = 0; currentFace < mesh->faces; currentFace++)
			{
				// Load face data
				face = &mesh->faceL[currentFace];

				vbcString faceMaterial = vbcString(face->material);

				// If face has the same material name as given as parameter, we load vertices data
				if(faceMaterial == material.name)
				{
					for(int currentVertex = 0; currentVertex < 3; currentVertex++)
					{
						// Copy vertex position data from file into S3DVertex structure inside the array
						memcpy(&vertices[faceWithCurrentMaterial * 3 + currentVertex].coord, mesh->pointL[face->points[currentVertex]].pos, sizeof(float[3]));

						// The same as above, but with normals: loading from already precalculated array into S3DVertex structure in array
						memcpy(&vertices[faceWithCurrentMaterial * 3 + currentVertex].normal, tmpNormals[currentFace * 3 + currentVertex] , sizeof(float[3]));

						// If mesh has any data about texture coordinations, we load them
						if(mesh->texels)
						{
							// texture coordination data
							u = mesh->texelL[face->points[currentVertex]][0];
							v = mesh->texelL[face->points[currentVertex]][1];

							// offset of the texture
							u += material.offset[0];
							v += material.offset[1];

							// scale of the texture
							u *= material.scale[0];
							v *= material.scale[1];

							// Calculated values we store in S3DVertex structure
							vertices[faceWithCurrentMaterial * 3 + currentVertex].texcoord[0] = u;
							vertices[faceWithCurrentMaterial * 3 + currentVertex].texcoord[1] = v;

						} // if (texels)

					} // for

					faceWithCurrentMaterial++;
				} // if

		    } // for

			delete[] tmpNormals;

		}
		
	}

	return vertices;
}


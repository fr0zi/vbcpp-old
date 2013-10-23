#ifndef __CVIDEONODE_HPP__
#define __CVIDEONODE_HPP__

#include "Includes.hpp"
#include "CNode.hpp"
#include "CMesh.hpp"


class CVideoNode : virtual public CNode
{
	public:
		CVideoNode(CNode* parent = 0, vbcString name = "VideoNode",
			vec3 position = vec3(0,0,0),
			float xRotation = 0.0f,
			float yRotation = 0.0f,
			float zRotation = 0.0f,
			vec3 scale = vec3(1,1,1));

		virtual ~CVideoNode();

		void setMesh(CMesh* mesh);

		CMesh* getMesh();

		void setShaderID(GLuint id);

		GLuint getShaderID();

		void render();
	
	protected:
		CMesh*	m_Mesh;
		GLuint	m_ShaderID;
};

#endif // __CVIDEONODE_HPP__

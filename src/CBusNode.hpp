#ifndef __CMESHNODE_HPP_INCLUDED__
#define __CMESHNODE_HPP_INCLUDED__


#include "CNode.hpp"
#include "CMesh.hpp"


class CBusNode : virtual public CNode
{
	public:
		CBusNode(CNode* parent = 0, vbcString name = "BusNode",
		vec3 position = vec3(0,0,0),
		float xRotation = 0.0f,
		float yRotation = 0.0f,
		float zRotation = 0.0f,
		vec3 scale = vec3(1,1,1));

		virtual ~CBusNode();


		void setMesh(CMesh* mesh);

		CMesh* getMesh();

		virtual void render();


	private:
		CMesh*	m_Mesh;

};

#endif // __CMESHNODE_HPP_INCLUDED__

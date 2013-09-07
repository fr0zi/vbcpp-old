#ifndef __CMESHNODE_HPP_INCLUDED__
#define __CMESHNODE_HPP_INCLUDED__


#include "CNode.hpp"
#include "CBusMesh.hpp"


class CBusNode : public CNode
{
	public:
		CBusNode(CNode* parent = 0, vbcString name = "",
		vec3 position = vec3(0,0,0),
		float xRotation = 0.0f, 
		float yRotation = 0.0f,
		float zRotation = 0.0f,
		vec3 scale = vec3(1,1,1));

		virtual ~CBusNode();


		void setBusMesh(CBusMesh* mesh);

		CBusMesh* getBusMesh();

		void render();
		
		
	private:
		CBusMesh*	m_BusMesh;

};

#endif // __CMESHNODE_HPP_INCLUDED__

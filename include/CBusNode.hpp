#ifndef __CBUSNODE_HPP_INCLUDED__
#define __CBUSNODE_HPP_INCLUDED__

#include "CNode.hpp"
#include "CBusMesh.hpp"

class CBusNode : public CNode
{
	public:
		CBusNode(CNode* parent = 0, vbcString name = "",
		vec3 position = vec3(0,0,0),
		vec3 rotationVector = vec3(0,1,0), float rotationAngle = 0.0f,
		vec3 scale = vec3(1,1,1))
		: CNode(parent, name, position,
			rotationVector, rotationAngle, scale)
		{

		}


		virtual ~CBusNode()
		{
			if(_BusMesh)
				_BusMesh->drop();
		}


		void setBusMesh(CBusMesh* mesh)
		{
			_BusMesh = mesh;
		}

		CBusMesh* getBusMesh()
		{
			return _BusMesh;
		}

	private:
		CBusMesh*	_BusMesh;

};

#endif // __CBUSNODE_HPP_INCLUDED__

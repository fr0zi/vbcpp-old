#ifndef __CVIDEOCOMPONENT_HPP__
#define __CVIDEOCOMPONENT_HPP__

#include "IComponent.hpp"
#include "CMesh.hpp"

class CVisioner;


class CVideoComponent : virtual public IComponent
{
	friend class CVisioner;

	public:
		CVideoComponent(EComponentType componentType);

		virtual ~CVideoComponent();

		virtual void setMesh(CMesh* mesh);
		virtual CMesh* getMesh();
		
		virtual void setShaderID(GLuint id);
		virtual GLuint	getShaderID();

		virtual void render();

	protected:
		CMesh*	m_Mesh;
		GLuint	m_ShaderID;
};

#endif // __CVIDEOCOMPONENT_HPP__


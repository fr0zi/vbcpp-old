#ifndef __CDIRECTOR_HPP__
#define __CDIRECTOR_HPP__

#include <list>

#include "Includes.hpp"
#include "CNode.hpp"
#include "CLoader3ds.hpp"
#include "CVisioner.hpp"
#include "shader.hpp"
#include "CVideoComponent.hpp"


typedef std::list<CNode*>	NodeList;


class CDirector : virtual public CNode
{
	public:
		CDirector(vbcString name = "Director")
		{
            printf("Creating Scene Manager\n");

			m_Visioner = new CVisioner;
			m_Warehouser = new CWarehouser;
		}


		virtual ~CDirector()
		{
            printf("Destroying Scene Manager\n");
			
			m_Visioner->drop();
			m_Warehouser->drop();

		}


		CNode* addMeshSceneNode(CNode* parent = 0, vbcString name = "", CMesh* mesh = 0,
			vec3 position = vec3(0,0,0), float rotationAngle = 0.0f, vec3 rotationVector = vec3(0,1,0), vec3 scale = vec3(1,1,1))
		{
			CNode* node;

            if (parent != 0)
                node = new CNode(parent, name);
            else
                node = new CNode(this, name);


			CVideoComponent* component = new CVideoComponent(VIDEO_COMPONENT);

			component->setMesh(mesh);
			component->setShaderID( LoadShaders("shader.vert", "shader.frag") );


			node->addComponent(component);

			component->drop();
			node->drop();

            m_Visioner->registerNodeForRender(node);

			return node;
		}


		CMesh* loadMesh(vbcString filename, vbcString texturePath)
		{
			// Loading mesh from 3ds file, adding Bus Node to Scene Manager and setting mesh for it
			CLoader3ds* loader3ds = new CLoader3ds(m_Warehouser);

			CMesh* mesh = loader3ds->getMesh(filename, texturePath);

			delete loader3ds;

			return mesh;
		}

		
		CVisioner* getVisioner()
		{
			return m_Visioner;
		}


		CWarehouser* getWarehouser()
		{
			return m_Warehouser;
		}


	protected:
		CVisioner*	    m_Visioner;
        CWarehouser*    m_Warehouser;
};

#endif // __CDIRECTOR_HPP__


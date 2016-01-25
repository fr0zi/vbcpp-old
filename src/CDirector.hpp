#ifndef __CDIRECTOR_HPP__
#define __CDIRECTOR_HPP__

#include <list>

#include "Includes.hpp"
#include "CNode.hpp"
#include "CLoader3ds.hpp"
#include "CVisioner.hpp"
#include "CMeshNode.hpp"
#include "vbTransform.hpp"

typedef std::list<CNode*>	NodeList;


class CDirector : virtual public CNode
{
	public:
		CDirector(vbcString name = "Director")
		{
			#ifdef DEBUG_MODE
            printf("Creating Scene Manager\n");
			#endif

			m_Visioner = new CVisioner;
			m_Warehouser = new CWarehouser;
		}


		virtual ~CDirector()
		{
			#ifdef DEBUG_MODE
            printf("Destroying Scene Manager\n");
			#endif

			m_Visioner->drop();
			m_Warehouser->drop();
		}


		CMeshNode* addMeshSceneNode(CNode* parent = 0, vbcString name = "", CMesh* mesh = 0,
			vec3 position = vec3(0,0,0), vec3 rotation = vec3(0,0,0), vec3 scale = vec3(1,1,1))
		{
			CMeshNode* node;

            if (parent != 0)
                node = new CMeshNode(parent, name);
            else
                node = new CMeshNode(this, name);

			node->setMesh(mesh);
			node->setShaderID(m_Warehouser->loadShader("bus.shdr"));

			vbTransform transform;
			transform.setPosition(position);
			transform.setRotation(rotation);
			transform.setScale(scale);

			node->setTransform(transform);

			m_Visioner->registerNodeForRender(node);

			node->drop();

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


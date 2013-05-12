#ifndef CSCENEMANAGER_HPP_INCLUDED
#define CSCENEMANAGER_HPP_INCLUDED

#include "CNode.hpp"
#include "CBusNode.hpp"
#include "CRenderer.hpp"

class CNode;

class CSceneManager : public CNode
{
	public:
		CSceneManager(CNode* parent = 0,
					vbcString name = "SceneManager",
					CRenderer* renderer = 0)
					: CNode(parent, name)
		{


            printf("Creating Scene Manager\n");
		}

		virtual ~CSceneManager()
		{
            printf("Destroying Scene Manager\n");

			removeAll();
		}
		
		
		CBusNode* addBusMeshSceneNode(CBusMesh* mesh, vbcString name = "", 
			vec3 position = vec3(0,0,0), float rotationAngle = 0.0f, vec3 rotationVector = vec3(0,1,0), vec3 scale = vec3(1,1,1))
		{
			CBusNode* node = new CBusNode(this, name);
			node->setBusMesh(mesh);

			node->drop();

			return node;
		}


		void renderAll()
		{
			// render all visible graphical nodes


            mat4 MVP;
            GLuint MatrixID;
			CMeshBuffer* mb;


			std::list<CNode*>::iterator it = Children.begin();

			for( ; it != Children.end(); ++it )
			{


			}
		}


	protected:
	mat4 Projection;
	mat4 View;

	CRenderer	_Renderer;
};

#endif // CSCENEMANAGER_HPP_INCLUDED

#ifndef __CDIRECTOR_HPP__
#define __CDIRECTOR_HPP__

#include <list>

#include "Includes.hpp"
#include "CReferenceCounter.hpp"
#include "CBusNode.hpp"
#include "CLoader3ds.hpp"
#include "CVisioner.hpp"
#include "shader.hpp"


typedef std::list<CNode*>	NodeList;


typedef struct CameraStruct
{
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

	float wHeight, wWidth;
	float viewAngle;

	// Near and far visibility values
	float nValue, fValue;

	vec3 Position;
	vec3 LookAt;
	vec3 UpVector;

	CameraStruct()
	: wHeight(640), wWidth(480), viewAngle(45.0f), nValue(0.1f), fValue(1000.0f),
            ProjectionMatrix(1.0), ViewMatrix(1.0),
			Position(vec3(0,0,10)), LookAt(vec3(0,0,0)), UpVector(vec3(0,1,0))
	{
		updateProjectionMatrix();
		updateViewMatrix();
	}

	void setWindowDimensions(float width, float height)
	{
		wWidth = width;
		wHeight = height;

		updateProjectionMatrix();
	}

	void setViewAngle(float angle)
	{
		viewAngle = angle;

		updateProjectionMatrix();
	}

	void setNearValue(float value)
	{
		nValue = value;

		updateProjectionMatrix();
	}

	void setFarValue(float value)
	{
		fValue = value;

		updateProjectionMatrix();
	}


	void updateProjectionMatrix()
	{
		ProjectionMatrix = glm::perspective(viewAngle, wWidth / wHeight, nValue, fValue);
	}

	void setPosition(vec3 position)
	{
		Position = position;

		updateViewMatrix();
	}

	void setLookAt(vec3 lookAt)
	{
		LookAt = lookAt;

		updateViewMatrix();
	}

	void setUpVector(vec3 up)
	{
		UpVector = up;

		updateViewMatrix();
	}

	void updateViewMatrix()
	{
		ViewMatrix = glm::lookAt(
						Position,
						LookAt,
						UpVector );
	}
} SCamera;


class CDirector : virtual public CNode
{
	public:
		CDirector(vbcString name = "Director")
		{
            printf("Creating Scene Manager\n");

			m_Camera = new SCamera();

			m_Camera->setWindowDimensions(1024,768);

			m_Camera->setPosition(vec3(0,5,-15));

			m_Visioner = new CVisioner;
			m_Warehouser = new CWarehouser;
		}


		virtual ~CDirector()
		{
            printf("Destroying Scene Manager\n");

			if (m_Camera)
			{
				delete m_Camera;
				m_Camera = 0;
			}

				m_Visioner->drop();
				m_Warehouser->drop();
		}


		CBusNode* addBusMeshSceneNode(CNode* parent = 0, vbcString name = "", CMesh* mesh = 0,
			vec3 position = vec3(0,0,0), float rotationAngle = 0.0f, vec3 rotationVector = vec3(0,1,0), vec3 scale = vec3(1,1,1))
		{
			CBusNode* node;

            if (parent != 0)
                node = new CBusNode(parent, name);
            else
                node = new CBusNode(this, name);


			node->setShaderProgramID( LoadShaders("shader.vert", "shader.frag") );

			node->setMesh(mesh);

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


		SCamera* getCamera()
		{
			return m_Camera;
		}



		void runAll()
		{
            m_Visioner->setProjectionMatrix(m_Camera->ProjectionMatrix);
            m_Visioner->setViewMatrix(m_Camera->ViewMatrix);

			// render all visible graphical nodes
            m_Visioner->renderNodes();

		}

	protected:
		SCamera*	    m_Camera;
		CVisioner*	    m_Visioner;
        CWarehouser*    m_Warehouser;
};

#endif // __CDIRECTOR_HPP__


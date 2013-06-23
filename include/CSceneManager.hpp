#ifndef __CSCENEMANAGER_HPP_INCLUDED__
#define __CSCENEMANAGER_HPP_INCLUDED__

#include "CEntity.hpp"
#include "CBusNode.hpp"

#include "shader.hpp"

typedef struct CameraStruct
{
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	
	float wHeight, wWidth;
	float viewAngle;
	float nValue, fValue;
	
	vec3 Position;
	vec3 LookAt;
	vec3 UpVector;
	
	CameraStruct() 
		: wHeight(640), wWidth(480), viewAngle(45.0f), nValue(0.1f), fValue(1000.0f), 
			Position(vec3(0,0,10)), LookAt(vec3(0,0,0)), UpVector(vec3(0,1,0)),
			ProjectionMatrix(1.0), ViewMatrix(1.0)
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
		ProjectionMatrix = glm::perspective(viewAngle, wWidth / wHeight, 0.1f, 1000.0f);
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


class CSceneManager : public CEntity
{
	public:
		CSceneManager(CEntity* parent = 0,
					vbcString name = "SceneManager")
					: CEntity(parent, name)
		{
            printf("Creating Scene Manager\n");
			
			m_Camera = new SCamera();
			
			m_Camera->setWindowDimensions(800,600);
			
			m_Camera->setPosition(vec3(0,5,-15));
		}

		
		virtual ~CSceneManager()
		{
            printf("Destroying Scene Manager\n");

			if( m_Camera )
			{
				delete m_Camera;
				m_Camera = 0;
			}
			
			removeAll();
		}
		

		CBusNode* addBusMeshSceneNode(CBusMesh* mesh, vbcString name = "", 
			vec3 position = vec3(0,0,0), float rotationAngle = 0.0f, vec3 rotationVector = vec3(0,1,0), vec3 scale = vec3(1,1,1))
		{
			CBusNode* node = new CBusNode(this, name);
			
			node->setShaderProgramID( LoadShaders("shader.vert", "shader.frag") );
			
			node->setBusMesh(mesh);

			node->drop();

			return node;
		}

		
		void render()
		{
		
		}
		
		
		void renderAll()
		{
			// render all visible graphical nodes

			glm::mat4 projection = m_Camera->ProjectionMatrix;
			glm::mat4 view = m_Camera->ViewMatrix;

			
			std::list<CEntity*>::iterator it = m_Children.begin();

			for( ; it != m_Children.end(); ++it )
			{	
				if( (*it)->getVisibility() == true )
				{
			
					GLuint shaderId =  (*it)->getShaderProgramID();
				
					GLuint ProjectionMatrixID = glGetUniformLocation(shaderId, "ProjectionMatrix");
					GLuint ModelMatrixID = glGetUniformLocation(shaderId, "ModelMatrix");
					GLuint ViewMatrixID = glGetUniformLocation(shaderId, "ViewMatrix");

					glUseProgram(shaderId);
				
					glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &projection[0][0]);
					glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(*it)->getAbsoluteTransformation()[0][0]);
			
					(*it)->render();
				}
			}
		}
		
	protected:
		SCamera*	m_Camera;
};

#endif // __CSCENEMANAGER_HPP_INCLUDED__

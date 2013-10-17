#ifndef __CNODE_HPP__
#define __CNODE_HPP__

#include <string>
#include <list>

#include <glew/glew.h>
#include <glfw/glfw3.h>

#include "Includes.hpp"
#include "CReferenceCounter.hpp"

#include "CVisioner.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

class CDirector;

//! \brief Node class
/*!
	Node represents single element on the scene.
 */

class CNode : virtual public CReferenceCounter
{
    public:

		//! CONSTRUCTOR
		/*!
			\param parent Parent node
			\param name Node name
			\param position Node position on the scene
			\param xRotation Node rotation in X axis
			\param yRotation Node rotation in Y axis
			\param zRotation Node rotation in Z axis
			\param scale Node scale
		*/
		CNode(CNode* parent = 0, vbcString name = "Node",
			vec3 position = vec3(0,0,0),
			float xRotation = 0.0f,
			float yRotation = 0.0f,
			float zRotation = 0.0f,
			vec3 scale = vec3(1,1,1)
		);

        //! DESTRUCTOR
		virtual ~CNode();

        //! Add child node
        virtual void addChild(CNode* child);

        //! Set node's name
        virtual void setName(const vbcString name);

        //! Get node's name
        virtual vbcString getName() const;

		//! Get absolute transformation matrix
        virtual mat4 getAbsoluteTransformation();

		//! Remove node from parent node
		/*!
			This removes node from parent's children list. If node is not attached to any other node nor the Director - it will be removed from memory 
			due to reference counting. 
		*/
        virtual void remove();

        //! Remove node from children list
        virtual bool removeChild(CNode* child);

        //! Remove all children
        virtual void removeAll();

        //! Set parent node
        virtual void setParent(CNode* parent);

		//! Set position
        virtual void setPosition(vec3 position);

		//! Set rotation in X axis
        virtual void setXRotation(GLfloat angle);

		//! Set rotation in Y axis
        virtual void setYRotation(GLfloat angle);

		//! Set rotation in Z axis
        virtual void setZRotation(GLfloat angle);

		//! Get rotation in X axis
		virtual float getXRotation();

		//! Get rotation in Y axis
		virtual float getYRotation();

		//! Get rotation in Z axis
		virtual float getZRotation();

		//! Set scale
        virtual void setScale(vec3 scale);

		//! Set absolute transformation
        virtual void updateAbsoluteTransformation();

		//! Get activity flag
		virtual bool getIsActive() const;

		//! Set activity flag
		virtual void setIsActive(bool state);

		//! Render node
		virtual void render();

		//! Return shader id
        virtual GLuint getShaderProgramID();

		//! Set shader id for node
        virtual void setShaderProgramID(GLuint programId);


    protected:
        //! Pointer to parent entity
        CNode*		m_Parent;

        //! Children list
        std::list<CNode*>	m_Children;

        //! Entity name
        vbcString	m_Name;

		//! Transformation matrix
		mat4 m_AbsoluteTransformation;

		//! Position vector
        vec3 m_RelativePosition;

		//! Rotation vector
        vec3 m_RelativeRotation;

        //! X Rotation angle
        GLfloat m_XRotationAngle;

        //! Y Rotation angle
        GLfloat m_YRotationAngle;

        //! Z Rotation angle
        GLfloat m_ZRotationAngle;

		//! Scale vector
        vec3 m_RelativeScale;

		//! Entity activity flag
        bool m_IsActive;

		//! Shader program id
		GLuint m_ShaderProgramID;
};

#endif // __CNODE_HPP__


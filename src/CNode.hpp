#ifndef CNODE_HPP_INCLUDED
#define CNODE_HPP_INCLUDED

#include <string>
#include <list>
#include <iostream>

#include <glew/glew.h>
#include <glfw/glfw.h>

#include "Includes.hpp"
#include "CReferenceCounter.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;


#define DEBUG_MODE

class CNode : virtual public CReferenceCounter
{
public:

        //! CONSTRUCTOR
	CNode(CNode* parent = 0, vbcString name = "",
		vec3 position = vec3(0,0,0),
		float xRotation = 0.0f,
		float yRotation = 0.0f,
		float zRotation = 0.0f,
                vec3 scale = vec3(1,1,1))
            : m_Parent(parent), m_Name(name),
                RelativePosition(position),
                m_XRotationAngle(xRotation),
		m_YRotationAngle(yRotation),
		m_ZRotationAngle(zRotation),
                RelativeScale(scale),
                m_IsVisible(true)
	{

		/* This line was couse of error - Segmentation Fault
			when it was inside 'if( parent != 0 )'
			When I moved it here - everything start working well :)
		*/
		m_Parent = parent;

		if( parent )
		{
			parent->addChild(this);
		}

            	updateAbsoluteTransformation();

		#ifdef DEBUG_MODE
			std::cout << "\t -- Creating object " << m_Name << std::endl;
		#endif
	}


        //! DESTRUCTOR
	virtual ~CNode()
	{
		#ifdef DEBUG_MODE
			std::cout << "\t -- Deleting object " << m_Name << " with all its children.\n";
		#endif

            	if( m_ShaderProgramID > 0 )
                	glDeleteProgram(m_ShaderProgramID);

		// Delete all children
		removeAll();
	}


        //! Add child Entity
        virtual void addChild(CNode* child)
        {
		if( child && (child != this) )
		{
			child->grab();
			child->remove();    // remove from old parent
			m_Children.push_back(child);
			child->m_Parent = this;
		}
        }


        //! Setting entity's name
        virtual void setName(const vbcString name)
        {
        	m_Name = name;
	}


        //! Getting entity's name
        virtual vbcString getName() const
        {
        	return m_Name;
	}


	//! Get Transform Matrix
        virtual mat4 getAbsoluteTransformation()
        {
            return m_AbsoluteTransformation;
        }


	//! Remove Entity from the scene
        virtual void remove()
        {
		// If Entity have parent - we remove it from its parent children list
		if( m_Parent )
		m_Parent->removeChild(this);
        }


        //! Remove entity from children list
        virtual bool removeChild(CNode* child)
        {
		std::list<CNode *>::iterator it = m_Children.begin();

		for(; it != m_Children.end(); ++it )
			if( (*it) == child )
			{
				(*it)->m_Parent = 0;
				(*it)->drop();
				m_Children.erase(it);

				return true;
			}

			return false;
        }


        //! Remove all children
        virtual void removeAll()
        {
		std::list<CNode *>::iterator it = m_Children.begin();

		for( ; it != m_Children.end(); ++it )
		{
			(*it)->m_Parent = 0;
			(*it)->drop();
		}

		m_Children.clear();
        }


        //! Set parent Entity
        virtual void setParent(CNode* parent)
        {
		grab();
		remove();

		m_Parent = parent;

		if( m_Parent )
			m_Parent->addChild(this);

		drop();
        }


	//! Set position
        virtual void setPosition(vec3 position)
        {
		RelativePosition = position;

		updateAbsoluteTransformation();
        }


	//! Set rotation
        virtual void setXRotation(GLfloat angle)
        {
		RelativeRotation = vec3(1,0,0);
		m_XRotationAngle = angle;

		updateAbsoluteTransformation();
        }

		
	//! Set rotation
        virtual void setYRotation(GLfloat angle)
        {
		RelativeRotation = vec3(0,1,0);
		m_YRotationAngle = angle;

		updateAbsoluteTransformation();
        }		
		
		
		//! Set rotation
        virtual void setZRotation(GLfloat angle)
        {
		RelativeRotation = vec3(0,0,1);
		m_ZRotationAngle = angle;

		updateAbsoluteTransformation();
        }		
		
		
	float getXRotation()
	{
		return m_XRotationAngle;
	}
		
		
	float getYRotation()
	{
		return m_YRotationAngle;
	}		
		
		
	float getZRotation()
	{
		return m_ZRotationAngle;
	}		
		
		
	//! Set scale
        virtual void setScale(vec3 scale)
        {
		RelativeScale = scale;

		updateAbsoluteTransformation();
        }

		
	//! Set absolute transformation
        virtual void updateAbsoluteTransformation()
        {
		mat4 position = glm::translate(RelativePosition);
		mat4 rotation = glm::rotate(m_XRotationAngle, vec3(1,0,0));
		rotation *= glm::rotate(m_YRotationAngle, vec3(0,1,0));
		rotation *= glm::rotate(m_ZRotationAngle, vec3(0,0,1));
		mat4 scale = glm::scale(RelativeScale);

		m_AbsoluteTransformation = position * rotation * scale;

		// if Entity has parent - multiply matrices to get actual Entity transformation relative to parent
		if (m_Parent)
			m_AbsoluteTransformation = m_Parent->getAbsoluteTransformation() * m_AbsoluteTransformation;
	}


	//! Get visibility flag
	const virtual bool getVisibility() const
	{
		return m_IsVisible;
	}


	//! Set visibility of Entity
	virtual void setVisibility(bool visibility)
	{
		m_IsVisible = visibility;

		std::list<CNode *>::iterator it = m_Children.begin();

		for( ; it != m_Children.end(); ++it )
			(*it)->setVisibility(visibility);
	}
		
	//! Render Entity
	virtual void render() = 0;

		//! Return shader id
        virtual GLuint getShaderProgramID()
        {
		return m_ShaderProgramID;
        }

		//! Set shader id for Entity
        virtual void setShaderProgramID(GLuint programId)
        {
		m_ShaderProgramID = programId;
        }


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
        vec3 RelativePosition;

	//! Rotation vector
        vec3 RelativeRotation;

        //! X Rotation angle
        GLfloat m_XRotationAngle;
		
        //! Y Rotation angle
        GLfloat m_YRotationAngle;		

        //! Z Rotation angle
        GLfloat m_ZRotationAngle;		
		
	//! Scale vector
        vec3 RelativeScale;

	//! Entity visibility flag
        bool m_IsVisible;

	//! Shader program id
        GLuint m_ShaderProgramID;
};

#endif // CNODE_HPP_INCLUDED

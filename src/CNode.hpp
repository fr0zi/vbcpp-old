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
			vec3 scale = vec3(1,1,1)
		);

        //! DESTRUCTOR
	virtual ~CNode();

        //! Add child Entity
        virtual void addChild(CNode* child);

        //! Setting entity's name
        virtual void setName(const vbcString name);

        //! Getting entity's name
        virtual vbcString getName() const;


	//! Get Transform Matrix
        virtual mat4 getAbsoluteTransformation();


	//! Remove Entity from the scene
        virtual void remove();

        //! Remove entity from children list
        virtual bool removeChild(CNode* child);

        //! Remove all children
        virtual void removeAll();

        //! Set parent Entity
        virtual void setParent(CNode* parent);

	//! Set position
        virtual void setPosition(vec3 position);

	//! Set rotation
        virtual void setXRotation(GLfloat angle);
		
	//! Set rotation
        virtual void setYRotation(GLfloat angle);		
		
		//! Set rotation
        virtual void setZRotation(GLfloat angle);		
		
	float getXRotation();		
		
	float getYRotation();		
		
	float getZRotation();		
		
	//! Set scale
        virtual void setScale(vec3 scale);
		
	//! Set absolute transformation
        virtual void updateAbsoluteTransformation();

	//! Get visibility flag
	const virtual bool getVisibility() const;

	//! Set visibility of Entity
	virtual void setVisibility(bool visibility);
		
	//! Render Entity
	virtual void render() = 0;

		//! Return shader id
        virtual GLuint getShaderProgramID();

		//! Set shader id for Entity
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

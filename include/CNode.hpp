#ifndef CNode_HPP_INCLUDED
#define CNode_HPP_INCLUDED

#include <string>
#include <list>
#include <iostream>

#include "Includes.hpp"
#include "CReferenceCounter.hpp"

#include "glm/glm.hpp"
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
                vec3 rotationVector = vec3(0,1,0),
                GLfloat rotationAngle = 0.0f,
                vec3 scale = vec3(1,1,1))
            : Parent(parent), Name(name),
                RelativePosition(position),
                RelativeRotation(rotationVector),
                RotationAngle(rotationAngle),
                RelativeScale(scale),
                isVisible(true)
		{

			/* This line was couse of error - Segmentation Fault
				when it was inside 'if( parent != 0 )'
				When I moved it here - everything start working well :)
			*/
			Parent = parent;

			if( parent )
			{
				parent->addChild(this);
			}

            updateAbsoluteTransformation();

			#ifdef DEBUG_MODE
				std::cout << "\t -- Creating object " << Name << std::endl;
			#endif
		}


        //! DESTRUCTOR
		virtual ~CNode()
		{
			#ifdef DEBUG_MODE
				std::cout << "\t -- Deleting object " << Name << " with all its children.\n";
			#endif

            if( ShaderProgramID > 0 )
                glDeleteProgram(ShaderProgramID);

			// Delete all children
			removeAll();
		}


        //! Add child node
        virtual void addChild(CNode* child)
        {
			if( child && (child != this) )
			{
				child->grab();
				child->remove();    // remove from old parent
				Children.push_back(child);
				child->Parent = this;
			}
        }


        //! Setting entity's name
        virtual void setName(const vbcString name)
        {
        	Name = name;
		}


        //! Getting entity's name
        virtual vbcString getName() const
        {
        	return Name;
		}


		//! Get Transform Matrix
        virtual mat4 getAbsoluteTransformation()
        {
            return AbsoluteTransformation;
        }


		//! Remove node from the scene
        virtual void remove()
        {
			// If node have parent - we remove it from its parent children list
			if( Parent )
			Parent->removeChild(this);
        }


        //! Remove entity from children list
        virtual bool removeChild(CNode* child)
        {
			std::list<CNode *>::iterator it = Children.begin();

			for(; it != Children.end(); ++it )
				if( (*it) == child )
				{
					(*it)->Parent = 0;
					(*it)->drop();
					Children.erase(it);

					return true;
				}

			return false;
        }


        //! Remove all children
        virtual void removeAll()
        {
			std::list<CNode *>::iterator it = Children.begin();

			for( ; it != Children.end(); ++it )
			{
				(*it)->Parent = 0;
				(*it)->drop();
			}

			Children.clear();
        }


        //! Set parent node
        virtual void setParent(CNode* parent)
        {
			grab();
			remove();

			Parent = parent;

			if( Parent )
				Parent->addChild(this);

			drop();
        }


		//! Set position
        virtual void setPosition(vec3 position)
        {
            RelativePosition = position;

            updateAbsoluteTransformation();
        }


		//! Set rotation
        virtual void setRotation(vec3 rotationVector, GLfloat angle)
        {
            RelativeRotation = rotationVector;
            RotationAngle = angle;

            updateAbsoluteTransformation();
        }


		//! Set scale
        virtual void setScale(vec3 scale)
        {
            RelativeScale = scale;

            updateAbsoluteTransformation();
        }



        virtual void updateAbsoluteTransformation()
        {
            mat4 position = glm::translate(RelativePosition);
            mat4 rotation = glm::rotate(RotationAngle, RelativeRotation);
            mat4 scale = glm::scale(RelativeScale);

            AbsoluteTransformation = position * rotation * scale;

			// if node has parent - multiply matrices to get actual node transformation relative to parent
            if (Parent)
				AbsoluteTransformation = Parent->getAbsoluteTransformation() * AbsoluteTransformation;
        }


		//!
		const virtual bool getVisible() const
		{
			return isVisible;
		}


		//! Set visibility of node
		virtual void setVisible(bool visibility)
		{
			isVisible = visibility;

			std::list<CNode *>::iterator it = Children.begin();

			for( ; it != Children.end(); ++it )
				(*it)->setVisible(visibility);
		}


		//! Render node
		virtual void render()
		{

		}


        virtual GLuint getShaderProgramID()
        {
            return ShaderProgramID;
        }



        virtual void setShaderProgramID(GLuint programId)
        {
            ShaderProgramID = programId;
        }


    protected:
        //! Pointer to parent entity
        CNode*		Parent;

        //! Children list
        std::list<CNode*>	Children;

        //! Node name
        vbcString	Name;

		//! Transformation matrix
		mat4 AbsoluteTransformation;

		//! Position vector
        vec3 RelativePosition;

		//! Rotation vector
        vec3 RelativeRotation;

        //! Rotation angle
        GLfloat RotationAngle;

		//! Scale vector
        vec3 RelativeScale;

		//! Node visibility
        bool isVisible;

        GLuint ShaderProgramID;
};

#endif // CNode_HPP_INCLUDED

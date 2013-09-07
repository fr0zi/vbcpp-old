#include "CNode.hpp"

//! CONSTRUCTOR
CNode::CNode(CNode* parent, vbcString name, vec3 position,
		float xRotation, float yRotation, float zRotation, vec3 scale)
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
CNode::~CNode()
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
void CNode::addChild(CNode* child)
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
void CNode::setName(const vbcString name)
        {
        	m_Name = name;
	}


        //! Getting entity's name
vbcString CNode::getName() const
        {
        	return m_Name;
	}


	//! Get Transform Matrix
mat4 CNode::getAbsoluteTransformation()
        {
            return m_AbsoluteTransformation;
        }


	//! Remove Entity from the scene
void CNode::remove()
        {
		// If Entity have parent - we remove it from its parent children list
		if( m_Parent )
		m_Parent->removeChild(this);
        }


        //! Remove entity from children list
bool CNode::removeChild(CNode* child)
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
void CNode::removeAll()
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
void CNode::setParent(CNode* parent)
        {
		grab();
		remove();

		m_Parent = parent;

		if( m_Parent )
			m_Parent->addChild(this);

		drop();
        }


	//! Set position
void CNode::setPosition(vec3 position)
        {
		RelativePosition = position;

		updateAbsoluteTransformation();
        }


	//! Set rotation
void CNode::setXRotation(GLfloat angle)
        {
		RelativeRotation = vec3(1,0,0);
		m_XRotationAngle = angle;

		updateAbsoluteTransformation();
        }

		
	//! Set rotation
void CNode::setYRotation(GLfloat angle)
        {
		RelativeRotation = vec3(0,1,0);
		m_YRotationAngle = angle;

		updateAbsoluteTransformation();
        }		
		
		
		//! Set rotation
void CNode::setZRotation(GLfloat angle)
        {
		RelativeRotation = vec3(0,0,1);
		m_ZRotationAngle = angle;

		updateAbsoluteTransformation();
        }		
		
		
	float CNode::getXRotation()
	{
		return m_XRotationAngle;
	}
		
		
	float CNode::getYRotation()
	{
		return m_YRotationAngle;
	}		
		
		
	float CNode::getZRotation()
	{
		return m_ZRotationAngle;
	}		
		
		
	//! Set scale
 void CNode::setScale(vec3 scale)
        {
		RelativeScale = scale;

		updateAbsoluteTransformation();
        }

		
	//! Set absolute transformation
     void CNode::updateAbsoluteTransformation()
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
	const bool CNode::getVisibility() const
	{
		return m_IsVisible;
	}


	//! Set visibility of Entity
void CNode::setVisibility(bool visibility)
	{
		m_IsVisible = visibility;

		std::list<CNode *>::iterator it = m_Children.begin();

		for( ; it != m_Children.end(); ++it )
			(*it)->setVisibility(visibility);
	}
		

		//! Return shader id
GLuint CNode::getShaderProgramID()
        {
		return m_ShaderProgramID;
        }

		//! Set shader id for Entity
void CNode::setShaderProgramID(GLuint programId)
        {
		m_ShaderProgramID = programId;
        }


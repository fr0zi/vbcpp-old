#include "CNode.hpp"




CNode::CNode(CNode* parent, vbcString name, vec3 position,
	float xRotation, float yRotation, float zRotation, vec3 scale)
	: m_Parent(parent), m_Name(name),
		m_RelativePosition(position),
		m_XRotationAngle(xRotation),
		m_YRotationAngle(yRotation),
		m_ZRotationAngle(zRotation),
		m_RelativeScale(scale),
		m_IsActive(true)
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


        
CNode::~CNode()
{
	#ifdef DEBUG_MODE
		std::cout << "\t -- Deleting object " << m_Name << " with all its children.\n";
	#endif

	// Delete all children
	removeAll();
}



void CNode::addChild(CNode* child)
{
	if( child && (child != this) )
	{
		child->grab();
		child->remove();    // remove from old parent
		m_Children.push_back(child);
		child->m_Parent = this;
	}

	child->setIsActive(m_IsActive);

	//std::cout << "Adding child: " << child->getName() << std::endl;
}



void CNode::setName(const vbcString name)
{
	m_Name = name;
}



vbcString CNode::getName() const
{
	return m_Name;
}



mat4 CNode::getAbsoluteTransformation()
{
    updateAbsoluteTransformation();

    return m_AbsoluteTransformation;
}



void CNode::remove()
{
	// If Entity have parent - we remove it from its parent children list
	if( m_Parent )
		m_Parent->removeChild(this);
}



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



void CNode::setParent(CNode* parent)
{
	grab();
	remove();

	m_Parent = parent;

	if( m_Parent )
		m_Parent->addChild(this);

	drop();

	m_IsActive = m_Parent->getIsActive();

	updateAbsoluteTransformation();
}



void CNode::setPosition(vec3 position)
{
	m_RelativePosition = position;

	updateAbsoluteTransformation();
}



void CNode::setXRotation(GLfloat angle)
{
    m_RelativeRotation = vec3(1,0,0);
    m_XRotationAngle = angle;

    updateAbsoluteTransformation();
}



void CNode::setYRotation(GLfloat angle)
{
    m_RelativeRotation = vec3(0,1,0);
    m_YRotationAngle = angle;

    updateAbsoluteTransformation();
}



void CNode::setZRotation(GLfloat angle)
{
    m_RelativeRotation = vec3(0,0,1);
    m_ZRotationAngle = angle;

    updateAbsoluteTransformation();
}


vec3 CNode::getRelativePosition()
{
	return m_RelativePosition;
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



void CNode::setScale(vec3 scale)
{
	m_RelativeScale = scale;

	updateAbsoluteTransformation();
}



void CNode::updateAbsoluteTransformation()
{
    mat4 position = glm::translate(m_RelativePosition);
    mat4 rotation = glm::rotate(m_XRotationAngle, vec3(1,0,0));
    rotation *= glm::rotate(m_YRotationAngle, vec3(0,1,0));
    rotation *= glm::rotate(m_ZRotationAngle, vec3(0,0,1));
    mat4 scale = glm::scale(m_RelativeScale);

    m_AbsoluteTransformation = position * rotation * scale;

    // if Entity has parent - multiply matrices to get actual Entity transformation relative to parent
    if (m_Parent)
    m_AbsoluteTransformation = m_Parent->getAbsoluteTransformation() * m_AbsoluteTransformation;
}



bool CNode::getIsActive() const
{
    return m_IsActive;
}



void CNode::setIsActive(bool state)
{
    m_IsActive = state;

    std::list<CNode *>::iterator it = m_Children.begin();

    for( ; it != m_Children.end(); ++it )
        (*it)->setIsActive(state);
}


void CNode::render()
{

}



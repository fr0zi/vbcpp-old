#ifndef __CVISIONER_HPP__
#define __CVISIONER_HPP__

#include "Includes.hpp"

#include "CReferenceCounter.hpp"
#include "CNode.hpp"
#include "CVideoComponent.hpp"
#include "CCamera.hpp"

class CNode;


class CVisioner : virtual public CReferenceCounter
{
	public:
		CVisioner(vbcString name = "Visioner");
		virtual ~CVisioner();

		//! Set projection matrix
		void setProjectionMatrix(glm::mat4 matrix);

		//! Set view matrix
		void setViewMatrix(glm::mat4 matrix);

		//! Set current shader
		void setShaderID(GLuint id);

		//! Register node for rendering process
        void registerNodeForRender(CNode* node);

		//! Render registrated nodes
        void renderNodes(CCamera* cam);

	protected:
		//! Internal name
		vbcString	m_Name;

		//! Current projection matrix
		glm::mat4	m_ProjectionMatrix;

		//! Current view matrix
		glm::mat4	m_ViewMatrix;

		//! Current Model-View-Projection matrix
		glm::mat4	m_MVPMatrix;

		//! Current shader
		GLuint		m_CurrentShaderID;

		//! Render list
		std::list<CNode*>   m_RenderList;
};

#endif //__CVISIONER_HPP__

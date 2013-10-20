#ifndef __CCAMERA_HPP__
#define __CCAMERA_HPP__

#include "Includes.hpp"
#include "CReferenceCounter.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class CCamera : virtual public CReferenceCounter
{
	public:
		CCamera(int width = 800, int height = 600, GLfloat viewAngle = 45.0f, GLfloat nearValue = 0.1f, GLfloat farValue = 1000.0f);

		virtual ~CCamera();

		vec3 getPosition();

		GLfloat getNearValue();

		GLfloat	getFarValue();

		glm::mat4 getProjectionMatrix();

		glm::mat4 getViewMatrix();

		void setPosition(vec3 position);
		
		void setPosition(GLfloat x, GLfloat y, GLfloat z);

		void lookAt(vec3 lookAt);

		void setUpVector(vec3 upVector);

		void setFarValue(GLfloat value);

		void setNearValue(GLfloat value);

		void setViewAngle(GLfloat angle);

		void setWindowDimensions(float width, float height);

	protected:
		glm::mat4	m_ProjectionMatrix;
		glm::mat4	m_ViewMatrix;
		vec3		m_Position;
		vec3		m_LookAt;
		vec3		m_UpVector;
		GLfloat		m_FarValue;
		GLfloat		m_NearValue;
		GLfloat		m_ViewAngle;
		GLint		m_WindowWidth;
		GLint		m_WindowHeight;

		void updateProjection();

		void updateView();
};

#endif // __CCAMERA_HPP__

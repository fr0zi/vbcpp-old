#include "CCamera.hpp"


CCamera::CCamera(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
	: m_WindowWidth(width), m_WindowHeight(height), m_ViewAngle(viewAngle), m_NearValue(nearValue), m_FarValue(farValue),
	m_Position(vec3(0,0,10)), m_LookAt(vec3(0,0,0)), m_UpVector(vec3(0,1,0)),
	m_ProjectionMatrix(1.0), m_ViewMatrix(1.0)
{
	updateProjection();
	updateView();
}


CCamera::~CCamera()
{

}


vec3 CCamera::getPosition()
{
	return m_Position;
}


GLfloat CCamera::getNearValue()
{
	return m_NearValue;
}


GLfloat	CCamera::getFarValue()
{
	return m_FarValue;
}


glm::mat4 CCamera::getProjectionMatrix()
{
	return m_ProjectionMatrix;
}


glm::mat4 CCamera::getViewMatrix()
{
	return m_ViewMatrix;
}


void CCamera::setPosition(vec3 position)
{
	m_Position = position;

	updateView();
}


void CCamera::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
	vec3 pos(x, y, z);

	setPosition(pos);
}


void CCamera::lookAt(vec3 lookAt)
{
	m_LookAt = lookAt;

	updateView();
}


void CCamera::setUpVector(vec3 upVector)
{
	m_UpVector = upVector;
	
	updateView();
}


void CCamera::setFarValue(GLfloat value)
{
	m_FarValue = value;

	updateProjection();
}


void CCamera::setNearValue(GLfloat value)
{
	m_NearValue = value;

	updateProjection();
}


void CCamera::setViewAngle(GLfloat angle)
{
	m_ViewAngle = angle;

	updateProjection();
}


void CCamera::setWindowDimensions(float width, float height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;

	updateProjection();
}


void CCamera::updateProjection()
{
	m_ProjectionMatrix = glm::perspective(m_ViewAngle, float(m_WindowWidth / m_WindowHeight), m_NearValue, m_FarValue);
}


void CCamera::updateView()
{
	m_ViewMatrix = glm::lookAt(
						m_Position,
						m_LookAt,
						m_UpVector );
}


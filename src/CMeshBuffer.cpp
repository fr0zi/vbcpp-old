#include "CMeshBuffer.hpp"

CMeshBuffer::CMeshBuffer()
{

}


CMeshBuffer::~CMeshBuffer()
{
	if(m_IsValid)
	{
		delete[] m_Vertices;

		glDeleteBuffers(1, &m_VertexBufferID);
	}
}


bool CMeshBuffer::isValid()
{
	return m_IsValid;
}


void CMeshBuffer::createVBO()
{
	if(m_IsValid)
	{
		glGenBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, m_QuantumOfVertices * sizeof(S3DVertex), m_Vertices, GL_STATIC_DRAW);
	}
}


GLuint CMeshBuffer::getVertexBufferID()
{
	return m_VertexBufferID;
}


unsigned int CMeshBuffer::getQuantumOfVertices()
{

	return m_QuantumOfVertices;
}


S3DVertex* CMeshBuffer::getVertices()
{
	return m_Vertices;
}


SMaterial& CMeshBuffer::getMaterial()
{
	return m_Material;
}


void CMeshBuffer::setVerticesData(S3DVertex* vertices, unsigned int quantumOfVertices, bool isValid)
{
	m_Vertices = vertices;

	m_QuantumOfVertices = quantumOfVertices;

	m_IsValid = isValid;

	// for debugging
	//printf("Number of vertices: %d\n", m_QuantumOfVertices);
}


void CMeshBuffer::setMaterial(SMaterial& material)
{
	m_Material = material;
}




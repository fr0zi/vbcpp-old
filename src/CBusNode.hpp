#ifndef __CMESHNODE_HPP_INCLUDED__
#define __CMESHNODE_HPP_INCLUDED__


#include "CNode.hpp"
#include "CBusMesh.hpp"


class CBusNode : public CNode
{
	public:
		CBusNode(CNode* parent = 0, vbcString name = "",
		vec3 position = vec3(0,0,0),
		float xRotation = 0.0f, 
		float yRotation = 0.0f,
		float zRotation = 0.0f,
		vec3 scale = vec3(1,1,1))
		: CNode(parent, name, position,
			xRotation, yRotation, zRotation, scale)
		{

		}


		virtual ~CBusNode()
		{
			if(m_BusMesh)
				m_BusMesh->drop();
		}


		void setBusMesh(CBusMesh* mesh)
		{
			m_BusMesh = mesh;
		}

		CBusMesh* getBusMesh()
		{
			return m_BusMesh;
		}

		void render()
		{	
			GLuint TextureID = glGetUniformLocation(m_ShaderProgramID, "myTextureSampler");
			GLuint AlphaValueID = glGetUniformLocation(m_ShaderProgramID, "alpha");
		
			CMeshBuffer* mb;
		
	  		//First we draw all solid objects
			for(unsigned int mbLoop = 0; mbLoop < m_BusMesh->getQuantumOfMeshBuffers(); mbLoop++)
			{
				mb = m_BusMesh->getMeshBuffer(mbLoop);

				if(mb->getMaterial().transparency == 0)
				{	
					glActiveTexture(GL_TEXTURE0);		
					glBindTexture(GL_TEXTURE_2D, mb->getMaterial().textureId);
					
					glUniform1i(TextureID, 0);
					glUniform1f(AlphaValueID, mb->getMaterial().transparency);
					
					
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, mb->getVertexBufferID() );
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
					
					glEnableVertexAttribArray(1);
					glBindBuffer(GL_ARRAY_BUFFER, mb->getNormalBufferID() );
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)0 );

					glEnableVertexAttribArray(2);
					glBindBuffer(GL_ARRAY_BUFFER, mb->getTexCoordBufferID() );
					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
				
					
					glDrawArrays(GL_TRIANGLES, 0, mb->getQuantumOfVertices() );
					

					glDisableVertexAttribArray(0);
					glDisableVertexAttribArray(1);
					glDisableVertexAttribArray(2);
				}
			}

	  			//Secondly - we draw all transparent objects.
	  			//Transparency ratio is set from material variable 'transparency'
			for(unsigned int mbLoop = 0; mbLoop < m_BusMesh->getQuantumOfMeshBuffers(); mbLoop++)
			{
				mb = m_BusMesh->getMeshBuffer(mbLoop);

				if(mb->getMaterial().transparency <= 1)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mb->getMaterial().textureId);
					glUniform1i(TextureID, 0);
					glUniform1f(AlphaValueID, mb->getMaterial().transparency);

					glDisable(GL_LIGHTING);
					glEnable(GL_BLEND);					
					
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, mb->getVertexBufferID() );
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
					
					glEnableVertexAttribArray(1);
					glBindBuffer(GL_ARRAY_BUFFER, mb->getNormalBufferID() );
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)0 );

					glEnableVertexAttribArray(2);
					glBindBuffer(GL_ARRAY_BUFFER, mb->getTexCoordBufferID() );
					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
				
				
					glDrawArrays(GL_TRIANGLES, 0, mb->getQuantumOfVertices() );

					
					glDisableVertexAttribArray(0);
					glDisableVertexAttribArray(1);
					glDisableVertexAttribArray(2);

					glDisable(GL_BLEND);					
					glEnable(GL_LIGHTING);
				}
			}
		}
		
		
	private:
		CBusMesh*	m_BusMesh;

};

#endif // __CMESHNODE_HPP_INCLUDED__

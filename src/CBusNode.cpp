#include "CBusNode.hpp"

		CBusNode::CBusNode(CNode* parent, vbcString name, vec3 position,
		float xRotation, float yRotation, float zRotation, vec3 scale)
		: CNode(parent, name, position,
			xRotation, yRotation, zRotation, scale)
		{

		}


		CBusNode::~CBusNode()
		{
			if(m_Mesh)
				m_Mesh->drop();
		}


		void CBusNode::setMesh(CMesh* mesh)
		{
			m_Mesh = mesh;
			m_Mesh->grab();
		}

		CMesh* CBusNode::getMesh()
		{
			return m_Mesh;
		}

		void CBusNode::render()
		{	
			
			GLuint TextureID = glGetUniformLocation(m_ShaderProgramID, "myTextureSampler");
			GLuint AlphaValueID = glGetUniformLocation(m_ShaderProgramID, "alpha");
		
			CMeshBuffer* mb;
		
	  		//First we draw all solid objects
			for(unsigned int mbLoop = 0; mbLoop < m_Mesh->getQuantumOfMeshBuffers(); mbLoop++)
			{
				mb = m_Mesh->getMeshBuffer(mbLoop);

				if(mb->getMaterial().transparency == 0)
				{	
					glActiveTexture(GL_TEXTURE0);		
					glBindTexture(GL_TEXTURE_2D, mb->getMaterial().textureId);
					
					glUniform1i(TextureID, 0);
					glUniform1f(AlphaValueID, mb->getMaterial().transparency);			


					// Setting interleaved VBO
					// Vertices: 3 x float, repeated in 32 bytes (8 * sizeof(float)), starting at 0 byte
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, mb->getVertexBufferID() );
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0 );

					// Normals: 3 x float, repeated in 32 bytes (8 * sizeof(float)), starting at 12 byte (3 * sizeof(float))
					glEnableVertexAttribArray(1);
					glBindBuffer(GL_ARRAY_BUFFER, mb->getVertexBufferID() );
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*) (sizeof(float)*3) );

					// Texture coords: 2 x float, repeated in 32 bytes (8 * sizeof(float)), starting at 24 byte (6 * sizeof(float))
					glEnableVertexAttribArray(2);
					glBindBuffer(GL_ARRAY_BUFFER, mb->getVertexBufferID() );
					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*) (sizeof(float)*6) );
					
					// Drawing VBO
					glDrawArrays(GL_TRIANGLES, 0, mb->getQuantumOfVertices() );
					

					glDisableVertexAttribArray(0);
					glDisableVertexAttribArray(1);
					glDisableVertexAttribArray(2);
				}
			}

	  			//Secondly - we draw all transparent objects.
	  			//Transparency ratio is set from material variable 'transparency'
			for(unsigned int mbLoop = 0; mbLoop < m_Mesh->getQuantumOfMeshBuffers(); mbLoop++)
			{
				mb = m_Mesh->getMeshBuffer(mbLoop);

				if(mb->getMaterial().transparency <= 1)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mb->getMaterial().textureId);
					glUniform1i(TextureID, 0);
					glUniform1f(AlphaValueID, mb->getMaterial().transparency);

					glDisable(GL_LIGHTING);
					glEnable(GL_BLEND);					
								

					// Setting interleaved VBO
					// Vertices: 3 x float, repeated in 32 bytes (8 * sizeof(float)), starting at 0 byte
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, mb->getVertexBufferID() );
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0 );

					// Normals: 3 x float, repeated in 32 bytes (8 * sizeof(float)), starting at 12 byte (3 * sizeof(float))
					glEnableVertexAttribArray(1);
					glBindBuffer(GL_ARRAY_BUFFER, mb->getVertexBufferID() );
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*) (sizeof(float)*3) );

					// Texture coords: 2 x float, repeated in 32 bytes (8 * sizeof(float)), starting at 24 byte (6 * sizeof(float))
					glEnableVertexAttribArray(2);
					glBindBuffer(GL_ARRAY_BUFFER, mb->getVertexBufferID() );
					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*) (sizeof(float)*6) );

				
					// Drawing VBO
					glDrawArrays(GL_TRIANGLES, 0, mb->getQuantumOfVertices() );

					
					glDisableVertexAttribArray(0);
					glDisableVertexAttribArray(1);
					glDisableVertexAttribArray(2);

					glDisable(GL_BLEND);					
					glEnable(GL_LIGHTING);
				}
			}
			
		}
		


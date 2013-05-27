#include "include/Includes.hpp"
#include "include/CVisioner.hpp"
#include "include/CBusNode.hpp"
#include "include/CSceneManager.hpp"

#include "include/shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

bool running = true;


void readInput()
{
	// Check if the ESC key was pressed or the window was closed
	if( glfwGetKey( GLFW_KEY_ESC ) == GLFW_PRESS )
			running = false;

}



int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("You must specify model name and texture path as parameter.\n");
		return 1;
	}

	CVisioner* renderer = new CVisioner;
	
	if(!renderer->createWindow(800, 600, 100, 100))
		return -1;


	renderer->setWindowTitle("Virtual Bus Core++");

	CSceneManager* smgr = new CSceneManager(0, "SceneManager", renderer);

	CBusMesh* busMesh = new CBusMesh(argv[1], argv[2]);
	
	smgr->addBusMeshSceneNode(busMesh, "Bus"); 


	GLuint shaderId = LoadShaders("shader.vert", "shader.frag");

	glm::mat4 Projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);

	glm::mat4 View = glm::lookAt(
						glm::vec3(-3,3,-12),
						glm::vec3(0,0,0),
						glm::vec3(0,1,0) );

	glm::mat4 position = glm::translate(vec3(0,0,0));
	glm::mat4 rotation = glm::rotate(-90.0f, vec3(1,0,0));
	glm::mat4 scale = glm::scale(vec3(1,1,1));

	glm::mat4 Model = position * rotation * scale;

	//glm::mat4 MVP = Projection * View * Model;

	GLuint ProjectionMatrixID = glGetUniformLocation(shaderId, "ProjectionMatrix");
	GLuint ModelMatrixID = glGetUniformLocation(shaderId, "ModelMatrix");
	GLuint ViewMatrixID = glGetUniformLocation(shaderId, "ViewMatrix");
	
	GLuint TextureID = glGetUniformLocation(shaderId, "myTextureSampler");
	GLuint AlphaValueID = glGetUniformLocation(shaderId, "alpha");
	//bool ValidTextureID = glGetUniformLocation(shaderId, "validTexture");
	

    while( running && glfwGetWindowParam( GLFW_OPENED ) )
    {
		renderer->startRender(0.7, 0.7, 0.9, 1.0);

			
			CMeshBuffer* mb;

			glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &Projection[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
			
			
	  		//First we draw all solid objects
			
			for(unsigned int mbLoop = 0; mbLoop < busMesh->getQuantumOfMeshBuffers(); mbLoop++)
			{
				mb = busMesh->getMeshBuffer(mbLoop);

				if(mb->getMaterial().transparency == 0)
				{
					glUseProgram(shaderId);
					
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
			for(unsigned int mbLoop = 0; mbLoop < busMesh->getQuantumOfMeshBuffers(); mbLoop++)
			{
				mb = busMesh->getMeshBuffer(mbLoop);

				if(mb->getMaterial().transparency <= 1)
				{
					glUseProgram(shaderId);

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
			//*/

		renderer->finishRender();

		readInput();
	}

	smgr->drop();
	renderer->drop();

	return 0;
}


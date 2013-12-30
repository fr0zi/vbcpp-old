#include "CTerrainNode.hpp"

CTerrainNode::CTerrainNode(CNode* parent, vbcString heightmap, vbcString name)
: CNode(parent, name)
{

}


CTerrainNode::~CTerrainNode()
{
    m_Mesh->drop();
}



void CTerrainNode::generateTerrainMesh(vbcString filename)
{
    unsigned char* imageData;

    imageData = loadBMP(filename.c_str());

    		const float CellSize = 1.0f;
		const float MaxHeight = 20.0f;
		glm::vec3 Position(0.0f, 0.0f, 0);


		std::vector<glm::vec3> Vertices;
		std::vector<glm::vec2> Coords;
		std::vector<glm::vec3> Normals;

		
		
		int a = 10;

		for (int z = 0; z < height; ++z)
		{
			Position.x = 0;

			for (int x = 0; x < width; ++x)
			{
				int Red = static_cast<int>(heightmapImage[(z * width + x)]);

				float h = static_cast<float>(static_cast<float>(Red) / 256.0f) * MaxHeight;
				Position.y = h;

				Vertices.push_back(Position);

				Normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
				//Coords.push_back(glm::vec2((x % a) / static_cast<float>(a), (z % a) / static_cast<float>(a)));

				Position.x += CellSize;
			}

			Position.z += CellSize;
		}
		

		std::vector<unsigned int> Indices;

		int Index = 0;
		int count = 0;

		for (int z = 0; z < height - 1; ++z)
		{
			for (int x = 0; x < width - 1; ++x)
			{
				Indices.push_back(Index);
				Indices.push_back(Index + 1);
				Indices.push_back(Index + width);

				count += 3;


				Indices.push_back(Index + width);
				Indices.push_back(Index + 1);
				Indices.push_back(Index + width + 1);

				count += 3;

				++Index;
			}
			++Index;
		}


		glGenBuffers(1, &_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * _QuantumOfVertices, _Vertices, GL_STATIC_DRAW);
}

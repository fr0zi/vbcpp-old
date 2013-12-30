#ifndef __CTERRAIN_HPP__
#define __CTERRAIN_HPP__

#include "Includes.hpp"
#include "CNode.hpp"
#include "CMesh.hpp"
#include "LoaderBMP.hpp"


class CTerrainNode : CNode
{
    public:
        CTerrainNode(CNode* parent = 0, vbcString heightmap = "", vbcString name = "Terrain");
        virtual ~CTerrainNode();

    private:
        void generateTerrainMesh(vbcString filename);

        CMesh*  m_Mesh;
        GLuint  M_ShaderID();
};

#endif //__CTERRAIN_HPP__

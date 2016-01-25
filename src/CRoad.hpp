#ifndef CROAD_HPP_INCLUDED
#define CROAD_HPP_INCLUDED

#include "Includes.hpp"
#include "CRefCounter.hpp"
#include "S3DVertex.hpp"
#include "CShaderResource.hpp"


class CRoad : public CRefCounter
{
    public:
        CRoad(GLfloat radius, glm::vec3 center, GLfloat start_angle, GLfloat end_angle, vbcString name = "Road" );
        virtual ~CRoad();

    void render();

    private:
        S3DVertex*  _vertices;
        GLfloat     _radius;
        glm::vec3   _center;
        GLfloat     _start_angle;
        GLfloat     _end_angle;
        GLint       _steps;
        GLfloat     _width;
        GLuint      _VertexBufferID;
        CShaderResource*      _shaderResource;

        void    _calculateMesh();
        void    _createVBO();
};

#endif // CROAD_HPP_INCLUDED

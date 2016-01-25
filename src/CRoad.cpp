#include "CRoad.hpp"

CRoad::CRoad(GLfloat radius, glm::vec3 center, GLfloat start_angle, GLfloat end_angle, vbcString name)
    : _radius(radius), _center(center), _start_angle(start_angle), _end_angle(end_angle), _steps(5), _width(3.0f)
{
    printf("Creating road object\n");

    _vertices = 0;

    _calculateMesh();

    _shaderResource = new CShaderResource("road.shdr");

    _createVBO();
}

CRoad::~CRoad()
{
    printf("Destroying road object\n");

    glDeleteBuffers(1, &_VertexBufferID);

    _shaderResource->drop();

    if (_vertices)
    {
        delete [] _vertices;
        _vertices = 0;
    }
}


void CRoad::_calculateMesh()
{
    printf("Creating road mesh\n");

    if (_vertices)
    {
        delete [] _vertices;
        _vertices = 0;
    }

    _vertices = new S3DVertex[_steps];


    float t = _start_angle;
    //int pos = 0;
    for (int i = 0; i < _steps; i++)
    {
        printf("Step %d\n", i);
        float x_inner = (_radius-_width) * cos(t) + _center[0];
        float y_inner = (_radius-_width) * sin(t) + _center[1];

        float x_outer = (_radius+_width) * cos(t) + _center[0];
        float y_outer = (_radius+_width) * sin(t) + _center[1];

        //printf("x_inner: %f\n", x_inner);
        //printf("y_inner: %f\n", y_inner);

        _vertices[_steps * 2].coord[0] = x_inner;
        _vertices[_steps * 2].coord[1] = y_inner;
        _vertices[_steps * 2].coord[2] = 0.0f;

        _vertices[_steps * 2 + 1].coord[0] = x_outer;
        _vertices[_steps * 2 + 1].coord[1] = y_outer;
        _vertices[_steps * 2 + 1].coord[2] = 0.0f;

        t += (_end_angle - _start_angle) / _steps;
    }
}

void CRoad::_createVBO()
{
    printf("Creating VBO for road mesh\n");

    printf("Number of vertices: %d\n", sizeof(_steps));
    printf("Number of vertex structure: %d\n", sizeof(S3DVertex));

    glGenBuffers(1, &_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, _steps * sizeof(S3DVertex), _vertices, GL_STATIC_DRAW);
}

void CRoad::render()
{
    // Setting interleaved VBO
    // Vertices: 3 x float, repeated in 32 bytes (8 * sizeof(float)), starting at 0 byte
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _VertexBufferID );
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0 );



    // Drawing VBO
	glDrawArrays(GL_TRIANGLE_STRIP, 0, _steps);

	glDisableVertexAttribArray(0);
}

#ifndef CSHADERRESOURCE_HPP
#define CSHADERRESOURCE_HPP

#include "CResource.hpp"

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <glew/glew.h>


class CShaderResource : virtual public CResource
{
	public:
		CShaderResource(vbcString filename = "");
		virtual ~CShaderResource();

	protected:
		virtual void loadResource();
		GLuint LoadVertexShader(const char* shader_file);
		GLuint LoadFragmentShader(const char* shader_file);
};

GLuint LoadShader(const char* shaderFilename);



#endif // CSHADERRESOURCE_HPP

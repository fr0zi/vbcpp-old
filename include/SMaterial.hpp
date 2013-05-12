#ifndef SMATERIAL_HPP
#define SMATERIAL_HPP

#include "Includes.hpp"

struct SMaterial
{
	SMaterial()	
	{
		name = "";
		textureName = "";
		transparency = 0.0f;
		textureId = 0;
		shininess = 0.0f;
	}

	SMaterial(const SMaterial& m)
	{
		name = m.name;
		textureName = m.textureName;
		textureId = m.textureId;
		transparency = m.transparency;
		ambient[0] = m.ambient[0];
		ambient[1] = m.ambient[1];
		ambient[2] = m.ambient[2];
		ambient[2] = m.ambient[3];
		diffuse[0] = m.diffuse[0];
		diffuse[1] = m.diffuse[1];
		diffuse[2] = m.diffuse[2];
		diffuse[3] = m.diffuse[3];
		specular[0] = m.specular[0];
		specular[1] = m.specular[1];
		specular[2] = m.specular[2];
		specular[3] = m.specular[3];
		shininess = m.shininess;
		offset[0] = m.offset[0];
		offset[1] = m.offset[1];
		scale[0] = m.scale[0];
		scale[1] = m.scale[1];
	}

	SMaterial& operator = (const SMaterial& m)
	{
		name = m.name;
		textureName = m.textureName;
		textureId = m.textureId;
		transparency = m.transparency;
		ambient[0] = m.ambient[0];
		ambient[1] = m.ambient[1];
		ambient[2] = m.ambient[2];
		ambient[3] = m.ambient[3];
		diffuse[0] = m.diffuse[0];
		diffuse[1] = m.diffuse[1];
		diffuse[2] = m.diffuse[2];
		diffuse[3] = m.diffuse[3];
		specular[0] = m.specular[0];
		specular[1] = m.specular[1];
		specular[2] = m.specular[2];
		specular[3] = m.specular[3];
		shininess = m.shininess;
		offset[0] = m.offset[0];
		offset[1] = m.offset[1];
		scale[0] = m.scale[0];
		scale[1] = m.scale[1];
	}


	std::string name;
	std::string textureName;
	GLuint textureId;
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
	GLfloat offset[2];
	GLfloat scale[2];
	GLfloat transparency;
};

#endif // SMATERIAL_HPP

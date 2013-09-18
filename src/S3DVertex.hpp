#ifndef __S3DVERTEX_HPP__
#define __S3DVERTEX_HPP__

struct S3DVertex
{
	S3DVertex()
	{
	
	}

	~S3DVertex()
	{

	}

	float coord[3];
	float normal[3];
	float texcoord[2];
};

#endif // __S3DVERTEX_HPP__

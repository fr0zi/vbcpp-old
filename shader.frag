#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform float alpha;
uniform float validTexture;

void main(){

	// Output color = color of the texture at the specified UV
	if(validTexture)
		color.rgb = texture2D( myTextureSampler, UV ).rgb;
	else
		color.rgb = vec3(1,1,1);

	color.a = alpha;
}

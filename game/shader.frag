#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform float alpha;


void main()
{
	color.rgb = texture2D( myTextureSampler, UV ).rgb;
	color.a = 1.0 - alpha;

	if ( color.a == 0.0f )
		discard;
}

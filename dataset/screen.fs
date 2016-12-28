#version 330


// Input Data from vertex shader
in vec2 UV;

// Output Data
layout(location=0) out vec4 color;

uniform sampler2D uSampler;



void main()
{
	color = texture(uSampler, UV);
}
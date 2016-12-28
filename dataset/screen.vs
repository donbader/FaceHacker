#version 330

// Input Data from CPU
layout(location=0) in vec3 position; //model's x,y,z
layout(location=1) in vec2 texcoord; //model's u,v
layout(location=2) in vec3 normal;	//model's normals


// Output Data to fragment shader
out vec2 UV;        	//UV


// Global Data
uniform mat4 M; //Model Matrix
uniform mat4 V; //View Matrix
uniform mat4 P; //Projection Matrix

// Temp Data
mat4 MVP = P*V*M;
mat4 MV = V*M;


void main()
{

	UV=texcoord;
	gl_Position=MVP*vec4(position, 1.0);
}

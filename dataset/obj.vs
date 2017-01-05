#version 330

// Input Data from CPU
layout(location=0) in vec3 position; //model's x,y,z
layout(location=1) in vec2 texcoord; //model's u,v
layout(location=2) in vec3 normal;	//model's normals
layout(location=3) in vec2 boneIndex;
layout(location=4) in vec2 weight;


// Output Data to fragment shader
out vec2 UV;        	//UV
out vec4 color_from_vs;
out vec3 position_c; 	//eye direction in the camera space
out vec3 normal_c;    	//model normal in the camera space


// Global Data
uniform mat4 M; //Model Matrix
uniform mat4 V; //View Matrix
uniform mat4 P; //Projection Matrix
uniform mat4 Bone[10];

// Temp Data
mat4 MVP = P*V*M;
mat4 MV = V*M;
mat4 VP = P*V;


void main()
{

	UV=texcoord;

	vec4 vertex_4 =  vec4(position, 1.0);
	vec4 normal_4 =  vec4(normal, 0.0);

	highp int index_x = int(boneIndex.x);
	highp int index_y = int(boneIndex.y);

	if(index_x == 0)
		color_from_vs = vec4(0,0,0,0);
	else if(index_x == 2)
		color_from_vs = vec4(0,1,0,0);
	else if (index_x == 3){
		color_from_vs = vec4(1,0,1,0);
	}
	else if(index_x ==5)
		color_from_vs = vec4(1,0.5,0.5,0);
	else
		color_from_vs = vec4(0,0,0,0);


	// vec4 vertex_4 = (Bone[int(boneIndex.x)] * vec4(position, 1.0) ) * weight.x \
	// 			+ (Bone[int(boneIndex.y)] * vec4(position, 1.0) ) * weight.y;

	// vec4 normal_4 = (Bone[int(boneIndex.x)] * vec4(normal, 0)) * weight.x \
	// 			+	(Bone[int(boneIndex.y)] * vec4(normal, 0)) * weight.y;

	// if(index_x == 2){
	// 	vertex_4 = vec4(0,0,0,0);
	// }

	gl_Position=MVP*vertex_4;
	// if(index_x == 3)gl_Position = vec4(0,0,-1000,0);

	position_c = ( MV * vertex_4).xyz;
	normal_c = ( MV * normal_4).xyz;
}






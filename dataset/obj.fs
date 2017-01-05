#version 330


// Input Data from vertex shader
in vec2 UV;
in vec4 color_from_vs;
in vec3 position_c;
in vec3 normal_c;

// Output Data
layout(location=0) out vec4 color;

uniform sampler2D uSampler;
uniform mat4 V;							//View matrix

// Light Related
//light position in the world space
uniform vec3 lightPos_w = vec3(0,0,320);

//Light emission properties
uniform vec3 lightColor = vec3(1,1,1);
uniform float lightPower = 60000.f;
uniform float lightAlpha = 50.f;

//Material factors
uniform vec3 materialDiffuseFactor = vec3(0.3, 0.3, 0.3);
uniform vec3 materialAmbientFactor = vec3(0.1, 0.1, 0.1);
uniform vec3 materialSpecularFactor = vec3(0.1, 0.1, 0.1);


// Mode Control
uniform int mode = 2; // 0 = texture mode, 1 = light mode, 2 = blend mode

void main()
{
	if(mode == 0){
		color = texture(uSampler, UV);
	}
	else if(mode == 1 || mode == 2){
		// Material's Property
		vec3 materialDiffuseColor = materialDiffuseFactor * vec3(1, 1, 1);
		vec3 materialAmbientColor = materialAmbientFactor * materialDiffuseColor;
		vec3 materialSpecularColor = materialSpecularFactor;



		//In camera space, camera is at (0,0,0)
		vec3 eyeDir_c = vec3(0, 0, 0) - position_c;

		//Vector that goes from the vertex to the light
		vec3 lightPos_c = (V * vec4(lightPos_w, 1)).xyz;
		vec3 lightDir_c = lightPos_c + eyeDir_c;

		//Distance from light to vertex
		float distance = length(lightPos_c - position_c);

		//Cosine between normal & light direction
		vec3 n = normalize(normal_c);
		vec3 l = normalize(lightDir_c);
		float cosTheta = clamp(dot(n, l), 0, 1);

		//Cosine for Blinn-Phong model
		vec3 E = normalize(eyeDir_c);
		vec3 h = normalize(l + E);
		float cosPsi = clamp(dot(n, h), 0 ,1);

		//Blinn-Phong model
		color.rgb = materialAmbientColor
			  + (materialDiffuseColor * lightColor * lightPower * cosTheta
			  + materialSpecularColor * lightColor * lightPower * pow(cosPsi, lightAlpha)) / (distance * distance);
	   	if(mode == 2){
			color *= texture(uSampler, UV);
  		}

	}
	else{
		color = color_from_vs;
	}

}

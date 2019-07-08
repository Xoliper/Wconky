###############################################################
##	1. Fragment Part				     
###############################################################

#fragment-start
#version 330 core

in vec2 UV;
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform mat4 MV;

void main(){


  color =  vec4(1,1,1,1)*vec4(1,1,1,texture2D(myTextureSampler, UV).r);
//	color = texture2D(myTextureSampler, UV).rrra;

}
#fragment-end



###############################################################
##	2. Vertex Part				    	     
###############################################################

#vertex-start
#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 V;
uniform mat4 M;


void main(){
	gl_Position = M*V* vec4(vertexPosition_modelspace,1);
	UV = vertexUV;
}
#vertex-end


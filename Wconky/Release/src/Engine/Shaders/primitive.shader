###############################################################
##	1. Fragment Part				     
###############################################################

#fragment-start
#version 330 core

out vec4 color;
uniform vec4 primitiveColor;

void main(){
	color = vec4(primitiveColor.r,primitiveColor.g,primitiveColor.b,primitiveColor.a);
}
#fragment-end



###############################################################
##	2. Vertex Part				    	     
###############################################################

#vertex-start
#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 V;
uniform mat4 M;


void main(){
	gl_Position = M*V* vec4(vertexPosition_modelspace,1);
	
}
#vertex-end


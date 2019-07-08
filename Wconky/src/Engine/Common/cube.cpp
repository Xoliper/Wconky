#include "cube.hpp"

Cube::Cube(Texture * tex, mat4 * pv) {

	params = ShaderLoader::GetProgram("simpleShader");

	this->tex = tex;
	this->pv = pv;
	change = false;

	model = glm::mat4(1.0f);
	mvp = (*pv) * model; // Remember, matrix multiplication is the other way around

	float w = 0.15f;//((float)tex->dimm.width/(float)1920)/2;
	float h = 0.15f;//((float)tex->dimm.height/(float)1080)/2;
	float z = 0.15f;


	//Vertex coords

	gVertexBufferData[0] = -w;
	gVertexBufferData[1] = -h;
	gVertexBufferData[2] =  z;
	gVertexBufferData[3] = -w;
	gVertexBufferData[4] = h;
	gVertexBufferData[5] = z;
	gVertexBufferData[6] = w;
	gVertexBufferData[7] = -h;
	gVertexBufferData[8] = z;
	gVertexBufferData[9] = w;
	gVertexBufferData[10] = -h;
	gVertexBufferData[11] = z;
	gVertexBufferData[12] = -w;
	gVertexBufferData[13] = h;
	gVertexBufferData[14] = z;
	gVertexBufferData[15] = w;
	gVertexBufferData[16] = h;
	gVertexBufferData[17] = z;

	gVertexBufferData[18] = -w;
	gVertexBufferData[19] = h;
	gVertexBufferData[20] =  z;
	gVertexBufferData[21] = -w;
	gVertexBufferData[22] = h;
	gVertexBufferData[23] = -z;
	gVertexBufferData[24] = w;
	gVertexBufferData[25] = h;
	gVertexBufferData[26] = -z;
	gVertexBufferData[27] = w;
	gVertexBufferData[28] = h;
	gVertexBufferData[29] = -z;
	gVertexBufferData[30] = -w;
	gVertexBufferData[31] = h;
	gVertexBufferData[32] = z;
	gVertexBufferData[33] = w;
	gVertexBufferData[34] = h;
	gVertexBufferData[35] = z;

	gVertexBufferData[36] = -w;
	gVertexBufferData[37] = -h;
	gVertexBufferData[38] =  -z;
	gVertexBufferData[39] = -w;
	gVertexBufferData[40] = h;
	gVertexBufferData[41] = -z;
	gVertexBufferData[42] = w;
	gVertexBufferData[43] = -h;
	gVertexBufferData[44] = -z;
	gVertexBufferData[45] = w;
	gVertexBufferData[46] = -h;
	gVertexBufferData[47] = -z;
	gVertexBufferData[48] = -w;
	gVertexBufferData[49] = h;
	gVertexBufferData[50] = -z;
	gVertexBufferData[51] = w;
	gVertexBufferData[52] = h;
	gVertexBufferData[53] = -z;

	gVertexBufferData[54] = -w;
	gVertexBufferData[55] = -h;
	gVertexBufferData[56] =  z;
	gVertexBufferData[57] = -w;
	gVertexBufferData[58] = -h;
	gVertexBufferData[59] = -z;
	gVertexBufferData[60] = w;
	gVertexBufferData[61] = -h;
	gVertexBufferData[62] = -z;
	gVertexBufferData[63] = w;
	gVertexBufferData[64] = -h;
	gVertexBufferData[65] = -z;
	gVertexBufferData[66] = -w;
	gVertexBufferData[67] = -h;
	gVertexBufferData[68] = z;
	gVertexBufferData[69] = w;
	gVertexBufferData[70] = -h;
	gVertexBufferData[71] = z;

	gVertexBufferData[72] = -w;
	gVertexBufferData[73] = h;
	gVertexBufferData[74] =  z;
	gVertexBufferData[75] = -w;
	gVertexBufferData[76] = h;
	gVertexBufferData[77] = -z;
	gVertexBufferData[78] = -w;
	gVertexBufferData[79] = -h;
	gVertexBufferData[80] = z;
	gVertexBufferData[81] = -w;
	gVertexBufferData[82] = -h;
	gVertexBufferData[83] = z;
	gVertexBufferData[84] = -w;
	gVertexBufferData[85] = h;
	gVertexBufferData[86] = -z;
	gVertexBufferData[87] = -w;
	gVertexBufferData[88] = -h;
	gVertexBufferData[89] = -z;

	gVertexBufferData[90] = w;
	gVertexBufferData[91] = h;
	gVertexBufferData[92] =  z;
	gVertexBufferData[93] = w;
	gVertexBufferData[94] = h;
	gVertexBufferData[95] = -z;
	gVertexBufferData[96] = w;
	gVertexBufferData[97] = -h;
	gVertexBufferData[98] = z;
	gVertexBufferData[99] = w;
	gVertexBufferData[100] = -h;
	gVertexBufferData[101] = z;
	gVertexBufferData[102] = w;
	gVertexBufferData[103] = h;
	gVertexBufferData[104] = -z;
	gVertexBufferData[105] = w;
	gVertexBufferData[106] = -h;
	gVertexBufferData[107] = -z;

	//Texture coords

	gUvBufferData[0] = 0.0f;
	gUvBufferData[1] = 0.0f;
	gUvBufferData[2] = 0.0f;
	gUvBufferData[3] = 1.0f;
	gUvBufferData[4] = 1.0f;
	gUvBufferData[5] = 0.0f;
	gUvBufferData[6] = 1.0f;
	gUvBufferData[7] = 0.0f;
	gUvBufferData[8] = 0.0f;
	gUvBufferData[9] = 1.0f;
	gUvBufferData[10] = 1.0f;
	gUvBufferData[11] = 1.0f;
	for(int i = 12; i<72; i++){
		gUvBufferData[i] = 0.0f;
	}


	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gUvBufferData), gUvBufferData, GL_STATIC_DRAW);
}

Cube::~Cube() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Cube::Render(){

	mvp = (*pv) * model;

	glUseProgram(params->at(0));

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glUniformMatrix4fv(params->at(1), 1, GL_FALSE, &mvp[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->texture_id);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(params->at(2), 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


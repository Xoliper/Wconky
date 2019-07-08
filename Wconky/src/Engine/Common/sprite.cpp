#include "sprite.hpp"

Sprite::Sprite(Texture * tex,  World * world, PureTranslation pt, int flags, int width = -1, int height = -1) {

	params = ShaderLoader::GetProgram("sprite");

	this->pt = pt;
	this->tex = tex;
	this->world = world;
	change = false;

	model = glm::mat4(1.0f);
	rotation = glm::rotate(0.0f, glm::vec3( 1.0f, 1.0f, 1.0f));
	translation = glm::translate(glm::mat4(), glm::vec3(pt.x*2, pt.y*2, pt.z));
	scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));



	float w_ver;
	float h_ver;

	if(width == -1){
		w_ver = (float)tex->dimm.width/world->width;
	} else {
		w_ver = (float)width/world->width;
	}
	if(height == -1){
		h_ver = (float)tex->dimm.height/world->height;
	} else {
		h_ver = (float)height/world->height;
	}
	float w_ver_start = -w_ver;
	float h_ver_start = -h_ver;


	if(flags == false){
		w_ver_start = 0.0f;
		h_ver_start = 0.0f;
		w_ver *= 2;
		h_ver *=2;
	}

	gVertexBufferData[0] = w_ver_start;
	gVertexBufferData[1] = h_ver_start;
	gVertexBufferData[2] =  0.f;

	gVertexBufferData[3] = w_ver_start;
	gVertexBufferData[4] = h_ver;
	gVertexBufferData[5] = 0.f;

	gVertexBufferData[6] = w_ver;
	gVertexBufferData[7] = h_ver_start;
	gVertexBufferData[8] = 0.f;

	gVertexBufferData[9] = w_ver;
	gVertexBufferData[10] = h_ver_start;
	gVertexBufferData[11] = 0.f;

	gVertexBufferData[12] = w_ver_start;
	gVertexBufferData[13] = h_ver;
	gVertexBufferData[14] = 0.f;

	gVertexBufferData[15] = w_ver;
	gVertexBufferData[16] = h_ver;
	gVertexBufferData[17] = 0.f;


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


	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gUvBufferData), gUvBufferData, GL_STATIC_DRAW);
}

Sprite::~Sprite() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Sprite::Render(){

	model = translation  * rotation * scale * glm::mat4(1.0f);
	mvp = world->defaultFov.projection * world->defaultFov.view * model;

	glUseProgram(params->at(0));
	glUniformMatrix4fv(params->at(3), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(params->at(2), 1, GL_FALSE, &world->defaultFov.view[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->texture_id);
	glUniform1i(params->at(1), 0);

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
	glDrawArrays(GL_TRIANGLES, 0, 6); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


#include "primitive.hpp"

Primitive::Primitive(World * world, WorldRect wr, int width, int height, Color color, int flags) {

	params = ShaderLoader::GetProgram("primitive");

	this->wr = wr;
	this->world = world;
	this->color = color;
	this->width = width;
	this->height = height;
	this->flags = flags;
	change = false;

	model = glm::mat4(1.0f);
	rotation = glm::rotate(0.0f, glm::vec3( 1.0f, 1.0f, 1.0f));
	translation = glm::translate(glm::mat4(), glm::vec3((float)(wr.x*2)/world->width, (float)(wr.y*2)/world->height, 0.0f));
	scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));



	float w_ver = (float)width/world->width;
	float h_ver =  (float)height/world->height;
	float w_ver_start = -w_ver;
	float h_ver_start = -h_ver;

	if(true){//flags == false
		w_ver_start = 0.0f;
		h_ver_start = 0.0f;
		w_ver *= 2;
		h_ver *=2;
	}

	size = 0;
	if(flags == 2){
		size = 12;
		gVertexBufferData.resize(size, 0.0f);

		gVertexBufferData[0] = w_ver_start;
		gVertexBufferData[1] = h_ver_start;
		gVertexBufferData[2] =  0.f;

		gVertexBufferData[3] = w_ver_start;
		gVertexBufferData[4] = h_ver;
		gVertexBufferData[5] = 0.f;

		gVertexBufferData[6] = w_ver;
		gVertexBufferData[7] = h_ver;
		gVertexBufferData[8] = 0.f;

		gVertexBufferData[9] = w_ver;
		gVertexBufferData[10] = h_ver_start;
		gVertexBufferData[11] = 0.f;

	} else {
	size = 18;
	gVertexBufferData.resize(size, 0.0f);

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

	}

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), &gVertexBufferData.front(), GL_STATIC_DRAW);

}

Primitive::~Primitive() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Primitive::SetPosition(WorldRect wr){
	this->wr = wr;
	translation = glm::translate(glm::mat4(), glm::vec3((float)(wr.x*2)/world->width, (float)(wr.y*2)/world->height, 0.0f));
}

void Primitive::Resize(int newWidth, int newHeight){
	if(newWidth >= 0) width = newWidth;
	if(newHeight >= 0) height = newHeight;

	float w_ver = (float)width/world->width;
	float h_ver =  (float)height/world->height;
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

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 18 * sizeof(float), &gVertexBufferData.front());
}

int Primitive::GetWidth(){
	return width;
}

int Primitive::GetHeight(){
	return height;
}

void Primitive::Render(){

	model = translation  * rotation * scale * glm::mat4(1.0f);
	mvp = world->defaultFov.projection * world->defaultFov.view * model;

	glUseProgram(params->at(0));
	glUniformMatrix4fv(params->at(2), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(params->at(1), 1, GL_FALSE, &world->defaultFov.view[0][0]);
	glUniform4f(params->at(3), color.r, color.g, color.b, color.a);

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


	// Draw the triangle !
	int mode = GL_TRIANGLES;
	if(size == 12){
		mode = GL_LINE;
	}
	glDrawArrays(GL_TRIANGLES, 0, size/3); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
}


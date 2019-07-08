/*
 * text.cpp
 *
 *  Created on: 21 lis 2017
 *      Author: Teresa Jarlaczyk
 */

#include "text.hpp"

Text::Text(World * world, Font * fnt, std::string text, int x, int y, int flags) {

	params = ShaderLoader::GetProgram("text");

	charsAmount = text.size();
	this->world = world;
	this->fnt = fnt;
	//this->text = text;
	this->x = (float)(x*2)/world->width;
	this->y = (float)(y*2)/world->height;
	this->flags = flags;
	model = glm::mat4(1.0f);
	rotation = glm::rotate(0.0f, glm::vec3( 1.0f, 1.0f, 1.0f));
	translation = glm::translate(glm::mat4(), glm::vec3(this->x, this->y, 0.0f));
	scale = glm::scale(glm::vec3(1.f, 1.f, 1.f));


	//Fill that shit
	gVertexBufferData.resize(18*charsAmount, 0.0f);
	gUvBufferData.resize(12*charsAmount, 0.0f);
	float sx = 1.0f;
	float sy = 1.0f;

	int n_v = 0;
	int n_t = 0;
	float xf = 0.f;
	float yf = 0.f;
	float totalW = 0.0f;

	  for(size_t s = 0; s<text.size(); s++) {

		float x2 =  xf + (fnt->glyphs_info[text[s]].bitmap_l/world->width) * sx;
	    float y2 = -yf - (fnt->glyphs_info[text[s]].bitmap_t/world->height) * sy;
	    float w = fnt->glyphs_info[text[s]].bitmap_w/world->width * sx;
	    float h = fnt->glyphs_info[text[s]].bitmap_h/world->height * sy;

	    totalW += w;

	    /* Advance the cursor to the start of the next character */
	    xf += (fnt->glyphs_info[text[s]].adv_x/world->width ) * sx;
	    yf += (fnt->glyphs_info[text[s]].adv_y/world->height) * sy;


	  //  std::cout<<"x2  " << x2  <<"adv "<<fnt->glyphs_info[text[s]].adv_x<<std::endl;

	    /* Skip glyphs that have no pixels */
	    if(!w || !h)
	      continue;

	    gVertexBufferData[n_v] = x2;
	    gVertexBufferData[n_v+1] = -y2;
	    gVertexBufferData[n_v+2] = 0.0f;

	    gVertexBufferData[n_v+3] = x2+w;
	    gVertexBufferData[n_v+4] = -y2;
	    gVertexBufferData[n_v+5] = 0.0f;

	    gVertexBufferData[n_v+6] = x2;
	    gVertexBufferData[n_v+7] = -y2 - h;
	    gVertexBufferData[n_v+8] = 0.0f;

	    gVertexBufferData[n_v+9] = x2+w;
	    gVertexBufferData[n_v+10] = -y2;
	    gVertexBufferData[n_v+11] = 0.0f;

	    gVertexBufferData[n_v+12] = x2;
	    gVertexBufferData[n_v+13] = -y2-h;
	    gVertexBufferData[n_v+14] = 0.0f;

	    gVertexBufferData[n_v+15] = x2+w;
	    gVertexBufferData[n_v+16] = -y2-h;
	    gVertexBufferData[n_v+17] = 0.0f;
	    n_v += 18;

	    gUvBufferData[n_t] = fnt->glyphs_info[text[s]].texture_offset; 																	gUvBufferData[n_t+1] = 0;
	    gUvBufferData[n_t+2] = fnt->glyphs_info[text[s]].texture_offset + fnt->glyphs_info[text[s]].bitmap_w / fnt->atlas_width;				gUvBufferData[n_t+3] = 0;
	    gUvBufferData[n_t+4] = fnt->glyphs_info[text[s]].texture_offset; 																gUvBufferData[n_t+5] = fnt->glyphs_info[text[s]].bitmap_h / fnt->atlas_height;
	    gUvBufferData[n_t+6] = fnt->glyphs_info[text[s]].texture_offset + fnt->glyphs_info[text[s]].bitmap_w / fnt->atlas_width; 				gUvBufferData[n_t+7] = 0;
	    gUvBufferData[n_t+8] = fnt->glyphs_info[text[s]].texture_offset; 																gUvBufferData[n_t+9] =  fnt->glyphs_info[text[s]].bitmap_h / fnt->atlas_height;
	    gUvBufferData[n_t+10] = fnt->glyphs_info[text[s]].texture_offset + fnt->glyphs_info[text[s]].bitmap_w / fnt->atlas_width; 			gUvBufferData[n_t+11] = fnt->glyphs_info[text[s]].bitmap_h / fnt->atlas_height;
	    n_t += 12;

	  }

	  if(flags == 1){ //Center
		  totalW = -(totalW/2);
		  SetPosition(this->x + totalW, this->y);
	  } else if(flags == 2){
		  totalW = -totalW;
		  SetPosition(this->x + totalW, this->y);
	  }

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 18*charsAmount * sizeof(GLfloat), &gVertexBufferData.front(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, 12*charsAmount * sizeof(GLfloat), &gUvBufferData.front(), GL_DYNAMIC_DRAW);

}

Text::~Text(){
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Text::SetText(std::string text){

	int charsAmountTemp = charsAmount;
	charsAmount = text.size();

	//Fill that shit
	//gVertexBufferData.clear();
	//gUvBufferData.clear();
	gVertexBufferData.resize(18*charsAmount);
	gUvBufferData.resize(12*charsAmount);
	float sx = 1.0f;
	float sy = 1.0f;

	int n_v = 0;
	int n_t = 0;
	float xf = 0.f;
	float yf = 0.f;
	float totalW = 0.0f;

	  for(size_t s = 0; s<text.size(); s++) {

		float x2 =  xf + (fnt->glyphs_info[text[s]].bitmap_l/world->width) * sx;
	    float y2 = -yf - (fnt->glyphs_info[text[s]].bitmap_t/world->height) * sy;
	    float w = fnt->glyphs_info[text[s]].bitmap_w/world->width * sx;
	    float h = fnt->glyphs_info[text[s]].bitmap_h/world->height * sy;

	    totalW += w;

	    /* Advance the cursor to the start of the next character */
	    xf += (fnt->glyphs_info[text[s]].adv_x/world->width ) * sx;
	    yf += (fnt->glyphs_info[text[s]].adv_y/world->height) * sy;


	    /* Skip glyphs that have no pixels */
	    if(!w || !h)
	      continue;

	    gVertexBufferData[n_v] = x2;
	    gVertexBufferData[n_v+1] = -y2;
	    gVertexBufferData[n_v+2] = 0.0f;

	    gVertexBufferData[n_v+3] = x2+w;
	    gVertexBufferData[n_v+4] = -y2;
	    gVertexBufferData[n_v+5] = 0.0f;

	    gVertexBufferData[n_v+6] = x2;
	    gVertexBufferData[n_v+7] = -y2 - h;
	    gVertexBufferData[n_v+8] = 0.0f;

	    gVertexBufferData[n_v+9] = x2+w;
	    gVertexBufferData[n_v+10] = -y2;
	    gVertexBufferData[n_v+11] = 0.0f;

	    gVertexBufferData[n_v+12] = x2;
	    gVertexBufferData[n_v+13] = -y2-h;
	    gVertexBufferData[n_v+14] = 0.0f;

	    gVertexBufferData[n_v+15] = x2+w;
	    gVertexBufferData[n_v+16] = -y2-h;
	    gVertexBufferData[n_v+17] = 0.0f;
	    n_v += 18;

	    gUvBufferData[n_t] = fnt->glyphs_info[text[s]].texture_offset; 																	gUvBufferData[n_t+1] = 0;
	    gUvBufferData[n_t+2] = fnt->glyphs_info[text[s]].texture_offset + fnt->glyphs_info[text[s]].bitmap_w / fnt->atlas_width;				gUvBufferData[n_t+3] = 0;
	    gUvBufferData[n_t+4] = fnt->glyphs_info[text[s]].texture_offset; 																gUvBufferData[n_t+5] = fnt->glyphs_info[text[s]].bitmap_h / fnt->atlas_height;
	    gUvBufferData[n_t+6] = fnt->glyphs_info[text[s]].texture_offset + fnt->glyphs_info[text[s]].bitmap_w / fnt->atlas_width; 				gUvBufferData[n_t+7] = 0;
	    gUvBufferData[n_t+8] = fnt->glyphs_info[text[s]].texture_offset; 																gUvBufferData[n_t+9] =  fnt->glyphs_info[text[s]].bitmap_h / fnt->atlas_height;
	    gUvBufferData[n_t+10] = fnt->glyphs_info[text[s]].texture_offset + fnt->glyphs_info[text[s]].bitmap_w / fnt->atlas_width; 			gUvBufferData[n_t+11] = fnt->glyphs_info[text[s]].bitmap_h / fnt->atlas_height;
	    n_t += 12;
	  }

	  if(flags == 1){ //Center
		  totalW = -(totalW/2);
		  SetPosition(x + totalW, y);
	  } else if(flags == 2){
		  totalW = -totalW;
		  SetPosition(x + totalW,y);
	  }


	  if(charsAmount > charsAmountTemp){
		  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		  glBufferData(GL_ARRAY_BUFFER, 18*charsAmount * sizeof(GLfloat), &gVertexBufferData.front(), GL_DYNAMIC_DRAW);

		  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		  glBufferData(GL_ARRAY_BUFFER, 12*charsAmount * sizeof(GLfloat), &gUvBufferData.front(), GL_DYNAMIC_DRAW);
	  } else {
		  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		  glBufferSubData(GL_ARRAY_BUFFER, 0, 18*charsAmount * sizeof(GLfloat), &gVertexBufferData.front());

		  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		  glBufferSubData(GL_ARRAY_BUFFER, 0, 12*charsAmount * sizeof(GLfloat), &gUvBufferData.front());
	  }

}

//Write manual for that function!!!!!!!!!! (x*****2222)/width
void Text::SetPosition(float new_x, float new_y){
	translation = glm::translate(glm::mat4(), glm::vec3(new_x, new_y, 0.0f));
}

void Text::SetPosition(int x, int y){
	this->x = (float)(x*2)/world->width;
	this->y = (float)(y*2)/world->height;
	translation = glm::translate(glm::mat4(), glm::vec3(x, y, 0.0f));
}

void Text::SetScale(float x, float y, float z){
	scale = glm::scale(glm::vec3(x, y, z));
}

void Text::SetRotation(float rot){
		rotation = glm::rotate(rot, glm::vec3( 1.0f, 1.0f, 1.0f));
}

void Text::Colorize(Color color){
	this->color = color;
}

void Text::Colorize(int r, int g, int b, int a){
	color.SetColor(r,g,b,a);
}
void Text::Colorize(float r, float g, float b, float a){
	color.SetColor(r,g,b,a);
}

void Text::Render(){

	model = translation  * rotation * scale * glm::mat4(1.0f);
	mvp = world->defaultFov.projection * world->defaultFov.view * model;

	glUseProgram(params->at(0));
	glUniformMatrix4fv(params->at(3), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(params->at(2), 1, GL_FALSE, &world->defaultFov.view[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fnt->tex);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(params->at(1), 0);
		glUniform4f(params->at(4), color.r, color.g, color.b, color.a);

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
		glDrawArrays(GL_TRIANGLES, 0, 6*charsAmount); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
}


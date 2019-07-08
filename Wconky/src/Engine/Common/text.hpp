/*
 * text.hpp
 *
 *  Created on: 21 lis 2017
 *      Author: Teresa Jarlaczyk
 */

#ifndef ENGINE_COMMON_TEXT_HPP_
#define ENGINE_COMMON_TEXT_HPP_

#include "config.hpp"
#include "font.hpp"
#include "world.hpp"
#include "shaderloader.hpp"
#include "Color.hpp"

using namespace glm;

class Text {
public:
	Text(World * world, Font * fnt, std::string text, int x, int y, int flags);
	virtual ~Text();

	void SetPosition(int x, int y);
	void SetScale(float x, float y, float z);
	void SetRotation(float rot);
	void SetText(std::string newText);
	void Colorize(Color color);
	void Colorize(int r, int g, int b, int a);
	void Colorize(float r, float g, float b, float a);
	void Render();

private:

	void SetPosition(float new_x, float new_y);


	Color color;
	//std::string text;
	int charsAmount;
	Font * fnt;
	float x, y;
	int flags;

	//Pointer for program
	std::vector<GLuint> * params;

	//Matrixes
	World * world;
	mat4 model;
	mat4 scale;
	mat4 rotation;
	mat4 translation;
	mat4 mvp;

	//Buffers
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	GLuint uvBuffer;

	std::vector<GLfloat> gVertexBufferData;//[6*3*2];
	std::vector<GLfloat> gUvBufferData;//[6*2*2];

};

#endif /* ENGINE_COMMON_TEXT_HPP_ */

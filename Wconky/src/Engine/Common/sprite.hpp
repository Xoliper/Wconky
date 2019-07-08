#ifndef ENGINE_COMMON_SPRITE_HPP_
#define ENGINE_COMMON_SPRITE_HPP_

#include "drawable.hpp"
#include "texture.hpp"
#include "shaderloader.hpp"
#include "world.hpp"
#include "macros.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

class Sprite : public Drawable {
public:
	Sprite(Texture * tex,  World * world, PureTranslation pt, int flags, int width, int height);
	virtual ~Sprite();

	void Render();

private:

	//Position handler
	PureTranslation pt;

	//Pointer for program
	std::vector<GLuint> * params;

	//Sprite texture
	Texture * tex;

	//Matrixes
	World * world;
	mat4 model;
	mat4 scale;
	mat4 rotation;
	mat4 translation;
	mat4 mvp;

	//Test vars
	bool change;

	//Buffers
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	GLuint uvBuffer;

	GLfloat gVertexBufferData[6*3];
	GLfloat gUvBufferData[6*2];
};

#endif /* ENGINE_COMMON_SPRITE_HPP_ */

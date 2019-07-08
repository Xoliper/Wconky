#ifndef ENGINE_COMMON_CUBE_HPP_
#define ENGINE_COMMON_CUBE_HPP_

#include "texture.hpp"
#include "shaderloader.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "drawable.hpp"
using namespace glm;

class Cube : public Drawable {
public:
	Cube(Texture * tex, mat4 *pv);
	virtual ~Cube();

	void Render();

private:
	//Pointer for program
	std::vector<GLuint> * params;

	//Sprite texture
	Texture * tex;

	//Matrixes
	mat4 * pv;
	mat4 model;
	mat4 mvp;

	//Test vars
	bool change;

	//Buffers
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	GLuint uvBuffer;

	GLfloat gVertexBufferData[6*3*6];
	GLfloat gUvBufferData[6*2*6];
};

#endif /* ENGINE_COMMON_SPRITE_HPP_ */

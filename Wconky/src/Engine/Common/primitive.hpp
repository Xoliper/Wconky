#ifndef ENGINE_COMMON_PRIMITIVE_HPP_
#define ENGINE_COMMON_PRIMITIVE_HPP_

#include "drawable.hpp"
#include "shaderloader.hpp"
#include "world.hpp"
#include "macros.hpp"
#include "color.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

class Primitive : public Drawable {
public:
	Primitive(World * world, WorldRect wr, int width, int height, Color color, int flags); //Rectangle
	virtual ~Primitive();

	void SetPosition(WorldRect wr);
	void Resize(int newWidth, int newHeight);
	int GetWidth();
	int GetHeight();
	void Render();

private:

	//Position handler
	WorldRect wr;

	//Pointer for program
	std::vector<GLuint> * params;

	//Color
	Color color;
	int flags;
	int size;

	int width;
	int height;

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
	std::vector<GLfloat> gVertexBufferData;

};

#endif /* ENGINE_COMMON_PRIMITIVE_HPP_ */

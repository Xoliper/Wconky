#ifndef ENGINE_COMMON_VISUALOBJECT_HPP_
#define ENGINE_COMMON_VISUALOBJECT_HPP_

#include "drawable.hpp"
#include "texture.hpp"
#include "shaderloader.hpp"
#include "objloader.hpp"

#include "world.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

extern float deltaTime;

using namespace glm;


enum ObjType {
	JUSTVISUAL,
	ENEMY,
	PLAYER,
	DK
};

class VisualObject;

class Object {
public:
	Object(){obj = NULL;};
	virtual ~Object(){};

	virtual void Render() = 0;

	VisualObject * obj;
	ObjType objType;


};

class VisualObject : public Object, Drawable {
	friend class CollisionDetector;
	friend class Weapon;
	friend class Enemy;
public:
	VisualObject(Texture * tex, World * world, std::string path);
	VisualObject(const VisualObject & obj);
	virtual ~VisualObject();

	void SetPosition(float x, float y, float z);
	void SetScale(float x, float  y, float z);
	void SetRotation(float x, float y, float z, float angle);

	void Render();

	std::vector<BoxDimm> boxdimm;
	std::vector<SubObject> meshes;

	ObjLoader objLoader;

private:

	bool ifCopy;

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


};




#endif /* ENGINE_COMMON_SPRITE_HPP_ */

/*
 * world.hpp
 *
 *  Created on: 8 paü 2017
 *      Author: Teresa Jarlaczyk
 */

#ifndef ENGINE_COMMON_WORLD_HPP_
#define ENGINE_COMMON_WORLD_HPP_

#include "config.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <windows.h>
#include <math.h>
#include <iostream>

class World;

class WorldRect{
public:
	WorldRect();
	WorldRect(int x, int y, int w, int h);
	WorldRect(const WorldRect & c);
	~WorldRect();

	int x,y,w,h;
};

class PureTranslation {
public:
	PureTranslation();
	PureTranslation(float x, float y, float z);
	PureTranslation(World * world, int x, int y);
	~PureTranslation();
	float x, y, z;
};

PureTranslation GetTranslation(glm::mat4 & matrix);

class WorldData {
public:
	WorldData();
	~WorldData();

	float x,y,z;
	float fov;
	glm::mat4 projection;
	glm::mat4 view;
};

class World {

public:
	World();
	virtual ~World();

	void SetupFov(float fov, float width, float height);
	void Move(float x, float y, float z);

	WorldData dynamicFov;
	WorldData defaultFov;
	int width, height;
	float deltaTime;

};

#endif /* ENGINE_COMMON_WORLD_HPP_ */

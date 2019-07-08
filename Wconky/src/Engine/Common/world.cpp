/*
 * world.cpp
 *
 *  Created on: 8 paü 2017
 *      Author: Teresa Jarlaczyk
 */

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "world.hpp"

WorldRect::WorldRect(){
	x = y = w = h = 0;
}

	WorldRect::WorldRect(int x, int y, int w, int h){
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	};
	WorldRect::WorldRect(const WorldRect & c){
		this->x = c.x;
		this->y = c.y;
		this->w = c.w;
		this->h = c.h;
	};
	WorldRect::~WorldRect(){};

PureTranslation::PureTranslation(){
	x = y = z = 0.0f;
}
PureTranslation::PureTranslation(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}
PureTranslation::PureTranslation(World * world, int x, int y){
	this->x = (float)x/world->width;
	this->y = (float)y/world->height;
	z = 0.0f;
}
PureTranslation::~PureTranslation(){
}

PureTranslation GetTranslation(glm::mat4 & matrix){
	PureTranslation pt;
	pt.x = matrix[3].x;
	pt.y = matrix[3].y;
	pt.z = matrix[3].z;
	return pt;
}

WorldData::WorldData(){
	x = y = z = fov = 0.0f;
}

WorldData::~WorldData(){
}


World::World() {
	deltaTime = 1.0f;
	width = 0;
	height = 0;

	defaultFov.fov = 60.f;
	defaultFov.z = 0.5 / tan(glm::radians(defaultFov.fov / 2.f));
	defaultFov.projection = glm::perspective(glm::radians(defaultFov.fov), 1.0f, 0.1f, 100.0f);
	defaultFov.view       = glm::lookAt(
								glm::vec3(0,0,defaultFov.z), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	dynamicFov = defaultFov;

}

World::~World() {
}

void World::SetupFov(float fov, float width, float height){
	this->width = width;
	this->height = height;
	dynamicFov.fov = fov;
	dynamicFov.z = 2 * tan(glm::radians(dynamicFov.fov / 2.f));
	dynamicFov.projection = glm::perspective(glm::radians(dynamicFov.fov), width/height, 0.1f, 100.0f);
	dynamicFov.view       = glm::lookAt(
								glm::vec3(0,0,dynamicFov.z), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
}

void World::Move(float x_temp, float y_temp, float z_temp){
	dynamicFov.x += x_temp*deltaTime;
	dynamicFov.y += y_temp*deltaTime;
	dynamicFov.z += z_temp*deltaTime;

	dynamicFov.view = glm::lookAt(
			glm::vec3(dynamicFov.x,dynamicFov.y,dynamicFov.z), // Camera is at (0,0,z), in World Space
			glm::vec3(defaultFov.x,defaultFov.y,0.0f), // and looks at the origin //x y 0
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	   );


	defaultFov.x += x_temp*deltaTime;
	defaultFov.y += y_temp*deltaTime;
	defaultFov.z += z_temp*deltaTime;
	defaultFov.view = glm::lookAt(
			glm::vec3(defaultFov.x,defaultFov.y,defaultFov.z), // Camera is at (0,0,z), in World Space
			glm::vec3(defaultFov.x,defaultFov.y,0.0f), // and looks at the origin //x y 0
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	   );
}

/*
 * collisiondetector.hpp
 *
 *  Created on: 8 paü 2017
 *      Author: Teresa Jarlaczyk
 */

#ifndef ENGINE_COMMON_COLLISIONDETECTOR_HPP_
#define ENGINE_COMMON_COLLISIONDETECTOR_HPP_

#include "objloader.hpp"
#include "visualobject.hpp"
#include <list>

class CollisionResult {
public:
	CollisionResult();
	virtual ~CollisionResult();

	bool state;
	Object * collidedObj;
	int boxNumber;

};

class CollisionDetector {
public:
	CollisionDetector();
	virtual ~CollisionDetector();

	CollisionResult CheckCollision(VisualObject * ref, std::list<Object *> * objs, float xOff = 0.0f, float yOff = 0.0f, float zOff = 0.0f, bool insideCheck = false, ObjType exclude = DK);

};

#endif /* ENGINE_COMMON_COLLISIONDETECTOR_HPP_ */

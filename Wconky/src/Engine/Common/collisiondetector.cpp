/*
 * collisiondetector.cpp
 *
 *  Created on: 8 paü 2017
 *      Author: Teresa Jarlaczyk
 */

#include "collisiondetector.hpp"

CollisionResult::CollisionResult(){
	state = false;
	collidedObj = NULL;
	boxNumber = -1;
}

CollisionResult::~CollisionResult(){
}





CollisionDetector::CollisionDetector() {
}

CollisionDetector::~CollisionDetector() {
}

CollisionResult CollisionDetector::CheckCollision(VisualObject *ref, std::list<Object *> * objs, float xOff, float yOff, float zOff, bool insideCheck, ObjType exclude){


	CollisionResult cr;
	BoxDimm  refBox = ref->boxdimm[0];
	PureTranslation pt = GetTranslation(ref->translation);
	/*refBox.x_max += pt.x + xOff;
	refBox.x_min += pt.x + xOff;
	refBox.y_max += pt.y + yOff;
	refBox.y_min += pt.y + yOff;
	refBox.z_max += pt.z + zOff;
	refBox.z_min += pt.z + zOff;
	 */

	if(insideCheck == true){
		pt.y += yOff;
	} else {
	//	pt.y += yOff*deltaTime;
	}
	//pt.x += xOff*deltaTime;
	//pt.z += zOff*deltaTime;

	//for(unsigned int i = 0; i<objs->size(); i++){
	for(std::list<Object *>::iterator it = objs->begin(); it!= objs->end(); it++){

		if(ref == (*it)->obj) continue;
		if((*it)->objType == exclude) continue;

		//std::cout<<(*it)->obj<<std::endl;

		for(unsigned int j = 0; j < (*it)->obj->boxdimm.size(); j++){//
			BoxDimm  box = (*it)->obj->boxdimm[j];
			PureTranslation ptt = GetTranslation((*it)->obj->translation);

			if(insideCheck == false){
				box.x_max += ptt.x + refBox.width/2;
				box.x_min += ptt.x - refBox.width/2;
				box.y_max += ptt.y + refBox.height/2;
				box.y_min += ptt.y - refBox.height/2;
				box.z_max += ptt.z + refBox.depth/2;
				box.z_min += ptt.z - refBox.depth/2;
			} else {
				box.x_max += ptt.x;
				box.x_min += ptt.x;
				box.y_max += ptt.y;
				box.y_min += ptt.y;
				box.z_max += ptt.z;
				box.z_min += ptt.z;
			}
			//AABB collision
			//if((box.x_min <= refBox.x_min && box.x_max >= refBox.x_max) &&
			//   (box.y_min <= refBox.y_min && box.y_max >= refBox.y_max) &&
			//   (box.z_min <= refBox.z_min && box.z_max >= refBox.z_max)) {


			//Point AABB collision
			if((pt.x >= box.x_min && pt.x <= box.x_max) &&
			   (pt.y >= box.y_min && pt.y <= box.y_max) &&
			   (pt.z >= box.z_min && pt.z <= box.z_max)){


				cr.state = true;
				cr.collidedObj = (*it);
				cr.boxNumber = j;
				return cr;
			}
		}


	}
	return cr;
}

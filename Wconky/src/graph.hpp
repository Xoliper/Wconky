/*
 * graph.hpp
 *
 *  Created on: 17.10.2016
 *      Author: Arkadiusz Jarlaczyk
 */

#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include "Engine/Common/drawable.hpp"
#include "Engine/Common/primitive.hpp"
#include "Engine/Common/world.hpp"
#include "Engine/Common/color.hpp"
#include "virtualclasses.hpp"
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;


class Graph : public Drawable, public ToUpdate {
public:
	Graph(World * world, Module * updater, int xstart, int ystart, int width, int height, Color barsCol, Color holderCol, int graphBarWidth, int graphBarSpacing);
	virtual ~Graph();

	void Update();
	void Render();

private:
	//Main stuff
	World * world;
	Module * updater;

	//Data holders
	Color holderCol, barsCol;
	int graphBarWidth;
	int x, y, w, h;
	double lastTime, currentTime;

	//Visual objects
	std::vector<Primitive*> bars;
	Primitive * holder;
};

#endif /* GRAPH_HPP_ */

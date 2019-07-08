#include "graph.hpp"

Graph::Graph(World * world, Module * updater, int xstart, int ystart, int width, int height, Color barsCol, Color holderCol, int graphBarWidth, int graphBarSpacing){
	this->world = world;
	this->updater = updater;
	x = xstart; y = ystart;
	w = width; h = height;
	this->barsCol = barsCol;
	this->holderCol = holderCol;
	this->graphBarWidth = graphBarWidth;

	for(int i = 0; i<width; i+=graphBarWidth+graphBarSpacing){
		bars.push_back(new Primitive(world, {x+i, y, 0, 0}, graphBarWidth, 0, barsCol, 0));
	}
	holder = new Primitive(world, {x, y,0,0}, width, 1, holderCol, 0);

	lastTime = glfwGetTime();
	currentTime = 0.0;
}

Graph::~Graph(){

	for(int i = 0; i<bars.size(); i++){
		delete bars[i];
	}
	delete holder;
}

void Graph::Update(){
	float value = 0;
	if(updater != nullptr){


		//"FPS" handling
		float rTime = (float)(1000/updater->refreshTime)/100;
		currentTime = glfwGetTime();
		if(currentTime - lastTime < rTime){
			return;
		}
		lastTime = currentTime;

		value = updater->GetValue();
	}

	//Move all bars
	for(int i = 0; i<bars.size()-1; i++){
		bars[i]->Resize(graphBarWidth, bars[i+1]->GetHeight());
	}
	//Update height of last element
	bars.back()->Resize(graphBarWidth, ((double)h*value));
}

void Graph::Render(){
	for(int i = 0; i<bars.size()-1; i++){
		bars[i]->Render();
	}
	holder->Render();
}


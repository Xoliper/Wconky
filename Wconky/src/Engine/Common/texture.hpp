#ifndef TEXTURE_HPP
#define TEXTURE_HPP

//-----------------------------------------------------
// Defines & Includes
//-----------------------------------------------------


// Include GLIMG
#include <memory>
//#include <glload/gl_all.h>
//#include <glload/gl_load.h>
#include <GL/glew.h>
#include <glimg/glimg.h>

// Standard libs
#include <iostream>
#include <string>


//-----------------------------------------------------
// Classes
//-----------------------------------------------------

class Texture {
public:
	Texture();
	Texture(std::string path);
	~Texture();

	bool IsLoaded();
	bool Load(std::string path);
	void LoadRaw(unsigned char * bytes, int width, int height); //planes only 3 (RGB)

	GLuint texture_id;
	glimg::Dimensions dimm;

};

#endif

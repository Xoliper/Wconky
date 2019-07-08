#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "texture.hpp"


Texture::Texture(){
	texture_id = 0;
}

Texture::Texture(std::string path){
    texture_id = 0;
    Load(path);
}

Texture::~Texture(){
	glDeleteTextures(1, &texture_id);
}

bool Texture::IsLoaded(){
	if(texture_id != 0){
		return true;
	}
	return false;
}

bool Texture::Load(std::string path){
	try
    {
      std::auto_ptr<glimg::ImageSet> pImgSet(glimg::loaders::stb::LoadFromFile(path.c_str()));
      texture_id = glimg::CreateTexture(pImgSet.get(), 0);
      dimm = pImgSet.get()->GetDimensions();
      return true;
    }
    catch(glimg::loaders::stb::StbLoaderException &e)
    {
      //Image file loading failed.
    	std::cerr<<"Image file loading failed."<<std::endl;
    	texture_id = 0;
    }
    catch(glimg::TextureGenerationException &e)
    {
      //Texture creation failed.
    	std::cerr<<"Texture creation failed."<<std::endl;
    	texture_id = 0;
    }
    return false;
}

void Texture::LoadRaw(unsigned char * bytes, int width, int height){

	glGenTextures(1, &texture_id);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bytes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	dimm.depth = 4;
	dimm.width = width;
	std::cout<<"h "<<height<<std::endl;
	dimm.height = height;
	dimm.numDimensions = 2;

}

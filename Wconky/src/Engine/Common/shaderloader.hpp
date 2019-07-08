#ifndef COMMON_SHADERLOADER_HPP_
#define COMMON_SHADERLOADER_HPP_

//-----------------------------------------------------
// Defines & Includes
//-----------------------------------------------------

#include "config.hpp"
#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <map>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <utility>


//-----------------------------------------------------
// Classes
//-----------------------------------------------------

class ShaderLoader {

	public:
		ShaderLoader();
		virtual ~ShaderLoader();

		int Load(std::string index_file_path);
		static std::vector<GLuint> * GetProgram(std::string name);

	private:
		GLuint CompileShader(std::string file_path);
		static std::map<std::string, std::vector<GLuint> > programs;

};



#endif /* COMMON_SHADERLOADER_HPP_ */

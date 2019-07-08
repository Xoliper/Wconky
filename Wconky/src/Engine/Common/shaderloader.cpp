/*
 * shaderloader.cpp
 *
 *  Created on: 16 wrz 2017
 *      Author: Teresa Jarlaczyk
 */

#include "shaderloader.hpp"

std::map<std::string, std::vector<GLuint> > ShaderLoader::programs;

ShaderLoader::ShaderLoader() {
}

ShaderLoader::~ShaderLoader() {
	glUseProgram(0);
	for(auto & sh : programs){
		glDeleteProgram(sh.second.front());
	}
}

std::vector<GLuint> * ShaderLoader::GetProgram(std::string name){
	return &programs.at(name);
}


int ShaderLoader::Load(std::string index_file_path){

    std::fstream index_file;
    index_file.open(index_file_path.c_str(), std::ios::in );

    if( index_file.good() ){
        std::string line;
        std::string path;
        while( !index_file.eof() )
        {
            getline( index_file, line );



            std::size_t path_f = line.find("Path: ");
            if (path_f!=std::string::npos){
            	path = line.substr(6, line.size()-6);
                continue;
            }
            //std::cout<<line<<std::endl;

            std::size_t found = line.find(" ");
            if (found!=std::string::npos){
            	std::string key = line.substr(0, found);
            	std::string file_path;
            	if(line.find("[") == std::string::npos){
            		file_path = line.substr(found+1, line.size()-found-1);
            	} else {
                	std::size_t params_start = line.find("[");
            		file_path = line.substr(found+1, params_start-found-1);
            	}
            	GLuint value = CompileShader(path+file_path);

            		std::cout<<"Shader: "<< value<<std::endl;

            	std::vector<GLuint> refs;
            	refs.push_back(value);

            	//Load params for Program
            	std::size_t params_start = line.find("[");
            	std::size_t params_end = line.find("]");
            	if(params_start != std::string::npos and params_end != std::string::npos){
            		std::string params = line.substr(params_start+1, params_end-params_start-1);
            		params.erase(std::remove_if(params.begin(), params.end(), ::isspace), params.end());

            		while(params.size() > 0){
            			std::size_t pos = params.find(";");
            			if(pos == std::string::npos) break;

            			std::size_t start = params.find("{");
            			std::size_t end = params.find("}");

            			if(start != std::string::npos and end != std::string::npos){

//            				std::string param_name = params.substr(0,start);
            				std::string param_ref_name = params.substr(start+1, end-start-1);

            				GLuint temp = glGetUniformLocation(value, param_ref_name.c_str());
            				refs.push_back(temp);

            			} else {
            				break;
            			}

            			params = params.substr(pos+1, params.size()-pos);
            		}
            	}

            	programs.insert(std::pair<std::string, std::vector<GLuint> >(key, refs));

            }

        }
        index_file.close();
    } else std::cerr << "Cannot open shaders-index file!" << std::endl;

	return programs.size();
}

GLuint ShaderLoader::CompileShader(std::string file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string VertexShaderCode;
	std::string FragmentShaderCode;


    std::fstream shader_file;
    shader_file.open(file_path.c_str(), std::ios::in );

    if( shader_file.good() ){
        std::string line;
        while( !shader_file.eof() )
        {
            getline( shader_file, line );
            //Load Fragment
            std::size_t found_frg = line.find("#fragment-start");
            if (found_frg!=std::string::npos){
            	std::size_t found_efrg = 0;
            	while(!shader_file.eof()){
                    getline( shader_file, line );
                    found_efrg = line.find("#fragment-end");
                    if(found_efrg == std::string::npos){
                    	FragmentShaderCode += "\n" + line;
                    } else {
                    	break;
                    }
            	}
            	continue;
            }



            //Load Vertex
            std::size_t found_vrx = line.find("#vertex-start");
            if (found_vrx!=std::string::npos){
            	std::size_t found_evrx = 0;
            	while(!shader_file.eof()){
                    getline( shader_file, line );
                    found_evrx = line.find("#vertex-end");
                    if(found_evrx == std::string::npos){
                    	VertexShaderCode += "\n" + line;
                    } else {
                    	break;
                    }
            	}
            	continue;
            }


        }
        shader_file.close();
    } else std::cerr << "Cannot open shaders file!" << std::endl;


	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	if(SHADER_DEBUG) std::cout<<"Compiling vertex shader file: "<<file_path<<std::endl;
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::cerr<<&VertexShaderErrorMessage[0]<<std::endl;
	}



	// Compile Fragment Shader
	if(SHADER_DEBUG) std::cout<<"Compiling fragment shader file: "<<file_path<<std::endl;
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		std::cerr<<&FragmentShaderErrorMessage[0]<<std::endl;
	}



	// Link the program
	if(SHADER_DEBUG) std::cout<<"Linking program..."<<std::endl;
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cerr<<&ProgramErrorMessage[0]<<std::endl;
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	if(SHADER_DEBUG) std::cout<<VertexShaderCode<<std::endl<<std::endl;
	if(SHADER_DEBUG) std::cout<<FragmentShaderCode<<std::endl<<std::endl;

	return ProgramID;
}

#ifndef ENGINE_COMMON_OBJLOADER_HPP_
#define ENGINE_COMMON_OBJLOADER_HPP_


#include "world.hpp"
#include <GL/glew.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
// Include AssImp
//#include <assimp/Importer.hpp>      // C++ importer interface
//#include <assimp/scene.h>           // Output data structure
//#include <assimp/postprocess.h>     // Post processing flags
#include <iostream>
#include <glm/gtx/string_cast.hpp>
//#include "animation.hpp"
//using namespace Assimp;
using namespace std;


class BoxDimm {
public:
	BoxDimm();
	virtual ~BoxDimm();
	float width, height, depth;
	float x_min, x_max;
	float y_min, y_max;
	float z_min, z_max;
};

class SubObject {
public:
	SubObject();
	~SubObject();

	void GenerateBuffers();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned short> indices;

	GLuint vertexArrayID;
	GLuint vertexBuffer;
	GLuint uvBuffer;
	GLuint normalBuffer;

};

class ObjLoader {
public:
	ObjLoader();
	~ObjLoader();

	bool LoadFile(const char * path);
	void LoadMesh(std::vector<SubObject> & meshes, std::vector<BoxDimm> & boxdimm);
	//void LoadModelFromNode(const aiScene* scene, aiNode* node, aiMatrix4x4 transformation, std::vector<SubObject> & meshes);

private:
	//Importer importer;
	//const aiScene* scene;
	//Animation animation;

};


bool LoadAssImp(
	const char * path,
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals,
	std::vector<BoxDimm> & boxdimm
);


#endif /* ENGINE_COMMON_OBJLOADER_HPP_ */

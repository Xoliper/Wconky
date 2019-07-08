#include "visualobject.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


VisualObject::VisualObject(Texture * tex, World * world, std::string path) {

	this->obj = this;
	objType = JUSTVISUAL;
	ifCopy = false;

	params = ShaderLoader::GetProgram("object");
	this->world = world;
	this->tex = tex;

	model = glm::mat4(1.0f);
	rotation = glm::rotate(0.0f, glm::vec3( 1.0f, 1.0f, 1.0f));
	translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
	scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

	int res = objLoader.LoadFile(path.c_str());
	objLoader.LoadMesh(meshes, boxdimm);


	std::cout<<meshes.size()<<std::endl;
	//bool res = LoadAssImp(path.c_str(), indices, vertices, uvs, normals, boxdimm);

	/*glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

*/
	/*glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
	*/
}

VisualObject::VisualObject(const VisualObject & obj){

	this->obj = this;
	ifCopy = true;

	objType = obj.objType;
	params = obj.params;
	world = obj.world;
	tex = obj.tex;
	model = obj.model;
	rotation = obj.rotation;
	translation = obj.translation;
	scale = obj.scale;
	boxdimm = obj.boxdimm;

	/*indices = obj.indices;
	vertices = obj.vertices;
	uvs = obj.uvs;
	normals = obj.normals;

	vertexArrayID = obj.vertexArrayID;
	vertexBuffer = obj.vertexBuffer;
	uvBuffer = obj.uvBuffer;
	normalBuffer = obj.normalBuffer;
*/
	/*glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
*/
}


VisualObject::~VisualObject() {

	if(ifCopy == false){
		//glDeleteBuffers(1, &vertexBuffer);
		//glDeleteBuffers(1, &uvBuffer);
		//glDeleteBuffers(1, &normalBuffer);
		//glDeleteVertexArrays(1, &vertexArrayID);
	}
	tex = NULL;
	params = NULL;
}

void VisualObject::SetPosition(float x, float y, float z){
	translation = glm::translate(glm::mat4(), glm::vec3(x, y, z));
}

void VisualObject::SetScale(float x, float y, float z){
	scale = glm::scale(glm::vec3(x,y,z));
}

void VisualObject::SetRotation(float x, float y, float z, float angle){
	rotation = glm::rotate(angle, glm::vec3( x, y, z));
}

void VisualObject::Render(){

	for(unsigned int i = 0; i<meshes.size(); i++){


		model = translation  * rotation * scale * glm::mat4(1.0f); // *rotation
		mvp = world->dynamicFov.projection * world->dynamicFov.view * model;

		glUseProgram(params->at(0));

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(params->at(1), 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(params->at(3), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(params->at(2), 1, GL_FALSE, &world->dynamicFov.view[0][0]);

		//x,y+6.5,z+4
		glm::vec3 lightPos = glm::vec3(world->dynamicFov.x,world->dynamicFov.y+12,world->dynamicFov.z-6);
		glUniform3f(params->at(5), lightPos.x, lightPos.y, lightPos.z);




		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex->texture_id);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(params->at(4), 0);


		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].uvBuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		 // 3rd attribute buffer : normals
		 glEnableVertexAttribArray(2);
		 glBindBuffer(GL_ARRAY_BUFFER, meshes[i].normalBuffer);
		 glVertexAttribPointer(
			 2,                                // attribute
			 3,                                // size
			 GL_FLOAT,                         // type
			 GL_FALSE,                         // normalized?
			 0,                                // stride
			 (void*)0                          // array buffer offset
		 );

		 // Index buffer
				/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

				// Draw the triangles !
				glDrawElements(
					GL_TRIANGLES,      // mode
					indices.size(),    // count
					GL_UNSIGNED_SHORT,   // type
					(void*)0           // element array buffer offset
				);*/

		// Draw the triangle !


		glDrawArrays(GL_TRIANGLES, 0, meshes[i].vertices.size()); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
}


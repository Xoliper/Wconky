#include "objloader.hpp"


BoxDimm::BoxDimm(){
	x_min = x_max = y_min = y_max = z_min = z_max = 0.0f;
	width = height = depth = 0.0f;
};

BoxDimm::~BoxDimm(){
}


SubObject::SubObject(){
}

SubObject::~SubObject(){
}

void SubObject::GenerateBuffers(){
	glGenVertexArrays(1, &vertexArrayID);
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
}



ObjLoader::ObjLoader(){
	//scene = NULL;
}

ObjLoader::~ObjLoader(){
}

bool ObjLoader::LoadFile(const char * path){
	/*scene = importer.ReadFile(path, 0 );
	if( !scene) {
		fprintf( stderr, importer.GetErrorString());
		getchar();
		return false;
	}*/
	return true;
}

/*void ObjLoader::LoadModelFromNode(const aiScene* scene, aiNode* node, aiMatrix4x4 transformation, std::vector<SubObject> & meshes){
	transformation *= node->mTransformation;

	for(UINT i = 0; i < node->mNumMeshes; ++i) {
	    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	    //SubObject & so = meshes[i];

	    SubObject so;

				/*so.uvs.reserve(so.uvs.size());
				for(unsigned int i=0; i<mesh->mNumVertices; i++){
					aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
					so.uvs.push_back(glm::vec2(UVW.x, UVW.y));
				}

				// Fill vertices normals
				so.normals.reserve(so.normals.size()+mesh->mNumVertices);
				for(unsigned int i=0; i<mesh->mNumVertices; i++){
					aiVector3D n = mesh->mNormals[i];
					so.normals.push_back(glm::vec3(n.x, n.y, n.z));
				}


				// Fill face indices
				so.indices.reserve(so.indices.size()+3*mesh->mNumFaces);
				for (unsigned int i=0; i<mesh->mNumFaces; i++){
					// Assume the model has only triangles.
					so.indices.push_back(mesh->mFaces[i].mIndices[0]);
					so.indices.push_back(mesh->mFaces[i].mIndices[1]);
					so.indices.push_back(mesh->mFaces[i].mIndices[2]);
				}
*/
/*

        for(int m = 0; m < mesh->mNumVertices; m++){
        	so.normals.push_back(glm::vec3(mesh->mNormals[m].x, mesh->mNormals[m].y, mesh->mNormals[m].z));

        }


	    for (UINT f = 0; f < mesh->mNumFaces; ++f) {
	        const aiFace* face = &mesh->mFaces[f];

	        for (UINT k = 0; k < 3; ++k) {


	        	aiVector3D pos = transformation * mesh->mVertices[face->mIndices[k]];
	            so.vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));

	        	//aiVector3D n = mesh->mNormals[face->mIndices[k]];
				//so.normals.push_back(glm::vec3(mesh->n.x, n.y, n.z));

	        	aiVector3D UVW = mesh->mTextureCoords[0][face->mIndices[k]];
				so.uvs.push_back(glm::vec2(UVW.x, UVW.y));


				so.indices.push_back(mesh->mFaces[0].mIndices[k]);
				//so.indices.push_back(mesh->mFaces.mIndices[k]);

	            //so.vertices.push_back(pos.y);
	            //so.vertices.push_back(pos.z);
	        }
	    }

	    meshes.push_back(so);

	}
	for(int i = 0; i < node->mNumChildren; ++i){
	    LoadModelFromNode(scene, node->mChildren[i], transformation, meshes);
	}
}
*/

void ObjLoader::LoadMesh(std::vector<SubObject> & meshes, std::vector<BoxDimm> & boxdimm){


	//LoadModelFromNode(scene, scene->mRootNode, aiMatrix4x4(), meshes);




	//For .obj
	//int i = 0;
	//for(unsigned int o = 0; o<scene->mNumMeshes; o++){
	//		aiMesh* mesh = scene->mMeshes[o]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

			//SubObject so;


			// Fill vertices positions
			/*std::vector<glm::vec3> verttemp;
			verttemp.reserve(mesh->mNumVertices);
			for(unsigned int i=0; i<mesh->mNumVertices; i++){
				aiVector3D pos = mesh->mVertices[i];
				verttemp.push_back(glm::vec3(pos.x, pos.y, pos.z));
			}
			so.vertices.insert( so.vertices.end(), verttemp.begin(), verttemp.end() );


			BoxDimm box;
			if(verttemp.size()>=1){
				box.x_max = box.x_min = verttemp[0].x;
				box.y_max = box.y_min = verttemp[0].y;
				box.z_max = box.z_min = verttemp[0].z;
			}
			for(unsigned int i = 0; i<verttemp.size(); i++){
				if(box.x_max < verttemp[i].x) box.x_max = verttemp[i].x;
				if(box.x_min > verttemp[i].x) box.x_min = verttemp[i].x;
				if(box.y_max < verttemp[i].y) box.y_max = verttemp[i].y;
				if(box.y_min > verttemp[i].y) box.y_min = verttemp[i].y;
				if(box.z_max < verttemp[i].z) box.z_max = verttemp[i].z;
				if(box.z_min > verttemp[i].z) box.z_min = verttemp[i].z;
			}
			box.width = abs(box.x_max-box.x_min);
			box.height = abs(box.y_max-box.y_min);
			box.depth = abs(box.z_max-box.z_min);
			boxdimm.push_back(box);
*/



			// Fill vertices texture coordinates
			/*so.uvs.reserve(so.uvs.size());
			for(unsigned int i=0; i<mesh->mNumVertices; i++){
				aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
				so.uvs.push_back(glm::vec2(UVW.x, UVW.y));
			}

			// Fill vertices normals
			so.normals.reserve(so.normals.size()+mesh->mNumVertices);
			for(unsigned int i=0; i<mesh->mNumVertices; i++){
				aiVector3D n = mesh->mNormals[i];
				so.normals.push_back(glm::vec3(n.x, n.y, n.z));
			}


		*/	// Fill face indices
		/*	SubObject & so = meshes[i];
			i++;
			so.indices.reserve(so.indices.size()+3*mesh->mNumFaces);
			for (unsigned int i=0; i<mesh->mNumFaces; i++){
				// Assume the model has only triangles.
				so.indices.push_back(mesh->mFaces[i].mIndices[0]);
				so.indices.push_back(mesh->mFaces[i].mIndices[1]);
				so.indices.push_back(mesh->mFaces[i].mIndices[2]);
			}
*/
			//meshes.push_back(so);




	//	}

	for(int i = 0; i < meshes.size(); i++){
		meshes[i].GenerateBuffers();
	}

}







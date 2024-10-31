#include "ModelMeshComponent.h"

// Includes for model loading
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Texture.h"
#include "SharedMaterials.h"
#include "SharedTransformations.h"

static const bool VERBOSE = true;

ModelMeshComponent::ModelMeshComponent (string filePathAndName, GLuint shaderProgram, int updateOrder)
	: MeshComponent(shaderProgram, updateOrder), filePathAndName(filePathAndName)
{
}


ModelMeshComponent::~ModelMeshComponent()
{

} // end destructor


void ModelMeshComponent::buildMesh()
{
	// Scale information needs to be added to the filePathAndName
	modelScale = owningGameObject->getScale(WORLD);

	this->scaleMeshName = filePathAndName + " "+ std::to_string(modelScale[0][0])
									+ " " + std::to_string(modelScale[1][1])
									+ " " + std::to_string(modelScale[2][2]);

	if ( previsouslyLoaded() == false ){

		// Model loading
		std::vector<pntVertexData> vData;
		std::vector<unsigned int> indices;

		// Create an instance of the Importer class
		Assimp::Importer importer;

		// Load the scene/model and associated meshes into a aiScene object
		// See http://assimp.sourceforge.net/lib_html/class_assimp_1_1_importer.html
		// for more details. Second argument specifies configuration that is optimized for 
		// real-time rendering.
		const aiScene* scene = importer.ReadFile(filePathAndName, aiProcessPreset_TargetRealtime_Quality);

		// Check if the scene/model loaded correctly
		if (!scene) {
			std::cerr << "ERROR: Unable to load " << filePathAndName << "\t" 
					  << importer.GetErrorString() << std::endl;
			exit(EXIT_FAILURE);
		}

		/*
		This is a concave shape made out of convex sub parts, called child shapes. Each
		child shape has its own local offset transform, relative to the btCompoundShape. It 
		is a good idea to approximate concave shapes using a collection of convex hulls, 
		and store them in a btCompoundShape.
		*/
		// Create compound shape to hold the shapes of the individual meshes
		btCompoundShape* modelCompondShape = new btCompoundShape();

		// Iterate through each mesh
		for (size_t i = 0; i < scene->mNumMeshes; i++) {

			// Get the vertex mesh 
			aiMesh* mesh = scene->mMeshes[i];

			// Create a collision shape for the sub mesh
			btConvexHullShape* meshCollisionShape = new btConvexHullShape();

			// Read in the vertex data associated with the model
			readVertexData(mesh, vData, indices, *meshCollisionShape);

			// Get the Material*for the mesh
			aiMaterial* meshMaterial = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];

			// Create an empty material for the sub mesh
			Material material;

			// Read in the Material*properties for this mesh
			if (mesh->mMaterialIndex >= 0) {

				material = readInMaterialProperties(meshMaterial, filePathAndName);

			}

			SubMesh subMesh = buildSubMesh(vData, indices);

			subMesh.material = material;

			// Add the mesh collision shape for collision detection
			// Do NOT use the default btTransform constructor for this! It  
			// makes a zero matrix and everything disappears. No problem for collision spheres! 
			modelCompondShape->addChildShape(btTransform(btQuaternion(0, 0, 0)), meshCollisionShape);

			subMeshes.push_back(subMesh);

			//webGPU

			vData.clear();
			indices.clear();

		} // needs to be moved up

		// Set the collision shape for this model
		this->collisionShape = modelCompondShape;

		saveInitialLoad();
	}

} // end initialize


void ModelMeshComponent::readVertexData(aiMesh* mesh, std::vector<pntVertexData>& vertexData, std::vector<unsigned int>& indices, btConvexHullShape& hull)
{
	// Read in vertex positions, normals, and texture coordinates. See 
	// http://www.assimp.org/lib_html/structai_MeshComponent.html for more details
	if (mesh->HasPositions()) {
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {

			// Read in vertex position data
			glm::vec4 tempPosition;
			tempPosition.x = mesh->mVertices[i].x;
			tempPosition.y = mesh->mVertices[i].y;
			tempPosition.z = mesh->mVertices[i].z;
			tempPosition.w = 1.0f;

			// Apply the World scale set before initialization to the
			// collision shape. If the model scale is changed to collision
			// shape will not be adjusted in the present implementation
			vec4 scalePos = modelScale * tempPosition;

			// Add the vertex for the collision shape
			hull.addPoint(btVector3(scalePos.x, scalePos.y, scalePos.z), true);

			// Read in vertex normal vectors
			glm::vec3 tempNormal;
			tempNormal.x = mesh->mNormals[i].x;
			tempNormal.y = mesh->mNormals[i].y;
			tempNormal.z = mesh->mNormals[i].z;

			// Read in texture coordinates
			glm::vec2 tempCoord(0.0f, 0.0f);
			// Should always be true since coordinates are auto generated if not present
			if (mesh->HasTextureCoords(0)) {
				tempCoord.x = mesh->mTextureCoords[0][i].x;
				tempCoord.y = mesh->mTextureCoords[0][i].y;
			}

			glm::vec3 tempTangent = glm::vec3(0, 0, 0);
			glm::vec3 tempbitTangent = glm::vec3(0, 0, 0);
			if (mesh->HasTangentsAndBitangents()) {

				tempTangent.x = mesh->mTangents[i].x;
				tempTangent.y = mesh->mTangents[i].y;
				tempTangent.z = mesh->mTangents[i].z;
				tempTangent = glm::normalize(tempTangent);

				tempbitTangent.x = mesh->mBitangents[i].x;
				tempbitTangent.y = mesh->mBitangents[i].y;
				tempbitTangent.z = mesh->mBitangents[i].z;;
				tempbitTangent = glm::normalize(tempbitTangent);
			}

			// Push all data for this vertex into the data vector in preparation for buffering
			vertexData.push_back(pntVertexData(tempPosition, tempNormal, tempCoord, tempTangent, tempbitTangent));
		}
	}

	// Read in the indices that describe faces in preparation for buffering
	if (mesh->HasFaces()) {
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}

}

std::string ModelMeshComponent::getDirectoryPath(std::string sFilePath)
{
	// Get directory path
	std::string sDirectory = "";

	for (int i = (int)sFilePath.size() - 1; i >= 0; i--) {

		if (sFilePath[i] == '\\' || sFilePath[i] == '/') {
			sDirectory = sFilePath.substr(0, i + 1);
			break;
		}
	}
	return sDirectory;
}

Material ModelMeshComponent::readInMaterialProperties( const aiMaterial* assimpMaterial, std::string filename)
{
	// This is dynamically allocated here and will be deleted in the 
	// destructor when the subMesh that is is assigned to is deleted.
	Material meshMaterial;

	// Read in the name of the material
	aiString name;
	assimpMaterial->Get(AI_MATKEY_NAME, name);
	if (VERBOSE) std::cout << std::endl << "Loading " << name.C_Str() << " material:" << std::endl;

	// Temporary assimp color to read into
	aiColor3D matColor(0.0f, 0.0f, 0.0f);

	// Process materials (See https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html#material-system details)
	
	float shininess;
	if (assimpMaterial->Get(AI_MATKEY_SHININESS  , shininess) == AI_SUCCESS) {			// Ns

		if (VERBOSE) cout << "shininess " << shininess << endl;
		meshMaterial.setSpecularExponentMat(64.0f);

	}

	float opacity;
	if (assimpMaterial->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS) {				// d 

		if (VERBOSE) cout << "material alpha " << opacity << endl;
		meshMaterial.setTransparencyMat(opacity);

	}

	// Query for ambient color
	if (assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, matColor) == AI_SUCCESS) {			// Ka

		meshMaterial.setAmbientColor(glm::vec3(matColor[0], matColor[1], matColor[2]));
	}
	// Query for diffuse color
	if (assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, matColor) == AI_SUCCESS) {			// Kd

		meshMaterial.setDiffuseColor(glm::vec3(matColor[0], matColor[1], matColor[2]));
	}
	// Query for specular color
	if (assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, matColor) == AI_SUCCESS) {		//Ks

		meshMaterial.setSpecularColor(glm::vec3(matColor[0], matColor[1], matColor[2]));
	}
	// Query for emissive color
	if (assimpMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, matColor) == AI_SUCCESS) {		

		meshMaterial.setEmissiveMat(glm::vec3(matColor[0], matColor[1], matColor[2]));
	}

	// Temporary to hold the path to a texture
	aiString path;

	// Load diffuse, specular, and normal maps
	if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {				// map_Kd

		if (AI_SUCCESS == assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr)) {

			std::string relativeFilePath = getDirectoryPath(filename) + path.C_Str();
			if (VERBOSE) std::cout << "Loading diffuse texture: " << relativeFilePath << std::endl;

			meshMaterial.setDiffuseTexture(Texture::GetTexture(relativeFilePath)->getTextureObject());
		}
	}
	if (assimpMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0) {				// map_Ks

		if (AI_SUCCESS == assimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr)) {

			std::string relativeFilePath = getDirectoryPath(filename) + path.C_Str();
			if (VERBOSE) std::cout << "Loading specular texture: " << relativeFilePath << std::endl;

			meshMaterial.setSpecularTexture(Texture::GetTexture(relativeFilePath)->getTextureObject());
		}
	}

	if (assimpMaterial->GetTextureCount(aiTextureType_NORMALS) > 0) {

		if (AI_SUCCESS == assimpMaterial->GetTexture(aiTextureType_NORMALS, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr)) {

			std::string relativeFilePath = getDirectoryPath(filename) + path.C_Str();
			if (VERBOSE) std::cout << "Loading Normal Map texture: " << relativeFilePath << std::endl;

			meshMaterial.setNormalMap(Texture::GetTexture(relativeFilePath)->getTextureObject());
		}
	}

	meshMaterial.setTextureMode(REPLACE_AMBIENT_DIFFUSE);

	return meshMaterial;

} // end readInMaterialProperties


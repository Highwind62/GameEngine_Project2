#include "SharedMaterials.h"

GLuint SharedMaterials::ambientColorLocation; // Byte offset ambient material color

GLuint SharedMaterials::diffuseColorLocation; // Byte offset diffuse material color

GLuint SharedMaterials::specularColorLocation; // Byte offset specular material color

GLuint SharedMaterials::emmissiveColorLocation; // Byte offset emmisive material color

GLuint SharedMaterials::specularExpLocation;  // Byte offset specular exponent location

GLuint SharedMaterials::alphaLocation; // Byte offset for alpha transparency location

GLuint SharedMaterials::diffuseTextureEnabledLocation;

GLuint SharedMaterials::specularTextureEnabledLocation;

GLuint SharedMaterials::normalMapEnabledLocation;

GLuint SharedMaterials::textureModeLoction;

SharedUniformBlock SharedMaterials::materialBlock(materialBlockBindingPoint);

const std::string SharedMaterials::materialBlockName = "MaterialBlock";


void SharedMaterials::setUniformBlockForShader(GLuint shaderProgram)
{
	std::vector<std::string> materialMemberNames = { "object.ambientMatColor", "object.diffuseMatColor", 
		"object.specularMatColor", "object.emmissiveMatColor", "object.specularExp", "object.alpha","object.diffuseTextureEnabled" ,
		"object.specularTextureEnabled", "object.normalMapTextureEnabled", "object.textureMode" };

	std::vector<GLint> uniformOffsets = materialBlock.setUniformBlockForShader(shaderProgram, materialBlockName, materialMemberNames);

	// Save locations
	ambientColorLocation = uniformOffsets[0];
	diffuseColorLocation = uniformOffsets[1];
	specularColorLocation = uniformOffsets[2];
	emmissiveColorLocation = uniformOffsets[3];
	specularExpLocation = uniformOffsets[4];
	alphaLocation = uniformOffsets[5];
	diffuseTextureEnabledLocation = uniformOffsets[6];
	specularTextureEnabledLocation = uniformOffsets[7];
	normalMapEnabledLocation = uniformOffsets[8];
	textureModeLoction = uniformOffsets[9];

} // end setUniformBlockForShader


void SharedMaterials::setShaderMaterialProperties(const Material & material)
{
	if (materialBlock.getSize() > 0) {

		// Bind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, materialBlock.getBuffer());

		// Set the Material properties in the shader.
		glBufferSubData(GL_UNIFORM_BUFFER, ambientColorLocation, sizeof(glm::vec3), &material.ambientColor);
		glBufferSubData(GL_UNIFORM_BUFFER, diffuseColorLocation, sizeof(glm::vec3), &material.diffuseColor);
		glBufferSubData(GL_UNIFORM_BUFFER, specularColorLocation, sizeof(glm::vec3), &material.specularColor);
		glBufferSubData(GL_UNIFORM_BUFFER, emmissiveColorLocation, sizeof(glm::vec3), &material.emissiveColor);
		glBufferSubData(GL_UNIFORM_BUFFER, specularExpLocation, sizeof(float), &material.specularExpMat);
		glBufferSubData(GL_UNIFORM_BUFFER, alphaLocation, sizeof(float), &material.alphaTransparency);
		glBufferSubData(GL_UNIFORM_BUFFER, diffuseTextureEnabledLocation, sizeof(bool), &material.diffuseTextureEnabled);
		glBufferSubData(GL_UNIFORM_BUFFER, specularTextureEnabledLocation, sizeof(bool), &material.specularTextureEnabled);
		glBufferSubData(GL_UNIFORM_BUFFER, normalMapEnabledLocation, sizeof(bool), &material.normalMapTextureEnabled);
		glBufferSubData(GL_UNIFORM_BUFFER, textureModeLoction, sizeof(int), &material.textureMode);

		// Unbind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Activate and set texture units.
		if (material.diffuseTextureEnabled == true) {

			glBindTextureUnit(0, material.diffuseTextureObject);

		}
		if (material.specularTextureEnabled == true) {

			glBindTextureUnit(1, material.specularTextureObject);
		}

		if (material.normalMapTextureEnabled == true) {

			glBindTextureUnit(2, material.normalMapTextureObject);
		}

	}
	if (material.alphaTransparency < 1.0) {

		glEnable(GL_BLEND);
	}

} // end setShaderMaterialProperties


void SharedMaterials::cleanUpMaterial(const Material & material)
{
	//if (material.diffuseTextureEnabled == true) {

	//	//glBindTextureUnit(0, 0);
	//}
	//if (material.specularTextureEnabled == true) {

	//	//glBindTextureUnit(1, 0);
	//}
	
	if (material.alphaTransparency < 1.0) { 

		glDisable(GL_BLEND);
	}
}

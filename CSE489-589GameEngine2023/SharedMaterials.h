#pragma once

#include "MathLibsConstsFuncs.h"
#include "SharedUniformBlock.h"

#include "Material.h"

static const GLuint materialBlockBindingPoint = 12;
static const GLuint diffuseSamplerLocation = 100;
static const GLuint specularSamplerLocation = 101;
static const GLuint normalMapSamplerLocation = 102;

using namespace constants_and_types;

class SharedMaterials
{
public:

	/**
	 * @fn	static void SharedMaterials::setUniformBlockForShader(GLuint shaderProgram);
	 *
	 * @brief	Should be called for each shader program that includes the Material uniform block.
	 *
	 * @param 	shaderProgram	The shader program.
	 */
	static void setUniformBlockForShader(GLuint shaderProgram);

	/**
	 * @fn	static void SharedMaterials::setShaderMaterialProperties(const Material & material);
	 *
	 * @brief	Called to set the Material properties in the shader before rendering the object.
	 *
	 * @param 	material	The material.
	 */
	static void setShaderMaterialProperties(const Material & material);

	/**
	 * @fn	static void SharedMaterials::cleanUpMaterial(const Material & material);
	 *
	 * @brief	Cleans Material properties after rendering an object.
	 *
	 * @param 	material	The material.
	 */
	static void cleanUpMaterial(const Material & material);

protected:

	static GLuint ambientColorLocation; // Byte offset ambient material color

	static GLuint diffuseColorLocation; // Byte offset diffuse material color

	static GLuint specularColorLocation; // Byte offset specular material color

	static GLuint emmissiveColorLocation; // Byte offset emmisive material color

	static GLuint specularExpLocation;  // Byte offset specular exponent

	static GLuint alphaLocation; // Byte offset for alpha transparency location

	static GLuint diffuseTextureEnabledLocation; //Byte offset of diffuse texture enabled indicator

	static GLuint specularTextureEnabledLocation; //Byte offset of diffuse texture enabled indicator

	static GLuint normalMapEnabledLocation; //Byte offset of diffuse texture enabled indicator

	static GLuint textureModeLoction; //Byte offset of texture mode indicator

	static SharedUniformBlock materialBlock; // Shared uniform block manager for material propertie

	const static std::string materialBlockName; // Name of the material uniform block

};
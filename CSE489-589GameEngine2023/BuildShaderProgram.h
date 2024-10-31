#pragma once

#include "MathLibsConstsFuncs.h"

using namespace constants_and_types;

/**
 * @struct	ShaderInfo
 *
 * @brief	Information about the shader that includes
 * 			the name of the shader and path as well as
 * 			the type of shader.
 */
typedef struct {
	GLenum       type;
	const char*  filename;
	GLuint       shader;
} ShaderInfo;

/**
 * @fn	GLuint BuildShaderProgram(ShaderInfo* shaders);
 *
 * @brief	Builds shader program composed of multiple shaders
 * 			and returns an integer reference to the shader 
 * 			program. Takes as input and array containing the
 * 			names of the shaders that will be used to build 
 * 			the program.
 *
 * @param [in,out]	shaders	If non-null, array containing the
 * 					names of the shaders that will be used to 
 * 					build the shader program.
 *
 * @returns	A GLuint.
 */
GLuint BuildShaderProgram(ShaderInfo* shaders);

/**
 * @fn	void deleteAllShaderPrograms();
 *
 * @brief	Deletes all shader programs that were created
 * 			by calls to BuildShaderProgram.
 */
void deleteAllShaderPrograms();

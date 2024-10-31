#pragma once

#include "MathLibsConstsFuncs.h"
#include <vector>

using namespace constants_and_types;

class SharedUniformBlock
{

public:

	/**
	 * @fn	SharedUniformBlock::SharedUniformBlock(GLint blockBindingPoint)
	 *
	 * @brief	Constructor
	 *
	 * @param	blockBindingPoint	The block binding point that will tie together the 
	 * 								uniform blocks and the buffer that stores the values
	 * 								for the variables in the block.
	 */
	SharedUniformBlock(GLint blockBindingPoint) : blockBindingPoint(blockBindingPoint) {}

	/**
	 * @fn	SharedUniformBlock::~SharedUniformBlock()
	 *
	 * @brief	Destructor. Deletes the buffer that holds the 
	 * 			values for variables in the uniform block
	 */
	~SharedUniformBlock() { if(blockBuffer>0) glDeleteBuffers(1, &blockBuffer); }

	/**
	 * @fn	std::vector<GLint> SharedUniformBlock::setUniformBlockForShader(GLuint shaderProgram, std::string blockName, std::vector<std::string> blockMembers);
	 *
	 * @brief	Sets uniform block for shader
	 *
	 * @param	shaderProgram	Identifier for a shader program in which the uniform block is declared
	 * @param	blockName	 	Name of the block in the shader programs
	 * @param	blockMembers 	The names of block members (variables in the block).
	 *
	 * @returns	A std::vector&lt;GLint&gt;
	 */
	std::vector<GLint>  setUniformBlockForShader(GLuint shaderProgram, std::string blockName, std::vector<std::string> blockMembers);

	/**
	 * @fn	GLint SharedUniformBlock::getSize()
	 *
	 * @brief	Gets the size of the buffer and uniform block in bytes
	 *
	 * @returns	The size of the buffer and uniform block in bytes.
	 */
	GLint getSize() { return blockSize; }

	/**
	 * @fn	GLuint SharedUniformBlock::getBuffer()
	 *
	 * @brief	Gets the integer identifier for buffer that stores values
	 * 			for values in a uniform block
	 *
	 * @returns	The buffer.
	 */
	GLuint getBuffer(){ return blockBuffer; }

private:

	/**
	 * @fn	void SharedUniformBlock::determineBlockSizeSetBindingPoint(GLuint shaderProgram, std::string blockName);
	 *
	 * @brief	Determines the size in bytes of the block and binds it to the binding point.
	 *
	 * @param	shaderProgram	The shader program.
	 * @param	blockName	 	Name of the uniform block.
	 */
	void determineBlockSizeSetBindingPoint(GLuint shaderProgram, std::string blockName);

	/**
	 * @fn	void SharedUniformBlock::allocateBuffer(GLuint shaderProgram);
	 *
	 * @brief	Creates the buffer and binds it to the binding point.
	 *
	 * @param	shaderProgram	The shader program that defines the uniform block
	 */
	void allocateBuffer(GLuint shaderProgram);

	/**
	 * @fn	void SharedUniformBlock::findOffsets(GLuint shaderProgram, std::vector<std::string> blockMembers);
	 *
	 * @brief	Finds the byte offsets of the variables in the two uniform  blocks.
	 *
	 * @param	shaderProgram	The shader program that defines the uniform block
	 * @param	blockMembers 	The names of the block members.
	 */
	void findOffsets(GLuint shaderProgram, std::vector<std::string> blockMembers);

	// Size in bytes of both the buffer and the uniform block in all the the shaders.
	GLint blockSize = 0;

	// Binding point to which the blocks and buffer will be bound
	GLuint blockBindingPoint = 0;

	// Identifier for the buffer. There would only be one buffer to feed several uniform blocks.
	GLuint blockBuffer = 0;

	// Indicates whether or not the buffer for the blocks has been setup
	bool blockSizeAndOffetsSet = false; 

	// Holds byte offsets for the members of the uniform block
	std::vector<GLint> offsets;

}; // end SharedUniformBlock


bool checkBlockLocationFound(const GLchar* locationName, GLuint indice);



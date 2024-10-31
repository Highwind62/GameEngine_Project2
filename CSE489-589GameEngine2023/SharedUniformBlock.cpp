#include "SharedUniformBlock.h"

static const bool VERBOSE = false;

bool checkBlockLocationFound(const GLchar* locationName, GLuint indice)
{
	if (indice == GL_INVALID_INDEX) {
		std::cerr << locationName << " not found in shader." << std::endl;

		return false;
	}
	else {

		//cout << locationName << " index is " <<  indice << endl; 
		return true;
	}

} // end checkBlockLocationFound

std::vector<GLint> SharedUniformBlock::setUniformBlockForShader(GLuint shaderProgram, std::string blockName, std::vector<std::string> blockMembers)
{
	// Determine the size of the block and set the binding point for the block(s)
	determineBlockSizeSetBindingPoint(shaderProgram, blockName);

	// Has the buffer been created and have the byte offset been found?
	if (blockSizeAndOffetsSet == false ) {

		// Set up the buffers and bind to binding points
		allocateBuffer(shaderProgram);

		// Find the byte offsets of the uniform block variables
		findOffsets(shaderProgram, blockMembers);

		// Indicate the buffer has already been allocated and the offsets have been determined
		blockSizeAndOffetsSet = true;
	}

	return offsets;

} // end setUniformBlockForShader


void SharedUniformBlock::determineBlockSizeSetBindingPoint(GLuint shaderProgram, std::string blockName)
{
	// Get the index of the block
	GLuint blockIndex = glGetUniformBlockIndex(shaderProgram, blockName.c_str());

	if (checkBlockLocationFound(blockName.c_str(), blockIndex)) {

		// Determine the size in bytes of the uniform block.
		glGetActiveUniformBlockiv(shaderProgram, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
		if ( VERBOSE ) std::cout << blockName.c_str() << " size is " << blockSize << std::endl;

		// Assign the block to a binding point. 
		glUniformBlockBinding(shaderProgram, blockIndex, blockBindingPoint);
	}

} // end determineBlockSizeSetBindingPoint


void SharedUniformBlock::allocateBuffer(GLuint shaderProgram )
{
	if (blockSize > 0) {

		// Get an identifier for a buffer
		glGenBuffers(1, &blockBuffer);

		// Bind the buffer
		glBindBuffer(GL_UNIFORM_BUFFER, blockBuffer);

		// Allocate the buffer. Does not load data. Note the use of nullptr where the data would normally be.
		// Data is not loaded because the above struct will not be byte alined with the uniform block.
		glBufferData(GL_UNIFORM_BUFFER, blockSize, nullptr, GL_DYNAMIC_DRAW);

		// Assign the buffer to a binding point to be the same as the uniform block in the shader(s). 
		glBindBufferBase(GL_UNIFORM_BUFFER, blockBindingPoint, blockBuffer);
	}

} // end allocateBuffer


void SharedUniformBlock::findOffsets(GLuint shaderProgram, std::vector<std::string> blockMembers)
{
	const int numberOfNames = static_cast<int>(blockMembers.size());

	GLuint* uniformIndices = new GLuint[numberOfNames];
	GLint* uniformOffsets = new GLint[numberOfNames];
	GLchar ** charStringNames = new GLchar *[numberOfNames];

	// Represent all the variable names stored as strings as character arrays
	for (int i = 0; i < numberOfNames; i++) {

		charStringNames[i] = (GLchar *)blockMembers[i].c_str();
	}

	// Get the index for each on of the names in the uniform block
	glGetUniformIndices(shaderProgram, numberOfNames, (const GLchar **)charStringNames, uniformIndices);

	// Verify that all the names were acually found and print out and error message if not
	for (int i = 0; i < numberOfNames; i++) {

		checkBlockLocationFound(charStringNames[i], uniformIndices[i]);
	}

	//Get the byte offsets for each of the uniforms members using the indices. The 
	// offsets in the buffer will be the same.
	glGetActiveUniformsiv(shaderProgram, numberOfNames, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);

	// Copy the byte offsets into a vector that can be returned to the to 
	// the subclass that is setting up a uniform block. 
	for (int i = 0; i < numberOfNames; i++) {

		if (VERBOSE) std::cout << '\t' << charStringNames[i] << " offset is " << uniformOffsets[i] << std::endl;
			offsets.push_back(uniformOffsets[i]);
	}

	// Deallocate memory
	delete [] uniformIndices;
	delete [] uniformOffsets;
	delete [] charStringNames;

} // findOffsets



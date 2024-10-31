#include "SharedTransformations.h"

GLuint SharedTransformations::projectionLocation; // Byte offset of the projection matrix
glm::mat4 SharedTransformations::projectionMatrix; // Current projection matrix that is held in the buffer

GLuint SharedTransformations::viewLocation; // Byte offset of the viewing matrix
glm::mat4 SharedTransformations::viewMatrix; // Current viewing matrix that is held in the buffer

GLuint SharedTransformations::modelLocation; // Byte offset of the modeling matrix
glm::mat4 SharedTransformations::modelMatrix; // Current modeling matrix that is held in the buffer

GLuint SharedTransformations::normalModelLocation; // Byte offset of the modeling matrix to transform normal vectors

GLuint SharedTransformations::eyePositionLocation;  // Byte offset of the eye position

SharedUniformBlock SharedTransformations::projViewBlock(projectionViewBlockBindingPoint);
SharedUniformBlock SharedTransformations::worldEyeBlock(worldEyeBlockBindingPoint);

const std::string SharedTransformations::transformBlockName = "transformBlock";

const std::string SharedTransformations::eyeBlockName = "worldEyeBlock";


void SharedTransformations::setUniformBlockForShader(GLuint shaderProgram)
{
	std::vector<std::string> projViewMemberNames = { "modelMatrix", "viewMatrix", "projectionMatrix","normalModelMatrix", };

	std::vector<GLint> uniformOffsets = projViewBlock.setUniformBlockForShader(shaderProgram, transformBlockName, projViewMemberNames);

	// Save locations
	modelLocation = uniformOffsets[0];
	viewLocation = uniformOffsets[1];
	projectionLocation = uniformOffsets[2];
	normalModelLocation = uniformOffsets[3];

	uniformOffsets.clear();
	std::vector<std::string> worldEyeMemberNames = { "worldEyePosition" };

	uniformOffsets = worldEyeBlock.setUniformBlockForShader(shaderProgram, eyeBlockName, worldEyeMemberNames);

	// Save location
	eyePositionLocation = uniformOffsets[0];

} // end setUniformBlockForShader


void SharedTransformations::setViewMatrix( glm::mat4 viewMatrix)
{
	if (projViewBlock.getSize() > 0  ) {

		// Bind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, projViewBlock.getBuffer());

		SharedTransformations::viewMatrix = viewMatrix;

		glBufferSubData(GL_UNIFORM_BUFFER, viewLocation, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
	}

	if (worldEyeBlock.getSize() > 0 ) {

		// Bind the buffer.
		glBindBuffer(GL_UNIFORM_BUFFER, worldEyeBlock.getBuffer());

		glm::vec3 viewPoint = vec3(glm::inverse(viewMatrix)[3]);

		glBufferSubData(GL_UNIFORM_BUFFER, eyePositionLocation, sizeof(glm::vec3), glm::value_ptr(viewPoint));
	}

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setViewMatrix


// Accessor for the current viewing matrix
glm::mat4 SharedTransformations::getViewMatrix()
{
	return viewMatrix;

} // end getViewMatrix


void SharedTransformations::setProjectionMatrix( glm::mat4 projectionMatrix)
{
	if (projViewBlock.getSize()  > 0  ) {
	
		// Bind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, projViewBlock.getBuffer());

		SharedTransformations::projectionMatrix = projectionMatrix;

		glBufferSubData(GL_UNIFORM_BUFFER, projectionLocation, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));

		// Unbind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

} // end setProjectionMatrix


// Accessor for the current projection matrix
glm::mat4 SharedTransformations::getProjectionMatrix()
{
	return projectionMatrix;

} // end getProjectionMatrix


void SharedTransformations::setModelingMatrix(glm::mat4 modelingMatrix)
{
	if (projViewBlock.getSize() > 0) {

		// Bind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, projViewBlock.getBuffer() );

		SharedTransformations::modelMatrix = modelingMatrix;

		glBufferSubData(GL_UNIFORM_BUFFER, modelLocation, sizeof(glm::mat4), glm::value_ptr(modelMatrix));

		// Create a modeling transform for normals that will be correct under non-uniform scale
		mat4 normalModelMatrix = modelMatrix;// glm::transpose(glm::inverse(modelMatrix));

		mat3 normalModelMatrix3X3;

		normalModelMatrix3X3[0][0] = normalModelMatrix[0][0];
		normalModelMatrix3X3[0][1] = normalModelMatrix[0][1];
		normalModelMatrix3X3[0][2] = normalModelMatrix[0][2];

		normalModelMatrix3X3[1][0] = normalModelMatrix[1][0];
		normalModelMatrix3X3[1][1] = normalModelMatrix[1][1];
		normalModelMatrix3X3[1][2] = normalModelMatrix[1][2];

		normalModelMatrix3X3[2][0] = normalModelMatrix[2][0];
		normalModelMatrix3X3[2][1] = normalModelMatrix[2][1];
		normalModelMatrix3X3[2][2] = normalModelMatrix[2][2];

		//mat3 normalModelMatrix3X3 = mat3(glm::transpose(glm::inverse(modelMatrix)));

		glBufferSubData(GL_UNIFORM_BUFFER, normalModelLocation, sizeof(glm::mat4), glm::value_ptr(normalModelMatrix));

		// Unbind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

} // end setModelingMatrix


// Accessor for the current modeling matrix
glm::mat4 SharedTransformations::getModelingMatrix()
{
	return modelMatrix;

} // end getModelingMatrix


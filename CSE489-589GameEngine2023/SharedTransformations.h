#ifndef _SHARED_PROJECTION_AND_VIEWING_H_
#define	_SHARED_PROJECTION_AND_VIEWING_H_

#include "SharedUniformBlock.h"

static const GLuint projectionViewBlockBindingPoint = 2;
static const GLuint worldEyeBlockBindingPoint = 3;

/**

A static class that supports working with the two uniform blocks shown below.

setUniformBlockForShader should be called for every shader program that includes
the uniform block below. This will create a buffers for each uniform block
and bind the buffers and uniform blocks together through the binding points.
Binding points for the two blocks and the associated buffers are defined above.

Adding a #include for this header file makes the functionality avaible through
the class name.

layout(shared) uniform transformBlock
{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 normalModelMatrix;
};

layout (shared) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};

*/
class SharedTransformations
{
	public:

	// Should be called for each shader program that includes the
	// projectionViewBlock and worldEyeBlock uniform blocks.
	static void setUniformBlockForShader(GLuint shaderProgram);

	// Accessor for the current projection matrix
	static glm::mat4 getProjectionMatrix();

	// Mutator for the projection matrix. Sets the projection
	// matrix in the buffer.
	static void setProjectionMatrix(glm::mat4 projectionMatrix);

	// Accessor for the current viewing matrix
	static glm::mat4 getViewMatrix();

	// Mutator for the viewing matrix. Sets the viewing
	// matrix in the buffer. Also sets the world eye position
	static void setViewMatrix(glm::mat4 viewMatrix);

	// Accessor for the current modeling matrix
	static glm::mat4 getModelingMatrix();

	// Mutator for the modeling matrix. Sets the modeling transformation
	// for both the vertex positions and normals in the buffer. 
	static void setModelingMatrix(glm::mat4 modelingMatrix);

	protected:

	static GLuint projectionLocation; // Byte offset of the projection matrix
	static glm::mat4 projectionMatrix; // Current projection matrix that is held in the buffer

	static GLuint viewLocation; // Byte offset of the viewing matrix
	static glm::mat4 viewMatrix; // Current viewing matrix that is held in the buffer

	static GLuint modelLocation; // Byte offset of the modeling matrix
	static glm::mat4 modelMatrix; // Current modeling matrix that is held in the buffer

	static GLuint normalModelLocation; // Byte offset of the modeling matrix to transform normal vectors

	static GLuint eyePositionLocation;  // Byte offset of the eye position

	static SharedUniformBlock projViewBlock;
	
	static SharedUniformBlock worldEyeBlock;

	static const std::string transformBlockName;

	static const std::string eyeBlockName;

}; // end SharedProjectionAndViewing class

#endif // _SHARED_PROJECTION_AND_VIEWING_H_


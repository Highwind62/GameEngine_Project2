#include "SharedLighting.h"

#include <sstream> 

GeneralLight SharedLighting::lights[MAX_LIGHTS];

const std::string SharedLighting::generalLightBlockName = "LightBlock";

SharedUniformBlock SharedLighting::lightBlock(generalLightBlockBindingPoint);

void SharedLighting::setUniformBlockForShader(GLuint shaderProgram)
{
	std::vector <std::string > lightBlockMemberNames = buildUniformBlockNameList();

	std::vector<GLint> uniformOffsets = lightBlock.setUniformBlockForShader(shaderProgram,
		generalLightBlockName,
		lightBlockMemberNames);
	int offsetIndex = 0;

	for (int i = 0; i < MAX_LIGHTS; i++) {

		// Light color locations
		lights[i].ambientColorLoc = uniformOffsets[offsetIndex++];
		lights[i].diffuseColorLoc = uniformOffsets[offsetIndex++];
		lights[i].specularColorLoc = uniformOffsets[offsetIndex++];

		// Position and direction location
		lights[i].positionOrDirectionLoc = uniformOffsets[offsetIndex++];

		// Spotlight locations
		lights[i].spotDirectionLoc = uniformOffsets[offsetIndex++];
		lights[i].isSpotLoc = uniformOffsets[offsetIndex++];
		lights[i].spotCutoffCosLoc = uniformOffsets[offsetIndex++];
		lights[i].spotExponentLoc = uniformOffsets[offsetIndex++];

		//Attenuation factor locations
		lights[i].constantLoc = uniformOffsets[offsetIndex++];
		lights[i].linearLoc = uniformOffsets[offsetIndex++];
		lights[i].quadraticLoc = uniformOffsets[offsetIndex++];

		// Enabled location
		lights[i].enabledLoc = uniformOffsets[offsetIndex++];

		// Initialize the attributes of the light to something meaningful
		initilizeAttributes(i);
	}

} // end setUniformBlockForShader


std::vector<std::string> SharedLighting::buildUniformBlockNameList()
{
	static const int NUM_LIGHT_ATTRIBUTES = 12;

	// Get the indices of the uniform block variables in the shader
	static const std::string names[NUM_LIGHT_ATTRIBUTES]
		= { "ambientColor", "diffuseColor", "specularColor",
		"positionOrDirection", "spotDirection",
		"isSpot", "spotCutoffCos", "spotExponent",
		"constant", "linear", "quadratic", "enabled" };

	std::vector<std::string> uniformBlockNames;

	for (int lightNumber = 0; lightNumber < MAX_LIGHTS; lightNumber++) {

		// Get string representation of the int
		std::string numberStrg = std::to_string(lightNumber);

		std::string uniformNames[NUM_LIGHT_ATTRIBUTES];

		for (int i = 0; i < NUM_LIGHT_ATTRIBUTES; i++) {

			uniformBlockNames.push_back("lights[" + numberStrg + "]." + names[i]);
		}

	}

	return uniformBlockNames;

} // end buildUniformBlockNameList


void SharedLighting::initilizeAttributes(GLint lightNumber)
{
	setEnabled(lightNumber, false);

	setAmbientColor(lightNumber, 0.15f * WHITE_RGB);
	setDiffuseColor(lightNumber, WHITE_RGB);
	setSpecularColor(lightNumber, WHITE_RGB);

	setPositionOrDirection(lightNumber, vec4(1.0f, 1.0f, 1.0f, 0.0f));

	setIsSpot(lightNumber, false);
	setSpotDirection(lightNumber, vec3(0.0f, 0.0f, -1.0f));
	setSpotCutoffCos(lightNumber, glm::radians(180.0f));
	setSpotExponent(lightNumber, 50.0f);

	setConstantAttenuation(lightNumber, 1.0f);
	setLinearAttenuation(lightNumber, 0.0f);
	setQuadraticAttenuation(lightNumber, 0.0f);

} // end initilizeAttributes


void SharedLighting::setEnabled(int lightIndex, bool on)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[lightIndex].enabled = on;

	glBufferSubData(GL_UNIFORM_BUFFER, lights[lightIndex].enabledLoc, sizeof(bool), &lights[lightIndex].enabled);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setEnabled

void SharedLighting::setAmbientColor(int lightIndex, glm::vec3 color4)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[lightIndex].ambientColor = color4;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[lightIndex].ambientColorLoc, sizeof(glm::vec3), value_ptr(lights[lightIndex].ambientColor));

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setAmbientColor

void SharedLighting::setDiffuseColor(int lightIndex, glm::vec3 color4)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[lightIndex].diffuseColor = color4;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[lightIndex].diffuseColorLoc, sizeof(glm::vec3), value_ptr(lights[lightIndex].diffuseColor));

	// Unbind the buffer.
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setDiffuseColor

void SharedLighting::setSpecularColor(int lightIndex, glm::vec3 color4)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[lightIndex].specularColor = color4;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[lightIndex].specularColorLoc, sizeof(glm::vec3), value_ptr(lights[lightIndex].specularColor));

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setSpecularColor

void SharedLighting::setPositionOrDirection(int lightIndex, glm::vec4 positOrDirect)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[lightIndex].positionOrDirection = positOrDirect;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[lightIndex].positionOrDirectionLoc, sizeof(glm::vec4), value_ptr(lights[lightIndex].positionOrDirection));

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setPositionOrDirection

void SharedLighting::setAttenuationFactors(int lightIndex, glm::vec3 factors)
{
	setConstantAttenuation(lightIndex, factors.x);
	setLinearAttenuation(lightIndex, factors.y);
	setQuadraticAttenuation(lightIndex, factors.z);

} // end setAttenuationFactors

void SharedLighting::setConstantAttenuation(int lightIndex, float factor)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[lightIndex].constant = factor;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[lightIndex].constantLoc, sizeof(float), &lights[lightIndex].constant);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setConstantAttenuation

void SharedLighting::setLinearAttenuation(int lightIndex, float factor)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[lightIndex].linear = factor;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[lightIndex].linearLoc, sizeof(float), &lights[lightIndex].linear);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setLinearAttenuation

void SharedLighting::setQuadraticAttenuation(int lightIndex, float factor)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[lightIndex].quadratic = factor;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[lightIndex].quadraticLoc, sizeof(float), &lights[lightIndex].quadratic);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setQuadraticAttenuation

void SharedLighting::setIsSpot(int lightIndex, bool spotOn)
{
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[lightIndex].isSpot = spotOn;

	glBufferSubData(GL_UNIFORM_BUFFER, lights[lightIndex].isSpotLoc, sizeof(bool), &lights[lightIndex].isSpot);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setIsSpot

void SharedLighting::setSpotDirection(int lightIndex, glm::vec3 spotDirect)
{
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[lightIndex].spotDirection = glm::normalize(spotDirect);
	glBufferSubData(GL_UNIFORM_BUFFER, lights[lightIndex].spotDirectionLoc, sizeof(glm::vec3), value_ptr(lights[lightIndex].spotDirection));

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setSpotDirection

void SharedLighting::setSpotCutoffCos(int lightIndex, float cutoffCosRadians)
{
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[lightIndex].spotCutoffCos = cutoffCosRadians;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[lightIndex].spotCutoffCosLoc, sizeof(float), &lights[lightIndex].spotCutoffCos);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setSpotCutoffCos

void SharedLighting::setSpotExponent(int lightIndex, float spotEx)
{
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[lightIndex].spotExponent = spotEx;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[lightIndex].spotExponentLoc, sizeof(float), &lights[lightIndex].spotExponent);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setSpotExponent
#pragma once

#include "SharedUniformBlock.h"

static const int generalLightBlockBindingPoint = 24;

// Maximum number of lights
// If this is increased, the size of the lights array in the shader 
// program needs to be adjusted.
static const int MAX_LIGHTS = 16;

// Structure for holding the attributes of an individual light source
// Note: See initilizeAttributes method of SharedLighting class to see where
// initial values of light attributes are set.
struct GeneralLight {

	GLint ambientColorLoc;
	vec3 ambientColor; //0.15f * WHITE_RGBA;	// ambient color of the light
	GLint diffuseColorLoc;
	vec3 diffuseColor; // WHITE_RGBA;			// diffuse color of the light
	GLint specularColorLoc;
	vec3 specularColor; // WHITE_RGBA;			// specular color of the light

	// Either the position or direction
	// if w = 0 then the light is directional
	// if w = 1 then the light is positional
	// direction is the negative of the direction the light is shinning
	GLint positionOrDirectionLoc;
	glm::vec4 positionOrDirection; // vec4(1.0f, 1.0f, 1.0f, 0.0f);

	// spotlight attributes
	GLint spotDirectionLoc;
	glm::vec3 spotDirection; // vec3(0.0f, 0.0f, -1.0f);		// the direction the cone of light is shinning      
	GLint isSpotLoc;
	bool isSpot; // false;					// true if the light is a spotlight
	GLint spotCutoffCosLoc;
	float spotCutoffCos; // lm::radians(180.0f);	// Cosine of the spot cutoff angle
	GLint spotExponentLoc;
	float spotExponent; // 50.0f;				// spot exponent for falloff calculation

	// attenuation coefficients
	GLint constantLoc;
	float constant; // 1.0f;
	GLint linearLoc;
	float linear; // 0.0f;
	GLint quadraticLoc;
	float quadratic; // 0.0f;

	GLint enabledLoc;
	bool enabled; // false;			// true if light is "on"

	bool inUse = false;
};

class SharedLighting
{
public:

	friend class LightComponent;

	static void setUniformBlockForShader(GLuint shaderProgram);

	static bool getEnabled(int lightIndex) { return lights[lightIndex].enabled; }
	static void setEnabled(int lightIndex, bool on);

	static glm::vec3 getAmbientColor(int lightIndex) { return lights[lightIndex].ambientColor; }
	static void setAmbientColor(int lightIndex, glm::vec3 color4);

	static glm::vec3 getDiffuseColor(int lightIndex) { return lights[lightIndex].diffuseColor; }
	static void setDiffuseColor(int lightIndex, glm::vec3 color4);

	static glm::vec3 getSpecularColor(int lightIndex) { return lights[lightIndex].specularColor; }
	static void setSpecularColor(int lightIndex, glm::vec3 color4);

	static glm::vec4 getPositionOrDirection(int lightIndex) { return lights[lightIndex].positionOrDirection; }
	static void setPositionOrDirection(int lightIndex, glm::vec4 positOrDirect);

	static void setAttenuationFactors(int lightIndex, glm::vec3 factors);

	static float getConstantAttenuation(int lightIndex) { return lights[lightIndex].constant; }
	static void setConstantAttenuation(int lightIndex, float factor);

	static float getLinearAttenuation(int lightIndex) { return lights[lightIndex].linear; }
	static void setLinearAttenuation(int lightIndex, float factor);

	static float getQuadraticAttenuation(int lightIndex) { return lights[lightIndex].quadratic; }
	static void setQuadraticAttenuation(int lightIndex, float factor);

	static bool getIsSpot(int lightIndex) { return lights[lightIndex].isSpot; }
	static void setIsSpot(int lightIndex, bool spotOn);

	static glm::vec3 getSpotDirection(int lightIndex) { return lights[lightIndex].spotDirection; }
	static void setSpotDirection(int lightIndex, glm::vec3 spotDirect);

	static float getSpotCutoffCos(int lightIndex) { return lights[lightIndex].spotCutoffCos; }
	static void setSpotCutoffCos(int lightIndex, float cutoffCosRadians);

	static float getSpotExponent(int lightIndex) { return lights[lightIndex].spotExponent; }
	static void setSpotExponent(int lightIndex, float spotEx);

protected:

	static std::vector<std::string> buildUniformBlockNameList();

	static void initilizeAttributes(GLint lightNumber);

	static GeneralLight lights[MAX_LIGHTS];

	static SharedUniformBlock lightBlock;

	const static std::string generalLightBlockName;
};



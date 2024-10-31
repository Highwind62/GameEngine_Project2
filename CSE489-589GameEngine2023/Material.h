#pragma once

#include "MathLibsConstsFuncs.h"

using namespace constants_and_types;

struct Material
{
	friend class SharedMaterials;

	Material(string materialName = "default" )
	{
		static int id = 0;
		_id = id++;
	}

	void setAmbientColor(glm::vec3 ambientColor)
	{
		this->ambientColor = glm::clamp(ambientColor, 0.0f, 1.0f);
	}

	void setDiffuseColor(glm::vec3 diffuseColor)
	{
		this->diffuseColor = glm::clamp(diffuseColor, 0.0f, 1.0f);
	}

	void setSpecularColor(glm::vec3 specularColor)
	{
		this->specularColor = glm::clamp(specularColor, 0.0f, 1.0f);
	}

	void setSpecularExponentMat(float shininess)
	{
		this->specularExpMat = glm::clamp(shininess, 0.0f, INFINITY);
	}

	void setTransparencyMat(float alphaTransperancy)
	{
		this->alphaTransparency = glm::clamp(alphaTransperancy, 0.0f, 1.0f);
	}


	void setEmissiveMat(glm::vec3 emissiveColor)
	{
		this->emissiveColor = glm::clamp(emissiveColor, 0.0f, 1.0f);
	}

	void setAmbientAnddiffuseMatColor(glm::vec3 objectColor)
	{
		setAmbientColor(objectColor);
		setDiffuseColor(objectColor);

	}

	void setTextureMode(TextureMode textureMode)
	{
		if (textureMode >= NO_TEXTURE && textureMode <= REPLACE_AMBIENT_DIFFUSE)

			this->textureMode = textureMode;
	}

	void setDiffuseTexture(GLint textureObject)
	{
		this->diffuseTextureObject = textureObject;
		setTextureMode(REPLACE_AMBIENT_DIFFUSE);
		diffuseTextureEnabled = true;

	} // end setDiffuseTexture

	void setSpecularTexture(GLint textureObject)
	{
		this->specularTextureObject = textureObject;
		setTextureMode(REPLACE_AMBIENT_DIFFUSE);
		specularTextureEnabled = true;

	} // end setSpecularTexture

	void setNormalMap(GLint textureObject)
	{
		this->normalMapTextureObject = textureObject;
		normalMapTextureEnabled = true;

	} // end setNormalMap


	int _id;

protected:

	glm::vec3 ambientColor = glm::vec3(0.75f, 0.75f, 0.75f);

	glm::vec3 diffuseColor = glm::vec3(0.75f, 0.75f, 0.75f);

	glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 emissiveColor = glm::vec3(0.0f, 0.0f, 0.0f);

	float specularExpMat = 64.0f;

	float alphaTransparency = 1.0f;

	TextureMode textureMode = NO_TEXTURE;

	GLuint diffuseTextureObject = 0;
	bool diffuseTextureEnabled = false;

	GLuint specularTextureObject = 0;
	bool specularTextureEnabled = false;

	GLuint normalMapTextureObject = 0;
	bool normalMapTextureEnabled = false;

};



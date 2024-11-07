#pragma once
#include "ModelMeshComponent.h"

class ModelMakerComponent : public Component
{
public:

	ModelMakerComponent(GLuint shaderProgram, string pathAndFileName = "Assets/jet_models/F-15C_Eagle.obj");

	~ModelMakerComponent();

	virtual void initialize() override;

	virtual void processInput() override;

protected:

	GameObjectPtr pausedObject;

	string pathAndFileName;

	GLuint shaderProgram = 0;

	bool j_key_up = true;
};


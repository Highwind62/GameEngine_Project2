#pragma once

#include "GameEngine.h"

class Scene2 : public Game
{
	void loadScene() override
	{
		// Set the window title
		glfwSetWindowTitle(renderWindow, "Scene 2");

		// Set the clear color
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// Build and use the shader program
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/vertexShader.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/fragmentShader.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		GLuint shaderProgram = BuildShaderProgram(shaders);

		// Make the shader program "current" Not needed if transformations are stored in uniform buffers
		//glUseProgram(shaderProgram);

		SharedMaterials::setUniformBlockForShader(shaderProgram);
		SharedTransformations::setUniformBlockForShader(shaderProgram);
		SharedLighting::setUniformBlockForShader(shaderProgram);

		// ****** Brick Box *********
		GameObjectPtr boxObject2 = std::make_shared<GameObject>();

		addChildGameObject(boxObject2);
		boxObject2->setPosition(vec3(0.0f, 0.0f, -40.0f), WORLD);

		Material boxMaterial2;
		boxMaterial2.setDiffuseColor(vec3(0.2f, 0.2f, 0.5f));

		boxMaterial2.setDiffuseTexture(Texture::GetTexture("Textures/BRICK.BMP")->getTextureObject());

		boxMaterial2.setTextureMode(DECAL);

		std::shared_ptr<BoxMeshComponent> bowMesh2 = std::make_shared<BoxMeshComponent>(shaderProgram, boxMaterial2, 10.0f, 10.0f, 10.0f);
		boxObject2->addComponent(bowMesh2);

		boxObject2->addComponent(make_shared<ArrowRotateComponent>(glm::radians(25.0f)));

	}
};

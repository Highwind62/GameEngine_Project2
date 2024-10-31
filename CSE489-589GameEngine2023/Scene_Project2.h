#pragma once

#include "GameEngine.h"

class Scene_Project2 : public Game
{
	void loadScene() override
	{
		// Set the window title
		glfwSetWindowTitle(renderWindow, "Scene Project 2");

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

		// ****** Blue Sphere  *********
		GameObjectPtr sphereObject2 = std::make_shared<GameObject>();

		addChildGameObject(sphereObject2);
		sphereObject2->setPosition(vec3(0.0f, 0.0f, -40.0f), WORLD);

		Material sphereMaterial2;
		sphereMaterial2.setDiffuseTexture(Texture::GetTexture("Textures/Earthmap.jpg")->getTextureObject());

		sphereMaterial2.setTextureMode(DECAL);

		std::shared_ptr<SphereMeshComponent> sphereMesh2 = std::make_shared<SphereMeshComponent>(shaderProgram, sphereMaterial2, 10.0f, 16, 32);
		sphereObject2->addComponent(sphereMesh2);

		sphereObject2->addComponent(make_shared<ArrowRotateComponent>(glm::radians(25.0f)));
		sphereObject2->addComponent(make_shared<MoveForwardComponent>(20.0f, 200));

		shared_ptr<SoundSourceComponent> mySound = make_shared<SoundSourceComponent>("Assets/Sounds/pop.wav");
		mySound->setLooping(true);
		mySound->play();
		mySound->setGain(2.0f);

		sphereObject2->addComponent(mySound);

		// ****** Floor Game Object *********

		GameObjectPtr floorGO = std::make_shared<GameObject>();
		this->addChildGameObject(floorGO);
		floorGO->setPosition(vec3(0.0f, -5.0f, 0.0f), WORLD);

		Material flloorMat;

		flloorMat.setDiffuseTexture(Texture::GetTexture("Textures/wood.png")->getTextureObject());
		std::shared_ptr<BoxMeshComponent> floorMesh = std::make_shared<BoxMeshComponent>(shaderProgram, flloorMat, 100.0f, 1.0f, 100.0f);

		floorGO->addComponent(floorMesh);

		// Camera
		GameObjectPtr cameraGO = std::make_shared<GameObject>();
		addChildGameObject(cameraGO);

		cameraGO->setPosition(vec3(10.0f, 0.0f, 40.0f), WORLD);
		cameraGO->rotateTo(sphereObject2->getPosition() - cameraGO->getPosition(), WORLD);

		std::shared_ptr<CameraComponent> camera = make_shared<CameraComponent>();
		cameraGO->addComponent(camera);

		cameraGO->addComponent(make_shared<SoundListenerComponent>());
	}
};

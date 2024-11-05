#pragma once

#include "GameEngine.h"

class Scene0 : public Game
{
	void loadScene() override
	{
		// Set the window title
		glfwSetWindowTitle(renderWindow, "Scene 0");

		// Set the clear color
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// Build and use the shader program
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/vertexShader.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/fragmentShader.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		GLuint shaderProgram = BuildShaderProgram(shaders);

		SharedMaterials::setUniformBlockForShader(shaderProgram);
		SharedTransformations::setUniformBlockForShader(shaderProgram);
		SharedLighting::setUniformBlockForShader(shaderProgram);

		// ****** Directional light *********

		GameObjectPtr directionaLightGO = std::make_shared<GameObject>();

		addChildGameObject(directionaLightGO);
		directionaLightGO->rotateTo(vec3(1.0f, -0.25f, 0.0f), WORLD);

		std::shared_ptr<DirectionalLightComponent> dlc = std::make_shared<DirectionalLightComponent>(GLFW_KEY_D);
		dlc->setAmbientColor(vec3(0.2f, 0.2f, 0.2f));
		dlc->setDiffuseColor(vec3(1.0f, 1.0f, 1.0f));
		dlc->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));
		dlc->setEnable(true);
		directionaLightGO->addComponent(dlc);



		// ****** Falling Dinosaurs  *********

		for (int i = 0; i < 10; i++) {

			GameObjectPtr dinoObject = std::make_shared<GameObject>();

			this->addChildGameObject(dinoObject);
			std::shared_ptr<ModelMeshComponent> dino = std::make_shared<ModelMeshComponent>("Assets/Dinosaur/Trex.obj", shaderProgram);
			dinoObject->addComponent(dino);
			dinoObject->setPosition(vec3(rand() % 10 - 5.0f, 100.0f + rand() % 50 - 25.0f, rand() % 10 - 5.0f -85.0f), WORLD);
			//dinoObject->addComponent(std::make_shared< RemoveComponent>(15.0f));
			dinoObject->addComponent(std::make_shared< RigidBodyComponent>(dino, DYNAMIC));
			//dinoObject->addComponent(std::make_shared< CollisionComponent());
		}

		// ****** Red Sphere  *********

		// Add a game object to hold a red sphere
		GameObjectPtr sphereObject = std::make_shared<GameObject>();
		sphereObject->gameObjectName = "red sphere";

		addChildGameObject(sphereObject);
		sphereObject->setPosition(vec3(1.0f, 0.0f, 0.0f), WORLD);

		// Create a red material for the sphere
		Material sphereMaterial;
		sphereMaterial.setAmbientAnddiffuseMatColor(RED_RGB);
		sphereMaterial.setSpecularColor(WHITE_RGB);

		// Create the sphere mesh
		std::shared_ptr<SphereMeshComponent> sphereMesh = std::make_shared<SphereMeshComponent>(shaderProgram, sphereMaterial, 1.0f, 16, 32);
		sphereObject->addComponent(sphereMesh);

		std::shared_ptr<RigidBodyComponent> rg = std::make_shared<RigidBodyComponent>(sphereMesh, DYNAMIC, 1000.0f);
		sphereObject->addComponent(rg);
		rg->setVelocity(20.0f * vec3(0.0f, 1.0f, -1.0f));
		rg->setGravityOn(true);

		std::shared_ptr<CollisionComponent> cc = std::make_shared<CollisionComponent>();

		sphereObject->addComponent( cc);
		// sphereObject->addComponent(std::make_shared<RemoveComponent>());

		// Add a point light that will be inside the sphere
		std::shared_ptr<LightComponent> pointlight = std::make_shared< PositionalLightComponent>(GLFW_KEY_P);
		pointlight->setAmbientColor(vec3(0.1f, 0.1f, 0.1f));
		pointlight->setDiffuseColor(vec3(1.0f, 1.0f, 1.0f));
		pointlight->setSpecularColor(0.5f * vec3(1.0f, 1.0f, 1.0f));
		pointlight->setEnable(true);

		sphereObject->addComponent(pointlight);
		
		// ****** Blue Sphere  *********
		GameObjectPtr sphereObject2 = std::make_shared<GameObject>();

		addChildGameObject(sphereObject2);
		//sphereObject2->setPosition(vec3(0.0f, -2.5f, -15.0f), WORLD);
		sphereObject2->setRotation(glm::rotate(-PI/4.0f, UNIT_Y_V3 ), LOCAL);
		sphereObject2->setPosition(vec3(0.0f, 0.0f, -85.0f), WORLD);
		sphereObject2->gameObjectName = "blue sphere";

		sphereObject2->addComponent(make_shared<ArrowRotateComponent>(5.0f, 100));
		//sphereObject2->addComponent(make_shared<MoveForwardComponent>(5.0f, 200));

		Material sphereMaterial2;
		sphereMaterial2.setAmbientAnddiffuseMatColor(BLUE_RGB);
		sphereMaterial2.setDiffuseTexture(Texture::GetTexture("Textures/earthmap.jpg")->getTextureObject());
		sphereMaterial2.setSpecularExponentMat(256.0f);
		
		//std::shared_ptr<ModelMeshComponent> sphereMesh2 = std::make_shared<ModelMeshComponent>("Assets/teapot/teapot.obj", shaderProgram);
		std::shared_ptr<SphereMeshComponent> sphereMesh2 = std::make_shared<SphereMeshComponent>(shaderProgram, sphereMaterial2, 10.0f, 16, 32);
		sphereObject2->addComponent(sphereMesh2);

		std::shared_ptr<RigidBodyComponent> rg2 = std::make_shared<RigidBodyComponent>(sphereMesh2, STATIONARY);
		sphereObject2->addComponent(rg2);

		// ****** Camera *********

		GameObjectPtr cameraObject = std::make_shared<GameObject>();

		addChildGameObject(cameraObject);
		cameraObject->setPosition(vec3(0.0f, 3.0f, 0.0f), WORLD);
		cameraObject->rotateTo(sphereObject2->getPosition(WORLD) - cameraObject->getPosition(WORLD), WORLD);

		std::shared_ptr<CameraComponent> cam = std::make_shared<CameraComponent>();
		cam->setViewPort(0.0f, 0.0f, 1.0f, 1.0f);
		cameraObject->addComponent(cam);

		// cameraObject->addComponent(std::make_shared< ModelMakerComponent>(shaderProgram, "Assets/Dinosaur/Trex.obj"));

	}
};

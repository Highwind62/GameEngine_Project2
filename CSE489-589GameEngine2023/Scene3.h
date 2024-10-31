#pragma once

#include "GameEngine.h"

class Scene3 : public Game
{

	void loadScene() override
	{
		// Set the window title
		glfwSetWindowTitle(renderWindow, "Scene 3");

		// Set the clear color
		glClearColor(YELLOW_RGBA.r, YELLOW_RGBA.g, YELLOW_RGBA.b, YELLOW_RGBA.a);

		// Build shader program
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/vertexShader.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/fragmentShader.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		// Build the shader program
		GLuint shaderProgram = BuildShaderProgram(shaders);

		// Make the shader program "current" Not needed if transformations are stored in uniform buffers
		//glUseProgram(shaderProgram);

		// Set up uniform blocks
		SharedTransformations::setUniformBlockForShader(shaderProgram);
		SharedMaterials::setUniformBlockForShader(shaderProgram);
		SharedLighting::setUniformBlockForShader(shaderProgram);

		// ****** boxGameObject *********

		GameObjectPtr boxGameObject = std::make_shared<GameObject>();
		this->addChildGameObject(boxGameObject);
		boxGameObject->setPosition(vec3(0.0f, -5.0f, 0.0f), WORLD);

		Material boxMat;

		boxMat.setDiffuseTexture(Texture::GetTexture("Textures/wood.png")->getTextureObject());
		std::shared_ptr<BoxMeshComponent> boxMesh = std::make_shared<BoxMeshComponent>(shaderProgram, boxMat,  100.0f, 1.0f, 100.0f);

		boxGameObject->addComponent(boxMesh);
		boxGameObject->gameObjectName = "box - STATIONARY";
	
		//// ****** dinoGameObject *********

		GameObjectPtr dinoObject = std::make_shared<GameObject>();
		this->addChildGameObject(dinoObject);
		std::shared_ptr<ModelMeshComponent> dino = std::make_shared<ModelMeshComponent>("Assets/Dinosaur/Trex.obj", shaderProgram);
		dinoObject->addComponent(dino);
		dinoObject->setPosition(vec3(4.0f, -3.0f, -2.0f), WORLD);
		dinoObject->setRotation(glm::rotate(-PI/6.0f, UNIT_Y_V3), WORLD);

		GameObjectPtr jetObject = std::make_shared<GameObject>();
		this->addChildGameObject(jetObject);

		std::shared_ptr <MeshComponent> jet = make_shared<ModelMeshComponent>("Assets/jet_models/F-15C_Eagle.obj", shaderProgram);
		jetObject->addComponent(jet);

		jetObject->setPosition(vec3(-4.0f, -3.0f, 0.0f), WORLD);
		jetObject->setRotation(glm::rotate(PI, UNIT_Y_V3), WORLD);

		// ****** Directional light *********

		GameObjectPtr directionaLightGO = std::make_shared<GameObject>();;
		addChildGameObject(directionaLightGO);
		directionaLightGO->rotateTo(vec3(-1.0f, -1.0f, -1.0f), WORLD);

		std::shared_ptr<DirectionalLightComponent> dlc =
			std::make_shared<DirectionalLightComponent>(GLFW_KEY_D);

		dlc->setAmbientColor(vec3(0.0f, 0.0f, 1.0f));
		dlc->setDiffuseColor(vec3(0.0f, 0.0f, 1.0f));
		dlc->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));
		dlc->setEnable(true);
		directionaLightGO->addComponent(dlc);

		// ****** Positional light *********
		GameObjectPtr positionalLightGO = std::make_shared<GameObject>();
		addChildGameObject(positionalLightGO);
		positionalLightGO->setPosition(jetObject->getPosition(), WORLD);

		std::shared_ptr<PositionalLightComponent> plc = 
			std::make_shared<PositionalLightComponent>(GLFW_KEY_P);
		plc->setAmbientColor(vec3(0.0f, 1.0f, 0.0f));
		plc->setDiffuseColor(vec3(0.0f, 1.0f, 0.0f));
		plc->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));
		plc->setAttenuationFactors(1.0f, 0.1f, 0.01f);
		plc->setEnable(true);
		positionalLightGO->addComponent(plc);

		// ****** Spot light *********
		GameObjectPtr spotLightGO = std::make_shared<GameObject>();
		addChildGameObject(spotLightGO);
		vec3 jetPosition = jetObject->getPosition();
		spotLightGO->setPosition(jetPosition + vec3(0.0f, 15.0f, 0.0f), WORLD);
		vec3 spotlightPosition = spotLightGO->getPosition();
		vec3 directionToJet = glm::normalize(jetPosition - spotlightPosition);
		spotLightGO->rotateTo(directionToJet, WORLD);

		std::shared_ptr<SpotLightComponent> slc = 
			std::make_shared<SpotLightComponent>(GLFW_KEY_S);
		slc->setCutoffAngleInDegrees(25.0f);
		slc->setAmbientColor(vec3(1.0f, 0.0f, 0.0f));
		slc->setDiffuseColor(vec3(1.0f, 0.0f, 0.0f));
		slc->setSpecularColor(vec3(1.0f, 1.0f, 1.0f));
		slc->setAttenuationFactors(1.0f, 0.1f, 0.01f);
		slc->setEnable(true);
		spotLightGO->addComponent(slc);

		// Camera 1
		GameObjectPtr cameraGO = std::make_shared<GameObject>();
		addChildGameObject(cameraGO);

		cameraGO->setPosition(vec3(10.0f, 0.0f, 40.0f), WORLD);
		cameraGO->rotateTo(jetObject->getPosition() - cameraGO->getPosition(), WORLD);

		std::shared_ptr<CameraComponent> camera = make_shared<CameraComponent>();
		camera->setCameraClearColor(YELLOW_RGBA);
		cameraGO->addComponent(camera);

	} // end loadScene
};
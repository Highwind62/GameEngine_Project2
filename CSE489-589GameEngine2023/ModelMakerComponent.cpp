#include "ModelMakerComponent.h"
#include "RemoveComponent.h"
#include "RigidBodyComponent.h"

static const bool VERBOSE = true;

ModelMakerComponent::ModelMakerComponent(GLuint shaderProgram, string pathAndFileName)
	: pathAndFileName(pathAndFileName), shaderProgram(shaderProgram)
{

}

ModelMakerComponent::~ModelMakerComponent()
{
}

void ModelMakerComponent::initialize()

{
	// Add a "paused" game object with an a mesh attached (will not be rendered)
	// to avoid delays in loading at run time.

	pausedObject = std::make_shared<GameObject>();
	pausedObject->setState(PAUSED);
	this->owningGameObject->addChildGameObject(pausedObject);

	std::shared_ptr<ModelMeshComponent> baseMesh = std::make_shared<ModelMeshComponent>(this->pathAndFileName, this->shaderProgram);
	pausedObject->addComponent(baseMesh);
}



void ModelMakerComponent::processInput()
{
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_J) && j_key_up == true) {

		GameObjectPtr gameObject = std::make_shared<GameObject>();

		owningGameObject->addChildGameObject(gameObject);

		gameObject->setPosition(owningGameObject->getFowardDirection(LOCAL) * 20.0f, LOCAL);
		gameObject->addComponent(std::make_shared<RemoveComponent>(10.0));
		
		// Create empty game object to rotate the model to face -Z i.e. "forward"
		// Only necessary with some models
		GameObjectPtr emptyGameObject = std::make_shared<GameObject>();

		gameObject->addChildGameObject(emptyGameObject);
		emptyGameObject->setRotation(glm::rotate(PI, UNIT_Y_V3), LOCAL);

		std::shared_ptr<ModelMeshComponent> model = std::make_shared<ModelMeshComponent>(this->pathAndFileName, this->shaderProgram);
		emptyGameObject->addComponent(model);
		std::shared_ptr<RigidBodyComponent> rgb = std::make_shared<RigidBodyComponent>(model, DYNAMIC);

		emptyGameObject->addComponent(rgb);
		rgb->setVelocity(owningGameObject->getFowardDirection(WORLD) * 100.0f);

		emptyGameObject->gameObjectName = "projectile";

		j_key_up = false;
	}
	if (!glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_J)) {

		j_key_up = true;
	}

}
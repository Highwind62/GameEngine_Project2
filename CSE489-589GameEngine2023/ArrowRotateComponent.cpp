#include "ArrowRotateComponent.h"


static const bool VERBOSE = false;

ArrowRotateComponent::ArrowRotateComponent(float rotationRateRadians, int updateOrder)
	: Component(updateOrder), rotationRate(rotationRateRadians)
{}


ArrowRotateComponent::~ArrowRotateComponent()
{
	if (VERBOSE) cout << "ArrowRotateComponent destructor called " << endl;

}

void ArrowRotateComponent::processInput()
{
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS) {
		RotX = -this->rotationRate;
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS) {
		RotX = this->rotationRate;
	}
	else {
		RotX = 0.0f;
	}
	
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
		RotY = this->rotationRate;
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS) {
		RotY = -this->rotationRate;
	}
	else {
		RotY = 0.0f;
	}

}

void ArrowRotateComponent::update(const float& deltaTime)
{
	mat4 gameObjectRotation = owningGameObject->getRotation();

	gameObjectRotation = glm::rotate( RotY * deltaTime, UNIT_Y_V3)  * glm::rotate(RotX * deltaTime, UNIT_X_V3 ) * gameObjectRotation;

	owningGameObject->setRotation( gameObjectRotation);


}
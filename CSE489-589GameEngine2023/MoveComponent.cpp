#include "MoveComponent.h"

MoveComponent::MoveComponent(int updateOrder)
	:Component(updateOrder)
{
	componentType = MOVE;
}

void MoveComponent::update(const float& deltaTime)
{
	switch (xyz) {

		case 1:
			rotAngleX += deltaTime * glm::radians(angularRate);
			break;
		case 2:
			rotAngleY += deltaTime * glm::radians(angularRate);
			break;
		case 3:
			rotAngleZ += deltaTime * glm::radians(angularRate);
			break;

	}

	mat4 rotate = glm::rotate(rotAngleZ, UNIT_Z_V3) * 
				  glm::rotate(rotAngleY, UNIT_Y_V3) * 
				  glm::rotate(rotAngleX, UNIT_X_V3);

	owningGameObject->setRotation(rotate);

	vec3 currentPos = owningGameObject->getPosition();

	currentPos += deltaTime * speed * movementVector;

	owningGameObject->setPosition(currentPos);

}

void MoveComponent::processInput() 
{
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_X)) {

		xyz = 1;
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Y)) {

		xyz = 2;
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Z)) {

		xyz = 3;
	} if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S)) {

		xyz = 0;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_R)) {

		rotAngleX = 0.0f;
		rotAngleY = 0.0f;
		rotAngleZ = 0.0f;
		xyz = 0;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP)) {

		movementVector = UNIT_Y_V3;
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN)) {

		movementVector = -UNIT_Y_V3;
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT)) {

		movementVector = UNIT_X_V3;
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT)) {

		movementVector = -UNIT_X_V3;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q) && Q_KEY_DOWN == false) {

		speed += 0.1f;
		Q_KEY_DOWN = true;
	}
	else {

		Q_KEY_DOWN = false;
	}


	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) && A_KEY_DOWN == false) {

		speed += 0.1f;
		A_KEY_DOWN = true;
	}
	else {

		A_KEY_DOWN = false;
	}

}
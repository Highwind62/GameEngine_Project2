#include "SpinComponent.h"


SpinComponent::SpinComponent(vec3 axis, float rotRateDegrees)
	: axis(axis), rotationRateRadians(glm::radians(rotRateDegrees)) 
{
	componentType = MOVE;

}

void SpinComponent::update(const float& deltaTime)
{
	mat4 rot = owningGameObject->getRotation(LOCAL);

	owningGameObject->setRotation(rot * glm::rotate(deltaTime * rotationRateRadians, axis));

}
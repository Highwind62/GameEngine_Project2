#include "MoveForwardComponent.h"


MoveForwardComponent::MoveForwardComponent(float speedUnitsPerSec, int updateOrder)
	: Component(updateOrder), speedUnitsPerSec(speedUnitsPerSec)
{

	componentType = MOVE;

}






void MoveForwardComponent::update(const float& deltaTime)
{
	vec3 fwd = owningGameObject->getFowardDirection(WORLD);
	vec3 pos = owningGameObject->getPosition(WORLD);

	owningGameObject->setPosition(pos + speedUnitsPerSec * fwd * deltaTime, WORLD);


}
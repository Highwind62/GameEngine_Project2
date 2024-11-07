#include "RemoveComponent.h"

static const bool VERBOSE = false;

RemoveComponent::RemoveComponent(double lifeSpan, int updateOrder)
	: Component(updateOrder),  lifeSpan(lifeSpan)
{}



RemoveComponent::~RemoveComponent()
{
	if (VERBOSE) cout << "RemoveComponent destructor called " << endl;

	
}

void RemoveComponent::update(const float & deltaTime)
{
	age += deltaTime;

	if (age > lifeSpan) {

		if (VERBOSE) cout << "Removing Game Object " << endl;

		this->owningGameObject->removeAndDelete();
	}

}

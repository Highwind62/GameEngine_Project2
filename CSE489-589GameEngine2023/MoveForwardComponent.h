#pragma once
#include "Component.h"
class MoveForwardComponent : public Component
{
public:

	MoveForwardComponent(float speedUnitsPerSec, int updateOrder = 100);

	virtual void update(const float& deltaTime) override;

protected:

	float speedUnitsPerSec = 0.0f;


};


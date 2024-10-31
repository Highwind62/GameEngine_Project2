#pragma once
#include "Component.h"

class SpinComponent : public Component
{
public:

	SpinComponent(vec3 axis = UNIT_Y_V3, float rotRateDegrees = 30.0f);

	virtual void update(const float& deltaTime);

protected:

	vec3 axis;

	float rotationRateRadians;

};


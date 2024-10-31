#pragma once
#include "Component.h"
class ArrowRotateComponent : public Component
{
public:

	ArrowRotateComponent(float rotationRateRadians = glm::radians(5.0f), int updateOrder = 100);

	virtual ~ArrowRotateComponent();

	virtual void processInput() override;

	virtual void update(const float& deltaTime) override;

protected:

	float RotX = 0.0f;
	float RotY = 0.0f;

	float rotationRate = 0.0f;
};


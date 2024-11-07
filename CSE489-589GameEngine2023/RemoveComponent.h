#pragma once
#include "Component.h"

class RemoveComponent : public Component
{
public:
	RemoveComponent(double lifeSpan = 10.0f, int  updateOrder = 100);

	virtual ~RemoveComponent();

	virtual void update(const float & deltaTime) override;

protected:

	double lifeSpan = 10.0f;
    double age = 0.0;
};


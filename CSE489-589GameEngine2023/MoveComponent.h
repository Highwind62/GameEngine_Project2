#pragma once
#include "Component.h"

class MoveComponent :
    public Component
{
public:

	MoveComponent(int updateOrder = 100);

	virtual void update(const float& deltaTime) override;

	virtual void processInput();

protected:

	float rotAngleX = 0.0f;
	float rotAngleY = 0.0f;
	float rotAngleZ = 0.0f;

	int xyz = 0; // 1 for x, 2 for y, 3 for z

	vec3 axis = UNIT_X_V3;

	vec3 movementVector = ZERO_V3;

	float speed = 1.0f;

	float angularRate = 30.0f;

	bool Q_KEY_DOWN = false;
	bool A_KEY_DOWN = false;


};


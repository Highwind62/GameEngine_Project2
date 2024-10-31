#pragma once
#include "WaypointComponent.h"

class SteeringComponent : public WaypointComponent
{
public:
	SteeringComponent(std::vector<std::weak_ptr<class GameObject>> waypoints, vec3 velocity = vec3(20, 0, 0));

	virtual void update(const float& deltaTime) override;

	float currentRoll = 0;

}; // end SteeringComponent class



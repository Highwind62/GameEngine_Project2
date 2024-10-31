#pragma once
#include "LightComponent.h"

class DirectionalLightComponent : public LightComponent
{

public:

	DirectionalLightComponent(int controlKey) : LightComponent(controlKey) {}

	/**
	 * @fn	virtual void DirectionalLightComponent::update(float deltaTime) override;
	 *
	 * @brief	Updates the direction (light vector) of the directional light
	 *
	 * @param 	deltaTime	The delta time.
	 */
	virtual void update(const float& deltaTime) override;


};


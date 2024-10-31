#pragma once
#include "LightComponent.h"

class PositionalLightComponent : public LightComponent
{
public:

	PositionalLightComponent(int controlKey) : LightComponent(controlKey) {}

	/**
	 * @fn	virtual void PositionLightComponent::update(float deltaTime) override;
	 *
	 * @brief	Updates the position of the point light.
	 *
	 * @param 	deltaTime	The delta time.
	 */
	virtual void update(const float& deltaTime) override;

	/**
	 * @fn	void PositionalLightComponent::setAttenuationFactors(float constant = 1.0f, float linear = 2.0f, float quadratic = 2.0f);
	 *
	 * @brief	Sets attenuation factors for a light with a position.
	 *
	 * @param 	constant 	(Optional) The constant attentuation factor.
	 * @param 	linear   	(Optional) The linear attenuation factor.
	 * @param 	quadratic	(Optional) The quadratic attentuation factor.
	 */
	void setAttenuationFactors(float constant = 1.0f, float linear = 2.0f, float quadratic = 2.0f);
};


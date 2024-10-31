#pragma once

#include "PositionalLightComponent.h"

class SpotLightComponent : public PositionalLightComponent
{
public:

	SpotLightComponent(int controlKey) : PositionalLightComponent(controlKey) {}

	/**
	 * @fn	virtual void SpotLightComponent::update(float deltaTime) override;
	 *
	 * @brief	Updates the position and direction of the spot light.
	 *
	 * @param 	deltaTime	The delta time.
	 */
	virtual void update(const float& deltaTime) override;

	/**
	 * @fn	virtual void SpotLightComponent::initialize( )override;
	 *
	 * @brief	Initializes the spot light by setting isSpot to true
	 */
	virtual void initialize( )override;

	/**
	 * @fn	void SpotLightComponent::setCutoffAngleInDegrees(float cutOffInDegrees);
	 *
	 * @brief	Sets cutoff angle in degrees.
	 *
	 * @param 	cutOffInDegrees	The cut off in degrees.
	 */
	void setCutoffAngleInDegrees(float cutOffInDegrees);

};


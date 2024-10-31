#pragma once

#include "SoundEngine.h"

#include "Component.h"
#include "MathLibsConstsFuncs.h"

using namespace constants_and_types;

class SoundBaseComponent : public Component
{
	public:

	SoundBaseComponent( int updateOrder = 500 );

	/**
	 * @fn	virtual void SoundBaseComponent::initialize() override;
	 *
	 * @brief	Initializes the position of the component for velocity
	 * 			calculations.
	 */
	virtual void initialize() override;

	/**
	 * @fn	virtual void SoundBaseComponent::update( float deltaTime ) override;
	 *
	 * @brief	Performs update calculations are are common to all SoundBase sub-classes i.e. the
	 * 			data members of this class based on the world transformation of the SceneNode.
	 *
	 * @param	deltaTime	The delta time.
	 */
	virtual void update(const float& deltaTime) override;

	protected:

	vec3 soundPosition = ZERO_V3;
	vec3 soundVelocity = ZERO_V3;
	vec3 soundForward = ZERO_V3;
	vec3 soundUp = ZERO_V3;

	vec3 lastPosition = ZERO_V3;

};


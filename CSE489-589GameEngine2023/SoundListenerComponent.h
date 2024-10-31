#pragma once
#include "SoundBaseComponent.h"

class SoundListenerComponent : public SoundBaseComponent
{
public:

	SoundListenerComponent( int updateOrder = 500);

//	virtual void initialize() override;

	/**
	 * @fn	virtual void SoundListenerComponent::update( float deltaTime ) override;
	 *
	 * @brief	Updates the position, velocity, and orientation of the listener
	 * 			
	 *			Calls:
	 *			SoundBaseComponent::update( deltaTime );
	 *			SoundEngine::check_al_errors();
	 *
	 * @param	deltaTime	The delta time.
	 */
	virtual void update(const float & deltaTime) override;
};


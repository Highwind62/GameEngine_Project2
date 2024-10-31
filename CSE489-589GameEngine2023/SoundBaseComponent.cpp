#include "SoundBaseComponent.h"

SoundBaseComponent::SoundBaseComponent( int updateOrder)
	:Component( updateOrder )
{
}

void SoundBaseComponent::initialize()
{
	// Initialize last position for used in velocity calculations. The
	// velocity magnitude will be zero for one frame.
	lastPosition = this->owningGameObject->getPosition(WORLD);

	// Call the appropirate subclass update method which in turn
	// should call SoundBaseComponent::update before doing a component
	// specific update.
	update(0.0f);

} // end initialize

void SoundBaseComponent::update(const float& deltaTime)
{
	// Get current position and the forward and up directions
	soundPosition = this->owningGameObject->getPosition( WORLD );
	soundForward = this->owningGameObject->getFowardDirection( WORLD );
	soundUp = this->owningGameObject->getUpDirection( WORLD );

	// Calculate the velocity 
	if( deltaTime > 0.0f ) {

		soundVelocity = (soundPosition - lastPosition) / deltaTime;
	}

	// Save position for next update
	lastPosition = soundPosition;

} // end soundUpdate


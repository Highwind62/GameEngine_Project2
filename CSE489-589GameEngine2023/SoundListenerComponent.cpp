#include "SoundListenerComponent.h"

static const bool VERBOSE = false;

SoundListenerComponent::SoundListenerComponent( int updateOrder )
	:SoundBaseComponent( updateOrder )
{

}

void SoundListenerComponent::update(const float& deltaTime)
{
	// Call the super class update method to calculate the 
	//World position, velocity, and orientation 
	SoundBaseComponent::update(deltaTime);

	// Set the position, velocity and orientation of the listner
	ALfloat listenOrientation[] = { soundForward.x, soundForward.y, soundForward.z,
									soundUp.x, soundUp.y, soundUp.z };
	alListenerfv(AL_ORIENTATION, listenOrientation);
	alListener3f(AL_POSITION, soundPosition.x, soundPosition.y, soundPosition.z);
	alListener3f(AL_VELOCITY, soundVelocity.x, soundVelocity.y, soundVelocity.z);
	
	// check for errors
	SoundEngine::check_al_errors();

	if (VERBOSE) {

		cout << "listener position " << soundPosition << endl;

	}

} // end update

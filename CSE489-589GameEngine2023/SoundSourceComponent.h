#pragma once
#include "SoundBaseComponent.h"
#include <string>

/**
 * @class	SoundSourceComponent
 *
 * @brief	
 * 			
 */
class SoundSourceComponent : public SoundBaseComponent
{
public:

	/**
	 * @fn	SoundSourceComponent::SoundSourceComponent(std::string soundFileName, float refDistance, float rollOffFactor, float maxDistance, int updateOrder);
	 *
	 * @brief	Constructor
	 *
	 * @param 	soundFileName	Filename of the sound file. (Must be a mono channel wave)
	 * @param 	refDistance  	Distance at which gain is one.
	 * @param 	rollOffFactor	Controls falloff with distance. Higher number = faster falloff
	 * @param 	maxDistance  	Distance at which sounds stops attenuating and gain is constant.
	 * @param 	updateOrder  	The update order.
	 */
	SoundSourceComponent(std::string soundFileName, float refDistance = 4.0f, 
		float rollOffFactor = 2.0f, float maxDistance = 50.0f, int updateOrder = 100);

	/**
	 * @fn	SoundSourceComponent::~SoundSourceComponent();
	 *
	 * @brief	Destructor - Deletes the source and buffer.
	 * 			
	 */
	~SoundSourceComponent();

	/**
	 * @fn	virtual void SoundSourceComponent::initialize() override;
	 *
	 * @brief	Initializes this object by setting the World position
	 * 			of the sound source. Calls update method.
	 */
	//virtual void initialize() override;

	/**
	 * @fn	virtual void SoundSourceComponent::update( float deltaTime ) override;
	 *
	 * @brief	Updates the position, velocity, and orientation of the source
	 * 			
	 *			Calls:
	 *			SoundBaseComponent::update( deltaTime );
	 *			SoundEngine::check_al_errors();
	 *
	 * @param	deltaTime	The delta time.
	 */
	virtual void update(const float& deltaTime) override;

	/**
	 * @fn	void SoundSourceComponent::play( bool loop = false );
	 *
	 * @brief	Start playing the sound.
	 *
	 */
	void play();

	/**
	 * @fn	void SoundSourceComponent::stop();
	 *
	 * @brief	Stop playing the sound.
	 */
	void stop();

	/**
	 * @fn	void SoundSourceComponent::pause();
	 *
	 * @brief	Pause the sound.
	 */
	void pause();

	/**
	 * @fn	void SoundSourceComponent::continuePlaying();
	 *
	 * @brief	Continue playing a paused sound.
	 */
	void continuePlaying();

	/**
	 * @fn	bool SoundSourceComponent::isPlaying( );
	 *
	 * @brief	Query if this object is playing
	 *
	 * @returns	True if playing, false if not.
	 */
	bool isPlaying( );

	/**
	 * @fn	void SoundSourceComponent::setGain(float volume);
	 *
	 * @brief	Set the volume of the sound. Volume levels above
	 * 			1 amplify the signal. 
	 *
	 * @param 	volume	- volume level for the sound.
	 */
	void setGain(float volume);

	/**
	 * @fn	float SoundSourceComponent::getGain();
	 *
	 * @brief	Returns the current volume level for the sound.
	 * 			@returne - volume level for the sound.
	 *
	 * @returns	The volume.
	 */
	float getGain();

	/**
	 * @fn	bool SoundSourceComponent::isPaused();
	 *
	 * @brief	Query if this sound is paused
	 *
	 * @returns	True if paused, false if not.
	 */
	bool isPaused();

	/**
	 * @fn	void SoundSourceComponent::setLooping(bool loopingOn);
	 *
	 * @brief	Set this sound to loop.
	 *
	 * @param	loopingOn	- true to loop the sound. false to play it once.
	 */
	void setLooping(bool loopingOn);

	/**
	 * @fn	void SoundSourceComponent::setReferenceDistance(float refDistance);
	 *
	 * @brief	Sets reference distance. Reference distance determines the distance at which
	 * 			gain will be 1 (all attenuation models).
	 *
	 * @param 	refDistance	The reference distance.
	 */
	void setReferenceDistance(float refDistance);

	/**
	 * @fn	void SoundSourceComponent::setRollOffFactor(float rollOffFactor);
	 *
	 * @brief	Sets roll off factor. 
	 * 			Under AL_EXPONENT_DISTANCE, AL_EXPONENT_DISTANCE_CLAMPED,
	 * 				AL_INVERSE_DISTANCE &amp; AL_INVERSE_DISTANCE_CLAMPED
	 * 			Roll off factor determines how quickly sound falls off.
	 * 				Higher values makes it fall off more quickly. Decrease to allow 
	 * 				the sounds to be heard from far away.
	 * 			Under AL_LINEAR_DISTANCE &amp; AL_LINEAR_DISTANCE_CLAMPED
	 * 				If roll off factor is 1, the reference distance is the distance from the source
	 * 				where the gain is 1.
	 *
	 * @param 	rollOffFactor	The roll off factor.
	 */
	void setRollOffFactor(float rollOffFactor);

	/**
	 * @fn	void SoundSourceComponent::setMaxDistance(float maxDistance);
	 *
	 * @brief	Sets maximum distance. 
	 * 			AL_EXPONENT_DISTANCE_CLAMPED & AL_INVERSE_DISTANCE_CLAMPED
	 * 				Further than the Max distance sound is no longer attenuated
	 *			AL_LINEAR_DISTANCE 
	 *				Gain is zero at the Max Distance
	 * @param 	maxDistance	The maximum distance.
	 */
	void setMaxDistance(float maxDistance);

protected:

	ALuint buffer;
	ALuint source;
};


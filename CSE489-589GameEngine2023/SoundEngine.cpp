#include "SoundEngine.h"

//https://indiegamedev.net/2020/02/15/the-complete-guide-to-openal-with-c-part-1-playing-a-sound/

// The OpenAL Utility Toolkit (ALUT)
//http://distro.ibiblio.org/rootlinux/rootlinux-ports/more/freealut/freealut-1.1.0/doc/alut.html

static const bool VERBOSE = false;

// ********** Static data members ************************
//ALCdevice* SoundEngine::device;

//ALCcontext* SoundEngine::context;

// ********** Helper Functions ************************
static void list_audio_devices(const ALCchar* devices);
static bool check_alc_errors(ALCdevice* device);

bool SoundEngine::Init(ALint distanceModel, ALfloat dopplerFactor)
{
	if (VERBOSE) cout << "Init called" << endl;

	alutInit(0, 0);
	check_alut_errors();

	//device = alcOpenDevice(NULL);

	//check_alc_errors(device);

	//if (!device) {
	//	// handle errors
	//	std::cerr << "openAL device error" << endl;
	//	return false;
	//}
	//else {

	//	if (VERBOSE) cout << "device created" << endl;
	//}

	//ALboolean enumeration;

	//enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	//if (enumeration == AL_FALSE) {

	//	std::cerr << "alc enumeration not supported" << endl;
	//}

	//else if (VERBOSE) {

	//	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
	//}

	//context = alcCreateContext(device, NULL);

	//check_alc_errors(device);

	//if (!alcMakeContextCurrent(context)) {

	//	std::cerr << "error: context not created" << endl;
	//	return false;
	//}
	//else {

	//	if (VERBOSE) cout << "openAL context created" << endl;
	//}

	//AL_EXPONENT_DISTANCE
	//AL_EXPONENT_DISTANCE_CLAMPED
	// Source Properties
	// Reference distance determines when the gain will be 1 (all models)
	// Roll off factor how quickly sound falls off. 
	//		Higher makes it fall off more quickly. 
	//		Decrease to allow the sounds to be heard from far away.
	// Clamped 
	//		Closer than reference distance gain is clamped to 1
	//		Further than the Max distance sound is no longer attenuated

	//AL_INVERSE_DISTANCE
	//AL_INVERSE_DISTANCE_CLAMPED
	// Similar to exponential with different curve shape that is not as steep

	//AL_LINEAR_DISTANCE); // Not as realistic
	//AL_LINEAR_DISTANCE_CLAMPED
	// If roll off factor is 1, the reference distance is the 
	// distance from the source where the gain is 1.
	// Gain is zero at the Max Distance
	// Clamped 
	//		Closer than reference distance gain is clamped to 1
	
	alDistanceModel( distanceModel );
	check_al_errors();

	alDopplerFactor( dopplerFactor );
	check_al_errors();

	return true;

} // end SoundEngineInit


void SoundEngine::Update(const float & deltaTime)
{
	// Not necessary with OpenAL Other sound APIs require and update to 
	// take into account the changes in the positions of sound sources
	// and the listeners.
}

void SoundEngine::Stop()
{
	//alcMakeContextCurrent(0);
	//alcDestroyContext(context);
	//alcCloseDevice(device);

	alutExit();
	check_alut_errors();

}

static void list_audio_devices(const ALCchar* devices)
{
	const ALCchar* device = devices, * next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}


bool SoundEngine::check_al_errors()
{
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		switch (error)
		{
		case AL_INVALID_NAME:
			std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
			break;
		case AL_INVALID_ENUM:
			std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
			break;
		case AL_INVALID_VALUE:
			std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
			break;
		case AL_INVALID_OPERATION:
			std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
			break;
		case AL_OUT_OF_MEMORY:
			std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
			break;
		default:
			std::cerr << "UNKNOWN AL ERROR: " << error;
		}
		std::cerr << std::endl;
		return false;
	}
	return true;
}


bool check_alc_errors(ALCdevice* device)
{
	ALCenum error = alcGetError(device);
	if (error != ALC_NO_ERROR)
	{
		switch (error)
		{
		case ALC_INVALID_VALUE:
			std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
			break;
		case ALC_INVALID_DEVICE:
			std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
			break;
		case ALC_INVALID_CONTEXT:
			std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
			break;
		case ALC_INVALID_ENUM:
			std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
			break;
		case ALC_OUT_OF_MEMORY:
			std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
			break;
		default:
			std::cerr << "UNKNOWN ALC ERROR: " << error;
		}
		std::cerr << std::endl;
		return false;
	}
	return true;
}


bool SoundEngine::check_alut_errors() 
{
	ALenum error = alutGetError();

	if (error != ALUT_ERROR_NO_ERROR)
	{
		switch (error)
		{
		case ALUT_ERROR_OUT_OF_MEMORY:
			std::cerr << "ALUT ran out of memory.";
			break;
		case ALUT_ERROR_INVALID_ENUM:
			std::cerr << "ALUT was given an invalid enumeration token";
			break;
		case ALUT_ERROR_INVALID_VALUE:
			std::cerr << "ALUT was given an invalid value";
			break;
		case ALUT_ERROR_INVALID_OPERATION:
			std::cerr << "The operation is invalid in the current ALUT state";
			break;
		case ALUT_ERROR_NO_CURRENT_CONTEXT:
			std::cerr << "There is no current AL context";
			break;
		case ALUT_ERROR_AL_ERROR_ON_ENTRY:
			std::cerr << "There was already an AL error on entry to an ALUT function";
			break;
		case ALUT_ERROR_ALC_ERROR_ON_ENTRY:
			std::cerr << "There was already an ALC error on entry to an ALUT function";
			break;
		case ALUT_ERROR_OPEN_DEVICE:
			std::cerr << "There was an error opening the ALC device";
			break;
		case ALUT_ERROR_CLOSE_DEVICE:
			std::cerr << "There was an error closing the ALC device";
			break;
		case ALUT_ERROR_CREATE_CONTEXT:
			std::cerr << "There was an error creating an ALC context";
			break;
		case ALUT_ERROR_MAKE_CONTEXT_CURRENT:
			std::cerr << "Could not change the current ALC context";
			break;
		case ALUT_ERROR_DESTROY_CONTEXT:
			std::cerr << "There was an error destroying the ALC context";
			break;
		case ALUT_ERROR_GEN_BUFFERS:
			std::cerr << "There was an error generating an AL buffer";
			break;
		case ALUT_ERROR_BUFFER_DATA:
			std::cerr << "There was an error passing buffer data to A";
			break;
		case ALUT_ERROR_IO_ERROR:
			std::cerr << "I/O error, consult errno for more details";
			break;
		case ALUT_ERROR_UNSUPPORTED_FILE_TYPE:
			std::cerr << "Unsupported file type";
			break;
		case ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE:
			std::cerr << "Unsupported mode within an otherwise usable file type";
			break;
		case ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA:
			std::cerr << "The sound data was corrupt or truncated";
			break;
		default:
			std::cerr << "UNKNOWN ALUT ERROR: " << error;
		}

		std::cerr << std::endl;
		return false;
	}

	return true;
}





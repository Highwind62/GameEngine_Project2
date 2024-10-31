#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static const bool VERBOSE = false;


// Map containing all textures that have been loaded
// (Static variables must be defined outside the declaration)
std::unordered_map<std::string, class Texture*> Texture::loadedTextures;

bool Texture::load(const std::string& fileName)
{	
	// Passing true to this function will cause it to output images the way OpenGL expects
	stbi_set_flip_vertically_on_load(true);
	
	int width = 0, height = 0, nrChannels;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);

	// Check bitmap parameters to determine is a valid image was loaded
	if (data == nullptr || width == 0 || height == 0) {
		std::cerr << "ERROR: Unable to load " << fileName << "!" << std::endl;
		exit(EXIT_FAILURE);
		//return false;
	}

	if (VERBOSE) cout << fileName.c_str() << " number of channels " << nrChannels << endl;

	glGenTextures(1, &this->textureID);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	if (nrChannels == 3) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	} else if (nrChannels == 4) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	}
	else {
		std::cerr << fileName << " has " << nrChannels << " channels!" << std::endl;
		return false;

	}

	if( VERBOSE ) std::cout << "Loaded: " << fileName.c_str() << " texture. width " << width << " height " << height << std::endl;

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	return true;

} // end load


void Texture::unload()
{
	// Remove the Texture object from the Map
	loadedTextures.erase(fileName);

	// Delete the texture object
	glDeleteTextures(1, &textureID);

} // end unload


Texture* Texture::GetTexture(const std::string& fileName)
{
	// Pointer to the texture to be loaded or retrieved.
	Texture* texturePtr = nullptr;

	// Search for the texture among those that were previously loaded
	auto iter = loadedTextures.find(fileName);

	// Check if the texture was previously loaded
	if (iter != loadedTextures.end()) {

		if (VERBOSE) std::cout << "Retrieving texture: " << fileName << std::endl;
		texturePtr = iter->second;
	}
	else {

		if (VERBOSE) std::cout << "Loading texture: " << fileName << std::endl;
		texturePtr = new Texture();

		texturePtr->fileName = fileName;

		// Load the texture
		if (texturePtr->load(fileName)) {

			// Add the loaded texture to those that were previously loaded
			loadedTextures.emplace(fileName, texturePtr);
		}
		else {
			delete texturePtr;
			texturePtr = nullptr;
		}
	}

	return texturePtr;

} // end GetTexture


void Texture::unloadTextures()
{
	// Destroy textures
	for (auto i : loadedTextures) {
		i.second->unload();
	}
	loadedTextures.clear();

} // end unloadTextures

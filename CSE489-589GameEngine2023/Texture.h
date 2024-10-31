#pragma once

#include <unordered_map>

#include "MathLibsConstsFuncs.h"

using namespace constants_and_types;

class Texture
{
public:

	/**
	 * @fn	static Texture* Texture::GetTexture(const std::string& fileName);
	 *
	 * @brief	Load a texture or retrieves it if it was loaded previously
	 *
	 * @param	fileName	Contains the relative path and the name of the file.
	 *
	 * @returns	Null if it fails, else a pointer to the texture.
	 */
	static Texture* GetTexture(const std::string& fileName);

	/**
	 * @fn	static void Texture::unloadTextures();
	 *
	 * @brief	Unloads ALL previously loaded textures.
	 */
	static void unloadTextures();

	/**
	 * @fn	int Texture::getWidth() const
	 *
	 * @brief	Gets the width in texels of a two dimensional texture
	 *
	 * @returns	The width.
	 */
	int getWidth() const { return width; }

	/**
	 * @fn	int Texture::getHeight() const
	 *
	 * @brief	Gets the height in texels of a two dimensional texture
	 *
	 * @returns	The height.
	 */
	int getHeight() const { return height; }

	/**
	 * @fn	unsigned int Texture::getTextureObject( ) const
	 *
	 * @brief	Gets unsigned integer identifier of the OpenGL texture object.
	 *
	 * @returns	The texture object.
	 */
	GLuint getTextureObject() const { return textureID; }

	/**
	 * @fn	void Texture::unload();
	 *
	 * @brief	Deletes the texture object associated with this
	 * 			texture. CURRENT IMPLEMENTATION DOES NOT ACCOUNT
	 * 			MULTIPLE USES OF THE SAME TEXTURE BY DIFFERENT
	 * 			SUB MESHES. UNLOADING IT HERE WILL REMOVE IT
	 * 			FOR ALL SUBMESHES.
	 */
	void unload();

protected:

	/**
	 * @fn	Texture::Texture()
	 *
	 * @brief	Default constructor. Protected so that is not possible to 
	 * 			create Texture objects without them being added to an unordered
	 * 			map containing all textures that have been loaded
	 */
	Texture() {}

	/**
	 * @fn	bool Texture::Load(const std::string& fileName);
	 *
	 * @brief	Loads a texture image from the specified file name and
	 * 			creates an associated texture object.
	 *
	 * @param	fileName	The name of the file containing the texture image.
	 *
	 * @returns	True if it succeeds, false if it fails.
	 */
	bool load(const std::string& fileName);


	/** @brief	OpenGL ID of this texture */
	unsigned int textureID = 0;

	/** @brief	Filename and relative path of the texture file */
	std::string  fileName;

	/** @brief	Width/height of the texture */
	int width = 0;
	int height = 0;

	/** @brief	Map of ALL textures that have been loaded. textures loaded */
	static std::unordered_map<std::string, class Texture*> loadedTextures;

};




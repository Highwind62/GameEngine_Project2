#include "BuildShaderProgram.h"
#include <cstdlib>

static const bool VERBOSE = false;

// Static variable definition. Holds all shader programs created usig
// BuildShaderProgram. Allows all the shader programs to be 
// deleted when the game ends.
static std::vector<GLuint> shaderProgramsCreated;

// Reads in the source code of a shader program.  
const GLchar* ReadShader(const char* filename)
{
	FILE* infile;
	fopen_s(&infile, filename, "rb");

	if (!infile) {

		std::cerr << "Unable to open file '" << filename << "'" << std::endl;

		return nullptr;
	}
	else {

		if ( VERBOSE ) std::cout << std::endl << "Reading '" << filename << "' shader source code." << std::endl;
	}

	fseek(infile, 0, SEEK_END);
	size_t len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar* source = new GLchar[len + (__int64)1];

	fread(source, (size_t)1, len, infile);
	fclose(infile);

	source[len] = 0;

	return const_cast<const GLchar*>(source);

} // end ReadShader


GLuint BuildShaderProgram(ShaderInfo* shaders)
{
	if (shaders == nullptr) { return 0; }

	// Creates an empty Shader Program object and returns an unsigned int by which
	// it can be referenced. Shader objects will be attached to the program
	// object.
	GLuint program = glCreateProgram();

	// Declare and array of structures to hold information particular shaders.
	ShaderInfo* entry = shaders;

	// Loop though all shaders specified in entry array until a GL_NONE
	// is encountered in the type field. All shaders will be attached
	// to the shader program.
	while (entry->type != GL_NONE) {

		// Creates an empty Shader object and returns an unsigned int by which
		// it can be referenced.  A shader object is used to maintain the 
		// source code strings that define a shader.
		GLuint shader = glCreateShader(entry->type);

		// Store the int ID for the shader in a ShaderInfo structure
		entry->shader = shader;

		// Read in the source code for a shader
		const GLchar* source = ReadShader(entry->filename);
		if (source == nullptr) {
			for (entry = shaders; entry->type != GL_NONE; ++entry) {

				// Release the name (ID) that was allocated.
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}

			return 0;
		}

		// Associate the shader source code with the Shader object
		glShaderSource(shader, 1, &source, nullptr);

		// Release the memory holding the character array into which
		// the shader source was read
		delete[] source;

		// Complie the shader source code
		glCompileShader(shader);

		// Determine if the shader compiled without errors.
		// "complied" will be set to GL_TRUE if compile operation 
		// is a success.
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {

			GLsizei len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + (__int64)1];
			glGetShaderInfoLog(shader, len, &len, log);
			std::cerr << "\nShader compilation failed: \n" << log << "\n" << std::endl;
			delete[] log;
			exit(EXIT_FAILURE);
		}
		else {

			if (VERBOSE) std::cout << entry->filename << " successfully compiled. " << std::endl;

		}

		// Associate the compiled shader with the shader program.
		// Shader functionality will not be available until it has be linked.
		glAttachShader(program, shader);

		++entry;
	}

	// Generates a complete shader program. All required shader objects
	// must be attached prior to linking.
	glLinkProgram(program);

	// Determine if the shader program successfully linked.
	// "linked" will be set to GL_TRUE if link is a success.
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
	
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + (__int64)1];
		glGetProgramInfoLog(program, len, &len, log);
		std::cerr << "\nShader linking failed: \n" << log << "\n" << std::endl;
		delete[] log;


		for (entry = shaders; entry->type != GL_NONE; ++entry) {
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}

		exit(EXIT_FAILURE);
	}
	else {

		if (VERBOSE) std::cout << std::endl << "Shader Program " << program << " successfully linked";

	}
	// Check whether the program can execute given the current pipeline state.
	glValidateProgram(program);
	GLint valid;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &valid);
	if (!valid) {

		GLsizei len;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &len);

		GLchar* log = new GLchar[len + (__int64)1];
		glGetProgramInfoLog(program, len, &len, log);
		std::cerr << "." << std::endl << "Shader program is invalid: " << log << std::endl;

		return 0;
	}
	else {

		if (VERBOSE) std::cout << " and is valid. " << std::endl << std::endl;
	}

	// Save a reference to the shader program so that is can
	// later be deleted.
	shaderProgramsCreated.push_back(program);

	return program;

} // end BuildShaderProgram


void deleteAllShaderPrograms()
{
	for (auto& shaderProgram : shaderProgramsCreated) {

		glDeleteProgram(shaderProgram);

	}

} // end deleteAllShaderPrograms




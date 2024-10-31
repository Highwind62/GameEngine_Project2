#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "MathLibsConstsFuncs.h"
#include "GameObject.h"

//The initial screen width when the game starts.
static const int initialScreenWidth = 1024;

//The initial screen height when the game starts.
static const int initialScreenHeight = 768;

// Desired maximum number of frames per second.
static const GLint FRAMES_PER_SECOND = 60;

// Interval in milliseconds between frames.
static const GLdouble FRAME_INTERVAL = 1.0 / FRAMES_PER_SECOND;

using namespace constants_and_types;

class Game : public GameObject
{

public:

	// Allow GameObject methods to access protected and private members of the 
	// this class
	friend class GameObject;

	/**
	 * @fn	Game::Game(std::string windowTitle = "CSE489/589");
	 *
	 * @brief	Constructor
	 *
	 * @param 	windowTitle	(Optional) title for the rendering window.
	 */
	Game(std::string windowTitle = "CSE489/589");

	/**
	 * @fn	void Game::runGame();
	 *
	 * @brief	Initializes the game. Starts and runs the game loop. Frees
	 * 			resources after the game has ended.
	 */
	void runGame();

	/**
	 * @fn	Game::~Game();
	 *
	 * @brief	Destructor. Deletes all dynamically allocated memory and releases
	 * 			all resources.
	 */
	~Game();

	/**
	 * @fn	void Game::window_close_callback(GLFWwindow* window);
	 *
	 * @brief	Callback, method that is called by a function when the window is 
	 * 			closed. Sets isRunning to false to stop the game loop.
	 *
	 * @param [in]	window	If non-null, the window being closed.
	 */
	void window_close_callback(GLFWwindow* window);

	/**
	 * @fn	void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height);
	 *
	 * @brief	Callback, method that is called by a function when the framebuffer
	 * 			is resized; usually the	result of the rendering window being resized.
	 *
	 * @param [in,out]	window	If non-null, the window containing the
	 * 							framebuffer.
	 * @param 		  	width 	The width in pixels of the framebuffer.
	 * @param 		  	height	The height in pixels of the framebuffer.
	 */
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	/**
	 * @fn	glm::ivec2 Game::getWindowDimensions();
	 *
	 * @brief	Gets both the width and height of the rendering window.
	 *
	 * @returns	The width [0] and the height [1] of the rendering window.
	 */
	glm::ivec2 getWindowDimensions();

	/**
	 * @fn	bool Game::gameIsInitialized()
	 *
	 * @brief	Determines if we can game is initialized and is 
	 * 			actively "running."
	 *
	 * @returns	True if it succeeds, false if it fails.
	 */
	bool gameIsRunning() { return isRunning; }

protected:

	/**
	 * @fn	virtual bool Game::initializeGame();
	 *
	 * @brief	Initializes graphics as well as sound and physics engines. Loads
	 * 			the initial scene and initializes all game objects.
	 *
	 * @returns	True if it succeeds, false if it fails.
	 */
	virtual bool initializeGame();

	/**
	 * @fn	bool Game::initializeRenderWindow();
	 *
	 * @brief	Initializes the rendering window
	 *
	 * @returns	True if it succeeds, false if it fails.
	 */
	bool initializeRenderWindow();

	/**
	 * @fn	void Game::bindCallBacks();
	 *
	 * @brief	used to bind all call back functions
	 */
	void bindCallBacks();

	/**
	 * @fn	bool Game::initializeGraphics();
	 *
	 * @brief	Initializes the rendering API
	 *
	 * @returns	True if it succeeds, false if it fails.
	 */
	bool initializeGraphics();

	/**
	 * @fn	virtual void Game::loadScene() = 0;
	 *
	 * @brief	Creates game objects and attaches components. Created game
	 * 			objects are added to the game.
	 */
	virtual void loadScene() = 0;

	/**
	 * @fn	void Game::gameLoop();
	 *
	 * @brief	Game loop. Repeatedly processes user input, updates all game
	 * 			objects, and renders the scene until isRunning is false and the
	 * 			game ends.
	 */
	void gameLoop();

	/**
	 * @fn	virtual void Game::processGameInput();
	 *
	 * @brief	Process the input
	 */
	virtual void processGameInput();

	/**
	 * @fn	virtual void Game::updateGame();
	 *
	 * @brief	Updates all game objects and the attached components.
	 */
	virtual void updateGame();

	/**
	 * @fn	void Game::renderScene();
	 *
	 * @brief	Renders the game scene.
	 */
	void renderScene();

	/**
	 * @fn	void Game::shutdown();
	 *
	 * @brief	Deletes all game objects in the game.
	 */
	void shutdown();

	/** @brief	True if the game is running (has been initialized), false if not */
	bool isRunning = false;

	/** @brief	Title for the window */
	std::string windowTitle;

	/** @brief	The rendering window */
	GLFWwindow* renderWindow = NULL;

	/** @brief	True to wire frame key was down on the last input input cycle */
	bool WireFrame_KeyDown = false;

}; // end game class

//********************* static function declarations *****************************************

/**
 * @fn	static void glfw_error_callback(int error, const char* description);
 *
 * @brief	Callback function (not method), that is called when there is a GLFW error
 *
 * @param 	error	   	The error.
 * @param 	description	The description.
 */
static void glfw_error_callback(int error, const char* description);

/**
 * @fn	static void displayOpenGlInfo(void);
 *
 * @brief	Displays an open gl information/
 */
static void displayOpenGlInfo(void);

/**
 * @fn	static void GLAPIENTRY openglMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
 *
 * @brief	Callback function (not method), that called when an OpenGL error occurs.
 *
 * @param 	source   	Source for the.
 * @param 	type	 	The type.
 * @param 	id		 	The identifier.
 * @param 	severity 	The severity. See
 * 						https://www.khronos.org/registry/OpenGL/extensions/KHR/KHR_debug.txt
 * 						for description.
 * @param 	length   	The length.
 * @param 	message  	The message.
 * @param 	userParam	The user parameter.
 *
 * @returns	A GLAPIENTRY.
 */
static void GLAPIENTRY openglMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam);

/**
 * @fn	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
 *
 * @brief	Callback function (not method), called when the framebuffer is resized (occurs when the
 * 			window is resized).
 *
 * @param [in,out]	window	If non-null, the window.
 * @param 		  	width 	The width of the framebuffer in pixels.
 * @param 		  	height	The height of the framebuffer in pixels.
 */
static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

/**
 * @fn	static void windowCloseCallback(GLFWwindow* window);
 *
 * @brief	Callback function (not method), called when the window is closed by the "X" in the
 * 			upper right hand window
 *
 * @param [in,out]	window	If non-null, the window.
 */
static void windowCloseCallback(GLFWwindow* window);

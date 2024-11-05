#include "GameEngine.h"
static const bool  VERBOSE = false;

#include "SoundEngine.h"
#include "PhysicsEngine.h"


//********************* Initialization Methods *****************************************

Game::Game(std::string windowTitle)
	: windowTitle(windowTitle)
{
	// Set the owning game to this game. The will be passed to all
	// other GameObjects when addChildGameObject is called.
	// This is where the shared pointer for the game is created.
	// This should only happen in one place. Otherwise there will be 
	// multiple different shared pointers to the same game object.
	this->OwningGame = this;

} // end Game Constructor

void Game::runGame()
{
	// The Game is the root of the scene graph. It has no parent.
	this->parent = nullptr;

	// Initialize the game
	bool success = initializeGame();

	// Enter the game loop
	if (success) {
		gameLoop();
	}

	// Free up resources
	shutdown();

} // end runGame

Game::~Game()
{
	if (VERBOSE) cout << "Game destructor " << endl;

} // end Game Destructor

bool Game::initializeGame()
{
	// Initialize the various libararies
	bool windowInit = initializeRenderWindow();
	bool graphicsInit = initializeGraphics();

	// Initialize the sound engine
	bool soundInit = SoundEngine::Init();

	// Initialize the physics engine
	bool physicsInit = PhysicsEngine::Init();

	// Check if all libraries initialized correctly
	if (windowInit && graphicsInit && soundInit && physicsInit)
	{
		// Build the scene graph
		loadScene();

		// Explicitly call the resize method to set the initial projection transformation
		// and viewport based on framebuffer size.
		glm::ivec2 dim = getWindowDimensions();
		framebuffer_size_callback(this->renderWindow, dim.x, dim.y);

		// Intialize all Gameobjects in the scene graph and the Components that they contain
		this->initialize();

		// Return true to indicate the game has initialized
		return true;
	}
	else
	{
		std::cerr << "Game Initialization Failure." << endl;

		// Return false to indicate the game did not initialize correctly
		return false;
	}

} // end initialize

bool Game::initializeRenderWindow()
{
	// Initialize the GLFW window. If a failure, then exit
	if (!glfwInit()) {

		std::cerr << "GLFW Initialization Failure." << endl;
		exit(EXIT_FAILURE);
	}

	// Register function to get GLFW error messages displayed on the console
	glfwSetErrorCallback(glfw_error_callback);

	// Specify that OpenGL will be the client API for the rendering context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

	// Use the core OpenGL profile	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window in debug mode for additional error and diagnostic reporting 
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	// Flush the rendering pipeline when the context is released from being current
	glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_FLUSH);

	// Explicitly request double buffers i.e. two frame buffers
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	// Create rendering window and the OpenGL context.
	renderWindow = glfwCreateWindow(initialScreenWidth, initialScreenHeight, windowTitle.c_str(), NULL, NULL);

	if (!renderWindow) {

		std::cerr << "Render Window Creation Failure." << endl;
		std::cerr << "Make sure requested OpenGL version is supported." << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//	Makes the OpenGL rendering context of the renderWindow current..
	glfwMakeContextCurrent(renderWindow);

	//  Ensure that glfwGetKey returns GLFW_PRESS the each time
	//  it is called even if the key had been released before the call
	glfwSetInputMode(renderWindow, GLFW_STICKY_KEYS, true);
	
	// Set the swap interval for the OpenGL context i.e. the number of screen 
	// updates to wait between before swapping the buffer and returning.
	glfwSwapInterval(1);

	// Bind all callback functions to handle window events
	bindCallBacks();

	if (VERBOSE) cout << "Render Window Initialized" << endl;

	return true;

} // end initializeRenderWindow

bool Game::initializeGraphics()
{
	// ****** Initialize GLEW *********

	// Prevent possible initialization error when using the core context
	glewExperimental = GL_TRUE;

	GLenum res = glewInit(); // Must be done after glut is initialized!

	if (res != GLEW_OK) {

		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return false;
	}

	// On some platforms, GLEW will emit a benign error code. So clear it.
	glGetError();

	// ********* Initialize OpenGL **************

	// Register OpenGL error callback to print error messages to the console
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(openglMessageCallback, 0);

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);

	// Cull back faces of polygons
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// Render "filled in" polygons
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Set source and destination blending functions
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable Gamma correction
	//glEnable(GL_FRAMEBUFFER_SRGB);

	if (VERBOSE) cout << "Graphics Initialized" << endl;

	// Display OpenGL context information (OpenGL and GLSL versions) on the
	// command console.
	if (VERBOSE) displayOpenGlInfo();

	return true;

} // end initializeGraphics

void Game::bindCallBacks()
{
	// Set this game as the user defined window associated with the 
	// rendering window.
	glfwSetWindowUserPointer(renderWindow, this);

	// Register window resize callback function
	glfwSetFramebufferSizeCallback(renderWindow, framebufferResizeCallback);

	// Register window close function
	glfwSetWindowCloseCallback(renderWindow, windowCloseCallback);

} // end bindCallBacks


//********************* Run Methods *****************************************

void Game::gameLoop()
{
	isRunning = true;

	while (isRunning) {

		processGameInput();
		updateGame();
		renderScene();
	}

	if (VERBOSE) cout << "Exited Game Loop" << endl;

} // end gameLoop

void Game::processGameInput()
{
	// Must be called in order for callback functions
	// to be called for registered events.
	glfwPollEvents();

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ESCAPE)) {

		isRunning = false;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) && WireFrame_KeyDown == false) {

		GLint mode;
		glGetIntegerv(GL_POLYGON_MODE, &mode);

		if (mode == GL_FILL) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		WireFrame_KeyDown = true;

	}
	else if (!glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W)) {
		WireFrame_KeyDown = false;
	}

	// Start an input traversal of all SceneGrapNode/GameObjects in the game
	GameObject::processInput();

	// Add pending, delete removed, and reparent GameObjects in the game.
	GameObject::UpdateSceneGraph();

} // end processInput

void Game::updateGame()
{
	// Compute delta time
	static double lastRenderTime = glfwGetTime(); // static initilization only occurs once
	double currentTime = glfwGetTime();
	float deltaTime = static_cast<float>(currentTime - lastRenderTime);

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	if (deltaTime >= FRAME_INTERVAL) {

		// Update the physics engine
		PhysicsEngine::Update(deltaTime);

		// Start an update traversal of all SceneGrapNode/GameObjects in the game
		GameObject::update(deltaTime);

		// Update the sound engine
		SoundEngine::Update(deltaTime);

		// Add pending, delete removed, and reparent
		GameObject::UpdateSceneGraph();

		// Update the last time the game was updated
		lastRenderTime = currentTime;
	}

	// Add pending, delete removed, and reparent GameObjects in the game.
	GameObject::UpdateSceneGraph();

} // end updateGame()

void Game::renderScene()
{

	for (auto& camera : CameraComponent::GetActiveCameras()) {

		camera->setCameraTransformations();

		// Render the Scene ...
		for (auto& mesh : MeshComponent::GetMeshComponents()) {

			mesh->draw();
		}

	}

	// Swap the front and back buffers
	glfwSwapBuffers(renderWindow);

} // end renderScene

//********************* Shutdown Methods *****************************************

void Game::shutdown()
{
	// Destroy the window
	glfwDestroyWindow(renderWindow);

	// Frees GLFW allocated resources
	glfwTerminate();

	// Delete all shader programs that have been created
	deleteAllShaderPrograms();

	// Free up sound engine resources
	SoundEngine::Stop();

	// Free up physics engine resources
	PhysicsEngine::Stop();


} // end shutDown

//********************* Accessor Methods *****************************************

glm::ivec2 Game::getWindowDimensions()
{
	int width, height;
	glfwGetFramebufferSize(this->renderWindow, &width, &height);

	return glm::ivec2(width, height);

} // end getWindowDimensions

//********************* Event Handlers *****************************************

void Game::window_close_callback(GLFWwindow* window)
{
	// Stop the game loop
	this->isRunning = false;

} // end window_close_callback


void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// TEMPORARY: NO longer necessary once CamearaComponent is fully implemented
	// Set parameters for the window transformation ...
	// glViewport(0, 0, width, height);

	// TEMPORARY: NO longer necessary once CamearaComponent is fully implemented
	// Set the projection transformation ...
	mat4 perspectProjection = glm::perspective(PI / 6.0f, (float)width / height, 1.0f, 1000.f);
	//glUniformMatrix4fv(1001, 1, GL_FALSE, glm::value_ptr(perspectProjection));
	SharedTransformations::setProjectionMatrix(perspectProjection);

} // end framebuffer_size_callback

//********************* static function definitions *****************************************

void glfw_error_callback(int error, const char* description) {

	fprintf(stderr, "Error: %s\n", description);

} // end glfw_error_callback


void displayOpenGlInfo(void)
{
	// Display the company responsible for this GL implementation
	fprintf(stdout, "OpenGL INFO:\n\tOpenGL Implementor: %s\n",
		glGetString(GL_VENDOR));
	// Display the renderer/graphics card
	fprintf(stdout, "\tRenderer: %s\n", glGetString(GL_RENDERER));
	// Display the OpenGL version that is implemented
	fprintf(stdout, "\tOpenGL Version: %s\n", glGetString(GL_VERSION));
	// Display the GLSL version
	fprintf(stdout, "\tGLSL Version: %s\n\n",
		glGetString(GL_SHADING_LANGUAGE_VERSION));

} // end displayOpenGlInfo


void GLAPIENTRY openglMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	if (severity != 0x826B) {
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}

} // end MessageCallback

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	// Cast the user point to a Game* type
	Game* that = static_cast<Game*>(glfwGetWindowUserPointer(window));

	// Call the Game class method
	that->framebuffer_size_callback(window, width, height);

} // end framebuffer_size_callback

static void windowCloseCallback(GLFWwindow* window)
{
	// Cast the user point to a Game* type
	Game* that = static_cast<Game*>(glfwGetWindowUserPointer(window));

	// Call the Game class method
	that->window_close_callback(window);

} // end window_close_callback



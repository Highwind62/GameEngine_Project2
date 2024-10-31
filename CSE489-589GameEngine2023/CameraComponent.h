#pragma once
#include "Component.h"
#include "memory.h"

class CameraComponent : public Component
{
public:

	/**
	 * @fn	CameraComponent::CameraComponent( int cameraDepth = 0,
	 * 										  float vertFovDegrees = 30.0f,
	 * 										  float nearClip = 1.0f,
	 * 										  float farClip = 1000.0f );
	 *
	 * @brief	Constructor. By default, Cameras render to the entire window with
	 * 			a depth of zero. Cameras are added to the static activeCameras vector
	 * 			when GameObject::addComponent is called to place them in a GameObject
	 * 			container. The implementation uses the updater order as the depth value.
	 *
	 * @param 	updateOrder   	(Optional) The update order. Used as the Camera depth.
	 * @param 	vertFovDegrees	(Optional) The vertical field of view (fov) in degrees.
	 * @param 	nearClip	  	(Optional) Distance to the near clipping plane.
	 * @param 	farClip		  	(Optional) Distance to the far clipping plane.
	 */
	CameraComponent(const int& cameraDepth = 0, const float& vertFovDegrees = 30.0f,
		const float& nearClip = 1.0f, const float& farClip = 1000.0f);

	/**
	 * @fn	void CameraComponent::setViewingTransformation();
	 *
	 * @brief	Called before the scene is rendered from the perspective of this
	 * 			camera. Sets the view port (using glViewport), projection matrix,
	 * 			and viewing transformation based upon this properties of the Camera
	 * 			and the GameObject that holds it. The scene is rendered in the
	 * 			specified view port without distortion. Use the scissor test and a 
	 * 			scissor rectangle to clear only the viewport of the camera to the
	 * 			camear clear color.
	 *
	 *			Calls:
	 *				Game::getWindowDimensions()
	 *				SceneGraphNode::getModelingTransformation
	 *				SharedTransformations::setViewMatrix
	 * 				SharedTransformations::setProjectionMatrix
	 * 			Called by:
	 * 				Game::renderScene
	 *
	 *
	 */
	void setCameraTransformations();

	/**
	 * @fn	void CameraComponent::setViewPort( GLfloat xLowerLeft, GLfloat yLowerLeft,
	 * 										   GLfloat viewPortWidth, GLfloat viewPortHeight);
	 *
	 * @brief	Sets the rendering area for the camera. In normalized coordinate the
	 * 			width and height of the viewport are 1.0.
	 *
	 * @param	xLowerLeft	  	normalized x coordinate of the lower left hand corner
	 * 							of the viewport.
	 * @param	yLowerLeft	  	normalized y coordinate of the lower left hand corner
	 * 							of the viewport.
	 * @param	viewPortWidth 	normalized width of the view port in pixels.
	 * @param	viewPortHeight	normalized height of the view port in pixels.
	 */
	void setViewPort(GLfloat xLowerLeft, GLfloat yLowerLeft, GLfloat viewPortWidth, GLfloat viewPortHeight);

	/**
	 * @fn	CameraComponent
	 *
	 * @brief	Removes this camera component from the list of active cameras.
	 */
	~CameraComponent();


	virtual void setSkyBox(std::shared_ptr<class SkyBoxComponent> skyBox) { this->skyBox = skyBox; }

	const std::shared_ptr<class SkyBoxComponent> getSkyBox() { return this->skyBox; }

	/**
	 * @fn	static void CameraComponent::addCamera(std::shared_ptr<class CameraComponent> cameraComponen);
	 *
	 * @brief	Adds camera the camera to the list of active cameras and sorts the vector based on
	 * 			the depth values that the cameras contain.
	 *
	 * @param 	cameraComponen	The camera componen.
	 */
	static void addCamera(std::shared_ptr<class CameraComponent> cameraComponent);

	/**
	 * @fn	static void CameraComponent::removeCamera(std::shared_ptr<class CameraComponent> cameraComponent);
	 *
	 * @brief	Removes the camera from the list of active cameras
	 *
	 * @param 	cameraComponent	The camera component.
	 */
	static void removeCamera(std::shared_ptr<class CameraComponent> cameraComponent);

	/**
	 * @fn	static bool CameraComponent::CompareCameraDepth(const CameraComponent* left, const CameraComponent* right)
	 *
	 * @brief	Binary function that accepts two Components to determine which has the lower
	 * 			updateOrder value.
	 *
	 * @param 	left 	The left.
	 * @param 	right	The right.
	 *
	 * @returns	true if the left argument has lower camera depth than the right. false is returned if
	 * 			the right has a lower camera depth. Allows Cameras to be sorted based on
	 * 			camera depth.
	 */
	static bool CompareCameraDepth(const std::shared_ptr<class CameraComponent> left, const std::shared_ptr<class CameraComponent> right)
	{
		return (left->cameraDepth < right->cameraDepth);
	}

	/**
	 * @fn	static std::vector<CameraComponent*> CameraComponent::getActiveCameras()
	 *
	 * @brief	Gets the ordered vector of active cameras
	 *
	 * @returns	Null if it fails, else the active cameras.
	 */
	static const std::vector<std::shared_ptr<class CameraComponent>> GetActiveCameras();

	/**
	 * @fn	void CameraComponent::setCameraClearColor(vec4 clearColor);
	 *
	 * @brief	Sets camera clear color for the camear viewport
	 *
	 * @param 	clearColor	The clear color.
	 */
	void setCameraClearColor(vec4 clearColor);

protected:

	// Normalized viewport settings
	// The lower left x position (from 0.0 (left) to 1.0 (right)).
	GLfloat xLowerLeft = 0.0f;
	// The lower left y position (from 0.0 (bottom) to 1.0 (top)).
	GLfloat yLowerLeft = 0.0f;
	// The viewport width (from 0.0-1.0).
	GLfloat viewPortWidth = 1.0f;
	// The viewport height (from 0.0-1.0).
	GLfloat viewPortHeight = 1.0f;

	/** @brief	The vertical fov in radians */
	float vertFovRadians;

	/** @brief	Distance to the near clipping plane for this camera. */
	float nearClip;

	/** @brief	Distance to the far clipping plane for this camera. */
	float farClip;

	// Skybox associated with this camera
	std::shared_ptr<class SkyBoxComponent> skyBox = nullptr;

	/** @brief	Depth of the camera Higer dept cameras render on top of others.*/
	int cameraDepth = 0;


	/** @brief	The camera clear color Color to which the viewport will be cleared.*/
	vec4 cameraClearColor = 0.5f * WHITE_RGBA;

	/**
	 * @brief	Vector containing the Cameras that are active. The vector should be sorted based
	 * 			upon the depth values of the cameras. 
	 */
	//static std::vector<CameraComponent*> activeCameras;
	static std::vector<std::shared_ptr<class CameraComponent>> activeCameras;

};
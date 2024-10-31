#pragma once

#include "MathLibsConstsFuncs.h"

using namespace constants_and_types;

class SceneGraphNode
{
public:

	friend class RigidBodyComponent;

	/**
	 * @fn	glm::mat4 getModelingTransformation();
	 *
	 * @brief	Gets modeling transformation that will be used to
	 * 			render any meshes associated with this scene graph
	 * 			node. The modeling transformation will include
	 * 			the localScale (and any fixed transform) combined
	 * 			and the world transformation of the parents, of
	 * 			the scene graph node.
	 *
	 * @returns	The modeling transformation.
	 */
	glm::mat4 getModelingTransformation();

	/**
	 * @fn	glm::vec3 SceneGraphNode::getPosition(Frame frame = WORLD);
	 *
	 * @brief	Returns the position this scene graph node relative to either the
	 * 			parent (LOCAL) or the World (WORLD)coordinate frame depending on
	 * 			the specified frame.
	 *
	 * @param	frame	(Optional) The frame.
	 *
	 * @returns	position of this node relative to the specified coordinate frame.
	 */
	glm::vec3 getPosition(Frame frame = WORLD);

	/**
	 * @fn	void SceneNode::setPosition(glm::vec3 position, Frame frame = WORLD);
	 *
	 * @brief	Sets the position of the scene graph node relative to either the
	 * 			World or local coordinate frame. Orientation is not changed.
	 *
	 * @param	position	vector xyz coordinates of the position.
	 * @param	frame   	(Optional) frame relative to which the position is to be set.
	 */
	void setPosition(vec3 position, Frame frame = WORLD);

	/**
	 * @fn	glm::mat4 SceneGraphNode::getRotation(Frame frame = WORLD);
	 *
	 * @brief	Gets the orientation of the scene graph node relative to either
	 * 			the World or local coordinate frame.
	 *
	 * @param	frame	(Optional) The frame.
	 *
	 * @returns	The orientation of the scene graph node relative to the
	 * 			specified frame.
	 */
	glm::mat4 getRotation(Frame frame = WORLD);

	/**
	 * @fn	void SceneGraphNode::setRotation(mat4 rotation, Frame frame = WORLD);
	 *
	 * @brief	Set the orientation of the scene graph node relative to either
	 * 			the World or local coordinate frame. Position is not changed.
	 *
	 * @param	rotation	4 x 4 matrix describing the orientation.
	 * @param	frame   	(Optional) frame relative to which the rotation is to be set.
	 */
	void setRotation(mat4 rotation, Frame frame = WORLD);

	void setRotation(float rotX, float rotY, float rotZ, Frame frame = WORLD)
	{
		mat4 eulerRotation = glm::yawPitchRoll(rotX, rotY, rotZ);

		setRotation(eulerRotation, frame);

	}


	/**
	 * @fn	glm::mat4 SceneGraphNode::getScale(Frame frame = WORLD);
	 *
	 * @brief	Gets the scale of the scene graph node relative to either the World
	 * 			or local local coordinate frame.
	 *
	 * @param	frame	(Optional) The frame.
	 *
	 * @returns	The scale relative to the specified coordinate frame.
	 */
	glm::mat4 getScale(Frame frame = WORLD);

	/**
	 * @fn	void SceneGraphNode::setScale(const vec3& scale, Frame frame = WORLD);
	 *
	 * @brief	Sets a scale for the scene graph node relative to either the
	 * 			World or local coordinate frame.
	 *
	 * @param	scale	The scale.
	 * @param	frame	(Optional) The frame.
	 */
	void setScale(const vec3& scale, Frame frame = WORLD);

	/**
	 * @brief	Returns the forward direction of the scene graph node
	 * 			relative to the specified coordinate frame.
	 *
	 * @returns	facing direction relative to the specified coordinate frame.
	 */
	glm::vec3 getFowardDirection(Frame frame = WORLD);

	/**
	 * @brief	Returns the up direction of the scene graph node
	 * 			relative to the specified coordinate frame.
	 *
	 * @returns	up direction relative to the specified coordinate frame.
	 */
	glm::vec3 getUpDirection(Frame frame = WORLD);

	/**
	 *
	 * @brief	Gets right direction 0f the scene graph node
	 * 			relative to the specified coordinate frame.
	 *
	 * @returns	The right direction.
	 */
	glm::vec3 getRightDirection(Frame frame = WORLD);

	///**
	// * @fn	void SceneGraphNode::setFixedRotation(const glm::mat4 fixed);
	// *
	// * @brief	Sets the fixed rotation of the game object so that the 
	// * 			forward rotation is in the -Z direction.
	// *
	// * @param 	fixed	The fixed.
	// */
	//void setFixedRotation(const glm::mat4 fixed);

	/**
	 * @brief	Rotate the object to "face' in the World coordinate direction
	 * 			described by the input parameter.
	 *
	 * @param	direction	The direction.
	 */
	void rotateTo(const glm::vec3& direction, Frame frame = WORLD);

	/**
	 * @fn	mat4 SceneGraphNode::getWorldTransform();
	 *
	 * @brief	Recursive methods that follows the scene graph up to the root to
	 * 			get world transform of this scene graph node relative to its
	 * 			parent. The world transform does not include the local scale
	 * 			(or the fixed transform) of this scene graph node. Depending
	 * 			applyScaleToChildren setting for ancestors it may include
	 * 			scale settings from them.
	 *
	 * @returns	The world transform.
	 */
	mat4 getWorldTransform();

protected:

	/**
	 * @fn	void SceneGraphNode::updateModelingTransformation();
	 *
	 * @brief	Updates the modeling transformation for this scene
	 * 			graph node. Unlike the world transformation, the
	 * 			modeling transformation will include
	 * 			the localScale (and any fixed transform).
	 */
	void updateModelingTransformation();

	/** @brief	The local scale the expresses any scaling of this this scene graph
	* 			node relative to its parent.
	*/
	mat4 localScale = mat4(1.0f);

	/**
	* @brief	Local transform that expresses the position and orientation of this scene graph
	* 			node relative to its parent.
	*/
	mat4 localTransform = mat4(1.0f);

	/**
	* @brief	The modeling transformation for the game object. It
	* 			is set based or local transformation, (fixed transformation),
	* 			and local scale and the world transformation of the
	* 			parents, grandparents, etc of the scene graph node. This is
	* 			the transformation used in the vertex shader to render the
	* 			object.
	*/
	mat4 modelingTransformation = mat4(1.0f);

	/**
	* @brief	The parent of this node in the scene graph. nullptr indicates
	* 			that this scene node has no parent and is likely the root of
	* 			a scene graph.
	*/
	class GameObject* parent = nullptr;

	/** @brief	True to apply scale to children */
	bool applyScaleToChildren = false;

};


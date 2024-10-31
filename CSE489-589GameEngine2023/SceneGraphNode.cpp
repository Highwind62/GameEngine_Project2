#include "SceneGraphNode.h"
#include "GameObject.h"

static const bool VERBOSE = false;

mat4 SceneGraphNode::getWorldTransform()
{
	// Base case
	if (parent == nullptr) {

		return mat4(1.0f);
	}
	else { // Recursive call

		// Determine if the scale is to be applied to chidren.
		if (parent->applyScaleToChildren == true) {

			return parent->getWorldTransform() * parent->localScale * localTransform;
		}
		else {
			return parent->getWorldTransform() * localTransform;
		}
	}

} // end getWorldTransform

void SceneGraphNode::updateModelingTransformation()
{
	//modelingTransformation = glm::translate(position) * glm::mat4_cast(orientation) * localScale;

	if (parent != nullptr) {
		modelingTransformation = getWorldTransform() * /*fixedRotation **/ localScale;
	}
	else {
		modelingTransformation = localTransform * /* fixedRotation **/ localScale;
	}

} // end updateModelingTransformation

glm::mat4 SceneGraphNode::getModelingTransformation()
{
	// Return the modeling transformation that will be used to 
	// render any meshes associate with this scene graph node.
	return this->modelingTransformation;

} // end getModelingTransformation

glm::vec3 SceneGraphNode::getPosition(Frame frame)
{
	if (frame == Frame::LOCAL) {

		// Get the position in local coordinates
		return getPositionVec3FromTransform(localTransform);
	}
	else {
		// Extract the position from the world transformation for
		// this scene graph node.
		return getPositionVec3FromTransform(getWorldTransform());
	}

} // end getPosition

glm::mat4 SceneGraphNode::getRotation(Frame frame)
{
	if (frame == Frame::LOCAL) {

		// Get the rotation in local coordinates
		return getRotationMatrixFromTransform(localTransform);
	}
	else {
		// Extract the orientation relative to the world transformation for
		// this scene graph node.
		return getRotationMatrixFromTransform(getWorldTransform());
	}

} // end getRotation

glm::mat4 SceneGraphNode::getScale(Frame frame)
{
	if (frame == Frame::LOCAL) {

		// Get the scale in local coordinates
		return this->localScale;
	}
	else {

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;

		// Extract the scale relative to the world transformation for
		// this scene graph node.
		glm::decompose(getWorldTransform(), scale, rotation, translation, skew, perspective);

		return glm::scale(scale) * localScale;
	}

} // end getScale


void SceneGraphNode::setPosition(vec3 position, Frame frame)
{
	if (frame == Frame::LOCAL) {

		// Set the position in local coordinates
		setPositionVec3ForTransform(localTransform, position);
	}
	else {

		if (parent != nullptr) {

			mat4 worldT = getWorldTransform();
			mat4 invParentT = glm::inverse(parent->getWorldTransform());
			setPositionVec3ForTransform(worldT, position);
			localTransform = invParentT * worldT;
		}
		else {
			std::cerr << "ERROR: Setting position relative to WORLD coordinates"
				<< " with null parent. \nFirst attach GameObject to scene graph using addChildGameObject." << endl;
		}
	}

} // end setPosition


void SceneGraphNode::setRotation(glm::mat4 rotation, Frame frame)
{
	if (frame == Frame::LOCAL) {

		// Set the rotation in local coordinates
		setRotationMat3ForTransform(localTransform, rotation);
	}
	else {

		if (parent != nullptr) {

			glm::mat4 parentWorldRotation = parent->getRotation(Frame::WORLD);
			glm::mat4 newRotation = glm::inverse(parentWorldRotation) * rotation;
			setRotationMat3ForTransform(localTransform, newRotation);

		}
		else {
			std::cerr << "ERROR: Setting rotation relative to WORLD coordinates"
					  << " with null parent. \nFirst attach GameObject to scene graph using addChildGameObject." << endl;
		}
	}

} // end setRotation


void SceneGraphNode::setScale(const vec3& scale, Frame frame)
{
	if (frame == Frame::LOCAL) {

		// Get the scale in local coordinates
		this->localScale = glm::scale(scale);
	}
	else {

		if (parent != nullptr) {

			mat4 parentScale = glm::scale(getScaleFromTransform(parent->getWorldTransform()));
			this->localScale = glm::inverse(parentScale) * glm::scale(scale);

		}
		else {
			std::cerr << "ERROR: Setting scale relative to WORLD coordinates"
				<< " with null parent. \nFirst attach GameObject to scene graph using addChildGameObject." << endl;
		}
	}

} // end setScale

glm::vec3 SceneGraphNode::getFowardDirection(Frame frame)
{
	if (frame == Frame::LOCAL) {
		return FORWARD;
	}
	else {

		// Rotate the local forward based on the orientation 
		// of this scene graph node.
		return glm::mat3(getRotation(WORLD)) * FORWARD;
	}

} // end getFowardDirection

glm::vec3 SceneGraphNode::getUpDirection(Frame frame)
{
	if (frame == Frame::LOCAL) {
		return UP;
	}
	else {
		// Rotate the local up based on the orientation 
		// of this scene graph node.
		return glm::mat3(getRotation(WORLD)) * UP;
	}

} // end getUpDirection


glm::vec3 SceneGraphNode::getRightDirection(Frame frame)
{
	if (frame == Frame::LOCAL) {
		return RIGHT;
	}
	else {

		// Rotate the local up based on the orientation 
		// of this scene graph node.
		return glm::mat3(getRotation(WORLD)) * RIGHT;
	}

} // end getRightDirection

void SceneGraphNode::rotateTo(const glm::vec3& direction, Frame frame)
{
	// Normalize the new direction
	vec3 newDirection = glm::normalize(direction);

	if (frame == WORLD) {

		if (parent != nullptr) {
			// Transform the direction to local coordinates
			newDirection = (glm::inverse(parent->getWorldTransform()) * glm::vec4(newDirection, 0.0f)).xyz;
		}
		else {
			std::cerr << "ERROR: Rotating to a direction relative to WORLD coordinates"
				<< " with null parent. \nAttach GameObject to scene graph." << endl;
		}
	}

	// Find the perpendicular vector to rotate about
	glm::vec3 axis = glm::cross(FORWARD, newDirection);

	// Check if direction is equal to FORWARD
	if (glm::length(axis) > 0.0f) {

		// Find the angle to rotate between the current direction and the new direction
		float angle = glm::acos(glm::dot(FORWARD, newDirection));

		// Set the local transformation
		setRotation(glm::rotate(angle, axis), LOCAL);
	}

} //  endl rotateTo

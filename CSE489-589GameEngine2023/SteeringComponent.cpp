#include "SteeringComponent.h"

static const bool VERBOSE = false;


SteeringComponent::SteeringComponent(std::vector<std::weak_ptr<class GameObject>> waypoints, vec3 velocity)
	: WaypointComponent(waypoints, velocity)
{

}

void SteeringComponent::update(const float& deltaTime)
{
	// Check if next waypoint has been reached
	if (distanceToTargetWaypoint() < (speed * deltaTime * 10.0f)) {
		targetWaypointIndex = getNexWaypointIndex();
		if (VERBOSE) cout << "point reached" << endl;
	}

	// Get the current facing direction
	vec3 facingDirection = owningGameObject->getFowardDirection(WORLD);

	// Calculate current yaw angle
	float currentYaw = (float)atan2(-facingDirection.x, -facingDirection.z);

	// Calculate current pitch angle
	float currentPitch = (float)atan2(facingDirection.y, glm::sqrt(glm::pow(facingDirection.x, 2) + pow(facingDirection.z, 2)));

	// Figure out direction to next waypoint
	vec3 desiredDirection = getDirectionToNextWaypoint();

	// Get heading angle to the next waypoint
	float desiredYaw = (float)atan2(-desiredDirection.x, -desiredDirection.z);

	// Account for the "branch cut" in yaw angles
	if (fabs(currentYaw - desiredYaw) > PI) {

		if (currentYaw < desiredYaw) {

			currentYaw += 2 * PI;
		}
		else {
			desiredYaw += 2 * PI;
		}
	}

	// Determine the desired pitch
	float desiredPitch = (float)atan2(desiredDirection.y, glm::sqrt(glm::pow(desiredDirection.x, 2) + pow(desiredDirection.z, 2)));

	float desiredRoll = 0;

	// Determine the turn direction and set the roll angle accordingly
	if (desiredYaw < currentYaw) {
		// Left turn
		desiredRoll = fabs(desiredYaw - currentYaw);
	}
	else {
		// Right turn
		desiredRoll = -fabs(desiredYaw - currentYaw);
	}

	// Interpolate towards desired roll, pitch, and yaw angles
	currentRoll = glm::lerp(currentRoll, desiredRoll, deltaTime);
	float yaw = glm::lerp(currentYaw, desiredYaw, deltaTime);
	float pitch = glm::lerp(currentPitch, desiredPitch, deltaTime/2);

	// Apply Euler angles
	//mat4 eulerRotation = glm::rotate(yaw, UP) * glm::rotate(pitch, RIGHT) * glm::rotate(currentRoll, FORWARD);

	mat4 eulerRotation = glm::yawPitchRoll(yaw, pitch, -currentRoll);

	// Set the orientation of the owning game object
	owningGameObject->setRotation(eulerRotation,WORLD);

	// Get the new facing direction after the orientation has been applied
	vec3 newFacingDirection = owningGameObject->getFowardDirection(WORLD);

	// Get the current position
	vec3 position = owningGameObject->getPosition(WORLD);

	// Determine the new position based on the speed
	position = position + newFacingDirection * speed * deltaTime;

	if (VERBOSE) cout << position << endl;

	// Update the position of the owning game object
	this->owningGameObject->setPosition(position, WORLD);

} // end update



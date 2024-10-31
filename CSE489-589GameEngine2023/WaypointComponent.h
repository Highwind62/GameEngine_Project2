#pragma once

#include "Component.h"



/*
	shared_ptr are designed to delete automatically the object they manage, once it is no longer needed. 
	This happens when no shared pointer point anymore to the object (e.g. all the shared pointers pointing 
	to the same object were deleted, or reset to point to another object).

	There is no need to delete it yourself. By design, there is even no way to delete the object yourself 
	directly, as this might result in some dangling pointers and inconsistencies.

	unique_ptr are another kind of smart pointers. They are designed to keep ownership of an allocated object 
	and destroy the object when the unique pointer itself is destroyed.

	Again, there is no need to delete the object yourself. unique_ptr was created to release you from this 
	duty. But here you can destroy the managed object, if you want, by reseting the pointer. As the pointer 
	is unique, there is no risk of creating dangling pointers here.

	Finally there are also weak_ptr. These do not own an object, but refer to an object managed by some 
	shared_ptr. The idea is that the existence of a weak_ptr does not prevent the object to be deleted. 
	As the weak_ptr does not own an object, it can't delete it, whatever you do.
*/
class WaypointComponent : public Component
{
public:

	/**
	 * @fn	WaypointComponent::WaypointComponent(std::vector< glm::vec3> waypoints, vec3 velocity = vec3(10, 0, 0));
	 *
	 * @brief	Constructor
	 *
	 * @param	waypoints	The waypoints.
	 * @param	velocity 	(Optional) The velocity.
	 */
	WaypointComponent(std::vector<std::weak_ptr<class GameObject>> waypoints, vec3 velocity = vec3(10, 0, 0));

	virtual void update(const float& deltaTime) override;

protected:

	int getNexWaypointIndex();
	vec3 getDirectionToNextWaypoint();
	GLfloat distanceToTargetWaypoint();

	std::vector<std::weak_ptr<class GameObject>> waypoints;

	vec3 velocity;
	float speed;

	int targetWaypointIndex = 0;


};


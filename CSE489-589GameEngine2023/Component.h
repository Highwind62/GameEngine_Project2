#pragma once

#include <algorithm> 

#include "MathLibsConstsFuncs.h"
#include "GameObject.h"
#include "Game.h"

using namespace constants_and_types;

// Enumerated type to support quick "instanceOf" checking. Enables the
// Game to support specialized handling of different Component types
enum COMPONENT_TYPE { COMPONENT = 0, MESH, COLLISION, CAMERA, LIGHT, 
					  SKYBOX, BILLBOARD, PARTICLE_SYSTEM, RIGID_BODY, MOVE  };

class Component : public std::enable_shared_from_this<Component>
{
public:

	/**
	 * @fn	const COMPONENT_TYPE Component::getComponentType()
	 *
	 * @brief	Gets update order (the lower the update order, the earlier the
	 * 			component is initialized and updates).
	 *
	 * @returns	The type of Component.
	 */
	const COMPONENT_TYPE getComponentType() { return componentType; } 

	/**
	 * @fn	Component::Component(int updateOrder = 100);
	 *
	 * @brief	Constructor (the lower the update order, the earlier the
	 * 			component is initialized and updates)
	 *
	 * @param 	updateOrder	(Optional) The update order.
	 */
	Component(int updateOrder = 100);

	/**
	 * @fn	virtual Component::~Component();
	 *
	 * @brief	Destructor. Deletes all dynamically allocated data.
	 */
	virtual ~Component();

	/**
	 * @fn	virtual void Component::update(const float & deltaTime);
	 *
	 * @brief	Update this component by delta time.
	 *
	 * @param 	deltaTime	The time in seconds since the component was last
	 * 						updated.
	 */
	virtual void update(const float & deltaTime);

	/**
	 * @fn	virtual void Component::ProcessInput()
	 *
	 * @brief	Process user input for this component
	 */
	virtual void processInput() {}

	/**
	 * @fn	virtual void Component::initialize();
	 *
	 * @brief	Initializes this component
	 */
	virtual void initialize();

	/**
	 * @fn	int Component::getUpdateOrder() const
	 *
	 * @brief	Gets update order (the lower the update order, the earlier the
	 * 			component is initialized and updated).
	 *
	 * @returns	The update order of this component.
	 */
	int getUpdateOrder() const { return updateOrder; }

	/**
	 * @fn	static bool Component::CompareUpdateOrder(const ComponentPtr left, const ComponentPtr right)
	 *
	 * @brief	Binary function that accepts two Components to determine which
	 * 			has the lower updateOrder value.
	 *
	 * @param 	left 	The left.
	 * @param 	right	The right.
	 *
	 * @returns	true if the left argument has lower update order than the right.
	 * 			false is returned if the right has a lower update order. Allows
	 * 			Components to be sorted based on updateOrder.
	 */
	static bool CompareUpdateOrder(const ComponentPtr left, const ComponentPtr right)
	{
		return (left->updateOrder < right->updateOrder);
	}

	 /** @brief	friend declaration
	  * Gives the GameObject class access to protected and private members
	  * of the Component class. Specifically, the game object addComponent
	  * method sets the owningGameObject member of this class.
	 */
	friend class GameObject;

	/**
	 * @class	GameObject*
	 *
	 * @brief	A game object that this component is attached to
	 */
	//GameObjectPtr owningGameObject;
	class GameObject* owningGameObject;

	virtual void collisionEnter(const class RigidBodyComponent* collisionData) 
	{ /*cout << "collision enter" << endl; */ };

	virtual void collisionStay(const class RigidBodyComponent* collisionData) 
	{ /*cout << "collision stay" << endl; */};

	virtual void collisionExit(const class RigidBodyComponent* collisionData) 
	{ /*cout << "collision exit" << endl;*/ };

protected:

	// Data member specifying specialized Component type
	COMPONENT_TYPE componentType = COMPONENT;

	/** @brief	Initialization and update order of this component. Components with 
	a lower update order will be updated first. */
	int updateOrder;

}; // end Component



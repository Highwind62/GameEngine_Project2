#pragma once

// Physics
#include "PhysicsEngine.h"
#include "MeshComponent.h"


class RigidBodyComponent : public Component, public btMotionState
{
public:

	/**
	 * @fn	RigidBodyComponent::RigidBodyComponent(class MeshComponent* meshComponent, DynamicsState state = DYNAMIC, int updateOrder = 500);
	 *
	 * @brief	Constructor
	 *
	 * @param [in,out]	meshComponent	Pointer the mesh component that the collision shape will be based on.
	 * @param 		  	state		 	(Optional) The state.
	 * @param 		  	updateOrder  	(Optional) The update order.
	 */
	RigidBodyComponent( std::shared_ptr<class MeshComponent> meshComponent, DynamicsState state = DYNAMIC, 
		float mass = 1.0f, vec3 intialInertia = ZERO_V3, int updateOrder = 500);


	/**
	 * @fn	virtual RigidBodyComponent::~RigidBodyComponent( );
	 *
	 * @brief	Destructor
	 */
	virtual ~RigidBodyComponent();

	/**
	 * @fn	virtual void RigidBodyComponent::getWorldTransform( btTransform &worldTrans ) const;
	 *
	 * @brief	Called by bullet to get the position and orientation of the object. It is only called
	 * 			once by the physics engine. After that the engine assumes it is in control and that it
	 * 			knows where the object is. Method must be implemented in order to sub-class btMotionState
	 *
	 *
	 * @param [in,out]	worldTrans	The world transaction.
	 */
	virtual void getWorldTransform(btTransform& worldTrans) const;

	/**
	 * @fn	virtual void RigidBodyComponent::setWorldTransform( const btTransform &worldTrans );
	 *
	 * @brief	Callled repeatedly by bullet to set the position and orientation of the object.
	 * 			Method must be implemented in order to sub-class btMotionState.
	 *
	 * @param	worldTrans	The world transaction.
	 */
	virtual void setWorldTransform(const btTransform& worldTrans);

	/**
	 * @fn	virtual void RigidBodyComponent::setVelocity( vec3 worldVelocity );
	 *
	 * @brief	Called to set a velocity for the game object in meters per second.
	 *
	 * @param	worldVelocity	The linear velocity relative to the World coordinate frame
	 */
	void setVelocity(vec3 worldVelocity);

	/**
	 * @fn	virtual void RigidBodyComponent::setAngularVelocity( vec3 worldAngVelocity );
	 *
	 * @brief	Sets angular velocity. Direction of the vector determines the axis of
	 * 			rotation. Length of the vector determines the rotational speed.
	 *
	 * @param	worldAngVelocity	The angular velocity relative to the World coordinate frame
	 */
	void setAngularVelocity(vec3 worldAngVelocity);

	/**
	 * @fn	virtual void RigidBodyComponent::setGravityOn(bool gravityOn);
	 *
	 * @brief	Sets gravity on
	 *
	 * @param	gravityOn	True to enable, false to disable gravity.
	 */
	void setGravityOn(bool gravityOn);

	/**
	 * @fn	void RigidBodyComponent::enableCCD(bool ccdOn);
	 *
	 * @brief	Enables the continous collision detection (CCD). Should
	 * 			be true for fast or small moving objects.
	 *
	 * @param 	ccdOn	True to enable, false to disable the CCD.
	 */
	void enableCCD(bool ccdOn);

	/**
	 * @fn	void RigidBodyComponent::initialize() override;
	 *
	 * @brief	Initializes this object based on the Dynamics start supplied to the constructor.
	 */
	virtual void initialize() override;

	/**
	 * @fn	void RigidBodyComponent::broadcastCollisionEnter(const RigidBodyComponent* collisionData) const;
	 *
	 * @brief	Method is called whenever this rigid body first collides with
	 * 			another rigid body.
	 *
	 * @param 	collisionData	Information describing the collision.
	 */
	void broadcastCollisionEnter(const RigidBodyComponent* collisionData) const;

	/**
	 * @fn	void RigidBodyComponent::broadcastCollisionStay(const RigidBodyComponent* collisionData) const;
	 *
	 * @brief	Method is called after first collides with another rigid body
	 * 			first collides with another rigid body.
	 *
	 * @param 	collisionData	Information describing the collision.
	 */
	void broadcastCollisionStay(const RigidBodyComponent* collisionData) const;

	/**
	 * @fn	void RigidBodyComponent::broadcastCollisionExit(const RigidBodyComponent* collisionData) const;
	 *
	 * @brief	Called whenever this rigid body exits a collision with another
	 * 			rigid body.
	 *
	 * @param 	collisionData	Information describing the collision.
	 */
	void broadcastCollisionExit(const RigidBodyComponent* collisionData) const;

protected:

	/**
	 * @class	btRigidBody*
	 *
	 * @brief	The rigid body that bullet holds in its physics simulation.
	 */
	class btRigidBody* bulletRigidBody = nullptr;
//	class btRigidBody* ghostRigidBody = nullptr;

	/**
	 * @class	btCollisionShape*
	 *
	 * @brief	A bullet collision shape that is based of a mesh. The collision shape is
	 * 			created by MeshComponent sub classes.
	 */
	class btCollisionShape* bulletCollisionShape;

	/**
	 * @class	btTransform
	 *
	 * @brief	4x4 homogenous transformation for the rigid body
	 */
	class btTransform bulletTransform;

	/** @brief	The rigidbody dynamics state.
	 *  NONE indicates that the object will be ignored by the physics engine.
	 *  STATIONARY indicates that the object will not move, but that objects can collide with it
	 *  KINEMATIC_MOVING indicates that the object will be moved and updated by a Component
	 *  instead of the physics engine, but collisions will still be detected.
	 *  DYNAMIC indicates that the object will be moved and updated by the physics engine.
	 */
	DynamicsState rigidbodyDynamics = NONE;

	/** @brief	Reference to the mesh component with the collision shape for this rigid body. */
	std::shared_ptr<class MeshComponent> meshComponent;

	// Data members to hold rigid body setting prior to initialization
	bool applyGravity = true;
	
	/** @brief	The mass associated with the rigid body in the physics simulation. Set in the
	 *	constructor. Kinematic rigid bodies have a mass of zero.
	 */
	btScalar mass = 1.0f;

	/** @brief	The set linear velocity. Data member stores the linear velocity prior to
	 *	initilization of the rigid body. 
	 */
	vec3 linearVelocity = vec3(0.0f, 0.0f, 0.0f);
	
	/** @brief	The set angularvelocity. Data member stores the angular velocity prior to
	 *	initilization of the rigid body. 
	 */
	vec3 angularvelocity = vec3(0.0f, 0.0f, 0.0f);

	/** @brief	The intial inertia to which the rigid body will have when after it is initialized. */
	btVector3 intialInertia;

	/** @brief	True to enable continuous collision detection, false to disable the CCD. Should be true
	 *	for small fast moving rigidbodies.
	 */
	bool ccdOn = false;
};


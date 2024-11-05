#include "RigidBodyComponent.h"

static const bool VERBOSE = false;

RigidBodyComponent::RigidBodyComponent(std::shared_ptr<MeshComponent> meshComponent, DynamicsState state,
									   float mass, vec3 intialInertia, int updateOrder)
	: Component(updateOrder), meshComponent(meshComponent), rigidbodyDynamics(state), 
	  mass(static_cast<btScalar>(mass)), 
	  intialInertia(btVector3(intialInertia.x, intialInertia.y, intialInertia.z)),
	  bulletCollisionShape( meshComponent->getCollisionShape())
{
	componentType = RIGID_BODY;
}

RigidBodyComponent::~RigidBodyComponent()
{
	if (VERBOSE) cout << "RigidBodyComponent Destructor called" << endl;

	if (bulletRigidBody != nullptr && PhysicsEngine::dynamicsWorld != nullptr) {

		PhysicsEngine::RemoveFromColPairsLastUpdate(bulletRigidBody->getMotionState());

		PhysicsEngine::dynamicsWorld->removeRigidBody(bulletRigidBody);
		delete bulletRigidBody;
	}

} // end destructor


void RigidBodyComponent::initialize()
{
	if (VERBOSE) std::cerr << "RigidBodyComponent::initialize" << endl;

	// Get the collision shape for the mesh component after the buildMesh has been
	// called. The collision shape does not exist until after this has been completed.
	// Only some types of collision shapes can be scaled. Thus, no attempt is made here.
	//this->bulletCollisionShape = meshComponent->getCollisionShape();

	if (this->rigidbodyDynamics == DYNAMIC) {

		bulletCollisionShape->calculateLocalInertia(mass, intialInertia);

		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			mass,							// mass SHOULD BE NON-ZERO FOR DYNAMIC RIGIDBODIES
			this,							// motion state
			bulletCollisionShape,			// collision shape of body
			intialInertia					// local inertia
			);

		this->bulletRigidBody = new btRigidBody(rigidBodyCI);
		bulletRigidBody->setCollisionFlags( btCollisionObject::CF_DYNAMIC_OBJECT);

		PhysicsEngine::dynamicsWorld->addRigidBody(bulletRigidBody);

		// Enable or disable gravity
		setGravityOn(applyGravity);

		// Set linear velocity
		setVelocity(linearVelocity);

		// Set the angular velocity
		setAngularVelocity(this->angularvelocity);

		// Enable of disable continous collision detection
		enableCCD(ccdOn);
	}
	else if (this->rigidbodyDynamics == KINEMATIC || this->rigidbodyDynamics == STATIONARY) {

		// Static object with no inertia.
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			0.0f,							// mass SHOULD BE ZERO FOR KINEMATIC AND STATIONARY RIGIDBODIES
			this,							// motion state
			bulletCollisionShape,			// collision shape of body
			btVector3(0.0f, 0.0f, 0.0f)		// local inertia
			);

		this->bulletRigidBody = new btRigidBody( rigidBodyCI );

		if (this->rigidbodyDynamics == KINEMATIC) {

			bulletRigidBody->setCollisionFlags(bulletRigidBody->getCollisionFlags() | 
											   btCollisionObject::CF_KINEMATIC_OBJECT);
			bulletRigidBody->setActivationState(DISABLE_DEACTIVATION);

		}
		else { // STATIONARY
		
			// Rigid bodies with a mass of zero are automatically treated as static objects.
			//bulletRigidBody->setCollisionFlags(bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		
		}

		if (VERBOSE) cout << "Adding bulletRigidBody to the dynamicsWorld" << endl;

		PhysicsEngine::dynamicsWorld->addRigidBody(bulletRigidBody);

	}
	else {
		this->bulletRigidBody = nullptr;
		this->bulletCollisionShape = nullptr;
	}

} // end initialize


void RigidBodyComponent::enableCCD(bool ccdOn)
{
	this->ccdOn = ccdOn;

	if (bulletRigidBody != nullptr) {

		bulletRigidBody->setCcdMotionThreshold((float)1.0e-7);
		bulletRigidBody->setCcdSweptSphereRadius(0.50f);
	}

}

void RigidBodyComponent::setVelocity(vec3 worldVelocity)
{
	linearVelocity = worldVelocity;

	if (bulletRigidBody != nullptr) {

		bulletRigidBody->setLinearVelocity(btVector3(linearVelocity.x, linearVelocity.y, linearVelocity.z));
	}

} // end setVelocity

void RigidBodyComponent::setAngularVelocity(vec3 worldAngVelocity)
{
	angularvelocity = worldAngVelocity;

	if (bulletRigidBody != nullptr) {

		bulletRigidBody->setAngularVelocity(btVector3(angularvelocity.x, angularvelocity.y, angularvelocity.z));
	}

} // end setAngularVelocity

void RigidBodyComponent::setGravityOn(bool gravityOn)
{
	applyGravity = gravityOn;

	if (bulletRigidBody != nullptr) {

		if (gravityOn) {
			bulletRigidBody->setGravity(btVector3(0.0f, -9.8f, 0.0f));
		}
		else {
			bulletRigidBody->setGravity(btVector3(0.0f, 0.0f, 0.0f));
		}
	}
}

// Called by bullet to get the initial position and orientation of the object
void RigidBodyComponent::getWorldTransform(btTransform& worldTrans) const
{
	// Calculate current world transformation and set world trans to it 
	worldTrans = PhysicsEngine::convertTransform(this->owningGameObject->getWorldTransform());

	if (VERBOSE) std::cout << "RigidBodyComponent::getWorldTransform " << getPositionVec3FromTransform(this->owningGameObject->getWorldTransform()) << std::endl;

} // end getWorldTransform

// Called by bullet to set the position and orient the object
void RigidBodyComponent::setWorldTransform(const btTransform& worldTrans)
{
	bulletTransform = worldTrans;

	glm::mat4 T = PhysicsEngine::convertTransform(worldTrans);

	// Get the world transform of the parent
	mat4 invParWorldTrans = glm::inverse(this->owningGameObject->parent->getWorldTransform());

	this->owningGameObject->localTransform = invParWorldTrans * T;

	if (VERBOSE) std::cout << "RigidBodyComponent::setWorldTransform" << std::endl;
}

void RigidBodyComponent::broadcastCollisionEnter(const RigidBodyComponent* collisionData) const
{
	if (VERBOSE) std::cout << "broadcastCollisionEnter" << std::endl;

	std::vector<ComponentPtr> buddys = owningGameObject->getComponents();

	int sendCount = 0;

	for (auto& bud : buddys) {

		if (bud != shared_from_this()) {

			bud->collisionEnter(collisionData);
		}
	}

} //  end CollisionEnter


void RigidBodyComponent::broadcastCollisionStay(const RigidBodyComponent* collisionData) const
{
	if (VERBOSE) std::cout << "broadcastCollisionStay" << std::endl;

	std::vector<ComponentPtr> buddys = owningGameObject->getComponents();

	for (auto& bud : buddys) {

		if (bud != shared_from_this()) {
			bud->collisionStay(collisionData);
		}
	}

} //  end CollisionStay


/**
 * @fn	void RigidBodyComponent::CollisionExit( const RigidBodyComponent * collisionData ) const
 *
 * @brief	Called whenever this rigid body exits a collision with another rigid body.
 *
 * @param	collisionData	Information describing the collision.
 */
void RigidBodyComponent::broadcastCollisionExit(const RigidBodyComponent* collisionData) const
{
	if (VERBOSE) std::cout << "broadcastCollisionExit" << std::endl;

	std::vector<ComponentPtr> buddys = owningGameObject->getComponents();

	for (auto& bud : buddys) {

		if (bud != shared_from_this()) {
			bud->collisionExit(collisionData);
		}
	}

} // end CollisionExit

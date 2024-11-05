#include "PhysicsEngine.h"

#include "RigidBodyComponent.h"

static const bool VERBOSE = false;

// Static Data Member Definitions
btBroadphaseInterface* PhysicsEngine::broadphase;
btDefaultCollisionConfiguration* PhysicsEngine::collisionConfiguration;
btCollisionDispatcher* PhysicsEngine::dispatcher;
btSequentialImpulseConstraintSolver* PhysicsEngine::solver;
btDiscreteDynamicsWorld* PhysicsEngine::dynamicsWorld;

// Set containing the collision pairs from the last update
CollisionPairs PhysicsEngine::colPairsLastUpdate;

bool PhysicsEngine::Init()
{
	// Bullet uses a right-handed coordinate system.
	// btScalar is a floating point number
	// btVector3 has 3 scalar x,y,z components and a 4th unused w component
	// 3D orientations and rotations can be represented using either btQuaternion or btMatrix3x3

	/*
	  The broadphase algorithm usually uses the bounding boxes of objects in the
	  world to quickly compute a conservative approximate list of colliding
	  pairs. The list will include every pair of objects that are colliding,
	  but it may also include pairs of objects whose bounding boxes intersect
	  but are still not close enough to collide. Later stages will calculate
	  exact collisions, and thus these extra pairs will be eliminated, but
	  those algorithms are much slower so we don't want to run them on every
	  pair of rigid bodies in the world.

	  btDbvtBroadphase implements a broadphase using two dynamic AABB
	  bounding volume hierarchies/trees.

	  See http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Broadphase
	*/
	broadphase = new btDbvtBroadphase();

	/*
	Set algorithms used for narrow phase collision detection to default.
	*/
	collisionConfiguration = new btDefaultCollisionConfiguration();

	/*
	btCollisionDispatcher supports algorithms that handle ConvexConvex
	and ConvexConcave collision pairs
	*/
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	/*
	This is what causes the objects to interact properly, taking
	into account gravity, game logic supplied forces, collisions, and
	hinge constraints.

	The btSequentialImpulseConstraintSolver is a fast SIMD implementation
	of the Projected Gauss Seidel (iterative LCP) method.
	*/
	solver = new btSequentialImpulseConstraintSolver;

	/*
	Entire physics pipeline computation and its data structures are represented in Bullet by a
	dynamics world. Create a btDiscreteDynamicsWorld or btSoftRigidDynamicsWorld.
	*/
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	// Set gravity direction in the dynamics world.
	dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));

	// Register the collision event generator. The register function will be called once each time
	// the physics engine updates. 
	dynamicsWorld->setInternalTickCallback(PhysicsEngine::collisionEventCallback, nullptr, false);

	if (VERBOSE) std::cout << "******** Bullet Physics Engine successfully initiliazed. **************" << std::endl;

	return true;

} // end Init


void PhysicsEngine::Update(const float& deltaTime)
{
	// Update the simulation by deltaTime on each update. Automatically takes into account 
	// variability in deltaTime performing interpolation up to 10 interpolation steps.
	// Each internal interpolation step will be a 60th of a second. deltaTime should
	// always be less than 10 * (1/60).
	dynamicsWorld->stepSimulation(deltaTime, 10, btScalar(1 / 60.0f));

} // end Update


void PhysicsEngine::Stop()
{
	/* Smart pointers are connected to some objects in the physics engine.
	* Deleting them here causes errors on shutdown. Probably because
	* rigid bodies and shapes have already been deleted by smart points.
	*/
	delete dynamicsWorld;
	dynamicsWorld = nullptr;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;

	if (VERBOSE) std::cout << "******** Bullet Physics Engine Shut Down. **************" << std::endl;

} // end Stop


void PhysicsEngine::collisionEventCallback(btDynamicsWorld* world, btScalar timeStep)
{
	// Set of collision pairs found during the current update
	CollisionPairs colPairsThisUpdate;

	// Iterate through all of the manifolds in the dispatcher
	// A contact manifold is a cache that contains all contact points between a pair of 
	// collision objects.
	int numManifolds = world->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++)
	{
		// Get the manifold
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);

		// Ignore manifolds that have no contact points.
		if (contactManifold->getNumContacts() > 0)
		{
			// get the two rigid bodies involved in the collision
			const btRigidBody* pBody0 = static_cast<const btRigidBody*>(contactManifold->getBody0());
			const btRigidBody* pBody1 = static_cast<const btRigidBody*>(contactManifold->getBody1());

			// Order the pair using the pointer value.
			bool const swapped = pBody0 > pBody1;
			const btRigidBody* pSortedBodyA = swapped ? pBody1 : pBody0;
			const btRigidBody* pSortedBodyB = swapped ? pBody0 : pBody1;

			// Create the pair
			CollisionPair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);

			// FCheck if the pair is in the current list
			auto iter = std::find(colPairsThisUpdate.begin(), colPairsThisUpdate.end(), thisPair);

			// If the pair is not in the current list, insert it
			if (iter == colPairsThisUpdate.end()) {

				// insert the pair into the current list
				colPairsThisUpdate.insert(thisPair);

			}
		}
	}
	
	// Set of new pairs during this update
	CollisionPairs newPairs;

	// Find the collisions that are new by finding the difference between colPairsThisUpdate and colPairsLastUpdate
	// and push the difference into newPairs
	std::set_difference(colPairsThisUpdate.begin(), colPairsThisUpdate.end(), colPairsLastUpdate.begin(), colPairsLastUpdate.end(),
		std::inserter(newPairs, newPairs.begin()));

	// Generate enter events for new pairs
	for (CollisionPairs::const_iterator iter = newPairs.begin();
		iter != newPairs.end(); ++iter)
	{
		SendCollisionEnterMessages(iter->first, iter->second);
	}


	// Set of pairs removed during this update
	CollisionPairs removedPairs;

	// Find the collisions that are no longer occuring by 
	// finding the difference between colPairsLastUpdate and colPairsThisUpdate
	// and push the difference into removedPairs
	std::set_difference(colPairsLastUpdate.begin(), colPairsLastUpdate.end(),
		colPairsThisUpdate.begin(), colPairsThisUpdate.end(),
		std::inserter(removedPairs, removedPairs.begin()));

	// Generate separation events for removed pairs
	for (CollisionPairs::const_iterator iter = removedPairs.begin();
		iter != removedPairs.end(); ++iter)
	{
		SendCollisionExitMessages(iter->first, iter->second);
	}

	// Set of pairs removed during this update
	CollisionPairs stayPairs;

	// Find the collisions that also occured during the last update by
	// finding the difference between gets colPairsLastUpdate and removedPairs
	// and push the difference into stayPairs
	std::set_difference(colPairsLastUpdate.begin(), colPairsLastUpdate.end(),
		removedPairs.begin(), removedPairs.end(),
		std::inserter(stayPairs, stayPairs.begin()));

	// Generate stay events for pairs that have remained in contact
	for (CollisionPairs::const_iterator iter = stayPairs.begin();
		iter != stayPairs.end(); ++iter)
	{
		SendCollisionStayMessages(iter->first, iter->second);
	}

	// Save set of collision pairs for next update
	colPairsLastUpdate = colPairsThisUpdate;

} // end collisionEventCallback


void PhysicsEngine::SendCollisionEnterMessages(const btRigidBody* rigBody0, const btRigidBody* rigBody1)
{
	const RigidBodyComponent* moState0 = static_cast<const RigidBodyComponent*>(rigBody0->getMotionState());
	const RigidBodyComponent* moState1 = static_cast<const RigidBodyComponent*>(rigBody1->getMotionState());

	moState0->broadcastCollisionEnter(moState1);
	moState1->broadcastCollisionEnter(moState0);

} // end CollisionEnter

void PhysicsEngine::SendCollisionStayMessages(const btRigidBody* rigBody0, const btRigidBody* rigBody1)
{
	const RigidBodyComponent* moState0 = static_cast<const RigidBodyComponent*>(rigBody0->getMotionState());
	const RigidBodyComponent* moState1 = static_cast<const RigidBodyComponent*>(rigBody1->getMotionState());

	moState0->broadcastCollisionStay(moState1);
	moState1->broadcastCollisionStay(moState0);

} // end CollisionExit

void PhysicsEngine::SendCollisionExitMessages(const btRigidBody* rigBody0, const btRigidBody* rigBody1)
{
	const RigidBodyComponent* moState0 = static_cast<const RigidBodyComponent*>(rigBody0->getMotionState());
	const RigidBodyComponent* moState1 = static_cast<const RigidBodyComponent*>(rigBody1->getMotionState());

	moState0->broadcastCollisionExit(moState1);
	moState1->broadcastCollisionExit(moState0);

} // end CollisionExit


// Converts a glm::mat4 into a btTransform
btTransform PhysicsEngine::convertTransform(const glm::mat4& trans)
{
	btVector3 origin(trans[3][0], trans[3][1], trans[3][2]);

	// Row Major
	btMatrix3x3 rot(trans[0][0], trans[1][0], trans[2][0],
		trans[0][1], trans[1][1], trans[2][1],
		trans[0][2], trans[1][2], trans[2][2]);

	btTransform btTrans(rot, origin);

	return btTrans;

} // end convertTransform


// Converts a btTransform into a glm::mat4
glm::mat4 PhysicsEngine::convertTransform(const btTransform& trans)
{
	btMatrix3x3 rot = trans.getBasis();

	btVector3 c1 = rot.getColumn(0);
	btVector3 c2 = rot.getColumn(1);
	btVector3 c3 = rot.getColumn(2);
	btVector3 c4 = trans.getOrigin();

	glm::mat4 glmTrans(c1[0], c1[1], c1[2], 0.0f,
		c2[0], c2[1], c2[2], 0.0f,
		c3[0], c3[1], c3[2], 0.0f,
		c4[0], c4[1], c4[2], 1.0f);

	return glmTrans;

} // end convertTransform

bool PhysicsEngine::checkForRayIntersection(glm::vec3 origin, glm::vec3 direction)
{
	// Make a ray using the origin and direction
	glm::vec3 out_end = origin + direction * 1000.0f;

	// Create a ray callback using the above array.
	btCollisionWorld::ClosestRayResultCallback RayCallback(
		btVector3(origin.x, origin.y, origin.z),
		btVector3(out_end.x, out_end.y, out_end.z)
		);

	// Use the callback to raytest against the world.
	dynamicsWorld->rayTest(
		btVector3(origin.x, origin.y, origin.z),
		btVector3(out_end.x, out_end.y, out_end.z),
		RayCallback
		);

	// If we have a hit, return the game object.
	if (RayCallback.hasHit()) {

		//?????????????????????
		//cout << "mesh " << (int)RayCallback.m_collisionObject->getUserPointer();
	}

	return RayCallback.hasHit();

} // checkForRayIntersection

void PhysicsEngine::RemoveFromColPairsLastUpdate(const btMotionState* removed)
{

	for (auto& pair : colPairsLastUpdate) {

		if (removed == pair.first->getMotionState() || removed == pair.second->getMotionState()) {

			if (VERBOSE) cout << "collision pair removed" << endl;

			// Remove the pair. Collision exit events will not be sent.
			colPairsLastUpdate.erase(pair);

			break;

		}
	}
}


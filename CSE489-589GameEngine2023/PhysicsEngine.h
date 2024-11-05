#pragma once

#include "Bullet/btBulletDynamicsCommon.h"
#include "MathLibsConstsFuncs.h"

#include <set>			// For holding sets or collision pairs
#include <algorithm>    // std::set_difference, std::sort
#include <iterator>		// std::inserter

using namespace constants_and_types;

// typedefs for collision events
typedef std::pair<const btRigidBody*, const btRigidBody*> CollisionPair;
typedef std::set<CollisionPair> CollisionPairs;


class PhysicsEngine
{
public:

	static void RemoveFromColPairsLastUpdate(const btMotionState* removed);

	/**
	 * Initialize the engine.
	 */
	static bool Init();

	/**
	 * Update the engine. Call this once each frame.
	 * @param deltaTime - time in seconds since the last update.
	 */
	static void Update(const float& deltaTime = 0.0f);

	/**
	 * Stop the engine. Call when closing down.
	 */
	static void Stop();

	/**
	 * Generates collision events that are passed to the game engine.
	 * @param world - Contains the physics simulation
	 * @param timeStep - incremental time step in the update.
	 */
	static void collisionEventCallback(btDynamicsWorld* world, btScalar timeStep);

	/**
	 * Passes events to RigidBodyComponents in the Game Engine at the beginning of a collision.
	 * @param rigBody0 - first rigid body involved in the collision.
	 * @param rigBody1 - second rigid body involved in the collision.
	 */
	static void SendCollisionEnterMessages(const btRigidBody* rigBody0, const btRigidBody* rigBody1);

	/**
	 * Passes events to RigidBodyComponents in the Game Engine after the beginning and before the
	 * end of a collision.
	 * @param rigBody0 - first rigid body involved in the collision.
	 * @param rigBody1 - second rigid body involved in the collision.
	 */
	static void SendCollisionStayMessages(const btRigidBody* rigBody0, const btRigidBody* rigBody1);

	/**
	 * Passes events to RigidBodyComponents in the Game Engine at the end of a collision.
	 * @param rigBody0 - first rigid body involved in the collision.
	 * @param rigBody1 - second rigid body involved in the collision.
	 */
	static void SendCollisionExitMessages(const btRigidBody* rigBody0, const btRigidBody* rigBody1);

	/**
	 * Converts a glm::mat4 into a btTransform.
	 * @param trans - mat4 to be converted to a btTransform
	 * @return transformation expressed as a btTransform
	 */
	static btTransform convertTransform(const glm::mat4& trans);

	/**
	* Converts a btTransform into a glm::mat4.
	* @param trans - btTransform to be converted to a glm::mat4
	* @return transformation expressed as a glm::mat4
	*/
	static glm::mat4 convertTransform(const btTransform& trans);

	/**
	* Checks to see if a ray intersects with any physics objects.
	*
	* @param origin:		The origin of the out world ray.
	* @param direction:		The direction of the out world ray.
	*
	* @return True if an intersection was found, false if not
	*/
	static bool checkForRayIntersection(glm::vec3 origin, glm::vec3 direction);

	// See Init method for purpose of each of these objects.
	static btBroadphaseInterface* broadphase;
	static btDefaultCollisionConfiguration* collisionConfiguration;
	static btCollisionDispatcher* dispatcher;
	static btSequentialImpulseConstraintSolver* solver;

	// Physic world in which all simulation is completed
	static btDiscreteDynamicsWorld* dynamicsWorld;

	// Collision event pairs from the last update
	static CollisionPairs colPairsLastUpdate;

};


#pragma once
#include "Component.h"
#include "RigidBodyComponent.h"


class CollisionComponent : public Component
{
public:
	CollisionComponent() : Component(100) {}

	virtual void collisionEnter(const class RigidBodyComponent* collisionData)
	{
		//if (collisionData->owningGameObject->gameObjectName == "projectile") {

		//	if (VERBOSE) cout << "HIT BY PROJECTILE - " << owningGameObject->gameObjectName << endl;

			//owningGameObject->removeAndDelete();
			//collisionData->owningGameObject->removeAndDelete();
		//}
	
		if(VERBOSE) cout << "Collision Component collision enter " << owningGameObject->gameObjectName << endl;

	};

	virtual void collisionStay(const class RigidBodyComponent* collisionData)
	{ 
		//owningGameObject->removeAndDelete();
		if (VERBOSE) cout << "collision stay " << owningGameObject->gameObjectName << endl;
	};

	virtual void collisionExit(const class RigidBodyComponent* collisionData)
	{ 
		if (VERBOSE) cout << "collision exit " << owningGameObject->gameObjectName << endl;
		//owningGameObject->removeAndDelete();
	};


protected:

	bool VERBOSE = true;

};


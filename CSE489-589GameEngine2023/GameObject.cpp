#include "GameObject.h"

#include "Component.h"
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "Game.h"

static const bool VERBOSE = false;

// ***** Definition of static members of the Game Object class *****
Game* GameObject::OwningGame;

std::vector<GameObjectPtr> GameObject::PendingChildren;

std::vector<GameObjectPtr> GameObject::RemovedGameObjects;

std::vector<ReparentPair> GameObject::ReparentedGameObjects;

// ********************************************************************

GameObject::GameObject()
	: gameObjectState(ACTIVE)
{

}


GameObject::~GameObject()
{
	// Remove the game object from the game
	if (VERBOSE) cout << "GameObject destructor called" << endl;
		
	for (auto& component : components) {

		// Remove the component from the game object
		removeComponent(component);
	};

	components.clear();
	
	children.clear();

} // end GameObject destructor


void GameObject::initialize()
{
	// Initialize the components that are attached to this game object
	for (auto & component : components) {

		component->initialize();
	}

	// Make sure the any initialization of position, orientation
	// or scale done by Components is accounted for on the first 
	// frame
	this->updateModelingTransformation();

	for (auto& gameObject : this->children) {

		gameObject->initialize();
	}

} // end initialize

void GameObject::update(const float & deltaTime)
{
	// Check to see if this game object is active
	if (gameObjectState == ACTIVE) {

		// Update the components that are attached to to this game object
		for (auto & component : this->components) {

			component->update(deltaTime);
		}

		// Call the super class method to update the modeling transformation
		updateModelingTransformation();

		// Update the children of this game object
		for (auto& gameObject : this->children) {

			gameObject->update(deltaTime);
		}
	}

} // end update

//void GameObject::updateGameObject(const float & deltaTime)
//{
//	// Override to create specialized update
//
//} // end updateGameObject

void GameObject::processInput()
{
	// Check to see if this game object is active
	if (gameObjectState == ACTIVE) {

		// First process input for components
		for (auto & component : this->components) {
			component->processInput();
		}
	}

	// Call process input on all the children
	for (auto& gameObject : this->children) {

		gameObject->processInput();
	}

} // end processInput

//void GameObject::gameObjectInput()
//{
//	// Override to create custom input response
//
//} // end gameObjectInput

void GameObject::addComponent(std::shared_ptr<Component> component)
{
	// Dependency injection (give the Component a reference to this GameObject
	component->owningGameObject = this;// getGameObjectPtr();

	// Add the component to the components vector
	components.emplace_back(component);

	// Sort the components vector based on their update order.
	std::sort(components.begin(), components.end(), Component::CompareUpdateOrder);

	// Check if the component is a MeshComponent
	if (component->getComponentType() == MESH) {

		// Add the mesh to the static vector of MeshComponents
		MeshComponent::addMeshComp( std::static_pointer_cast<MeshComponent>( component ) );
	}

	if (component->getComponentType() == CAMERA) {

		// Add the mesh to the static vector of MeshComponents
		CameraComponent::addCamera(std::static_pointer_cast<CameraComponent>(component));
	}

} // end addComponent


void GameObject::removeComponent(std::shared_ptr< Component> component)
{
	// Find the component to be removed
	auto iter = std::find(components.begin(), components.end(), component);

	// Check if the component was found
	if (iter != components.end()) {

		if (VERBOSE) cout << "Component" << " removed." << endl;

		// Check if the component is a Mesh
		if (component->getComponentType() == MESH) {

			// Add the mesh to the static vector of MeshComponents
			MeshComponent::removeMeshComp(std::static_pointer_cast<MeshComponent>(component));
		}

		if (component->getComponentType() == CAMERA) {

			// Add the mesh to the static vector of MeshComponents
			CameraComponent::removeCamera(std::static_pointer_cast<CameraComponent>(component));
		}

		std::iter_swap(iter, components.end() - 1);
		components.pop_back();
	}

} // end removeComponent


void GameObject::setState(STATE state)
{
	gameObjectState = state;
}

void GameObject::addChildGameObject(GameObjectPtr gameObject)
{
	if (gameObject != NULL) {

		// Dependency injection so the game object has a
		// reference to its parent GameObject
		gameObject->parent = this;

		// Check if the game has started
		if (OwningGame->isRunning) {

			if (VERBOSE) cout << "pending add" << endl;
			// Add to the pending list so the object is
			// added after the next update
			PendingChildren.emplace_back(gameObject);
		}
		else {

			if (VERBOSE) cout << "direct add" << endl;
			// Game has not started. Add directly to the 
			// vector of game objects in the game.
			children.emplace_back(gameObject);
		}
	}

} // end addGameObject

void GameObject::removeAndDelete()
{

	// Add to objects to be removed after update is complete
	RemovedGameObjects.emplace_back( GameObjectPtr(GameObjectPtr(), this) );

} // end removeGameObject


std::vector<GameObjectPtr> GameObject::GetChildren()
{
	return children;

} // end GetChildren


void GameObject::reparent(GameObjectPtr child)
{
	// Store the game object with its new parent for 
	// actual reparenting after the next update cycle.
	ReparentedGameObjects.emplace_back(ReparentPair( this, child));
	
} // end reparent


void GameObject::UpdateSceneGraph()
{
	AddPendingGameObjects();
	RemoveDeletedGameObjects();
	ReparentGameObjects();

} // end ManageGameObjectChanges


void GameObject::RemoveDeletedGameObjects()
{
	// Set a found bool to an initial value of false.
	bool found = false;

	// Handle game objects marked for removal
	for (auto gameObject : RemovedGameObjects) {

		class GameObject* parentGameObject = gameObject->parent;

		// Search children vector of the parent
		auto iter = std::find(parentGameObject->children.begin(), parentGameObject->children.end(), gameObject);

		// Check if the GameObject was found in children vector of the parent
		if (iter != parentGameObject->children.end()) {

			if (VERBOSE) cout << "found and removing " << gameObject->gameObjectName << ". use_count is " << gameObject.use_count() << endl;

			// Indicate the GameObject was found
			found = true;

			// Swap to end of vector and pop off (avoid erase copies)
			std::iter_swap(iter, parentGameObject->children.end() - 1);
			parentGameObject->children.pop_back();

		}

		// Was it found in the children of the parent?
		if (found == false) {

			// Search pendingChildren vector of the parent
			iter = std::find(PendingChildren.begin(), PendingChildren.end(), gameObject);

			// Check if the GameObject was found in pendingChildren vector of the parent
			if (iter != PendingChildren.end()) {

				// Indicate the GameObject was found
				found = true;

				// Swap to end of vector and pop off (avoid erase copies)
				std::iter_swap(iter, PendingChildren.end() - 1);
				PendingChildren.pop_back();
			}
		}
	}

	// Clear the list for the next update cycle
	RemovedGameObjects.clear();

} // end RemoveDeletedGameObjects

void GameObject::AddPendingGameObjects()
{
	// Attach any pending game objects to the game
	for (auto pending : PendingChildren) {

		if (VERBOSE) cout << "Delayed adddtion of pending object" << endl;

		// Cast to a GameObject * to get to the children vector
		//GameObjectPtr parentGameObject = pending->parent;
		class GameObject* parentGameObject = pending->parent;

		// Add the pending gameObject to the parent's child list
		parentGameObject->children.emplace_back(pending);

		// Same as initializing at the begining of the game
		pending->initialize();

		// Ensure the modeling transformation is updated
		pending->updateModelingTransformation();

		// Helps keep the object from appearing at the World
		// coordinate origin for one frame
		pending->update(0.0f);
	}

	// All pending game objects have been added. Clear for next update.
	PendingChildren.clear();

} // end AddPendingGameObjects

void GameObject::ReparentGameObjects()
{
	// Handle all of the reparented GameObject
	for (auto& reparentPair : ReparentedGameObjects) {

		if (VERBOSE) cout << "Reparenting game object." << endl;

		// Temporary to hold the old parent
		class GameObject* oldParent = reparentPair.child->parent;

		// Get the World transform of the child
		glm::mat4 oldWorldTransformation = reparentPair.child->getWorldTransform();

		// Get the World transform of the new parent
		mat4 newParentWorldTrans = reparentPair.newParent->getWorldTransform();

		// Get inverse of the new parent World transformation and multiply times the child World.
		glm::mat4 newChildTransform = glm::inverse(newParentWorldTrans) * oldWorldTransformation;

		// Overwrite the position scale and rotation so that the child will not move
		// or rotate when it is reparented.
		reparentPair.child->localTransform = newChildTransform;

		// Find the reparented child among the old parent's children
		auto iterator = std::find(oldParent->children.begin(), oldParent->children.end(), reparentPair.child);

		// Check if a child of the parent
		if (iterator != oldParent->children.end()) {

			// Remove the reparented child from the old parent's "family"
			std::iter_swap(iterator, oldParent->children.end() - 1);
			oldParent->children.pop_back();
		}

		// Have the new parent adopt the child
		reparentPair.child->parent = reparentPair.newParent;

		// Add the child to the new parent's children
		reparentPair.newParent->children.emplace_back(reparentPair.child);
	}

	// Clear the list for the next update cycle
	ReparentedGameObjects.clear();

} // end ReparentGameObjects

GameObjectPtr GameObject::findGameObject(string name)
{
	// Check if this game object has the search for name
	if (gameObjectName == name) {
		
		if (VERBOSE) cout << "findGameObject found " << gameObjectName << endl;

		return GameObjectPtr(std::shared_ptr<GameObjectPtr>(), this);
	}

	// Check if the game object has any child game objects
	if (children.size() > 0) {

		// Interate through all the child game objects of this game object
		for (auto& gameObject : this->children) {

			// Recursive call to check the children or the child gameObject
			GameObjectPtr gameObjectPtr = gameObject->findGameObject(name);

			// if a game object with the name was found, return it.
			if (gameObjectPtr != nullptr) {

					return gameObjectPtr;
			} 
		}
	}

	if (VERBOSE) cout << "not found" << endl;

	// End recursion
	return nullptr;

}

std::vector< GameObjectPtr> GameObject::findAllGameObjects(string name)
{
	std::vector< GameObjectPtr> foundObjects;

	// Check if this game object has the search for name
	if (gameObjectName == name) {
		
		if (VERBOSE) cout << "findAllGameObjects found one " << gameObjectName << endl;
		foundObjects.push_back( GameObjectPtr(std::shared_ptr<GameObjectPtr>(), this));
	}

	// Check if the game object has any child game objects
	if (children.size() > 0) {

		// Interate through all the child game objects of this game object
		for (auto& gameObject : this->children) {

			// Recursive call to check the children or the child gameObject
			std::vector< GameObjectPtr> findAny = gameObject->findAllGameObjects(name);

			// Add the any in findAny to the output vector
			foundObjects.insert(foundObjects.end(), findAny.begin(), findAny.end());
		}
	}
	return foundObjects;

}
#include "Component.h"

static const bool VERBOSE = false;

Component::Component(int updateOrder) 
	: updateOrder(updateOrder) 
{}

void Component::initialize() 
{}

Component::~Component()
{
	if (VERBOSE) cout << "Component destructor called " << endl;

}

void Component::update(const float& deltaTime)
{}
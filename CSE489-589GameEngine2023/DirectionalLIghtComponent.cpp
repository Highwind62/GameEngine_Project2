#include "DirectionalLightComponent.h"

void DirectionalLightComponent::update(const float& deltaTime)
{
    // Get the forward direction of the containing GameObject relative to World coordinates
    vec3 direction = owningGameObject->getFowardDirection(WORLD);

    // Set the direction of the light source. Note that the fourth element of the direction is
    // set to zero to indicate the the vector is expressing a direction not a position.
    SharedLighting::setPositionOrDirection(lightIndex, vec4(-normalize(direction), 0.0f));

} // end update

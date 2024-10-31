#include "SpotLightComponent.h"

void SpotLightComponent::initialize()
{
    SharedLighting::setIsSpot(lightIndex, true);

} // end initialize

void SpotLightComponent::update(const float& deltaTime)
{
    PositionalLightComponent::update(deltaTime); // Set the position of the spot light

    // Set the spot direction
    vec3 direction = owningGameObject->getFowardDirection();
    SharedLighting::setSpotDirection(lightIndex, direction);

}  // end update

void SpotLightComponent::setCutoffAngleInDegrees(float cutOffInDegrees)
{
    // Set the spot cutoff angle
    float cutoffRadians = glm::radians(cutOffInDegrees);
    SharedLighting::setSpotCutoffCos(lightIndex, cos(cutoffRadians));

}  // end setCutoffAngleInDegrees
#include "PositionalLightComponent.h"


void PositionalLightComponent::update(const float& deltaTime)
{
    // Get the position of the containing GameObject relative to World coordinates
    vec3 position = owningGameObject->getPosition();

    // Set the direction of the light source relative to World coordinates
    SharedLighting::setPositionOrDirection(lightIndex, vec4(position, 0.0f));

} // end update

void PositionalLightComponent::setAttenuationFactors(float constant, float linear, float quadratic)
{
	// Set the constant, linear, and quadratic attenuation factors for the light source
    SharedLighting::setConstantAttenuation(lightIndex, constant);
    SharedLighting::setLinearAttenuation(lightIndex, linear);
    SharedLighting::setQuadraticAttenuation(lightIndex, quadratic);

}
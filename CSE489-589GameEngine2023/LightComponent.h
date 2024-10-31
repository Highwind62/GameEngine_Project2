#pragma once
#include "Component.h"

#include "SharedLighting.h"

/**
 * @class	LightComponent - Supports all properties that are common to all
 * 			types of light sources. Super class for PointLightComponent,
 * 			DirectionalLightComponent, and SpotLightComponenet.
 * 			
 *          USES THE STATIC METHODS OF THE SharedLight CLASS TO SET LIGHT
 *          PROPERTIES THAT ARE DECLARED IN THE LightBlock UNIFORM BLOCK.
 *          THE LightBlock CONTAINS AN ARRAY OF GeneralLight STRUCTS.
 *
 * @brief	A light component.
 */
class LightComponent : public Component
{
public:

    /**
     * @fn	LightComponent::LightComponent(int controlKey);
     *
     * @brief	Constructor
     *      			
     *          Initializes a light course
     *
     * @param 	controlKey	The key that used to toggle the light on and off.
     */
    LightComponent(int controlKey);

    /**
     * @fn	LightComponent::~LightComponent();
     *
     * @brief	Destructor
     */
    ~LightComponent();

    /**
     * @fn	virtual void LightComponent::update(const float& deltaTime) override = 0;
     *
     * @brief	Virtual methods. Overrides set the position and/or direction of a 
     * 			light based on the forward direction and position of the GameObject
     * 			that contains the lightComponent
     *
     * @param 	deltaTime	The delta time.
     */
    virtual void update(const float& deltaTime) override = 0;

    /**
     * @fn	void LightComponent::setAmbientColor(vec3 ambColor);
     *
     * @brief	Sets ambient color of the lightsource
     *
     * @param 	ambColor	The amb color.
     */
    void setAmbientColor(vec3 ambColor);

    /**
     * @fn	void LightComponent::setDiffuseColor(vec3 difColor);
     *
     * @brief	Sets diffuse color of the lightsource
     *
     * @param 	difColor	The dif color.
     */
    void setDiffuseColor(vec3 difColor);

    /**
     * @fn	void LightComponent::setSpecularColor(vec3 specColor);
     *
     * @brief	Sets specular color of the lightsource
     *
     * @param 	specColor	The specifier color.
     */
    void setSpecularColor(vec3 specColor);

    /**
     * @fn	void LightComponent::setEnable(bool onOff);
     *
     * @brief	Sets an enable
     *
     * @param 	onOff	True to turn on the light, false to turn it off.
     */
    void setEnable(bool onOff);

    /**
     * @fn	bool LightComponent::getEnable();
     *
     * @brief	Accessor that can be used to determine if the light 
     * 			source is on.
     *
     * @returns	True if it is on, false if it is off.
     */
    bool getEnable();

    /**
     * @fn	virtual void LightComponent::processInput() override;
     *
     * @brief	Process the input
     * 			
     *          Checks to see if the control key of the light source
     *          has been pressed and toggles the light on or off if it 
     *          has.
     */
    virtual void processInput() override;

    protected:

    /** @brief	Zero-based index of the specific light */
    int lightIndex;

    /** @brief	The key used to toggle the light on and off*/
    int toggleKey;

    /** @brief	Tracks state of the toggle key to detect each press once.*/
    bool toggleKeyDown = false;

};


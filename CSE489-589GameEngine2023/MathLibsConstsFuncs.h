
#pragma once
#include <iostream> // Stream input and output operations
#include <vector> // Sequence containers for arrays that can change in size
#include <memory> // General utilities to manage dynamic memory
#include <limits>
#include <GL/glew.h>

// Initialize matrices to Identity and vectors to zero vector
#define GLM_FORCE_CTOR_INIT

// Require explicit conversion for GLM types
#define GLM_FORCE_EXPLICIT_CTOR

// Enable swizzle operations on double precision matrices and vectors
#define GLM_FORCE_SWIZZLE

// Forward declaration to speed compilation
#include "glm/fwd.hpp"

// Basic GLM functionality
#include "glm/glm.hpp"

// Stable glm extensions
// https://glm.g-truc.net/0.9.9/api/a01364.html
#include <glm/gtc/matrix_transform.hpp>
// https://glm.g-truc.net/0.9.9/api/a00437.html
#include <glm/gtc/type_ptr.hpp>
// https://glm.g-truc.net/0.9.9/api/a00395.html
#include <glm/gtc/constants.hpp>
// https://glm.g-truc.net/0.9.9/api/a01370.html
#include <glm/gtc/quaternion.hpp>

// Allows experimental extensions of glm to be used
#define GLM_ENABLE_EXPERIMENTAL

// For simple scale, rotate, and translate functions
// https://glm.g-truc.net/0.9.9/api/a00596.html
#include <glm/gtx/transform.hpp>
// For additional quaterion functionality
// https://glm.g-truc.net/0.9.9/api/a01373.html
#include <glm/gtx/quaternion.hpp> 
// For matrices from euler angleseuler angles
// https://glm.g-truc.net/0.9.9/api/a00736.html#gae6aa26ccb020d281b449619e419a609e
#include <glm/gtx/euler_angles.hpp>
// For matrix decomposition
// https://glm.g-truc.net/0.9.6/api/a00204.html
#include <glm/gtx/matrix_decompose.hpp>

using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::shared_ptr;
using std::make_shared;

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::mat4;
using glm::mat2;
using glm::mat3;
using glm::quat;


typedef std::shared_ptr<class GameObject> GameObjectPtr;
typedef std::shared_ptr<class Component> ComponentPtr;

// Constants and types used throughout the program

namespace constants_and_types {

    // Simple texturing modes
    enum TextureMode { NO_TEXTURE = 0, DECAL, REPLACE_AMBIENT_DIFFUSE };

    const float PI = glm::pi<float>();
    const float TWO_PI = PI * 2.0f;
    const float PI_OVER_2 = PI / 2.0f;

    const float POS_INFINITY = std::numeric_limits<float>::infinity();
    const float NEG_INFINITY = -POS_INFINITY;

    const vec2 ZERO_V2(0.0f, 0.0f);
    const vec2 UNIT_X_V2(1.0f, 0.0f);
    const vec2 UNIT_Y_V2(0.0f, 1.0f);
    const vec2 NEG_UNIT_X_V2(-1.0f, 0.0f);
    const vec2 NEG_UNIT_Y_V2(0.0f, -1.0f);

    const vec4 ZERO_V4(0.0f, 0.0f, 0.0f, 0.0f);

    const vec3 ZERO_V3(0.0f, 0.0f, 0.0f);
    const vec3 UNIT_X_V3(1.0f, 0.0f, 0.0f);
    const vec3 UNIT_Y_V3(0.0f, 1.0f, 0.0f);
    const vec3 UNIT_Z_V3(0.0f, 0.0f, 1.0f);
    const vec3 NEG_UNIT_X_V3(-1.0f, 0.0f, 0.0f);
    const vec3 NEG_UNIT_Y_V3(0.0f, -1.0f, 0.0f);
    const vec3 NEG_UNIT_Z_V3(0.0f, 0.0f, -1.0f);
    const vec3 INFINITY_V3(INFINITY, INFINITY, INFINITY);
    const vec3 NEG_INFINITY_V3(NEG_INFINITY, NEG_INFINITY, NEG_INFINITY);

    const vec3 BLACK_RGB(0.0f, 0.0f, 0.0f);
    const vec3 WHITE_RGB(1.0f, 1.0f, 1.0f);
    const vec3 RED_RGB(1.0f, 0.0f, 0.0f);
    const vec3 GREEN_RGB(0.0f, 1.0f, 0.0f);
    const vec3 BLUE_RGB(0.0f, 0.0f, 1.0f);
    const vec3 YELLOW_RGB(1.0f, 1.0f, 0.0f);
    const vec3 LIGHT_YELLOW_RGB(1.0f, 1.0f, 0.88f);
    const vec3 LIGHT_BLUE_RGB(0.68f, 0.85f, 0.9f);
    const vec3 LIGHT_PINK_RGB(1.0f, 0.71f, 0.76f);
    const vec3 LIGHT_GREEN_RGB(0.56f, 0.93f, 0.56f);

    const vec4 BLACK_RGBA(0.0f, 0.0f, 0.0f, 1.0f);
    const vec4 WHITE_RGBA(1.0f, 1.0f, 1.0f, 1.0f);
    const vec4 RED_RGBA(1.0f, 0.0f, 0.0f, 1.0f);
    const vec4 GREEN_RGBA(0.0f, 1.0f, 0.0f, 1.0f);
    const vec4 BLUE_RGBA(0.0f, 0.0f, 1.0f, 1.0f);
    const vec4 YELLOW_RGBA(1.0f, 1.0f, 0.0f, 1.0f);
    const vec4 LIGHT_YELLOW_RGBA(1.0f, 1.0f, 0.88f, 1.0f);
    const vec4 LIGHT_BLUE_RGBA(0.68f, 0.85f, 0.9f, 1.0f);
    const vec4 LIGHT_PINK_RGBA(1.0f, 0.71f, 0.76f, 1.0f);
    const vec4 LIGHT_GREEN_RGBA(0.56f, 0.93f, 0.56f, 1.0f);

    // Coordinate frames
    enum Frame { WORLD = 0, LOCAL };

    // Rigidbody Dynamics
    enum DynamicsState { NONE = 0, STATIONARY, KINEMATIC, DYNAMIC };

    // Local directions
    static const vec3 FORWARD(-UNIT_Z_V3);
    static const vec3 UP(UNIT_Y_V3);
    static const vec3 RIGHT(1, 0, 0);
}
    inline bool NearZero(float val, float epsilon = 0.001f)
    {
        if (fabs(val) <= epsilon) {
            return true;
        }
        else {
            return false;
        }
    }

// Output stream operators for various GLM types
ostream& operator << (ostream& os, const vec2& v);
ostream& operator << (ostream& os, const vec3& v);
ostream& operator << (ostream& os, const vec4& v);
ostream& operator << (ostream& os, const quat& v);
ostream& operator << (ostream& os, const mat2& v);
ostream& operator << (ostream& os, const mat3& v);
ostream& operator << (ostream& os, const mat4& v);


glm::mat4 getRotationMatrixFromTransform(const glm::mat4& transform);
glm::vec3 getPositionVec3FromTransform(const glm::mat4& transform);
glm::vec3 getScaleFromTransform(const glm::mat4& transform);
void setPositionVec3ForTransform(glm::mat4& transform, const glm::vec3& position);
void setRotationMat3ForTransform(glm::mat4& transform, const glm::mat4& rotation);
void setScaleForTransform(glm::mat4& transform, const glm::vec3& scale);

/**
    * @fn	vec3 findUnitNormal(vec3 pZero, vec3 pOne, vec3 pTwo)
    *
    * @brief	Given three vertices on the face of a polygon in counter clockwise order calculates
    * 			a normal vector for the polygon.
    *
    * @param [in]	pZero 1st vertex in counter clockwise order.
    * @param [in]	pZero 2nd vertex in counter clockwise order.
    * @param [in]	pZero 2nd vertex in counter clockwise order.
    *
    * @returns	Normal vector for the polygon.
    */
vec3 findUnitNormal(vec3 pZero, vec3 pOne, vec3 pTwo);


void calculateTanAndBTan(const vec3& pos1, const vec3& pos2, const vec3& pos3,
    const vec2& uv1, const vec2& uv2, const vec2& uv3,
    vec3& mTangent, glm::vec3& mBiTangent);


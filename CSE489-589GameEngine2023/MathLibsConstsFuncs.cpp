#include "MathLibsConstsFuncs.h"
#include <iomanip>

using namespace constants_and_types;

glm::mat4 getRotationMatrixFromTransform(const glm::mat4& transform)
{
	return glm::mat4(glm::mat3(transform));

}

glm::vec3 getPositionVec3FromTransform(const glm::mat4& transform)
{
	float x = transform[3][0];
	float y = transform[3][1];
	float z = transform[3][2];
	return glm::vec3(x, y, z);
}

void setPositionVec3ForTransform(glm::mat4& transform, const glm::vec3& position)
{
	transform[3][0] = position.x;
	transform[3][1] = position.y;
	transform[3][2] = position.z;
}

void setRotationMat3ForTransform(glm::mat4& transform, const glm::mat4& rotation)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			transform[i][j] = rotation[i][j];
		}
	}
}

glm::vec3 getScaleFromTransform(const glm::mat4& transform)
{
	float x = transform[0][0];
	float y = transform[1][1];
	float z = transform[2][2];
	return glm::vec3(x, y, z);
}

void setScaleForTransform(glm::mat4& transform, const glm::vec3& scale)
{
	transform[0][0] = scale.x;
	transform[1][1] = scale.y;
	transform[2][2] = scale.z;
}

/**
 * @fn	ostream &operator<< (ostream &os, const vec2 &V) { os << "[ " << V.x << " " << V.y << " ]"; return os;
 *
 * @brief	Output stream for vec2.
 *
 * @param [in,out]	os	Output stream.
 * @param 		  	V 	The vector.
 *
 * @returns	The shifted result.
 */
ostream &operator << (ostream &os, const vec2 &V)
{
	os << "[ " << V.x << " " << V.y << " ]";
	return os;
}

/**
 * @fn	ostream &operator<< (ostream &os, const vec3 &V) { os << "[ " << V.x << " " << V.y << " " << V.z << " ]"; return os;
 *
 * @brief	Output stream for vec3.
 *
 * @param [in,out]	os	Output stream.
 * @param 		  	V 	The vector.
 *
 * @returns	The shifted result.
 */
ostream &operator << (ostream &os, const vec3 &V)
{
	os << "[ " << V.x << " " << V.y << " " << V.z << " ]";
	return os;
}

/**
 * @fn	ostream &operator<< (ostream &os, const vec4 &V) { os << "[ " << V.x << " " << V.y << " " << V.z << " " << V.w << " ]"; return os;
 *
 * @brief	Output stream for vec4.
 *
 * @param [in,out]	os	Output stream.
 * @param 		  	V 	The vector.
 *
 * @returns	The shifted result.
 */
ostream &operator << (ostream &os, const vec4 &V)
{
	os << "[ " << V.x << " " << V.y << " " << V.z << " " << V.w << " ]";
	return os;
}

ostream& operator << (ostream& os, const quat& V)
{
	os << "[ " << V.w << " " << V.x << " " << V.y << " " << V.z << " ]";
	return os;
}

/**
 * @fn	ostream &operator<< (ostream &os, const mat2 &M) { os << " n"; for (int row = 0; row < 2; row++) { os << "| t"; for (int col = 0; col < 2; col++) { os << std::setw(8) << std::setprecision(4) << M[col][row] << " t";
 *
 * @brief	Output stream for mat2.
 *
 * @param [in,out]	os	Output stream.
 * @param 		  	M 	The matrix.
 *
 * @returns	The shifted result.
 */
ostream &operator << (ostream &os, const mat2 &M)
{
	os << "\n";
	for (int row = 0; row < 2; row++) {
		os << "|\t";
		for (int col = 0; col < 2; col++) {
			os << std::setw(8) << std::setprecision(4) << M[col][row] << "\t";
		}
		os << "|\n";
	}
	os << "\n";
	return os;
}

/**
 * @fn	ostream &operator<< (ostream &os, const mat3 &M) { os << " n"; for (int row = 0; row < 3; row++) { os << "| t"; for (int col = 0; col < 3; col++) { os << std::setw(8) << std::setprecision(4) << M[col][row] << " t";
 *
 * @brief	Output stream for mat3.
 *
 * @param [in,out]	os	Output stream.
 * @param 		  	M 	The matrix.
 *
 * @returns	The shifted result.
 */
ostream &operator << (ostream &os, const mat3 &M)
{
	os << "\n";
	for (int row = 0; row < 3; row++) {
		os << "|\t";
		for (int col = 0; col < 3; col++) {
			os << std::setw(8) << std::setprecision(4) << M[col][row] << "\t";
		}
		os << "|\n";
	}
	os << "\n";
	return os;
}

/**
 * @fn	ostream &operator<< (ostream &os, const mat4 &M) { os << " n"; for (int row = 0; row < 4; row++) { os << "| t"; for (int col = 0; col < 4; col++) { os << std::setw(8) << std::setprecision(4) << M[col][row] << " t";
 *
 * @brief	Output stream for mat4.
 *
 * @param [in,out]	os	Output stream.
 * @param 		  	M 	The matrix.
 *
 * @returns	The shifted result.
 */
ostream &operator << (ostream &os, const mat4 &M)
{
	os << "\n";
	for (int row = 0; row < 4; row++) {
		os << "|\t";
		for (int col = 0; col < 4; col++) {
			os << std::setw(8) << std::setprecision(4) << M[col][row] << "\t";
		}
		os << "|\n";
	}
	os << "\n";
	return os;
}


vec3 findUnitNormal(vec3 pZero, vec3 pOne, vec3 pTwo)
{
	return glm::normalize(glm::cross(pOne - pZero, pTwo - pOne));

} // end findUnitNormal


//template <class T>
//ostream &operator << (ostream &os, const std::vector<T> &V)
//{
//	os << "[" << endl;
//	for (size_t i = 0; i < V.size(); i++) {
//		os << '\t' << V[i] << endl;
//	}
//	os << "]" << endl;
//	return os;
//}

void calculateTanAndBTan(const vec3&pos1, const vec3&pos2, const vec3&pos3,
	const vec2&uv1, const vec2&uv2, const vec2&uv3,
	vec3& mTangent, glm::vec3& mBiTangent)
{
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	mTangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	mTangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	mTangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	mTangent = glm::normalize(mTangent);

	mBiTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	mBiTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	mBiTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	mBiTangent = glm::normalize(mBiTangent);
}
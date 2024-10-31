#include "SphereMeshComponent.h"

vec4 sphericalToCartesian(float sliceAngle, float stackAngle,
	float radius)
{
	vec4 v(glm::cos(stackAngle) * glm::sin(sliceAngle) * radius,
		glm::sin(stackAngle) * radius,
		glm::cos(stackAngle) * cos(sliceAngle) * radius,
		1.0f);

	return v;

} // end sphericalToCartesian

// Get the Spherical texture mapping coordinates based on slice and 
// stack angles
glm::vec2 getSphericalTextCoords(float sliceAngle, float stackAngle)
{
	float s = sliceAngle / (2.0f * PI);
	float t = (stackAngle + PI / 2) / PI;

	return glm::vec2(s, t);
}



SphereMeshComponent::SphereMeshComponent(GLuint shaderProgram, Material mat, float radius, int stacks, int slices, int updateOrder)
	: MeshComponent(shaderProgram, updateOrder), sphereMat(mat), radius(radius), stacks(stacks), slices(slices)
{
	sliceInc = (2.0f * PI) / slices;
	stackInc = PI / stacks;

}

void SphereMeshComponent::buildMesh()
{
	this->scaleMeshName = "Sphere " + std::to_string(radius) + " " + std::to_string(this->sphereMat._id);

	if (previsouslyLoaded() == false) {

		initializeBottomSubMesh();

		initializeBodySubMesh();

		initializeTopSubMesh();

		// Create a collision shape that matches the sphere
		this->collisionShape = new btSphereShape(radius);

		this->saveInitialLoad();
	}

} // end bulidMesh

//void SphereMeshComponent::initializeTopSubMesh()
//{
//	std::vector<pntVertexData> pnt;
//
//	float sliceAngle = 0.0f;
//
//	vec4 top = vec4(0.0f, radius, 0.0f, 1.0f);
//
//	vec4 v0 = sphericalToCartesian(sliceAngle, PI_OVER_2 - stackInc, radius);
//	vec2 t0 = getSphericalTextCoords(sliceAngle, PI_OVER_2 - stackInc);
//	pntVertexData pntV0(v0, glm::normalize(v0).xyz, t0);
//
//
//
//	for (int j = 0; j < slices; j++) {
//
//		pntVertexData pntTop(top, glm::normalize(top).xyz, getSphericalTextCoords(sliceAngle + sliceInc/2.0f, PI/2.0f) );
//
//		sliceAngle += sliceInc;
//		vec4 v1 = sphericalToCartesian(sliceAngle, PI_OVER_2 - stackInc, radius);
//		vec2 t1 = getSphericalTextCoords(sliceAngle, PI_OVER_2 - stackInc);
//		pntVertexData pntV1(v1, glm::normalize(v1).xyz, t1);
//
//		pnt.push_back(pntTop);
//		pnt.push_back(pntV0);
//		pnt.push_back(pntV1);
//
//		pntV0 = pntV1;
//
//	}
//
//	SubMesh subMesh = buildSubMesh(pnt);
//
//	// Set the primitive mode for the vertex data
//	subMesh.primitiveMode = GL_TRIANGLES;
//
//	subMesh.material = sphereMat;
//
//	// Push the submesh into vector of Submeshes to be rendered
//	this->subMeshes.push_back(subMesh);
//
//} // end initializeTopSubMesh

//void SphereMeshComponent::initializeBottomSubMesh()
//{
//	std::vector<pntVertexData> pnt;
//
//	float sliceAngle = glm::radians(360.0f);
//
//	vec4 bottom = vec4(0.0f, -radius, 0.0f, 1.0f);
//
//	pntVertexData pntbottom(bottom, glm::normalize(bottom).xyz, ZERO_V2);
//
//	vec4 v0 = sphericalToCartesian(sliceAngle, -PI_OVER_2 + stackInc, radius);
//	vec2 t0 = getSphericalTextCoords(sliceAngle, -PI_OVER_2 + stackInc);
//	pntVertexData pntV0(v0, glm::normalize(v0).xyz, t0);
//
//	for (int j = 0; j < slices; j++) {
//
//		pntVertexData pntbottom(bottom, glm::normalize(bottom).xyz, getSphericalTextCoords(sliceAngle - (sliceInc / 2.0f), -PI / 2.0f));
//
//		sliceAngle -= sliceInc;
//
//		vec4 v1 = sphericalToCartesian(sliceAngle, -PI_OVER_2 + stackInc, radius);
//		vec2 t1 = getSphericalTextCoords(sliceAngle, -PI_OVER_2 + stackInc);
//		pntVertexData pntV1(v1, glm::normalize(v1).xyz, t1);
//
//		pnt.push_back(pntbottom);
//		pnt.push_back(pntV0);
//		pnt.push_back(pntV1);	
//
//		pntV0 = pntV1;
//	}
//
//	SubMesh subMesh = buildSubMesh(pnt);
//
//	// Set the primitive mode for the vertex data
//	subMesh.primitiveMode = GL_TRIANGLES;
//
//	subMesh.material = sphereMat;
//
//	// Push the submesh into vector of Submeshes to be rendered
//	this->subMeshes.push_back(subMesh);
//
//}// end initializeBottomSubMesh

//void SphereMeshComponent::initializeBodySubMesh() 
//{
//	std::vector<pntVertexData> pnt;
//
//	float stackAngle = -PI_OVER_2 + stackInc;
//
//	for (int j = 0; j < stacks-2; j++) {
//
//		float sliceAngle = 0.0f;
//
//		vec4 v0 = sphericalToCartesian(sliceAngle, stackAngle + stackInc, radius);
//		vec2 t0 = getSphericalTextCoords(sliceAngle, stackAngle + stackInc);
//		vec4 v1 = sphericalToCartesian(sliceAngle, stackAngle, radius);
//		vec2 t1 = getSphericalTextCoords(sliceAngle, stackAngle);
//
//		pntVertexData pnt0(v0, glm::normalize(v0).xyz, t0);
//		pntVertexData pnt1(v1, glm::normalize(v1).xyz, t1);
//
//		for (int i = 0; i < slices; i++) {
//
//			sliceAngle += sliceInc;
//
//			vec4 v2 = sphericalToCartesian(sliceAngle, stackAngle, radius);
//			vec2 t2 = getSphericalTextCoords(sliceAngle, stackAngle);
//			vec4 v3 = sphericalToCartesian(sliceAngle, stackAngle + stackInc, radius);
//			vec2 t3 = getSphericalTextCoords(sliceAngle, stackAngle + stackInc);
//
//			pntVertexData pnt2(v2, glm::normalize(v2).xyz, t2);
//			pntVertexData pnt3(v3, glm::normalize(v3).xyz, t3);
//
//			pnt.push_back(pnt0);
//			pnt.push_back(pnt1);
//			pnt.push_back(pnt3);
//
//			pnt.push_back(pnt1);
//			pnt.push_back(pnt2);
//			pnt.push_back(pnt3);
//
//			pnt0 = pnt3;
//			pnt1 = pnt2;
//		}
//
//		stackAngle += stackInc;
//	}
//
//	SubMesh subMesh = buildSubMesh(pnt);
//
//	// Set the primitive mode for the vertex data
//	subMesh.primitiveMode = GL_TRIANGLES;
//
//	subMesh.material = sphereMat;
//
//	// Push the submesh into vector of Submeshes to be rendered
//	this->subMeshes.push_back(subMesh);
//
//} // end initializeBodySubMesh

void SphereMeshComponent::initializeBottomSubMesh()
{
	std::vector<pntVertexData> pnt;

	lower = createStackVertexData(sliceInc / 2.0f, -PI_OVER_2);

	//lowerBase = 0;

	upper = createStackVertexData(0.0f, -PI_OVER_2 + stackInc);

	//upperBase += lower.size();

	for (int j = slices; j > 0; j--) {

		pnt.push_back(lower[(j - 1)]);
		pnt.push_back(upper[j]);
		pnt.push_back(upper[(j - 1)]);
	}

	SubMesh subMesh = buildSubMesh(pnt);

	// Set the primitive mode for the vertex data
	subMesh.primitiveMode = GL_TRIANGLES;

	subMesh.material = sphereMat;

	// Push the submesh into vector of Submeshes to be rendered
	this->subMeshes.push_back(subMesh);

}// end initializeBottomSubMesh


void SphereMeshComponent::initializeTopSubMesh()
{
	std::vector<pntVertexData> pnt;
	std::vector<unsigned int> indices;

	lower = upper; // Assumes initializeBodySubMesh was previously called

	upper = createStackVertexData(sliceInc / 2.0f, PI_OVER_2);

	lower = createStackVertexData(0.0f, PI_OVER_2 - stackInc);

	for (int j = 0; j < slices; j++) {

		pnt.push_back(upper[j]);
		pnt.push_back(lower[j]);
		pnt.push_back(lower[(j+1)]);
	}

	SubMesh subMesh = buildSubMesh(pnt);

	// Set the primitive mode for the vertex data
	subMesh.primitiveMode = GL_TRIANGLES;

	subMesh.material = sphereMat;

	// Push the submesh into vector of Submeshes to be rendered
	this->subMeshes.push_back(subMesh);

} // end initializeTopSubMesh


void SphereMeshComponent::initializeBodySubMesh() 
{
	std::vector<pntVertexData> pnt;
	std::vector<unsigned int> indices;

	float stackAngle = -PI_OVER_2 + stackInc;

	lower = upper; // Assumes initializeBottomSubMesh was previously called

	lower = createStackVertexData(0.0f, stackAngle);

	for (int j = 0; j < stacks-2; j++) {

		stackAngle += stackInc;

		upper = createStackVertexData(0.0f, stackAngle);

		for (int i = 0; i < slices; i++) {

			pnt.push_back(upper[i]);
			pnt.push_back(lower[i]);
			pnt.push_back(lower[(i + 1)]);

			pnt.push_back(upper[i]);
			pnt.push_back(lower[(i + 1)]);
			pnt.push_back(upper[(i + 1)]);
		}

		lower = upper;
	}

	SubMesh subMesh = buildSubMesh(pnt);

	// Set the primitive mode for the vertex data
	subMesh.primitiveMode = GL_TRIANGLES;

	subMesh.material = sphereMat;

	// Push the submesh into vector of Submeshes to be rendered
	this->subMeshes.push_back(subMesh);

} // end initializeBodySubMesh


std::vector<pntVertexData> SphereMeshComponent::createStackVertexData(float startingSliceAngle, float stackAngle)
{
	std::vector<pntVertexData> stackVertexData;

	float sliceAngle = startingSliceAngle;

	for (int i = 0; i <= slices; i++) {

		vec4 v0 = sphericalToCartesian(sliceAngle, stackAngle, radius);
		vec2 t0 = getSphericalTextCoords(sliceAngle, stackAngle);

		stackVertexData.push_back( pntVertexData(v0, glm::normalize(v0).xyz, t0) );

		sliceAngle += sliceInc;
	}

	return stackVertexData;
}
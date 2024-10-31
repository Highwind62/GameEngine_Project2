#include "CylinderMeshComponent.h"


vec2 getCylindericalTextCoords(float theta, vec3 vertex, float height)
{
	float s = theta / (2.0f * PI);

	float t = (vertex.y+height/2) / height;

	return vec2(s, t);
}


CylinderMeshComponent::CylinderMeshComponent(GLuint shaderProgram, Material mat, float radius, float height, int stacks, int slices, int updateOrder)
	: MeshComponent(shaderProgram, updateOrder), cylinderMat(mat), radius(radius), stacks(stacks), slices(slices), height(height)
{
	sliceInc = (2.0f * PI) / slices;
	stackInc = height / stacks;

}

void CylinderMeshComponent::buildMesh()
{
	this->scaleMeshName = "Cylinder " + std::to_string(radius) + + " " + std::to_string(height) + " " + std::to_string(this->cylinderMat._id);

	if (previsouslyLoaded() == false) {

		initializeTopSubMesh();

		initializeBodySubMesh();

		initializeBottomSubMesh();

		// Create a collision shape that matches the cylinder
		// Bullet does not have a Y alined cylinder collision shape. 
		// Cylinder needs to be refactored. For not approximating with a box.
		// https://pybullet.org/Bullet/BulletFull/classbtConvexShape.html
		this->collisionShape = new btBoxShape(btVector3(radius, height / 2.0f, radius));

		saveInitialLoad();
	}

}

void CylinderMeshComponent::initializeTopSubMesh()
{
	std::vector<pntVertexData> pnt;

	float sliceAngle = glm::radians(360.0f);

	vec4 top = vec4(0.0f, height / 2.0f, 0.0f, 1.0f);
	vec4 v = vec4(cos(sliceAngle) * radius, height / 2.0f, sin(sliceAngle) * radius, 1.0f);

	pntVertexData pntTop(top, UNIT_Y_V3, vec2(0.5f, 0.5f));
	pntVertexData pntV(v, UNIT_Y_V3, vec2(0.5f * cos(sliceAngle) + 0.5, 0.5f * sin(sliceAngle) + 0.5));

	pnt.push_back(pntTop);
	pnt.push_back(pntV);

	//cout << glm::degrees(sliceAngle) << ", " << pntV.m_textCoord << endl;

	for (int j = 0; j < slices; j++) {

		sliceAngle -= sliceInc;

		v = vec4(cos(sliceAngle) * radius, height / 2.0f, sin(sliceAngle) * radius, 1.0f);

		pntVertexData pntV0(v, UNIT_Y_V3, vec2(0.5f * cos(sliceAngle) + 0.5, 0.5f * sin(sliceAngle) + 0.5));

		pnt.push_back(pntV0);

		//cout << glm::degrees(sliceAngle) << ", " << pntV.m_textCoord << endl;
	}

	//cout << pntV.m_textCoord << endl;

	SubMesh subMesh = buildSubMesh(pnt);

	// Set the primitive mode for the vertex data
	subMesh.primitiveMode = GL_TRIANGLE_FAN;

	subMesh.material = cylinderMat;

	// Push the submesh into vector of Submeshes to be rendered
	this->subMeshes.push_back(subMesh);


}
void CylinderMeshComponent::initializeBottomSubMesh()
{
	std::vector<pntVertexData> pnt;

	float sliceAngle = glm::radians(360.0f);

	vec4 bottom = vec4(0.0f, -height / 2.0f, 0.0f, 1.0f);
	vec4 v = vec4(cos(sliceAngle) * radius, -height / 2.0f, sin(sliceAngle) * radius,  1.0f);


	pntVertexData pntbottom(bottom, -UNIT_Y_V3, vec2(0.5f, 0.5f));
	pntVertexData pntV(v, -UNIT_Y_V3, vec2(0.5f * cos(sliceAngle) + 0.5, 0.5f * sin(sliceAngle) + 0.5));

	pnt.push_back(pntbottom);
	pnt.push_back(pntV);

	for (int j = 0; j < slices; j++) {

		sliceAngle += sliceInc;

		v = vec4(cos(sliceAngle) * radius, -height / 2.0f, sin(sliceAngle) * radius, 1.0f);

		pntVertexData pntV0(v, -UNIT_Y_V3, vec2(0.5f * cos(sliceAngle) + 0.5, 0.5f * sin(sliceAngle) + 0.5));

		pnt.push_back(pntV0);
	}

	SubMesh subMesh = buildSubMesh(pnt);

	// Set the primitive mode for the vertex data
	subMesh.primitiveMode = GL_TRIANGLE_FAN;

	subMesh.material = cylinderMat;

	//subMesh.material.diffuseTextureEnabled = false;

	// Push the submesh into vector of Submeshes to be rendered
	this->subMeshes.push_back(subMesh);

}
void CylinderMeshComponent::initializeBodySubMesh()
{
	std::vector<pntVertexData> pnt;

	float stackCounter = -height / 2.0f;

	for (int j = 0; j < stacks; j++) {

		//cout << stackCounter << endl;
		float sliceAngle = 0.0f;

		vec4 v0 = vec4(cos(sliceAngle) * radius, stackCounter + stackInc, sin(sliceAngle) * radius,  1.0f);
		vec4 v1 = vec4(cos(sliceAngle) * radius, stackCounter, sin(sliceAngle) * radius,  1.0f);

		pntVertexData pnt0(v0, glm::normalize(vec3(v0.x, 0.0f, v0.z)), getCylindericalTextCoords(sliceAngle,v0.xyz,height));
		pntVertexData pnt1(v1, glm::normalize(vec3(v1.x, 0.0f, v1.z)), getCylindericalTextCoords(sliceAngle, v1.xyz, height));

		for (int i = 0; i < slices; i++) {

			sliceAngle += sliceInc;

			//cout << glm::degrees(sliceAngle) << ", ";

			vec4 v2 = vec4(cos(sliceAngle) * radius, stackCounter, sin(sliceAngle) * radius,  1.0f);
			vec4 v3 = vec4(cos(sliceAngle) * radius, stackCounter + stackInc, sin(sliceAngle) * radius, 1.0f);

			pntVertexData pnt2(v2, glm::normalize(vec3(v2.x, 0.0f, v2.z)), getCylindericalTextCoords(sliceAngle, v2.xyz, height));
			pntVertexData pnt3(v3, glm::normalize(vec3(v3.x, 0.0f, v3.z)), getCylindericalTextCoords(sliceAngle, v3.xyz, height));

			pnt.push_back(pnt0);
			pnt.push_back(pnt3);
			pnt.push_back(pnt1);

			pnt.push_back(pnt1);
			pnt.push_back(pnt3);
			pnt.push_back(pnt2);

			pnt0 = pnt3;
			pnt1 = pnt2;
		}

		//cout << endl;
		stackCounter += stackInc;
	}

	SubMesh subMesh = buildSubMesh(pnt);

	// Set the primitive mode for the vertex data
	subMesh.primitiveMode = GL_TRIANGLES;

	subMesh.material = cylinderMat;

	// Push the submesh into vector of Submeshes to be rendered
	this->subMeshes.push_back(subMesh);

}
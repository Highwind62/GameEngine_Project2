#pragma once
#include "MeshComponent.h"
class CylinderMeshComponent : public MeshComponent
{
public:
	CylinderMeshComponent(GLuint shaderProgram, Material mat, 
		float radius = 1.0f, float height = 2.0f, int stacks = 12,
		int slices = 16, int updateOrder = 100);

protected:

	virtual void buildMesh();

	void initializeTopSubMesh();
	void initializeBottomSubMesh();
	void initializeBodySubMesh();

	float sliceInc, stackInc, radius, height;
	int slices, stacks;
	Material cylinderMat;

};


#pragma once
#include "MeshComponent.h"
#include <vector>

class SphereMeshComponent : public MeshComponent
{
public:
	SphereMeshComponent(GLuint shaderProgram, Material mat, float radius = 1.0f, int stacks = 12, 
						int slices = 16, int updateOrder = 100);

	virtual void buildMesh() override;

protected:

	float sliceInc, stackInc, radius;
	int slices, stacks;
	Material sphereMat;

	void initializeTopSubMesh();
	void initializeBottomSubMesh();
	void initializeBodySubMesh();

	std::vector<pntVertexData> createStackVertexData(float startingSliceAngle, float stackAngle);

	std::vector<pntVertexData> upper;

	std::vector<pntVertexData> lower;

	std::vector<unsigned int> indices;

	//int upperBase = 0;
	//int lowerBase = 0;
};


#pragma once
#include "MeshComponent.h"

class BoxMeshComponent : public MeshComponent
{
public:
	BoxMeshComponent(GLuint shaderProgram, Material material, float width = 1.0f, float height = 1.0f, float depth = 1.0f, int updateOrder = 100);

	virtual void buildMesh() override;

protected:

	float halfWidth, halfHeight, halfDepth;

	Material boxMat;

}; // end BoxMeshComponent class


#pragma once
#include <string>
#include <unordered_map>

#include "MathLibsConstsFuncs.h"
#include "Component.h"
#include "Material.h"
#include "Bullet/btBulletDynamicsCommon.h"

using namespace constants_and_types;

/**
 * @struct	pntVertexData
 *
 * @brief	Structure for holding vertex data for a single vertex.
 */
struct pntVertexData
{
	// Position of the vertex in Object coordinates
	glm::vec4 m_pos;

	// Normal vector for the vertex in Object coordinates
	glm::vec3 m_normal;

	// 2D vertex texture coordinates
	glm::vec2 m_textCoord;

	// Normal Mapping
	glm::vec3 m_tangent;
	glm::vec3 m_bitangent;

	pntVertexData(glm::vec4 pos = ZERO_V4,
		glm::vec3 normal = ZERO_V3,
		glm::vec2 textCoord = ZERO_V2,
		glm::vec3 tangent = ZERO_V3, glm::vec3 bitangent = ZERO_V3)
	{
		m_pos = pos;
		m_normal = normal;
		m_textCoord = textCoord;

		// Normal mapping
		m_tangent = tangent;
		m_bitangent = bitangent;
	}
};


/**
 * @enum	RENDER_MODE
 *
 * @brief	Values that represent render modes to be used when a sub-mesh if rendered.
 */
enum RENDER_MODE { ORDERED, INDEXED };

/**
 * @struct	SubMesh
 *
 * @brief	stuct containing values needed to render a sub-mesh. All visible objects
 * 			(Meshes) are rendered using one or more sub-meshes.
 */
struct SubMesh {

	GLuint vao = GL_INVALID_VALUE; // ID for Vertex Array Object for the sub-mesh

	GLuint vertexBuffer = GL_INVALID_VALUE; // ID for vertex data buffer for the sub-mesh

	GLuint indexBuffer = GL_INVALID_VALUE; // ID for index buffer for the sub-mesh (if indexed rendering is used)

	GLuint count = 0; // Either the number of vertices in the mesh or the number of indices

	RENDER_MODE renderMode = INDEXED; // Render mode for the mesh. Either ORDERED or INDEXED

	GLenum primitiveMode = GL_TRIANGLES; // Primitive mode for the mesh GL_POINTS, GL_LINES, etc.

	Material material;  // Material properties used to render the object

}; // end SubMesh

/**
 * @struct	BaseMeshLoad
 *
 * @brief	Used to keep track of what models have been loaded in order to 
 * 			avoid loading the same model multiple times.
 */
struct BaseMeshLoad {

	std::vector<SubMesh> modelSubMeshes;

	btCollisionShape* collisionShape;

	int copyCount = 0;
};


/**
 * @class	Mesh
 *
 * @brief	Base class for all objects that will appear visually in the scene. All meshes
 * 			are rendered using one or more sub-meshes.
 */
class MeshComponent : public Component
{
public: 

	/**
	 * @fn	MeshComponent::MeshComponent(GLuint shaderProgram, int updateOrder = 100)
	 *
	 * @brief	Constructor
	 *
	 * @param 	shaderProgram	Shader program that will be used to render the
	 * 							mesh.
	 * @param 	updateOrder  	(Optional) The update order of the component.
	 */
	MeshComponent(GLuint shaderProgram, int updateOrder = 100)
		: shaderProgram(shaderProgram), Component(updateOrder)
	{
		componentType = MESH;
	};

	/**
	 * @fn	MeshComponent::virtual~MeshComponent();
	 *
	 * @brief	Destructor. Deletes all dynamically allocated data.
	 */
	virtual~MeshComponent();

	/**
	 * @fn	virtual void MeshComponent::buildMesh() = 0;
	 *
	 * @brief	Buffers all the data that is associated with the vertex data for
	 * 			this object. Creates a Vertex Array Object that holds references
	 * 			to the vertex buffers, the index buffer and the layout
	 * 			specification. This is a pure virtual method and must be
	 * 			overridden.
	 */
	virtual void buildMesh() = 0;

	/**
	 * @fn	virtual void MeshComponent::draw() const;
	 *
	 * @brief	Renders all sub-meshes that are part of the object. Binds the
	 * 			vertex array object, sets the material properties, and sets the
	 * 			modeling transformation based on the world transformation of the
	 * 			owning game object.
	 */
	virtual void draw() const;

	/**
	 * @fn	static void MeshComponent::addMeshComp(std::shared_ptr<class MeshComponent> meshComponent);
	 *
	 * @brief	Adds a mesh component to the Game
	 *
	 * @param 	meshComponent	If non-null, the mesh.
	 */
	static void addMeshComp(std::shared_ptr<class MeshComponent> meshComponent);

	/**
	 * @fn	static void MeshComponent::removeMeshComp(std::shared_ptr<class MeshComponent> meshComponent);
	 *
	 * @brief	Removes the mesh component from the Game
	 *
	 * @param 	meshComponent	If non-null, the mesh.
	 */
	static void removeMeshComp(std::shared_ptr<class MeshComponent> meshComponent);

	/**
	 * @fn	btCollisionShape* MeshComponent::getCollisionShape() const
	 *
	 * @brief	Gets collision shape for this mesh.
	 *
	 * @returns	Null if it fails, else the collision shape.
	 */
	btCollisionShape* getCollisionShape() const { return this->collisionShape; }

	/**
	 * @fn	static const std::vector<std::shared_ptr<class MeshComponent>> MeshComponent::GetMeshComponents();
	 *
	 * @brief	Gets mesh components
	 *
	 * @returns	Null if it fails, else the vector containing all mesh components
	 * 			that should be rendered.
	 */
	static const std::vector<std::shared_ptr<class MeshComponent>> & GetMeshComponents();

protected:

	/**
	 * @fn	SubMesh MeshComponent::buildSubMesh(const std::vector<pntVertexData>& vertexData);
	 *
	 * @brief	Builds one sub mesh  that will be rendered using sequential
	 * 			rendering based the vertex data that are passed to it. The vertex
	 * 			data is loaded into a buffer located in GPU memory.
	 *
	 * @param 	vertexData	Information describing the vertex.
	 *
	 * @returns	A SubMesh.
	 */
	SubMesh buildSubMesh(const std::vector<pntVertexData>& vertexData);

	/**
	 * @fn	SubMesh MeshComponent::buildSubMesh(const std::vector<pntVertexData>& vertexData, const std::vector<unsigned int>& indices);
	 *
	 * @brief	Builds one sub mesh  that will be rendered using indexed
	 * 			rendering based the vertex data, indices, and material properties
	 * 			that are passed to it. Both the vertex data and the indices are
	 * 			loaded into buffers located in GPU memory.
	 *
	 * @param 	vertexData	Information describing the vertex.
	 * @param 	indices   	indices that will be used for indexed rendering.
	 *
	 * @returns	A SubMesh.
	 */
	SubMesh buildSubMesh(const std::vector<pntVertexData>& vertexData, const std::vector<unsigned int>& indices);

	/** @brief	Indentifier for the shader program used to render all sub-meshes (Design
	 would have to incorporate the shader program into the SubMesh struct to support using
	 different shader programs for different parts of the same object. */
	GLuint shaderProgram = 0; 

	/** @brief	Container for all sub meshes that are part of this MeshComponent.*/
	std::vector<SubMesh> subMeshes;

	/**
	 * @class	btCollisionShape*
	 *
	 * @brief	Pointer to bt collision shape that can be used by the Physics
	 * 			Engine for collision detection. The collision shape is based on
	 * 			vertex data in the rendered sub meshes.
	 */
	class btCollisionShape* collisionShape = nullptr;

	/** @brief	Name of model that includes the scale. One
	copy of each model will be loaded for specified scale */
	string scaleMeshName;

	/************** Static data members used by the Game to manage MeshComponents **********/

	void listLoadedMeshes();

	bool previsouslyLoaded();

	void saveInitialLoad();

	/** @brief	All mesh components that need to be rendered. */
	static std::vector<std::shared_ptr<class MeshComponent>> meshComps;

	/** @brief	Map of ALL meshes that have been loaded previously.*/
	static std::unordered_map<std::string, BaseMeshLoad> loadedModels;

}; // end MeshComponent class





#pragma once

#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh() : m_triCount(0), m_vao(0), m_vbo(0), m_ibo(0) {}
	virtual ~Mesh();

	struct Vertex 
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	void InitialiseQuad();
	void InitialiseFullscreenQuad(); // For Post Processing
	void Initialise(unsigned int vertCount, const Vertex* vertices, 
		unsigned int indexCount = 0, unsigned int* indices = nullptr); // For Implementing a Primitive
	void InitialiseCube();
	virtual void Draw();

protected:
	unsigned int m_triCount;
	unsigned int m_vao; // Vertex Array Object
	unsigned int m_vbo; // Vertex Buffer Object
	unsigned int m_ibo; // Index Buffer Object
};


#pragma once
#include "Application.h"

#include "Shader.h"
#include "Mesh.h"
#include "Space.h"

#include <glm/mat4x4.hpp>

class aieProject3D1App : public aie::Application {
public:

	aieProject3D1App();
	virtual ~aieProject3D1App();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	bool LaunchShaders();

	Space* space;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::ShaderProgram	m_simpleShader;
	Mesh				m_quadMesh;
	glm::mat4			m_quadTransform;
};
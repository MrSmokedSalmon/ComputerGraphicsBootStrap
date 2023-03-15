#pragma once
#include "Application.h"

#include "Shader.h"
#include "Mesh.h"
#include "Space.h"
#include "OBJMesh.h"


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
	void ImGUIRefresher();

	bool QuadLoader();
	bool BunnyLoader();
	bool PhongLoader();

	void QuadDraw(glm::mat4 pvm);
	void BunnyDraw(glm::mat4 pvm, float time);
	void PhongDraw(glm::mat4 pvm, glm::mat4 transform);

	Space* space;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::ShaderProgram	m_simpleShader;
	aie::ShaderProgram	m_colorShader;
	aie::ShaderProgram	m_phongShader;

	Mesh				m_quadMesh;
	glm::mat4			m_quadTransform;

	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTransform;

	struct Light {
		glm::vec3 direction;
		glm::vec3 color;
	};

	Light m_light;
	glm::vec3 m_ambientLight;
};
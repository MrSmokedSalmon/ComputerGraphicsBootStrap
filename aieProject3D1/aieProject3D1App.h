#pragma once

#define NR_POINT_LIGHTS 1
#define PI 3.14159265359

#include "Application.h"

#include "Shader.h"
#include "Mesh.h"
#include "Space.h"
#include "OBJMesh.h"

#include "FlyCamera.h"
#include "StationaryCamera.h"

#include <glm/mat4x4.hpp>

class aieProject3D1App : public aie::Application{
public:

	aieProject3D1App();
	virtual ~aieProject3D1App();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	bool LaunchShaders();

	void CreatePlane(Mesh& mesh);
	void CreateCube(Mesh& mesh);
	void CreateSphere(Mesh& mesh);
	Mesh::Vertex* CreateCircle(int point);

	bool QuadLoader();
	bool BunnyLoader();
	bool PhongLoader();
	bool QuadTextureLoader();

	void QuadDraw(glm::mat4 pvm);
	void BunnyDraw(glm::mat4 pvm, float time);
	void PhongDraw(glm::mat4 pvm, glm::mat4 transform);
	void QuadTexturedDraw(glm::mat4 pvm);

	void ImGUIRefresher();
	void ImGUIPointLight(int number);

	//Space* space;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture m_gridTexture;

	aie::ShaderProgram	m_simpleShader;
	aie::ShaderProgram	m_colorShader;
	aie::ShaderProgram	m_phongShader;
	aie::ShaderProgram	m_texturedShader;

	Mesh				m_quadMesh;
	glm::mat4			m_quadTransform;

	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTransform;

	struct Light {
		glm::vec3 direction;
		glm::vec3 color;
		float intensity;
	};
	struct PointLight {
		glm::vec3 position;

		float constant;
		float linear;
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	Light m_light;
	glm::vec3 m_ambientLight;

	PointLight* pointLight = new PointLight
	{
		glm::vec3(4,1,4),
		1.f, 0.35f, 0.44f,
		glm::vec3(1),
		glm::vec3(1),
		glm::vec3(1)
	};

	PointLight pointLights[1];

	bool enablePoints;

	glm::vec3 v_ambient = glm::vec3(51.f);
	glm::vec3 v_diffuse = glm::vec3(255.f);
	glm::vec3 v_specular = glm::vec3(204.f);
	float v_specularStrength = 320.f;

	glm::mat4 PointToMatEncode(PointLight& light);

	//FlyCamera m_camera;
	StationaryCamera m_camera;
	glm::vec3 stationaryRotation;
};
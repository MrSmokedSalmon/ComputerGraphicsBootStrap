#pragma once

#define PI 3.14159265359

#include "Application.h"

#include "Shader.h"
#include "Mesh.h"
#include "Space.h"
#include "OBJMesh.h"
#include "ParticleEmitter.h"

#include "FlyCamera.h"
#include "StationaryCamera.h"

#include <glm/glm.hpp>

#include "Scene.h"
#include "Instance.h"

#include "RenderTarget.h"

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
	bool QuadTextureLoader();

	bool OBJLoader(const char* filePath, aie::OBJMesh &mesh, glm::mat4 &transform, bool loadTextures, glm::vec3 scale = glm::vec3(1));

	void QuadDraw(glm::mat4 pvm);
	void OBJDraw(glm::mat4& pv, glm::mat4& transform, aie::OBJMesh* objMesh);
	void QuadTexturedDraw(glm::mat4 pvm);

	void ImGUIRefresher();



	Scene* m_scene;

	//Space* space;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture m_gridTexture;

	aie::ShaderProgram	m_simpleShader;
	aie::ShaderProgram	m_colorShader;
	aie::ShaderProgram	m_phongShader;
	aie::ShaderProgram	m_texturedShader;
	aie::ShaderProgram	m_normalLitShader;
	aie::ShaderProgram	m_particleShader;

	aie::ShaderProgram	m_postProcessShader;
	int m_ppEffect = -1;

	aie::RenderTarget m_renderTarget;

	Mesh				m_quadMesh;
	Mesh				m_fullScreenQuad;
	glm::mat4			m_quadTransform;

	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTransform;

	aie::OBJMesh		m_spearMesh;
	glm::mat4			m_spearTransform;

	glm::vec3			m_ambientLight;

	glm::vec2			m_mousePos;

	int					m_blurAmount = 5;
	int					m_distortAmount = 15;
	float				m_edgeAmount = 2.0f;

	FlyCamera			m_camera;
	//StationaryCamera m_camera;
	//glm::vec3 stationaryRotation;

	ParticleEmitter*	m_emitter;
	glm::mat4			m_particleEmitTransform;
};
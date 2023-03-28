#include "aieProject3D1App.h"
#include "Gizmos.h"
#include "Input.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui.h>
#include <string>

#define PPE_BLUR_MAX 15
#define PPE_BLUR_MIN 0
#define PPE_DISTORT_MAX 45
#define PPE_DISTORT_MIN 0
#define PPE_EDGE_MAX 20.0f
#define PPE_EDGE_MIN 0.0f

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;
//using namespace std;

aieProject3D1App::aieProject3D1App() {
	
}

aieProject3D1App::~aieProject3D1App() {

}

bool aieProject3D1App::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_camera.SetViewMatrix(m_camera.GetPosition(), m_camera.GetPosition() + glm::vec3(1,0,0), {0,1,0});
	m_camera.SetProjectionMatrix(0.25f, getWindowWidth(), getWindowHeight(), 0.1f, 1000.f);

	m_ambientLight = { 20.5f, 0.5f, 0.5f };

	Light light;
	light.color = { 1,1,1 };
	light.direction = { 1,-1,1 };

	m_emitter = new ParticleEmitter();
	m_emitter->Initialise(1000, 500, 0.1f, 1.0f, 
		1, 5, 1, 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1));

	m_scene = new Scene(&m_camera, glm::vec2(getWindowWidth(), getWindowHeight()),
		light, m_ambientLight);

	
	m_scene->AddPointLights({ 5,3,0 }, { 1,0,0 }, 1);
	m_scene->AddPointLights({ 3,3,0 }, { 0,0,1 }, 1);
	
	return LaunchShaders();
}

void aieProject3D1App::shutdown() {
	
	Gizmos::destroy();
	delete m_scene;
}

void aieProject3D1App::update(float deltaTime) {
	
	float time = getTime();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	m_mousePos = glm::vec2(input->getMouseX(), input->getMouseY());

	m_bunnyTransform = glm::rotate(m_bunnyTransform, glm::radians<float>(0.1f), glm::vec3(0, 1, 0));


	// Rotate the light to emulate a 'day/night' cycle
	m_scene->GetLight().direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	m_camera.Update(deltaTime);
	m_emitter->Update(deltaTime, m_scene->GetCamera()->GetTransform(
			m_camera.GetPosition(), glm::vec3(0), glm::vec3(1)));

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	ImGUIRefresher();
}

void aieProject3D1App::draw() 
{
	auto pv = m_camera.GetProjectionViewMatrix();

	float time = getTime();

	// Renders a distance texture to a texture buffer to be used in further rendering techniques
	m_distanceBuffer.bind();

	clearScreen();

	m_distanceShader.bind();
	m_distanceShader.bindUniform("ProjectionViewModel", pv);

	m_distanceBuffer.unbind();

	m_renderTarget.bind();

	// wipe the screen to the background colour
	clearScreen();

	m_scene->Draw();

	m_particleShader.bind();
	m_particleShader.bindUniform("ProjectionViewModel", pv * m_particleEmitTransform);
	m_emitter->Draw();

	Gizmos::draw(pv);

	m_renderTarget.unbind();

	clearScreen();

	// Bind the Post-Processing Shader and the texture
	m_postProcessShader.bind();
	m_postProcessShader.bindUniform("colorTarget", 0);
	m_postProcessShader.bindUniform("postProcessTarget", m_ppEffect);
	m_postProcessShader.bindUniform("ProjectionViewModel", pv);

	m_postProcessShader.bindUniform("windowWidth", (int)getWindowWidth());
	m_postProcessShader.bindUniform("windowHeight", (int)getWindowHeight());

	m_postProcessShader.bindUniform("blurAmount", m_blurAmount);
	m_postProcessShader.bindUniform("distortAmount", m_distortAmount);
	m_postProcessShader.bindUniform("edgeAmount", m_edgeAmount);
	m_renderTarget.getTarget(0).bind(0);

	

	m_fullScreenQuad.Draw();
}

bool aieProject3D1App::LaunchShaders()
{
	if (m_renderTarget.initialise(1, getWindowWidth(),
		getWindowHeight()) == false)
	{
		printf("Render Target Error!:\n");
		return false;
	}

#pragma region LaunchShader

	m_normalLitShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/normalLit.vert");
	m_normalLitShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/normalLit.frag");

	if (m_normalLitShader.link() == false)
	{
		printf("Normal Lit Shader Error: %s\n", m_normalLitShader.getLastError());
		return false;
	}

	m_postProcessShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/post.vert");
	m_postProcessShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/post.frag");

	// Post-Processing Shader
	if (m_postProcessShader.link() == false)
	{
		printf("Post-Process Shader Error: %s\n", m_postProcessShader.getLastError());
		return false;
	}

	m_particleShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/particle.vert");
	m_particleShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/particle.frag");

	if (m_particleShader.link() == false)
	{
		printf("Particle Shader Error: %s\n", m_particleShader.getLastError());
		return false;
	}
#pragma endregion
	
	m_particleEmitTransform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	if (!QuadTextureLoader())
		return false;

	// Create a full screen quad
	m_fullScreenQuad.InitialiseFullscreenQuad();


	if (!OBJLoader("./soulspear/soulspear.obj", m_spearMesh, m_spearTransform, true))
		return false;

	// Adding the multiple instances of the spears
	for (int i = 0; i < 10; i++)
		m_scene->AddInstance(new Instance(glm::vec3(i * 2, 0, 0),
			glm::vec3(0, i * 30, 0), glm::vec3(1),
			&m_spearMesh, &m_normalLitShader));

	return true;
}

void aieProject3D1App::CreatePlane(Mesh& mesh)
{
	Mesh::Vertex vertices[4];

	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	vertices[3].position = { 0.5f, 0, -0.5f, 1 };


	unsigned int indices[6] =
	{
		0,1,2,
		2,1,3
	};

	mesh.Initialise(4, vertices, 6, indices);
}

void aieProject3D1App::CreateCube(Mesh& mesh)
{
	Mesh::Vertex vertices[8];

	vertices[0].position = { -0.5f, -0.5f, 0.5f, 1 };
	vertices[1].position = { 0.5f, -0.5f, 0.5f, 1 };
	vertices[2].position = { -0.5f, -0.5f, -0.5f, 1 };
	vertices[3].position = { 0.5f, -0.5f, -0.5f, 1 };
	vertices[4].position = { -0.5f, 0.5f, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0.5f, 0.5f, 1 };
	vertices[6].position = { -0.5f, 0.5f, -0.5f, 1 };
	vertices[7].position = { 0.5f, 0.5f, -0.5f, 1 };

	unsigned int indices[36] =
	{
		2,1,0,
		3,1,2,
		0,1,4,
		1,5,4,
		1,3,5,
		7,5,3,
		3,2,7,
		6,7,2,
		2,0,6,
		4,6,0,
		4,5,6,
		6,5,7
	};

	mesh.Initialise(8, vertices, 36, indices);
}

void aieProject3D1App::CreateSphere(Mesh& mesh)
{
	Mesh::Vertex vertices[66];

	float angleX = 0;
	for (int x = 0; x < 8; x++) 
	{

		float angleY = 0;
		for (int y = 0; y < 8; y++)
		{
			vertices[x * 8 + y].position =
			{
				glm::sin(angleX * PI / 180)/2.f,
				glm::sin(angleY * PI / 180)/2.f,
				glm::cos(angleX * PI / 180)/2.f, 
				1
			};

			angleY += 360.0f / 8.f;
		}
		angleX += 360.0f / 8.f;
	}

	unsigned int indices[36] =
	{
		2,1,0,
		3,1,2,
		0,1,4,
		1,5,4,
		1,3,5,
		7,5,3,
		3,2,7,
		6,7,2,
		2,0,6,
		4,6,0,
		4,5,6,
		6,5,7
	};

	mesh.Initialise(8, vertices, 36, indices);
}

Mesh::Vertex* aieProject3D1App::CreateCircle(int point)
{
	Mesh::Vertex vertices[9];

	vertices[8].position = {0,0,0,0};
	for (int i = 0; i < 8; i++)
	{

	}

	return vertices;
}

void aieProject3D1App::ImGUIRefresher()
{


	ImGui::Begin("Light Settings");
	{
		if (ImGui::CollapsingHeader("Test"))

		{
			ImGui::DragFloat3("Position",
				&m_camera.GetPosition()[0]);
			//ImGui::DragFloat3("Rotation",
			//	&stationaryRotation[0], 0, 0, 360);
			//if (ImGui::Button("Set Rotation", 
			//	{ 10,10 }))
			//{
			//	m_camera.SetRotation(stationaryRotation);
			//}
			ImGui::DragFloat("Move Speed",
				&m_camera.m_moveSpeed, 0.1, 0.1, 10);
			ImGui::DragFloat("Sensitivity",
				&m_camera.m_turnSpeed, 0.01, 0, 10);
		}

		if (ImGui::CollapsingHeader("Test2"))
		{
			ImGui::DragFloat3("Global Light Direction",
				&m_scene->GetLight().direction[0], 0.1, 0, 1);
			ImGui::DragFloat3("Global Light Color",
				&m_scene->GetLight().color[0], 1, 0, 255);
			ImGui::DragFloat3("Ambient Light Color",
				&m_ambientLight[0], 1, 0, 255);
		}

		if (ImGui::CollapsingHeader("Post Processing"))
		{
			{ // No Post-Processing Effect
				if (ImGui::Button("Default", { 100, 25 }))
				{
					m_ppEffect = -1;
				}
			}

			{ // Blur Effect UI
				if (ImGui::Button("Blur", { 100, 25 }))
				{
					m_ppEffect = 0;
				}
				if (m_ppEffect == 0 && ImGui::CollapsingHeader("Blur Settings"))
				{
					ImGui::SliderInt("Blur Amount", &m_blurAmount, PPE_BLUR_MIN, PPE_BLUR_MAX);
				}
			}

			{ // Distort Effect UI
				if (ImGui::Button("Distort", { 100, 25 }))
				{
					m_ppEffect = 1;
				}
				if (m_ppEffect == 1 && ImGui::CollapsingHeader("Distort Settings"))
				{
					ImGui::SliderInt("Distort Amount", &m_distortAmount, PPE_DISTORT_MIN, PPE_DISTORT_MAX);
				}
			}

			{ // Edge Detection Effect UI
				if (ImGui::Button("Edge Detection", { 100, 25 }))
				{
					m_ppEffect = 2;
				}
				if (m_ppEffect == 2 && ImGui::CollapsingHeader("Edge Detection Settings"))
				{
					ImGui::SliderFloat("Edge Detection Amount", &m_edgeAmount, PPE_DISTORT_MIN, PPE_DISTORT_MAX);
				}
			}

			ImGui::InputInt("Processing Effect",
				&m_ppEffect, 1);
		}
	}
	ImGui::End();
}

bool aieProject3D1App::QuadLoader()
{
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");

	if (m_simpleShader.link() == false)
	{
		printf("Simple Shader has an Error: %s\n", m_simpleShader.getLastError());
		return false;
	}

	CreatePlane(m_quadMesh);

	// This is a 'unit' wide quad
	m_quadTransform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	return true;
}

bool aieProject3D1App::OBJLoader(const char* filePath, aie::OBJMesh& mesh, 
	glm::mat4& transform, bool loadTextures, glm::vec3 scale)
{
	if (mesh.load(filePath, loadTextures, loadTextures) == false)
	{
		printf("Mesh Error:\n");
		return false;
	}

	transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	return true;
}

bool aieProject3D1App::QuadTextureLoader() 
{
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert"); 
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag"); 
	if (m_texturedShader.link() == false) 
	{ 
		printf("textured has an Error: %s\n", m_texturedShader.getLastError()); 
		return false; 
	} if (m_gridTexture.load("./textures/numbered_grid.tga") == false) 
	{ 
		printf("Failed to load the grid texture correctly!\n"); 
	} 
	m_quadMesh.InitialiseQuad(); 
	// This is a 10 'unit' wide quad 
	m_quadTransform = { 
		10, 0, 0, 0, 
		0, 10, 0, 0, 
		0, 0, 10, 0, 
		0, 0, 0, 1 
	}; 
	return true;
}

void aieProject3D1App::QuadDraw(glm::mat4 pvm)
{
	// Bind the shader
	m_simpleShader.bind();

	// Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	// Draw the quad using Mesh's draw
	m_quadMesh.Draw();
}

void aieProject3D1App::QuadTexturedDraw(glm::mat4 pvm)
{
	// Bind the shader 
	m_texturedShader.bind(); 
	// Bind the transform 

	m_texturedShader.bindUniform("ProjectionViewModel", pvm); 
	// Bind the texture 

	m_texturedShader.bindUniform("diffuseTexture", 0); 
	// Bind the texture to a specific location 
	//m_gridTexture.bind(0); 
	m_renderTarget.getTarget(0).bind(0); 

	// Draw the quad using Mesh's draw 
	m_quadMesh.Draw();
}

void aieProject3D1App::OBJDraw(glm::mat4& pv, glm::mat4& transform, aie::OBJMesh* objMesh)
{
	// Bind the shader
	m_normalLitShader.bind();

	// Bind the camera position
	m_normalLitShader.bindUniform("CameraPosition",
		m_camera.GetPosition());

	// Bind the light
	m_normalLitShader.bindUniform("LightDirection", {1,-1,1});
	m_normalLitShader.bindUniform("LightColor", {1,1,1});
	m_normalLitShader.bindUniform("AmbientColor", m_ambientLight);

	// bind texture

	// Bind the transform
	m_normalLitShader.bindUniform("ProjectionViewModel", pv * transform);

	// Bind the model matrix
 	m_normalLitShader.bindUniform("ModelMatrix", transform);

	// Draw the quad using Mesh's draw
	objMesh->draw();
}
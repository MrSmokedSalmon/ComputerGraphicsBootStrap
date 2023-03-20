#include "aieProject3D1App.h"
#include "Gizmos.h"
#include "Input.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui.h>
#include <string>

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

	m_light.color = { 1, 1, 0 };
	m_light.intensity = 1;
	m_ambientLight = { 0.5f, 0.5f, 0.5f };


	//https://learnopengl.com/Lighting/Multiple-lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		PointLight newPL;
		newPL = *(new PointLight());
		newPL.position = glm::vec3(0);
		pointLights[i] = newPL;
	}
	
	//space = new Space();

	return LaunchShaders();
}

void aieProject3D1App::shutdown() {

	Gizmos::destroy();
}

void aieProject3D1App::update(float deltaTime) {
	
	float time = getTime();

	//space->Update(time);

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

	m_quadTransform = glm::rotate(m_quadTransform, glm::radians<float>(0.1f), glm::vec3(0, 1, 0));
	m_bunnyTransform = glm::rotate(m_bunnyTransform, glm::radians<float>(0.1f), glm::vec3(0, 1, 0));

	// Rotate the light to emulate a 'day/night' cycle
	m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	m_camera.Update(deltaTime);

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	ImGUIRefresher();
}

void aieProject3D1App::draw() 
{
	// wipe the screen to the background colour
	clearScreen();

	//space->Draw();

	// update perspective based on screen size
	m_viewMatrix = m_camera.GetViewMatrix();

	//m_projectionMatrix = m_camera.GetProjectionMatrix();

	auto pv = m_camera.GetProjectionViewMatrix();

	float time = getTime();

	//QuadDraw(pv * m_quadTransform);
	//BunnyDraw(pv * m_bunnyTransform, time);
	QuadTexturedDraw(pv * m_quadTransform);
	PhongDraw(pv * m_bunnyTransform, m_bunnyTransform);


	//Gizmos::draw(m_projectionMatrix * m_viewMatrix);
	//Gizmos::draw(m_viewMatrix * m_projectionMatrix);
	Gizmos::draw(pv);
}

bool aieProject3D1App::LaunchShaders()
{
	//if (!QuadLoader())
	//	return false;

	//if (!BunnyLoader())
	//	return false;

	if (!QuadTextureLoader())
		return false;
	
	if (!PhongLoader())
		return false;

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
	//vertices[64].position = {}

	//for (int i = 0; i < 64; i++)
	//{
	//
	//}

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
	ImGui::DragFloat3("Global Light Direction",
		&m_light.direction[0], 0.1, 0, 1);
	ImGui::DragFloat3("Global Light Color", 
		&m_light.color[0], 1, 0, 255);
	ImGui::DragFloat("Light Intensity",
		&m_light.intensity, 0.1, 1, 100);
	ImGui::DragFloat3("Ambient Light Color", 
		&m_ambientLight[0], 1, 0, 255);
	ImGui::End();

	ImGui::Begin("Camera Settings");
	ImGui::DragFloat3("Position",
		&m_camera.GetPosition()[0]);
	ImGui::DragFloat3("Rotation",
		&stationaryRotation[0], 0, 0, 360);
	if (ImGui::Button("Set Rotation", 
		{ 10,10 }))
	{
		m_camera.SetRotation(stationaryRotation);
	}
	//ImGui::DragFloat("Move Speed",
	//	&m_camera.m_moveSpeed, 0.1, 0.1, 10);
	//ImGui::DragFloat("Sensitivity",
	//	&m_camera.m_turnSpeed, 0.01, 0, 10);
	ImGui::End();

	ImGui::Begin("Material Settings");
	ImGui::DragFloat3("Ambient Colour",
		&v_ambient[0], 1, 0, 255);
	ImGui::DragFloat3("Diffuse Colour",
		&v_diffuse[0], 1, 0, 255);
	ImGui::DragFloat3("Specular Colour",
		&v_specular[0], 1, 0, 255);
	ImGui::DragFloat("Specular Strength",
		&v_specularStrength, 0.1, 1, 100);
	ImGui::End();

	ImGui::Begin("General Settings");
	ImGui::Checkbox("Enable Point Lights", &enablePoints);
	ImGui::End();

	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		ImGUIPointLight(i);
	}
}

void aieProject3D1App::ImGUIPointLight(int number)
{
	std::string title = "Point Light";
	title += std::to_string(number);

	PointLight pl = *pointLight;

	ImGui::Begin(title.c_str());
	ImGui::DragFloat3("Position",
		&((*pointLight).position[0]), 0.1f);
	ImGui::DragFloat3("Ambient",
		&((*pointLight).ambient[0]), 1, 0, 255);
	ImGui::DragFloat3("Diffuse",
		&((*pointLight).diffuse[0]), 1, 0, 255);
	ImGui::DragFloat3("Specular",
		&((*pointLight).specular[0]), 1, 0, 255);
	ImGui::DragFloat("Constant",
		&((*pointLight).constant), 0.1);
	ImGui::DragFloat("Linear",
		&((*pointLight).linear), 0.1);
	ImGui::DragFloat("Quadratic",
		&((*pointLight).quadratic), 0.1);
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
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1
	};

	return true;
}

bool aieProject3D1App::BunnyLoader()
{
	m_colorShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/color.vert");
	m_colorShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/color.frag");

	if (m_colorShader.link() == false)
	{
		printf("Color Shader Error: %s\n", m_colorShader.getLastError());
		return false;
	}

	if (m_bunnyMesh.load("./stanford/Bunny.obj") == false)
	{
		printf("Bunny Mesh Error:\n");
		return false;
	}


	m_bunnyTransform = {
		0.6f,0,0,0,
		0,0.6f,0,0,
		0,0,0.6f,0,
		0, 0, 0, 1
	};

	return true;
}

bool aieProject3D1App::PhongLoader()
{
	m_phongShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/phong.frag");

	if (m_phongShader.link() == false)
	{
		printf("Phong Shader Error: %s\n", m_phongShader.getLastError());
		return false;
	}

	if (m_bunnyMesh.load("./stanford/Bunny.obj") == false)
	{
		printf("Bunny Mesh Error:\n");
		return false;
	}

	m_bunnyTransform = {
		0.6f,0,0,0,
		0,0.6f,0,0,
		0,0,0.6f,0,
		0, 0, 0, 1
	};

	return true;
}

bool aieProject3D1App::QuadTextureLoader() 
{
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/textured.frag");

	if (m_texturedShader.link() == false)
	{
		printf("Textured Shader has an Error: %s\n", m_texturedShader.getLastError());
		return false;
	}

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load the grid texture correctly!\n");
		return false;
	}

	m_quadMesh.InitialiseQuad();

	m_quadTransform = {
		10,0,0,0,
		0,10,0,0,
		0,0,10,0,
		0,0,0,1
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

void aieProject3D1App::BunnyDraw(glm::mat4 pvm, float time)
{
	// Bind the shader
	m_colorShader.bind();

	// Bind the transform
	m_colorShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the base color
	glm::vec4 baseColor = { 0, 1, 0, 1 };
	m_colorShader.bindUniform("BaseColor", baseColor);

	// Bind the time
	m_colorShader.bindUniform("Time", time);

	// Draw the quad using Mesh's draw
	m_bunnyMesh.draw();
}

void aieProject3D1App::PhongDraw(glm::mat4 pvm, glm::mat4 transform)
{
	// Bind the shader
	m_phongShader.bind();

	// Bind the camera position
	m_phongShader.bindUniform("CameraPosition",
		glm::vec3(glm::inverse(m_viewMatrix)[3]));

	// Bind the light
	m_phongShader.bindUniform("LightDirection", m_light.direction);
	m_phongShader.bindUniform("LightColor", m_light.color / 255.f);
	m_phongShader.bindUniform("AmbientColor", m_ambientLight / 255.f);

	// Bind the enable and diable point lights
	m_phongShader.bindUniform("enablePoint", enablePoints);

	// Bind the material settings
	m_phongShader.bindUniform("Ka", v_ambient / 255.f);
	m_phongShader.bindUniform("Kd", v_diffuse / 255.f);
	m_phongShader.bindUniform("Ks", v_specular / 255.f);
	m_phongShader.bindUniform("specularPower", v_specularStrength);

	// Bind the transform
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	
	// Bind the model matrix
	m_phongShader.bindUniform("ModelMatrix", transform);
	
	// Bind the point lights
	glm::mat4 encodedPoints[NR_POINT_LIGHTS];
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		encodedPoints[i] = PointToMatEncode(*pointLight);
	}
	m_phongShader.bindUniform("encodedPL", *encodedPoints);

	// Draw the quad using Mesh's draw
	m_bunnyMesh.draw();
}

void aieProject3D1App::QuadTexturedDraw(glm::mat4 pvm)
{
	// Bind the shader
	m_texturedShader.bind();

	// Bind the transform
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the texture location
	m_texturedShader.bindUniform("diffuseTexture", 0);

	// Bind the texture to a specific location
	m_gridTexture.bind(0);

	// Draw the quad using Mesh's draw
	m_quadMesh.Draw();
}

glm::mat4 aieProject3D1App::PointToMatEncode(PointLight& light)
{
	glm::mat4 encodePoint;

	encodePoint[0] = *(new glm::vec4(light.position, light.constant));
	encodePoint[1] = *(new glm::vec4(light.ambient / 255.f, light.linear));
	encodePoint[2] = *(new glm::vec4(light.diffuse / 255.f, light.quadratic));
	encodePoint[3] = *(new glm::vec4(light.specular / 255.f, 0));

	return encodePoint;
}
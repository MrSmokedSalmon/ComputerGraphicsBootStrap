#include "Instance.h"

#include "FlyCamera.h"
#include "Scene.h"
#include "OBJMesh.h"
#include "Texture.h"
#include "Shader.h"

#include <glm/ext.hpp>

Instance::Instance(glm::mat4 transform, aie::OBJMesh* mesh, 
	aie::ShaderProgram* shader)
{
	m_transform = transform;
	m_mesh = mesh;
	m_shader = shader;
}

void Instance::Draw(Scene* scene)
{
	// Set the shader pipeline
	m_shader->bind();

	// Bind all relevant uniforms for our shaders
	auto pvm = scene->GetCamera()->GetProjectionViewMatrix() * m_transform;
	m_shader->bindUniform("ProjectionViewModel", pvm);
	m_shader->bindUniform("ModelMatrix", m_transform);

	// Bind the camera position
	m_shader->bindUniform("CameraPosition", scene->GetCamera()->GetPosition());

	// Bind the light
	m_shader->bindUniform("LightDirection", scene->GetLight().direction);
	m_shader->bindUniform("LightColor", scene->GetLight().color);
	m_shader->bindUniform("AmbientColor", scene->GetAmbientLightColor());

	m_mesh->draw();
}
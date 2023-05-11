#include "Scene.h"
#include "Instance.h"
#include <imgui.h>

Scene::Scene(FlyCamera* camera, glm::vec2 windowSize,
	Light& light, glm::vec3 ambientLightColor) : 
	m_camera(camera), m_windowSize(windowSize), m_light(light),
	m_ambientLightColor(ambientLightColor)
{

}

Scene::~Scene() {
	for (auto it = m_instances.begin();
		it != m_instances.end(); it++)
	{
		delete *it;
	}
}

void Scene::AddInstance(Instance* instance)
{
	m_instances.push_back(instance);
}

void Scene::Draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColors[i] = m_pointLights[i].color;
	}

	for (auto it = m_instances.begin(); 
		it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}
}

void Scene::DrawDepth(aie::ShaderProgram* shader)
{
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColors[i] = m_pointLights[i].color;
	}

	for (auto it = m_instances.begin();
		it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->DrawDepth(this, shader);
	}
}

void Scene::IMGuiScenePointlight(int lightIndex, const char indexAsString)
{
	float* position[3] = { &m_pointLights[lightIndex].direction.x, &m_pointLights[lightIndex].direction.y, &m_pointLights[lightIndex].direction.z };
	float* color[3] = { &m_pointLights[lightIndex].color.x, &m_pointLights[lightIndex].color.y, &m_pointLights[lightIndex].color.z };

	if (ImGui::CollapsingHeader("Point Light " + indexAsString))
	{
		ImGui::DragFloat3("Global Light Direction",
			*position);
		ImGui::DragFloat3("Global Light Color",
			*color, 1, 0, 255);
	}
}

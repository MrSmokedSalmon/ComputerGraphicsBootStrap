#pragma once

#include "Shader.h"

#include <glm/glm.hpp>
#include <vector>
#include <list>

class FlyCamera;
class Instance;

const int MAX_LIGHTS = 4;

struct Light 
{
	Light()
	{
		direction = glm::vec3(0);
		color = glm::vec3(1);
	}
	Light(glm::vec3 _position, glm::vec3 _color, float _intensity)
	{
		direction = _position;
		color = _color * _intensity;
	}

	glm::vec3 direction;
	glm::vec3 color;
};

class Scene 
{
public:
	Scene(FlyCamera* camera, glm::vec2 windowSize, 
		Light& light, glm::vec3 ambientLightColor);
	~Scene();

	void AddInstance(Instance* instance);

	void Draw();
	void DrawDepth(aie::ShaderProgram* shader);
	void AddPointLights(Light light) { m_pointLights.push_back(light); }
	void AddPointLights(glm::vec3 direction, glm::vec3 color, float intensity) 
	{ 
		m_pointLights.push_back(Light(direction, color, intensity)); 
	}

	void IMGuiScenePointlight(int lightIndex, char indexAsString);

	FlyCamera* GetCamera() { return m_camera; }
	glm::vec2 GetWindowSize() { return m_windowSize; }
	glm::vec3 GetAmbientLightColor() { return m_ambientLightColor; }
	Light GetLight() { return m_light; }

	std::vector<Light>& GetPointLights() { return m_pointLights; }
	int GetNumberOfLights() { return m_pointLights.size(); }

	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColors() { return &m_pointLightColors[0]; }

protected:
	FlyCamera* m_camera;
	glm::vec2 m_windowSize;

	Light m_sunLight;
	std::vector<Light> m_pointLights;

	Light m_light;
	glm::vec3 m_ambientLightColor;
	std::list<Instance*> m_instances;

	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColors[MAX_LIGHTS];
};
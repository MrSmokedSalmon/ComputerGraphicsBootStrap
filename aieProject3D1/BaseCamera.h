#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
class BaseCamera
{
public:
	BaseCamera();
	~BaseCamera() {};

	virtual void Update(float deltaTime);

	virtual glm::vec3 GetPosition() { return m_position; }

	virtual glm::mat4 GetTransform(glm::vec3 position, glm::vec3 eularAngles, glm::vec3 scale);

	virtual glm::mat4 GetViewMatrix();
	virtual glm::mat4 GetProjectionMatrix();
	virtual glm::mat4 GetProjectionViewMatrix();
	virtual float GetAspectRatio();

	virtual void SetPosition(glm::vec3 position);
	virtual void SetAspectRatio(float width, float height);

	virtual void SetViewMatrix(glm::vec3 from, glm::vec3 to, glm::vec3 up);
	virtual void SetProjectionMatrix(float FOV, float aspectRatio, float near, float far);
	virtual void SetProjectionMatrix(float FOV, float width, float height, float near, float far);

protected:

	glm::mat4 m_projectionViewTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
	glm::mat4 m_worldTransform;

	glm::vec3 m_position;

	float m_aspectRatio;

	float m_theta;
	float m_phi;

	glm::vec2 m_lastMouse;
};


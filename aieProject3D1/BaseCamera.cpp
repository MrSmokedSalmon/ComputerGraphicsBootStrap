#include "BaseCamera.h"

#include <glm/ext.hpp>
#include "Input.h"

BaseCamera::BaseCamera()
{
	m_position = glm::vec3(-10, 2, 0);
	m_phi = 0;
	m_theta = 0;
}

void BaseCamera::Update(float deltaTime)
{
}

glm::mat4 BaseCamera::GetViewMatrix()
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR),
		glm::cos(phiR) * glm::sin(thetaR));

	return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 BaseCamera::GetProjectionMatrix(float width, float height)
{
	return glm::perspective(glm::pi<float>() * 0.25f, width / height,
		0.1f, 1000.f);
}

glm::mat4 BaseCamera::GetTransform(glm::vec3 position, glm::vec3 eularAngles, glm::vec3 scale)
{
	return glm::mat4();
}
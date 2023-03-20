#include "BaseCamera.h"

#include "Input.h"

BaseCamera::BaseCamera()
{
	m_position = glm::vec3(-10, 2, 0);
	m_worldTransform =
	{
		1,  0,0,0,
		0,  1,0,0,
		0,  0,1,0,
		-10,2,0,1
	};
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

	m_viewTransform = glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));

	return m_viewTransform;
}

glm::mat4 BaseCamera::GetProjectionMatrix()
{
	return m_projectionTransform;
}

glm::mat4 BaseCamera::GetProjectionViewMatrix()
{
	m_projectionViewTransform = GetProjectionMatrix() * GetViewMatrix();
	return m_projectionViewTransform;
}

float BaseCamera::GetAspectRatio()
{
	return m_aspectRatio;
}

void BaseCamera::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void BaseCamera::SetAspectRatio(float width, float height)
{
	m_aspectRatio = width / height;
}

void BaseCamera::SetViewMatrix(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
	m_viewTransform = glm::lookAt(from, to, glm::vec3(0, 1, 0));
}

void BaseCamera::SetProjectionMatrix(float FOV, float aspectRatio, float near, float far)
{
	m_projectionTransform = glm::perspective(glm::pi<float>() * FOV, aspectRatio,
		near, far);
}

void BaseCamera::SetProjectionMatrix(float FOV, float width, float height, float near, float far)
{
	m_projectionTransform = glm::perspective(glm::pi<float>() * FOV, width / height,
		near, far);
}

glm::mat4 BaseCamera::GetTransform(glm::vec3 position, glm::vec3 eularAngles, glm::vec3 scale)
{
	m_worldTransform = {
		scale.x,	0,			0,			0,
		0,			scale.y,	0,			0,
		0,			0,			scale.z,	0,
		position.x, position.y, position.z, 1
	};

	float angle = glm::length(eularAngles);
	glm::vec3 normal = glm::normalize(eularAngles);

	glm::rotate(m_worldTransform, angle, normal);

	return m_worldTransform;
}
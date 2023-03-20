#include "FlyCamera.h"

#include <glm/ext.hpp>
#include "Input.h"

FlyCamera::FlyCamera()
{
	m_position = glm::vec3(-10, 2, 0);
	m_phi = 0;
	m_theta = 0;
}

void FlyCamera::Update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR),
		glm::cos(phiR) * glm::sin(thetaR));
	glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));
	glm::vec3 up(0, 1, 0);

	// Use WASD to move and Q & E to go up and down
	if (input->isKeyDown(aie::INPUT_KEY_W)) m_position += forward * m_moveSpeed * deltaTime;
	if (input->isKeyDown(aie::INPUT_KEY_S)) m_position -= forward * m_moveSpeed * deltaTime;
	if (input->isKeyDown(aie::INPUT_KEY_A)) m_position -= right * m_moveSpeed * deltaTime;
	if (input->isKeyDown(aie::INPUT_KEY_D)) m_position += right * m_moveSpeed * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_Q)) m_position -= up * m_moveSpeed * deltaTime;
	if (input->isKeyDown(aie::INPUT_KEY_E)) m_position += up * m_moveSpeed * deltaTime;

	// Get the mouse coordinates
	float mx = input->getMouseX();
	float my = input->getMouseY();

	// If the right button is held increment theta and phi (rotate)
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += m_turnSpeed * (mx - m_lastMouse.x) * deltaTime;
		m_phi += m_turnSpeed * (my - m_lastMouse.y) * deltaTime;
	}

	m_lastMouse = glm::vec2(mx, my);
}
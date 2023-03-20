#pragma once

#include "BaseCamera.h"

#include <glm/glm.hpp>
class SimpleCamera : public BaseCamera
{
public: 
	SimpleCamera();
	~SimpleCamera() {};

	void Update(float deltaTime);

	float m_turnSpeed = glm::radians(180.f);
	float m_moveSpeed = 4.f;

private:
	// Last position of the mouse
	glm::vec2 m_lastMouse;
};
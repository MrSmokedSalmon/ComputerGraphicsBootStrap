#pragma once

#include "BaseCamera.h"

#include <glm/glm.hpp>
class FlyCamera : public BaseCamera
{
public: 
	FlyCamera();
	~FlyCamera() {};

	void Update(float deltaTime);

	float m_turnSpeed = glm::radians(180.f);
	float m_moveSpeed = 4.f;
private:

};	